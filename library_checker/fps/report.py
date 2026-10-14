#!/usr/bin/env python3
"""Summarize saved online verdicts without claiming unjudged coverage."""
import collections
import datetime
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent.parent
tasks=json.loads((HERE/'manifest.json').read_text())
inventory=json.loads((HERE/'function_inventory.json').read_text())
records=json.loads((HERE/'results.json').read_text())
by_key={r['key']:r for r in records}
summaries=[]
lines=['# fps.cpp 全関数の Library Checker 検証','',
       '対象: [fps.cpp](../../fps.cpp) に明示的に定義された14関数（演算子・変換コンストラクタを含む）。',
       '元の fps.cpp と依存する modint_convolution.cpp をそのまま提出コードへ埋め込み、SHA-256で一致を確認しています。','',
       '作成日時: '+datetime.datetime.now().astimezone().isoformat(),'',
       '## 提出結果','',
       '| 提出 | 問題 | 判定 | 最大時間 | ケース判定数 |','| --- | --- | --- | ---: | --- |']
for task in tasks:
    r=by_key[task['key']]
    assert r.get('source_verified'),task['key']
    assert r['source_sha256']==task['source_sha256']
    source=(HERE/task['source']).read_bytes()
    assert hashlib.sha256(source).hexdigest()==task['source_sha256']
    for library in task['libraries']:
        raw=(ROOT/library['file']).read_bytes()
        assert hashlib.sha256(raw).hexdigest()==library['sha256']
        assert raw in source
    result=json.loads((HERE/'judge'/f"{r['id']}.json").read_text())
    counts=collections.Counter(c['status'] for c in result.get('case_results',[]))
    summary={'key':task['key'],'status':r['status'],'url':r['url'],'case_status_counts':dict(counts),'seconds':r['overview']['time']}
    summaries.append(summary)
    count_text=', '.join(f'{k}: {v}' for k,v in counts.items())
    problem=f"[{task['problem']}](https://judge.yosupo.jp/problem/{task['problem']})"
    lines.append(f"| [{task['key']}]({r['url']}) | {problem} | {r['status']} | {r['overview']['time']:.3f} 秒 | {count_text} |")
lines+=['','## 関数ごとの対応','',
        '| 関数 | 検証方法 | 判定・提出 |','| --- | --- | --- |']
function_status=[]
for function in inventory:
    r=by_key[function['primary_submission']]
    method='提出内の独立した係数検査＋公式入力の配列に対する検査' if function['primary_submission']=='fps_helpers' else '対応する公式問題の出力として直接検証'
    lines.append(f"| [`{function['function']}`](../../fps.cpp#L{function['line']}) | {method} | [{r['status']}]({r['url']}) |")
    function_status.append({**function,'status':r['status'],'url':r['url'],'method':method})
oracle=json.loads((HERE/'oracle_results.json').read_text())
lines+=['','## 補助関数と特殊ケースの検証','',
        '`fps_helpers` は Convolution の公式入力を読み、`pre`、加減算、スカラー倍の各演算子、微分、積分、vectorからのコンストラクタを独立した係数計算と照合します。',
        '積分は定数項・長さと、各係数に次数を掛けると元の係数に戻ることを確認します。対象の次数は法より小さいので、この条件で積分係数が一意に決まります。',
        '不一致の場合は `abort()` し、`NDEBUG` に関わらず提出が失敗するようにしています。これらは専用問題の直接判定ではなく、提出プログラム内の検査です。','',
        '同じ提出では全14関数を対象に、固定シードのランダム入力・小配列の全列挙・特殊ケースも検査します。',
        '- 空配列、定数、多項式の長さが異なる場合、全係数0、先頭に0が並ぶ場合。',
        '- 2のべき乗の前後の長さ、係数0・1・法-1、スカラー0・1・法-1。',
        '- inv/log/exp/powのデフォルトの精度指定と、0・1・入力より短い／長い精度指定。',
        '- powの指数0・1・2・5・31・法・法+1・10^18・LLONG_MAX。',
        '- 長さ0〜5の全ての0/1係数列について、指数0・1・2・3・LLONG_MAXを検査。',
        '- invは係数漸化式、logは単純な逆数・微分・積分、expは係数漸化式、powは単純な二乗法と比較。',
        '', 'ローカルの同一検査: `'+oracle['stdout'].strip()+'`、実行時間 '+f"{oracle['seconds']:.3f}"+' 秒。','',
        '## 再現と保存ファイル','',
        '- [全関数の検査コード](checks.hpp)',
        '- [提出コード生成](build.py)',
        '- [提出元・ライブラリのハッシュ](manifest.json)',
        '- [判定・提出URL・ソース照合記録](results.json)',
        '- [関数ごとの機械可読結果](function_status.json)',
        '- [公式サンプルの実行結果](local_results.json)',
        '- [独立計算との比較結果](oracle_results.json)','',
        '```sh','python3 library_checker/fps/build.py',
        'python3 library_checker/download_samples.py library_checker/fps',
        'python3 library_checker/check_samples.py library_checker/fps',
        'python3 library_checker/submit.py library_checker/fps',
        'python3 library_checker/fps/report.py','```','']
(HERE/'REPORT.md').write_text('\n'.join(lines))
(HERE/'function_status.json').write_text(json.dumps(function_status,ensure_ascii=False,indent=2)+'\n')
(HERE/'summary.json').write_text(json.dumps({'function_count':len(inventory),'submissions':summaries},ensure_ascii=False,indent=2)+'\n')
print('Saved',HERE/'REPORT.md')
for s in summaries:print(s)
