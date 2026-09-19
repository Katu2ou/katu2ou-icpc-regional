#!/usr/bin/env python3
"""Build a compact, two-column PDF from every C++ source in this repository."""

import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unicodedata

from pygments.lexers import CppLexer
from pygments.token import Comment, Keyword, Name, Number, String


ROOT = Path(__file__).resolve().parent.parent
MM = 72 / 25.4
ORDER = [
    "template.cpp", "dsu.cpp", "segtree.cpp", "lazy_segtree.cpp",
    "binarytrie.cpp", "trie.cpp", "suffix_array.cpp", "lcp_array.cpp",
    "z_algorithm.cpp", "rolling_hash.cpp", "scc.cpp", "twosat.cpp",
    "rerooting.cpp", "maxflow.cpp", "mincostflow.cpp", "math.cpp",
    "modint_convolution.cpp", "geometry.cpp",
]

PALETTE = {
    "plain": {"color": "#17212B", "bold": False},
    "keyword": {"color": "#174A91", "bold": True},
    "type": {"color": "#08656A", "bold": True},
    "function": {"color": "#225A80", "bold": False},
    "number": {"color": "#963D16", "bold": False},
    "string": {"color": "#8A2455", "bold": False},
    "comment": {"color": "#38683A", "bold": False},
    "preprocessor": {"color": "#703E89", "bold": True},
}


def format_source(source, filename, width):
    style = {
        "BasedOnStyle": "LLVM", "IndentWidth": 2,
        "ContinuationIndentWidth": 2, "ColumnLimit": width,
        "UseTab": "Never", "SortIncludes": "Never",
        "SortUsingDeclarations": "Never", "ReflowComments": "Never",
        "FixNamespaceComments": False, "BreakStringLiterals": False,
        "AllowShortFunctionsOnASingleLine": "InlineOnly",
        "AllowShortIfStatementsOnASingleLine": "Never",
        "AllowShortLoopsOnASingleLine": False,
        "AllowShortLambdasOnASingleLine": "Inline",
        "BreakTemplateDeclarations": "Yes",
        "BinPackParameters": "BinPack", "AlignAfterOpenBracket": "DontAlign",
        "AlignOperands": "DontAlign", "AlignTrailingComments": {"Kind": "Never"},
        "MaxEmptyLinesToKeep": 1, "PointerAlignment": "Left",
    }
    formatted = subprocess.run(
        ["clang-format", "--style=" + json.dumps(style),
         "--assume-filename=" + filename],
        input=source, text=True, capture_output=True, check=True,
    ).stdout
    if re.sub(r"\s", "", source) != re.sub(r"\s", "", formatted):
        raise ValueError(f"Formatting changed source characters: {filename}")
    # Whitespace inside string/character literals must also remain identical.
    def literals(code):
        return [(kind, value) for _, kind, value in CppLexer().get_tokens_unprocessed(code)
                if kind in String]
    if literals(source) != literals(formatted):
        raise ValueError(f"Formatting changed a literal: {filename}")
    return formatted


def highlight_lines(source):
    """Lex the whole file so multiline comments retain their correct style."""
    tokens = list(CppLexer().get_tokens_unprocessed(source))
    assert "".join(value for _, _, value in tokens) == source
    styles = ["plain"] * len(source)
    common_types = {"vector", "pair", "tuple", "array", "complex", "map", "set",
                    "queue", "stack", "deque", "priority_queue", "string",
                    "unordered_map", "unordered_set", "ll", "ld", "Point"}
    for offset, kind, value in tokens:
        style = "plain"
        if kind in Comment.Preproc:
            style = "preprocessor"
        elif kind in Comment:
            style = "comment"
        elif kind in Keyword.Type or kind in Name.Class or (kind in Name and value in common_types):
            style = "type"
        elif kind in Keyword:
            style = "keyword"
        elif kind in Number:
            style = "number"
        elif kind in String:
            style = "string"
        elif kind in Name and source[offset + len(value):].lstrip().startswith("("):
            style = "function"
        styles[offset:offset + len(value)] = [style] * len(value)
    lines = []
    offset = 0
    for line in source.splitlines(keepends=True):
        text = line.rstrip("\r\n")
        lines.append((text, styles[offset:offset + len(text)]))
        offset += len(line)
    return lines


