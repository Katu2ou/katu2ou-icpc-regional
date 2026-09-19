# 色付きの紙ライブラリ PDF

リポジトリ内の `.cpp` をすべて読み込み、`library.pdf` を生成します。
Python 3、Pygments、Typst、clang-format が必要です。
この環境の Typst 0.10.0、clang-format 20.1.6、Pygments 2.11.2 で動作確認しています。

```sh
python3 paper_library/build_pdf.py
```

- A4 横、2段組、上下左右 10 mm の余白、段間 6 mm。
- コードは Menlo 9 pt、日本語は Hiragino Sans。
- 行送り約 11.1 pt。各ページの右上に「ページ / 総ページ数」を表示。
- キーワードは青の太字、型は青緑、数値は茶、文字列は赤紫、コメントは緑。
  本文は濃い色を使い、印刷したときにも読める配色にしています。
- 各行に番号を表示し、5行ごとに薄い背景色を付けています。
- ファイル名の見出しを付け、段をまたぐときは `(cont.)` と表示。
- 関数宣言や閉じ括弧が段の端に孤立しにくいように配置します。
- 長い行は折り返し、続きに `↪` を表示。この印と追加の字下げはコードの一部ではありません。
- PDF用に clang-format で2スペースの字下げ、演算子の前後の空白、改行を整えます。
  コメントも収録し、元のコードの空白以外の文字列と、文字列・文字リテラルが
  変化していないことを検証します。
- 行番号は**PDF上で整形したコードの行番号**です。元ファイルの行番号とは異なります。
- `.cpp` 自体は変更しません。25ページを超えた場合は生成を停止します。
- `library.manifest.json` に収録ファイル、SHA-256、ページ数、組版条件を記録します。

読みやすさは視力や印刷環境にも左右されるため、最初の1ページを
**A4・倍率100%（実際のサイズ）・1枚に1ページ**で印刷して確認してください。
PDF自体が2段組なので、印刷設定で「1枚に2ページ」を選ぶ必要はありません。

文字サイズや用紙の向きを変えて別の PDF を作ることもできます。

```sh
python3 paper_library/build_pdf.py --font-size 10 --output library-10pt.pdf
python3 paper_library/build_pdf.py --font-size 11 --output library-11pt.pdf
python3 paper_library/build_pdf.py --orientation portrait --output library-portrait.pdf
```

50 cm での判読をフォントサイズだけで保証することはできません。
試し刷りで見づらい場合は 11 pt 版を作成してください。
