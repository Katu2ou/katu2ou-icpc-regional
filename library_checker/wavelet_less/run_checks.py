#!/usr/bin/env python3
"""Reproducible less() differential tests and separate invalid-LOG probes."""
import argparse
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
from zoneinfo import ZoneInfo

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent.parent
WORK=Path('/private/tmp/icpc-wavelet-less')
SOURCE=HERE/'check_less.cc'
LIBRARY=ROOT/'wavelet_matrix.cpp'
SEEDS=[0,1,0xDEADBEEF,0x123456789ABCDEF0,0xFFFFFFFFFFFFFFFF,20260921]
COMMANDS={
    'gcc':['g++','-std=c++17','-O2','-Wall','-Wextra','-Wconversion',str(SOURCE),'-o',str(WORK/'check_gcc')],
    'sanitizer':['clang++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',str(SOURCE),'-o',str(WORK/'check_sanitized')],
}
BINARIES={'gcc':WORK/'check_gcc','sanitizer':WORK/'check_sanitized'}


def build(name):
    with (HERE/(name+'_compile.log')).open('w') as log:
        p=subprocess.run(COMMANDS[name],stdout=log,stderr=subprocess.STDOUT)
    if p.returncode:raise RuntimeError(name+' compilation failed; inspect compile log')
    print(name,'COMPILED',flush=True)