def styled_spans(text, styles):
    assert len(text) == len(styles)
    spans = []
    for char, style in zip(text, styles):
        if spans and spans[-1]["style"] == style:
            spans[-1]["text"] += char
        else:
            spans.append({"text": char, "style": style})
    return spans


def keep_lines_together(lines, start):
    """Avoid isolated declarations and closing braces at column boundaries."""
    end = start + 1
    code = lines[start][0].strip()
    if code:
        # A continued parameter list should stay with its declaration.
        balance = 0
        for _, kind, value in CppLexer().get_tokens_unprocessed(code):
            if kind not in Comment and kind not in String:
                balance += value.count("(") + value.count("[")
                balance -= value.count(")") + value.count("]")
        while balance > 0 and end < min(start + 6, len(lines)):
            for _, kind, value in CppLexer().get_tokens_unprocessed(lines[end][0]):
                if kind not in Comment and kind not in String:
                    balance += value.count("(") + value.count("[")
                    balance -= value.count(")") + value.count("]")
            end += 1
        if code.startswith("template") or code.endswith("{"):
            end = min(max(end, start + 3), len(lines))
        # Carry the preceding statement with a short run of closing braces.
        while end < min(start + 5, len(lines)) and re.fullmatch(r"[};\s]+", lines[end][0]):
            end += 1
    return end


def cell_width(char):
    if unicodedata.combining(char):
        return 0
    return 2 if unicodedata.east_asian_width(char) in "WF" else 1


