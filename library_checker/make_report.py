#!/usr/bin/env python3
"""Render saved online judge responses as a Japanese submission report."""
import base64
from collections import Counter
from datetime import datetime
import hashlib
import json
from pathlib import Path
import subprocess
from zoneinfo import ZoneInfo

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
tasks = json.loads((HERE / 'manifest.json').read_text())
records = json.loads((HERE / 'results.json').read_text())
latest = {r['key']: r for r in records}
counts = Counter(r['status'] for r in latest.values())
stamp = datetime.now(ZoneInfo('Asia/Tokyo')).isoformat(timespec='seconds')
commit = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
libraries = {lib['file']: lib['sha256'] for task in tasks for lib in task['libraries']}
assert set(libraries) == {p.name for p in ROOT.glob('*.cpp')}
for name, digest in libraries.items():
    assert hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == digest

lines = [
    '# Library Checker 提出結果', '',
    f'更新日時: {stamp}  ',
    f'対象: `{ROOT}`  ',
    f'確認時の Git HEAD: `{commit}`', '',
    f'対象は {len(libraries)} 個の `.cpp` ファイルです。{len(tasks)} 件の提出で、'
    '各ライブラリの主要な機能を確認しました。', '',
    '**判定集計:** ' + ' / '.join(f'{k}: {v} 件' for k, v in sorted(counts.items())), '',
    '匿名・C++17 で提出しました。元の `.cpp` の内容は一切変更せず、提出ソースに全文をそのまま埋め込み、'
    'ヘッダー・型・依存ライブラリ・入出力用 `main()` を追加しています。'
    'ジャッジから取得した提出ソースと生成したソースの SHA-256 が一致することも確認しています。', '',
    'AC は対応する問題のテストに通過したという意味です。各ファイルの全関数・全用途の正しさを保証するものではありません。', '',
    '| 主な対象ファイル | 問題 | 判定・提出リンク |',
    '| --- | --- | --- |',
]
primary = {
    'bmbm_recurrence': 'bmbm.cpp', 'bmbm_kth': 'bmbm.cpp',
    'hld_path_sum': 'hld.cpp', 'hld_composite': 'hld.cpp',
    'linear_equation_det': 'linear_equation.cpp', 'linear_equation_rank': 'linear_equation.cpp',
    'wavelet_matrix_kth': 'wavelet_matrix.cpp', 'wavelet_matrix_rank': 'wavelet_matrix.cpp',
    'dynamic_segtree_composite': 'dynamic_segtree.cpp',
}
for task in sorted(tasks, key=lambda t: (primary.get(t['key'], t['key'] + '.cpp'), t['problem'])):
    r = latest.get(task['key'])
    verdict = f"[{r['status']}]({r['url']})" if r and 'url' in r else '未提出'
    name = primary.get(task['key'], task['key'] + '.cpp')
    problem = task['problem']
    title = r.get('overview', {}).get('problem_title', problem) if r else problem
    lines.append(f'| [{name}](../{name}) | [{title}](https://judge.yosupo.jp/problem/{problem}) | {verdict} |')

lines += ['', '## 確認範囲と接続方法', '',
    '- `bmbm.cpp`: Berlekamp–Massey と Bostan–Mori の両方を提出。',
    '- `dynamic_segtree.cpp`: 区間和と、順序に意味のあるアフィン関数合成を提出。',
    '- `hld.cpp`: 頂点のパス和と、向きを考慮したパス上の関数合成を提出。',
    '- `linear_equation.cpp`: 連立方程式・行列式・ランクを提出。係数型は ACL の `modint998244353`。'
    'このファイルの実装は変更していません。',
    '- `wavelet_matrix.cpp`: 区間 k 番目と区間内の指定値の出現回数を提出。',
    '- `math.cpp`: `floor_sum` を確認。`pow_mod`・`inv_mod`・`crt` は今回の提出では未検証。',
    '- `template.cpp`: 全文を名前空間で囲み、別の `main()` から `chmin`・`chmax` を使って A+B を提出。'
    '元の空の `solve()` や `rep` マクロは検証対象外。',
    '- `trie.cpp`: 数値キーを十進文字列として格納し、`insert`・`find` を確認。'
    '値は trie の終端ノード番号に対応づけて保存。',
    '- `binarytrie.cpp`: 重複追加・存在しない値の削除を入出力側で除外し、集合として扱っています。',
    '- `convex_hull_trick.cpp`: 係数と結果の符号反転で、最大値クエリを最小値問題に接続。',
    '- `mincostflow.cpp`: 全コストに共通の定数を加えて非負にし、最後に補正。',
    '- `manacher.cpp`: 区切り文字の挿入で奇数長・偶数長の両方を確認。',
    '- `rolling_hash.cpp`: ハッシュ一致の二分探索で各接尾辞と先頭との LCP を計算。',
    '- `eulertour.cpp` は同梱の `segtree.cpp`、`lcp_array.cpp` は同梱の `suffix_array.cpp` と組み合わせて確認。',
    '- 残りのデータ構造でも、問題に必要なモノイドやコールバックを入出力側で定義しています。',
    '', '## AC 以外の詳細', '',
]
failures = [r for r in latest.values() if r['status'] in {'WA','RE','TLE','MLE','CE','OLE','IE'}]
if not failures:
    lines.append('確定した不合格判定はありません。')