def run(name):
    env=os.environ.copy()
    env['ASAN_OPTIONS']='detect_leaks=0'
    env['UBSAN_OPTIONS']='halt_on_error=1:print_stacktrace=1'
    start=time.monotonic()
    with (HERE/(name+'.jsonl')).open('w') as output,(HERE/(name+'.stderr.log')).open('w') as error:
        p=subprocess.Popen([str(BINARIES[name])],stdout=subprocess.PIPE,stderr=error,text=True,env=env)
        records=[]
        for line in p.stdout:
            output.write(line);output.flush();records.append(json.loads(line))
            if len(records)%30==0:print(name,len(records),'completed batches',flush=True)
        code=p.wait()
    result=dict(compiler=name,returncode=code,wall_seconds=time.monotonic()-start,records=records)
    (HERE/(name+'_result.json')).write_text(json.dumps(result,indent=2)+'\n')
    if code or not records or records[-1].get('status')!='PASS':raise RuntimeError(name+' test failed; inspect logs')
    print(name,'PASS',records[-1],flush=True)
    return result


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--skip-build',action='store_true');args=parser.parse_args()
    WORK.mkdir(parents=True,exist_ok=True)
    digest=hashlib.sha256(LIBRARY.read_bytes()).hexdigest()
    if not args.skip_build:
        with ThreadPoolExecutor(max_workers=2) as pool:list(pool.map(build,COMMANDS))
    with ThreadPoolExecutor(max_workers=2) as pool:results=list(pool.map(run,COMMANDS))
    a,b=(r['records'] for r in results)
    assert a[:-1]==b[:-1], 'Compilers did not run identical deterministic cases'
    assert all(a[-1][key]==b[-1][key] for key in ('arrays','queries','oracle_elements'))
    assert hashlib.sha256(LIBRARY.read_bytes()).hexdigest()==digest,'Original library changed during testing'
    insufficient=subprocess.run([str(BINARIES['gcc']),'--insufficient-log'],capture_output=True,text=True,check=True)
    env=os.environ.copy();env['ASAN_OPTIONS']='detect_leaks=0';env['UBSAN_OPTIONS']='halt_on_error=1:print_stacktrace=1'
    oversized=subprocess.run([str(BINARIES['sanitizer']),'--oversized-log'],capture_output=True,text=True,env=env)
    (HERE/'oversized_log.stderr.log').write_text(oversized.stderr)
    probes={'insufficient_log':json.loads(insufficient.stdout),'oversized_log':{'returncode':oversized.returncode,'stderr':oversized.stderr,'stdout':oversized.stdout}}
    assert probes['insufficient_log']['actual']==1
    assert oversized.returncode!=0 and 'shift exponent 64' in oversized.stderr
    (HERE/'precondition_probes.json').write_text(json.dumps(probes,ensure_ascii=False,indent=2)+'\n')
    per_phase=defaultdict(lambda:dict(arrays=0,queries=0,oracle_elements=0))
    for record in a[:-1]:
        for key in per_phase[record['phase']]:per_phase[record['phase']][key]+=record[key]
    versions={name:subprocess.check_output([cmd[0],'--version'],text=True).splitlines()[0] for name,cmd in COMMANDS.items()}
    metadata={'date':datetime.now(ZoneInfo('Asia/Tokyo')).isoformat(timespec='seconds'),'library_sha256':digest,'test_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),'seeds':SEEDS,'versions':versions,'commands':COMMANDS,'per_phase':dict(per_phase),'total':a[-1],'configuration_count':sum(r['phase']=='random' for r in a),'valid_domain':'Nonnegative A[i], A[i] < 2**LOG, 0 <= LOG <= unsigned type width, 0 <= l <= r <= n, threshold representable by T, n fits int','compilers':{r['compiler']:dict(returncode=r['returncode'],wall_seconds=r['wall_seconds']) for r in results}}
    (HERE/'summary.json').write_text(json.dumps(metadata,ensure_ascii=False,indent=2)+'\n')
    lines=['# WaveletMatrix::less 検証結果','',f"実行日時: {metadata['date']}",'',
        f"対象: [wavelet_matrix.cpp](../../wavelet_matrix.cpp)（SHA-256: `{digest}`）",'',
        f"**有効な入力条件では、{a[-1]['arrays']:,} 配列・{a[-1]['queries']:,} 問い合わせが、単純走査による結果とすべて一致しました。**",
        '同じテストを GCC と、Clang の AddressSanitizer / UndefinedBehaviorSanitizer 有効ビルドで実行しました。'
        '両方とも成功し、有効な入力に対するメモリ違反・未定義動作の検出はありません。元のライブラリは変更していません。','',
        '## 前提条件','',
        '- 構築配列の値は非負で、`A[i] < 2^LOG`。',
        '- `0 <= LOG <= make_unsigned_t<T> のビット数`。`LOG=0` は全要素 0 の場合を追加確認。',
        '- `0 <= l <= r <= n`、配列長は `INT_MAX` 未満（`n+1` の符号付き整数オーバーフローを避けるため）。',
        '- しきい値 `x` は引数型 `T` で表現可能。符号付き型の負の `x` は正常に 0 を返す。','',
        '## テスト件数','',
        '| 分類 | 配列数 | 問い合わせ数 |', '| --- | ---: | ---: |']
    labels={'exhaustive':'小配列の全列挙','special':'特殊ケース','random':'固定シードのランダム比較','large':'大規模配列'}
    for phase,counts in per_phase.items():lines.append(f"| {labels[phase]} | {counts['arrays']:,} | {counts['queries']:,} |")
    lines+=['',f"件数は 1 ビルド分です。2 ビルドで同一のケースを確認しています。計 {metadata['configuration_count']} 通りの型・LOG の組み合わせを使用しました。",'',
        '- 型: 符号付き／符号なしの 8・16・32・64 ビット整数。',
        '- 全列挙: 2 値配列は長さ 8、4 値配列は長さ 6、8 値配列は長さ 4 まで。すべての半開区間と、値域内外のしきい値を確認。',
        '- 特殊ケース: 空配列、空区間、全体区間、先頭・末尾の 1 要素、全要素 0、全要素同値、最大値、交互配列、1 個だけ異なる値、重複の連続、昇順・降順。',
        '- ビット境界: `2^k-1`・`2^k`・`2^k+1`、`2^LOG`、型の最小値・最大値、64 ビット最上位ビット。',
        '- ランダム: 各設定につき 6 シード × 200 配列 × 96 問い合わせ。値の偏り・ソート済み・重複・外れ値も混ぜています。',
        '- 大規模: 長さ 200,000 の signed 32 ビット配列、長さ 131,073 の unsigned 64 ビット配列。','',
        '`seeds = '+repr(SEEDS)+'`','',
        '## アルゴリズムの確認','',
        '各段の `[l,r)` は「すでに見た上位ビットが x と一致する候補」を表します。'
        '`s[h][r]-s[h][l]` がその範囲の 1 の個数なので、`z=(r-l)-(R-L)` は 0 の個数です。','',
        'x の現在ビットが 1 なら、0 側の z 個はこのビットで厳密に小さいことが確定するため答えへ加え、1 側へ移動します。'
        'x の現在ビットが 0 なら、0 側だけへ移動します。構築時の安定分割により、移動先の区間はそれぞれ'
        '`[mid[h]+L, mid[h]+R)` と `[l-L, r-R)` になります。','',
        '加算する集合は各段で互いに重ならず、全ビットを処理して残る x と等しい値は加算しません。'
        'したがって「x 以下」ではなく「x 未満」の個数になります。非負配列に対する x<=0 の早期 return、'
        'および十分な LOG を指定した配列に対する x>=2^LOG の早期 return も正しい処理です。','',
        '## 前提違反に対するチェック不足','',
        '通常の入力条件での less の誤答は見つかりませんでした。一方、LOG の指定ミスを防ぐ検査は実装されていません。'
        '次の 2 点は実際に再現しています。','',
        '1. **LOG 不足で誤答:** `WaveletMatrix<int,2>({4}).less(0,1,1)` は `1` を返します。'
        '元の整数で数えると `4 < 1` は偽なので `0` です。値 4 には LOG>=3 が必要ですが、コンストラクタが上位ビットの欠落を検出しません。',
        '2. **LOG が型幅を超えると未定義動作:** `WaveletMatrix<uint64_t,65>({1})` の構築時に、'
        'UBSan が 64 ビット型の 64 ビットシフトを検出しました。このプローブは通常の有効入力テストとは別プロセスで実行しています。','',
        'これらは入力・テンプレート引数の前提に関する問題です。検査の追加やライブラリ本体の修正は行っていません。','',
        '## 保存ファイルと再実行','',
        '- [テストコード](check_less.cc)',
        '- [集計・シード・コンパイラ情報](summary.json)',
        '- [GCC 実行ログ](gcc.jsonl) / [Sanitizer 実行ログ](sanitizer.jsonl)',
        '- [前提違反の再現結果](precondition_probes.json) / [UBSan のエラー](oversized_log.stderr.log)','',
        '```sh','python3 library_checker/wavelet_less/run_checks.py','```','',
        '検証は有限の入力集合に対する結果です。上記の不変条件の確認と合わせて判断しています。','']
    (HERE/'REPORT.md').write_text('\n'.join(lines))
    print('WROTE',HERE/'REPORT.md',flush=True)

if __name__=='__main__':main()