def wrap_line(line, limit):
    """Keep every source character; added indentation is presentation only."""
    rest = line.expandtabs(4)
    chunks = []
    while rest:
        indent = "  " if chunks else ""
        available = limit - len(indent)
        width = 0
        end = 0
        for char in rest:
            if width + cell_width(char) > available:
                break
            width += cell_width(char)
            end += 1
        if not end:
            raise ValueError("Column is too narrow")
        if end < len(rest):
            # Prefer punctuation or spaces, without creating very short lines.
            candidates = [
                i for i in range(1, end + 1)
                if rest[i - 1] in " ,;({[}" and
                sum(cell_width(c) for c in rest[:i]) >= available * 0.72
            ]
            if candidates:
                end = candidates[-1]
        chunks.append((indent + rest[:end], bool(chunks), rest[:end]))
        rest = rest[end:]
    if not chunks:
        chunks = [("", False, "")]
    assert "".join(chunk[2] for chunk in chunks) == line.expandtabs(4)
    return chunks


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--font-size", type=float, default=9.0,
                        help="Code font size in points (default: 9)")
    parser.add_argument("--orientation", choices=("landscape", "portrait"),
                        default="landscape")
    parser.add_argument("--max-pages", type=int, default=25)
    parser.add_argument("--output", type=Path, default=ROOT / "library.pdf")
    args = parser.parse_args()
    if not 8 <= args.font_size <= 14:
        parser.error("--font-size must be between 8 and 14 points")
    if args.max_pages < 1:
        parser.error("--max-pages must be positive")
    for command in ("typst", "clang-format"):
        if not shutil.which(command):
            parser.error(f"{command} is required; install it before running this script")

    files = [p for p in ROOT.rglob("*.cpp")
             if not any(part.startswith(".") for part in p.relative_to(ROOT).parts)]
    ranks = {name: i for i, name in enumerate(ORDER)}
    files.sort(key=lambda p: (ranks.get(p.relative_to(ROOT).as_posix(), len(ORDER)),
                              p.relative_to(ROOT).as_posix()))
    if not files:
        parser.error("No .cpp files found")

    font_size = args.font_size
    line_height = font_size * 1.23
    heading_height = font_size + 9
    landscape = args.orientation == "landscape"
    page_width, page_height = (297, 210) if landscape else (210, 297)
    # A4, 10 mm margins, 6 mm column gap, 6 mm for line numbers.
    column_width = (page_width - 20 - 6) / 2 * MM
    # Menlo has a 0.60205-em advance. A slight buffer avoids rounding overflow.
    limit = math.floor((column_width - 6 * MM) / (font_size * 0.605))
    body_height = (page_height - 20 - 7) * MM
    columns = []
    rows = []
    used = 0
    sources = []
    total_lines = 0
    formatted_lines = 0
    wrap_count = 0

    def next_column():
        nonlocal rows, used
        columns.append(rows)
        rows = []
        used = 0

    def heading(name, continued=False):
        nonlocal used
        rows.append({"kind": "heading", "text": name,
                     "continued": continued})
        used += heading_height

    for source in files:
        name = source.relative_to(ROOT).as_posix()
        data = source.read_bytes()
        original = data.decode("utf-8-sig")
        source_lines = original.splitlines()
        total_lines += len(source_lines)
        formatted = format_source(original, name, limit)
        highlighted = highlight_lines(formatted)
        formatted_lines += len(highlighted)
        sources.append({"file": name, "lines": len(source_lines),
                        "printed_lines": len(highlighted),
                        "sha256": hashlib.sha256(data).hexdigest()})
        if used + heading_height + 2 * line_height > body_height:
            next_column()
        heading(name)
        wrapped = [wrap_line(line, limit) for line, _ in highlighted]
        for line_number, (line, styles) in enumerate(highlighted, 1):
            assert "\t" not in line, "clang-format must expand tabs before highlighting"
            chunks = wrapped[line_number - 1]
            wrap_count += len(chunks) - 1
            # Keep the pieces of a wrapped line together when possible.
            remaining = body_height - used
            group_end = keep_lines_together(highlighted, line_number - 1)
            group_height = sum(len(pieces) for pieces in wrapped[line_number - 1:group_end]) * line_height
            if (group_height > remaining and group_height + heading_height <= body_height):
                next_column()
                heading(name, continued=True)
            offset = 0
            for code, continued, original_chunk in chunks:
                if used + line_height > body_height:
                    next_column()
                    heading(name, continued=True)
                chunk_styles = styles[offset:offset + len(original_chunk)]
                if continued:
                    chunk_styles = ["plain", "plain"] + chunk_styles
                spans = styled_spans(code, chunk_styles)
                assert "".join(span["text"] for span in spans) == code
                rows.append({"kind": "code", "text": code, "spans": spans,
                             "continued": continued, "source_line": line_number,
                             "band": ((line_number - 1) // 5) % 2 == 1})
                offset += len(original_chunk)
                used += line_height
            assert offset == len(line)
    if rows:
        columns.append(rows)
    pages = [columns[i:i + 2] for i in range(0, len(columns), 2)]
    if len(pages) > args.max_pages:
        parser.error(f"{len(pages)} pages exceeds the limit of {args.max_pages}; "
                     "choose a smaller --font-size or review the source files")
    for page in pages:
        while len(page) < 2:
            page.append([])
    document = {"font_size": font_size, "line_height": line_height,
                "heading_height": heading_height, "pages": pages,
                "palette": PALETTE, "landscape": landscape}
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="icpc-paper-") as temporary:
        work = Path(temporary)
        shutil.copyfile(Path(__file__).with_name("layout.typ"), work / "layout.typ")
        (work / "content.json").write_text(json.dumps(document, ensure_ascii=False),
                                           encoding="utf-8")
        subprocess.run(["typst", "compile", str(work / "layout.typ"),
                        str(work / "library.pdf")], check=True)
        # Source files must not have changed while the PDF was being generated.
        for item in sources:
            if hashlib.sha256((ROOT / item["file"]).read_bytes()).hexdigest() != item["sha256"]:
                raise RuntimeError(f"Source changed during the build: {item['file']}")
        shutil.copyfile(work / "library.pdf", output)
    manifest = {"pdf": output.name, "pages": len(pages), "paper": "A4",
                "orientation": args.orientation,
                "columns": 2, "margin_mm": 10, "column_gap_mm": 6,
                "font_pt": font_size, "line_height_pt": line_height,
                "syntax_colors": PALETTE, "printed_lines": formatted_lines,
                "visual_wraps": wrap_count,
                "total_source_lines": total_lines, "sources": sources}
    output.with_suffix(".manifest.json").write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Created {output}")
    print(f"{len(files)} source files / {total_lines} lines / "
          f"{len(pages)} pages / {font_size:g} pt / {limit} cells per line / "
          f"{wrap_count} visual wraps")


if __name__ == "__main__":
    main()
