#!/usr/bin/env python3
import concurrent.futures
import json
import math
from pathlib import Path
import subprocess
from bs4 import BeautifulSoup

HERE=Path(__file__).resolve().parent/'aoj'
WORK=Path('/private/tmp/icpc-library-checker')
(HERE/'logs').mkdir(exist_ok=True)

def run(t):
    key=t['key'];binary=WORK/'bin'/('aoj__'+key)
    source=HERE/t['source'];log=HERE/'logs'/(key+'.compile.txt')
    code=0
    if not binary.exists() or binary.stat().st_mtime<source.stat().st_mtime:
        c=subprocess.run(['g++','-std=c++17','-O2',str(source),'-o',str(binary)],text=True,capture_output=True)
        log.write_text(c.stderr);code=c.returncode
    r=dict(key=key,compile=code,samples=[])
    if code:print(key,'CE',flush=True);return r
    p=WORK/'aoj/problems'/t['problem']/'description.json'
    soup=BeautifulSoup(json.loads(p.read_text())['html'],'html.parser')
    ins=[];outs=[]
    for h in soup.find_all(['h2','h3']):
        title=h.get_text().lower();pre=h.find_next('pre')
        if pre is None:continue
        if 'sample input and output' in title:continue
        if 'sample input' in title:ins.append(pre.get_text())
        if 'sample output' in title:outs.append(pre.get_text())
    assert len(ins)==len(outs) and ins,t['problem']
    for i,(inp,expected) in enumerate(zip(ins,outs)):
        try:
            p=subprocess.run([str(binary)],input=inp,capture_output=True,text=True,timeout=15)
            a=p.stdout.split();b=expected.split();ok=a==b
            if not ok and len(a)==len(b):
                try:ok=all(math.isfinite(float(x)) and abs(float(x)-float(y))<=1e-6 for x,y in zip(a,b))
                except ValueError:pass
            r['samples'].append(dict(index=i,ok=ok and p.returncode==0,exit_code=p.returncode,input=inp,expected=expected,stdout=p.stdout,stderr=p.stderr))
        except subprocess.TimeoutExpired:r['samples'].append(dict(index=i,ok=False,timeout=True))
    print(key,'SAMPLES_OK' if all(s['ok'] for s in r['samples']) else 'SAMPLE_FAILURE',flush=True)
    return r

tasks=json.loads((HERE/'manifest.json').read_text())
with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:results=list(pool.map(run,tasks))
(HERE/'local_results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
