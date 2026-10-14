#!/usr/bin/env python3
"""Function-level evidence, distinguishing submitted results from prepared tests."""
from collections import Counter,defaultdict
from datetime import datetime
import hashlib
import json
from pathlib import Path
from zoneinfo import ZoneInfo

HERE=Path(__file__).resolve().parent
rows=json.loads((HERE/'function_inventory.json').read_text())
directories={'previous':HERE,'lc':HERE/'full','geometry':HERE/'full_geometry','aoj':HERE/'aoj'}
tasks={};results={}
for prefix,directory in directories.items():
    for t in json.loads((directory/'manifest.json').read_text()):tasks[prefix+'/'+t['key']]=(t,directory)
    p=directory/'results.json'
    if p.exists():
        for r in json.loads(p.read_text()):results[prefix+'/'+r['key']]=r

for row in rows:
    for test in row['tests']:assert test in tasks,test
for name,(t,directory) in tasks.items():
    for lib in t['libraries']:
        assert hashlib.sha256((HERE.parent/lib['file']).read_bytes()).hexdigest()==lib['sha256'],lib['file']

def evidence(test):
    t,directory=tasks[test];r=results.get(test)
    title=(r or {}).get('overview',{}).get('problem_title',t['problem'])
    label=test+': '+(r['status'] if r else '未提出')
    if r and 'url' in r:return f"[{label}]({r['url']})"
    path=(directory/t['source']).relative_to(HERE)
    return f'[{label}]({path})'

def state(row):
    statuses=[results[t]['status'] for t in row['tests'] if t in results]
    if 'AC' in statuses:return 'AC確認あり'
    final=[s for s in statuses if s in {'CE','WA','TLE','RE','MLE','OLE','PE','JNA'}]
    if final:return '/'.join(sorted(set(final)))
    return 'オンライン結果待ち'

counts=Counter(state(row) for row in rows)
lines=['# 関数別オンライン検証結果','',f"更新: {datetime.now(ZoneInfo('Asia/Tokyo')).isoformat(timespec='seconds')}",'',
    f'32 ファイルに明示的に定義された名前付き関数・コンストラクタ・オーバーロード {len(rows)} 件を列挙しました。'
    '内部の再帰用関数や private な補助関数は、それらを実際に呼び出す公開 API の提出に対応づけています。'
    '無名ラムダは親関数の一部として扱い、関数ではないマクロ・型エイリアスは件数に含めていません。','',
    ' | '.join(f'{k}: **{v} 件**' for k,v in counts.items()),'',
    '**「AC確認あり」は、リンク先の少なくとも一つの提出で当該関数を実行し、提出全体が AC だったことを表します。**'
    'すべての型・引数・実行経路を数学的に証明するものではありません。未提出の検証も表中に残してあり、AC と区別しています。','',
    '## 今回の追加提出','',
    '| 検証コード | 問題 | オンライン結果 |', '| --- | --- | --- |']
for name,(t,directory) in tasks.items():
    if name.startswith('previous/'):continue
    judge_url=('https://onlinejudge.u-aizu.ac.jp/problems/' if name.startswith('aoj/') else 'https://judge.yosupo.jp/problem/')+t['problem']
    r=results.get(name)
    result=f"[{r['status']}]({r['url']})" if r and 'url' in r else '未提出（AOJ認証待ち）' if name.startswith('aoj/') else '未提出'
    path=(directory/t['source']).relative_to(HERE)
    lines.append(f"| [{name}]({path}) | [{t['problem']}]({judge_url}) | {result} |")

lines+=['','## 関数の一覧','','| ファイル | 関数・オーバーロード | 確認状態 | 提出の証跡 |','| --- | --- | --- | --- |']
for row in rows:
    lines.append(f"| [{row['file']}](../{row['file']}) | `{row['function']}` | {state(row)} | "+'<br>'.join(evidence(t) for t in row['tests'])+' |')

