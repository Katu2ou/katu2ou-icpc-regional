#!/usr/bin/env python3
"""Submit AOJ adapters using the explicitly prepared local session cookie."""
import hashlib
import http.cookiejar
import json
from pathlib import Path
import time
import urllib.request
from urllib.parse import quote
from datetime import datetime,timezone

ROOT=Path(__file__).resolve().parent.parent
HERE=ROOT/'library_checker/aoj'
API='https://onlinejudge.u-aizu.ac.jp/api'
COOKIE=Path('/private/tmp/icpc-library-checker/aoj/session.cookies')
STATUS={0:'CE',1:'WA',2:'TLE',3:'MLE',4:'AC',5:'WJ',6:'OLE',7:'RE',8:'PE',9:'JUDGING',-1:'JNA'}
if not COOKIE.exists():raise SystemExit('AOJ login is required. Run setup_aoj_auth.py in your own terminal.')
jar=http.cookiejar.MozillaCookieJar(str(COOKIE));jar.load(ignore_discard=True,ignore_expires=True)
opener=urllib.request.build_opener(urllib.request.HTTPCookieProcessor(jar))

def request(path,payload=None):
    req=urllib.request.Request(API+path,data=None if payload is None else json.dumps(payload).encode(),headers={'Content-Type':'application/json;charset=UTF-8'})
    with opener.open(req,timeout=45) as response:return json.load(response)

def save(records):
    p=HERE/'results.tmp';p.write_text(json.dumps(records,ensure_ascii=False,indent=2)+'\n');p.replace(HERE/'results.json')

def main():
    user=json.loads((COOKIE.parent/'session_info.json').read_text())['id']
    tasks=json.loads((HERE/'manifest.json').read_text())
    records=json.loads((HERE/'results.json').read_text()) if (HERE/'results.json').exists() else []
    queue=[];pending=[]
    for t in tasks:
        source=(HERE/t['source']).read_bytes();assert hashlib.sha256(source).hexdigest()==t['source_sha256']
        for lib in t['libraries']:
            raw=(ROOT/lib['file']).read_bytes();assert hashlib.sha256(raw).hexdigest()==lib['sha256']
            if 'parts' in lib:
                for p in lib['parts']:
                    fragment=raw[p['start']:p['end']];assert hashlib.sha256(fragment).hexdigest()==p['sha256'];assert fragment in source
            else:assert raw in source
        prev=[r for r in records if r['key']==t['key'] and r['source_sha256']==t['source_sha256']]
        if prev:
            r=prev[-1]
            if 'token' not in r:raise RuntimeError('Earlier POST has uncertain status; inspect before retrying: '+t['key'])
            if r['status'] in ('WJ','JUDGING','SUBMITTING') or not r.get('source_verified'):pending.append(r)
        else:queue.append(t)
    (HERE/'judge').mkdir(exist_ok=True)
    while queue or pending:
        while queue and len(pending)<2:
            t=queue.pop(0);r={**t,'user':user,'status':'SUBMITTING','started_at':datetime.now(timezone.utc).isoformat()};records.append(r);save(records)
            answer=request('/submissions',dict(problemId=t['problem'],language=t['lang'],sourceCode=(HERE/t['source']).read_text()))
            r.update(token=answer['token'],status='WJ');save(records);pending.append(r);print('SUBMITTED',r['key'],flush=True);time.sleep(1)
        for r in pending[:]:
            try:
                candidates=request('/submission_records/users/'+quote(user,safe='')+'/problems/'+r['problem'])
                found=next((v for v in candidates if v.get('token')==r['token']),None)
                if found is None:continue
                rid=found['judgeId'];r.update(id=rid,overview=found,status=STATUS[int(found['status'])])
                r['url']=f"https://onlinejudge.u-aizu.ac.jp/status/users/{quote(user,safe='')}/submissions/1/{r['problem']}/judge/{rid}/{quote(r['lang'],safe='')}"
                save(records)
                if r['status'] in ('WJ','JUDGING'):continue
                verdict=request('/verdicts/'+str(rid));review=request('/reviews/'+str(rid))
                received=review['sourceCode'].encode();assert hashlib.sha256(received).hexdigest()==r['source_sha256']
                r['source_verified']=True;save(records)
                (HERE/'judge'/f'{rid}.json').write_text(json.dumps(dict(overview=found,verdict=verdict,review=review),ensure_ascii=False,indent=2)+'\n')
                print('RESULT',r['key'],r['status'],r['url'],flush=True);pending.remove(r)
            except AssertionError:raise
            except Exception as e:print('POLL_RETRY',r['key'],type(e).__name__,flush=True)
        if pending:time.sleep(5)
    print('ALL_DONE',len(records),flush=True)

if __name__=='__main__':main()
