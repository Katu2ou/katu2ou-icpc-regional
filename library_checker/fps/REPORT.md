# fps.cpp 全関数の Library Checker 検証

対象: [fps.cpp](../../fps.cpp) に明示的に定義された14関数（演算子・変換コンストラクタを含む）。
元の fps.cpp と依存する modint_convolution.cpp をそのまま提出コードへ埋め込み、SHA-256で一致を確認しています。

作成日時: 2026-09-21T23:28:00.156438+09:00

## 提出結果

| 提出 | 問題 | 判定 | 最大時間 | ケース判定数 |
| --- | --- | --- | ---: | --- |
| [fps_multiply](https://judge.yosupo.jp/submission/404682) | [convolution_mod](https://judge.yosupo.jp/problem/convolution_mod) | AC | 0.263 秒 | AC: 53 |
| [fps_inv](https://judge.yosupo.jp/submission/404683) | [inv_of_formal_power_series](https://judge.yosupo.jp/problem/inv_of_formal_power_series) | AC | 0.529 秒 | AC: 25 |
| [fps_log](https://judge.yosupo.jp/submission/404685) | [log_of_formal_power_series](https://judge.yosupo.jp/problem/log_of_formal_power_series) | AC | 0.706 秒 | AC: 25 |
| [fps_exp](https://judge.yosupo.jp/submission/404686) | [exp_of_formal_power_series](https://judge.yosupo.jp/problem/exp_of_formal_power_series) | AC | 1.581 秒 | AC: 26 |
| [fps_pow](https://judge.yosupo.jp/submission/404687) | [pow_of_formal_power_series](https://judge.yosupo.jp/problem/pow_of_formal_power_series) | AC | 2.234 秒 | AC: 37 |
| [fps_helpers](https://judge.yosupo.jp/submission/404688) | [convolution_mod](https://judge.yosupo.jp/problem/convolution_mod) | AC | 0.526 秒 | AC: 53 |

## 関数ごとの対応

| 関数 | 検証方法 | 判定・提出 |
| --- | --- | --- |
| [`fps(vector<mint>)`](../../fps.cpp#L3) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`pre(int)`](../../fps.cpp#L5) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`operator+(fps,const fps&)`](../../fps.cpp#L9) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`operator-(fps,const fps&)`](../../fps.cpp#L14) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`operator*(const fps&,const fps&)`](../../fps.cpp#L19) | 対応する公式問題の出力として直接検証 | [AC](https://judge.yosupo.jp/submission/404682) |
| [`operator*=(mint)`](../../fps.cpp#L23) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`operator*(fps,mint)`](../../fps.cpp#L27) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`operator*(mint,fps)`](../../fps.cpp#L28) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`diff()`](../../fps.cpp#L30) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`integral()`](../../fps.cpp#L36) | 提出内の独立した係数検査＋公式入力の配列に対する検査 | [AC](https://judge.yosupo.jp/submission/404688) |
| [`inv(int=-1)`](../../fps.cpp#L48) | 対応する公式問題の出力として直接検証 | [AC](https://judge.yosupo.jp/submission/404683) |
| [`log(int=-1)`](../../fps.cpp#L61) | 対応する公式問題の出力として直接検証 | [AC](https://judge.yosupo.jp/submission/404685) |
| [`exp(int=-1)`](../../fps.cpp#L70) | 対応する公式問題の出力として直接検証 | [AC](https://judge.yosupo.jp/submission/404686) |
| [`pow(long long,int=-1)`](../../fps.cpp#L85) | 対応する公式問題の出力として直接検証 | [AC](https://judge.yosupo.jp/submission/404687) |

## 補助関数と特殊ケースの検証

`fps_helpers` は Convolution の公式入力を読み、`pre`、加減算、スカラー倍の各演算子、微分、積分、vectorからのコンストラクタを独立した係数計算と照合します。
積分は定数項・長さと、各係数に次数を掛けると元の係数に戻ることを確認します。対象の次数は法より小さいので、この条件で積分係数が一意に決まります。
不一致の場合は `abort()` し、`NDEBUG` に関わらず提出が失敗するようにしています。これらは専用問題の直接判定ではなく、提出プログラム内の検査です。

同じ提出では全14関数を対象に、固定シードのランダム入力・小配列の全列挙・特殊ケースも検査します。
- 空配列、定数、多項式の長さが異なる場合、全係数0、先頭に0が並ぶ場合。
- 2のべき乗の前後の長さ、係数0・1・法-1、スカラー0・1・法-1。
- inv/log/exp/powのデフォルトの精度指定と、0・1・入力より短い／長い精度指定。
- powの指数0・1・2・5・31・法・法+1・10^18・LLONG_MAX。
- 長さ0〜5の全ての0/1係数列について、指数0・1・2・3・LLONG_MAXを検査。
- invは係数漸化式、logは単純な逆数・微分・積分、expは係数漸化式、powは単純な二乗法と比較。

ローカルの同一検査: `PASS 12290 checks`、実行時間 0.621 秒。

## 再現と保存ファイル

- [全関数の検査コード](checks.hpp)
- [提出コード生成](build.py)
- [提出元・ライブラリのハッシュ](manifest.json)
- [判定・提出URL・ソース照合記録](results.json)
- [関数ごとの機械可読結果](function_status.json)
- [公式サンプルの実行結果](local_results.json)
- [独立計算との比較結果](oracle_results.json)

```sh
python3 library_checker/fps/build.py
python3 library_checker/download_samples.py library_checker/fps
python3 library_checker/check_samples.py library_checker/fps
python3 library_checker/submit.py library_checker/fps
python3 library_checker/fps/report.py
```