lines+=['','## 検証方法と注意点','',
    '- 元の 32 個の `.cpp` は変更していません。全文または指定した元ファイルの範囲の SHA-256 を manifest に記録しています。',
    '- `geometry.cpp` は、コンパイルできない `circumCircle` を除いた元の本文をコピーし、前方宣言を足して関数別に提出します。'
    '`circumCircle` は別の提出で元の本文のコンパイル結果を確認します。元ファイル全体をそのまま使用した場合の CE は残っています。',
    '- `segtree` の境界探索は Predecessor Problem の出力に使用。`lazy_segtree` は同じ問題に加え、'
    'アフィン更新後に区間長を条件として探索し、受理した区間の和から公式問題の答えを出しています。',
    '- Wavelet Matrix は `kth_largest` で公式の k 番目を出力し、`access` は入力配列と照合。'
    '前後の値・出現回数・範囲内個数は公式入力上の整合性検査と、小区間のソートによる独立検査を行います。'
    '`range_freq` と 2 引数 `rank` には出現回数問題の提出もあります。',
    '- 最小費用流は、最大 7 辺・容量 0〜2 の固定乱数グラフ 150 個について全辺の流量を全探索。'
    '`slope` の全整数流量での費用、折れ点の順序、同傾き区間の併合、流量上限、全 flow/slope オーバーロード、'
    '`edges`・`get_edge` と保存則を同じ提出内で検査しています。AOJ では折れ線から指定流量の費用を補間して出力します。',
    '- `change_edge` は 0 以外の既存フローも設定してから追加フローを計算。Mo の左右別コールバックは小配列の転倒数全探索と比較。'
    '全方位木 DP は根を `n-1` にした提出も追加しています。',
    '- `mint` の演算は ACL との固定乱数比較も実行。`BMBM` は得られた漸化式の逐次計算と比較。'
    'これらの補助検査もリンク先の提出ソースに含まれます。',
    '- 数論の AOJ 提出には負数・非互いに素・解のない CRT・負係数の floor_sum の小規模全探索を含めています。'
    'CRT は拡張 Euclid 問題でも、`x ≡ 0 (mod a), x ≡ gcd(a,b) (mod b)` を解いて出力に使います。',
    '- `template.cpp` の `chmin`・`chmax` は前回 AC。今回は元の `main`・`solve` もそのまま提出したため、'
    '空の `solve()` が出力しないことにより A+B は WA です。これは未実装のテンプレートの振る舞いです。',
    '- 前回の `mincostflow.cpp` は Assignment Problem で TLE。今回の追加提出でも、本体の修正や他の実装への差し替えは行っていません。',
    '- 今回の `mincost_slope_assignment` も Assignment Problem で TLE になりました。'
    '表中でこの提出に対応する関数に TLE と表示されるのは、提出全体の結果です。各補助関数が個別に時間超過したという意味ではありません。'
    'より小さい制約の AOJ GRL_6_B 用コードも用意してあり、認証後に提出できます。',
    '', '## AOJ 提出の再開','',
    'AOJ の結果 URL には認証済みの提出が必要です。まだ認証ファイルがない場合は、手元のターミナルで次を実行してください。'
    'パスワードは保存せず、提出用セッションのみ一時フォルダに保存します。','',
    '```sh','python3 library_checker/setup_aoj_auth.py','python3 library_checker/submit_aoj.py','python3 library_checker/make_function_report.py','```','',
    '再生成順序: `build_full.py` → `build_aoj.py` → `add_slope_submission.py` → `function_inventory.py`。'
    'Library Checker の追加提出は `submit.py library_checker/full` と `submit.py library_checker/full_geometry` で再開できます。','']
(HERE/'FUNCTIONS.md').write_text('\n'.join(lines))
(HERE/'function_status.json').write_text(json.dumps([{**r,'status':state(r),'results':[results[t] for t in r['tests'] if t in results]} for r in rows],ensure_ascii=False,indent=2)+'\n')
print(dict(counts))
print('AOJ submitted:',sum(n.startswith('aoj/') for n in results),'of',sum(n.startswith('aoj/') for n in tasks))
