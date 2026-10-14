# Min-cost flow の TLE と ACL 比較

問題: [Library Checker Assignment Problem](https://judge.yosupo.jp/problem/assignment)、時間制限 5 秒。

実施日: 2026-09-21T17:31:25.985229+09:00

## オンライン判定

| 実装・入力変換 | 判定 | 最大実行時間 | 提出 |
| --- | --- | ---: | --- |
| 現行・全体共通のシフト | TLE | 5.000 秒 | [404419](https://judge.yosupo.jp/submission/404419) |
| 現行・行ごとのシフト | TLE | 5.000 秒 | [404478](https://judge.yosupo.jp/submission/404478) |
| ACL・全体共通のシフト | AC | 1.078 秒 | [404495](https://judge.yosupo.jp/submission/404495) |
| ACL・行ごとのシフト | AC | 1.052 秒 | [404496](https://judge.yosupo.jp/submission/404496) |
| 現行に探索打ち切りとポテンシャル更新の変更・行ごとのシフト | AC | 1.762 秒 | [404498](https://judge.yosupo.jp/submission/404498) |

実行時間はジャッジが報告したケースごとの最大値。TLE の 5 秒は打ち切り時間です。
現行・全体共通のシフトは hand_minus_00 で、現行・行ごとのシフトは multiplication_table_00 で TLE でした。
既存の行ごとのシフト提出は小規模グラフの全探索検査を先に実行しています。今回の比較用提出にはその検査を含めず、ローカルで独立に実行しています。

## 変更内容

比較用の最小変更版は、優先度付きキューから終点が取り出され距離が確定した時点で探索を終了します。
その際、確定済みの頂点だけ h[v] += d[v] - d[t] と更新します。単に break を加え、従来の未確定距離を使うポテンシャル更新を残す変更ではありません。
ACL も同種の打ち切りとポテンシャル更新を行います。ほかに距離が増えない更新をヒープに入れない処理と、辺を連続領域に格納する CSR を使っています。

参照: [ACL公式実装](https://github.com/atcoder/ac-library/blob/864245a00b00dd008d1abfdc239618fdb7d139da/atcoder/mincostflow.hpp)、[公式ドキュメント](https://atcoder.github.io/ac-library/production/document_ja/mincostflow.html)。

元の mincostflow.cpp は変更していません。改善版と差分は [early_exit_mincostflow.cpp](early_exit_mincostflow.cpp)、[early_exit.patch](early_exit.patch) に保存しました。

## ローカル検証

元実装・ACL・最小変更版について、150 個の固定シードの小規模グラフで可能な辺流量を全列挙し、slope の全区間、flow の各オーバーロード、流量制限、各辺の流量と保存則を確認しました。すべて成功しました。
公式生成器と同じ N=500 の hand_minus、multiplication_table の行列でも、出力した割当てが置換であることと、その費用が理論上の最適値に等しいことを確認しました。

| ケース | 比較版 | ローカル実測時間 |
| --- | --- | ---: |
| hand_minus_00 | original_global | 7.694 秒 |
| hand_minus_00 | acl_global | 0.756 秒 |
| hand_minus_00 | original_row | 0.853 秒 |
| hand_minus_00 | early_exit_row | 0.735 秒 |
| hand_minus_00 | acl_row | 0.666 秒 |
| multiplication_table_00 | original_global | 6.633 秒 |
| multiplication_table_00 | acl_global | 1.331 秒 |
| multiplication_table_00 | original_row | 6.560 秒 |
| multiplication_table_00 | early_exit_row | 1.216 秒 |
| multiplication_table_00 | acl_row | 1.398 秒 |

ローカル計測は各 1 回・逐次実行です。GCC 14.2、C++17、-O2 を使用しました。オンラインの秒数とは計測環境が異なります。

## 再現情報

- [取得した公式コードのコミットとSHA-256](upstream.json)
- [提出結果とソース照合](results.json)
- [比較コード生成](build.py)
- [ローカル測定](benchmark.py)
- [コンパイル・小規模全探索の結果](compile_results.json)
- [測定結果JSON](benchmark_results.json)

元ライブラリ SHA-256: `56de4e37599146e56d522e39c5486e71253b259ea6e71538c31ca4cd63cbe7a6`