for r in sorted(failures, key=lambda r: r['key']):
    lines += [f"### {r['key']}: [{r['status']}]({r['url']})", '']
    if r['key'] == 'geometry' and r['status'] == 'CE':
        lines += [
            'ジャッジのログでは、`onSegment` が未宣言の `ccw` を呼び出していること'
            '（元ファイル 55 行目）と、`circumCircle` 内の `(a+b)/2` で'
            '`complex<long double>` と `int` の型が一致しないこと'
            '（元ファイル 60 行目）が報告されています。元コードの修正は行っていません。', '',
        ]
    if r['key'] == 'mincostflow' and r['status'] == 'TLE':
        lines += [
            'Assignment Problem の `hand_minus_00` で制限時間の 5 秒に達しました。'
            '各行・列を容量 1 の二部グラフにして最小費用流を呼び出した結果です。'
            'この制約下での速度不足を示す判定であり、これだけで計算結果が誤っているとは判断できません。'
            '本体の高速化や差し替えは行っていません。', '',
        ]
    data = json.loads((HERE / 'judge' / f"{r['id']}.json").read_text())
    if data.get('compile_error'):
        err = base64.b64decode(data['compile_error']).decode(errors='replace')
        path = HERE / 'logs' / f"{r['key']}.judge_compile.txt"
        path.write_text(err)
        lines.append(f'[ジャッジのコンパイルログ](logs/{path.name})')
        lines.append('')
        errors = [s for s in err.splitlines() if 'error:' in s][:8]
        lines += ['```text', *errors, '```', '']
    bad = [c for c in data.get('case_results', []) if c['status'] in {'WA','RE','TLE','MLE','CE','OLE','IE'}]
    if bad:
        lines += ['失敗ケース（先頭 10 件）:', '', '| ケース | 判定 | 秒 |', '| --- | --- | --- |']
        for c in bad[:10]:
            lines.append(f"| `{c['case']}` | {c['status']} | {c['time']} |")
        lines.append('')
    if any(c['status'] == '-' for c in data.get('case_results', [])):
        lines += ['判定が `-` のケースは未判定です（コンパイル失敗や TLE 後の打ち切り）。', '']

lines += ['## 保存ファイル', '',
    '- `submissions/*.cc`: 実際の提出ソース（元ライブラリの SHA-256 をコメントに記載）。',
    '- `manifest.json`: 問題と元ライブラリの対応・ハッシュ。',
    '- `results.json`: 提出 ID・URL・判定・提出ソースの照合結果。',
    '- `judge/*.json`: Library Checker API の判定応答全文。',
    '- `local_results.json` と `logs/`: 提出前の公式サンプル実行結果とコンパイルログ。',
    '', '## 再生成・判定再取得', '',
    'リポジトリ直下で実行します。`submit.py` は既存の提出 ID を保存しており、'
    '同じソースの提出を自動で重複させません。ソースが変わった場合は新規提出されます。', '',
    '```sh',
    'python3 library_checker/build_submissions.py',
    'python3 library_checker/download_samples.py',
    'python3 library_checker/check_samples.py',
    'python3 library_checker/submit.py',
    'python3 library_checker/make_report.py',
    '```', '',
    '`download_samples.py` と `submit.py` はネット接続が必要です。'
    '`check_samples.py` のコンパイラ・ACL パスはこの Mac の環境に合わせています。', '',
]
(HERE / 'RESULTS.md').write_text('\n'.join(lines))
print('Wrote', HERE / 'RESULTS.md', dict(counts))
