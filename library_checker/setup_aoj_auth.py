#!/usr/bin/env python3
"""Run in your terminal: sign in to AOJ without storing your password."""
import getpass
import http.cookiejar
import json
import os
from pathlib import Path
import urllib.request
import urllib.error

API='https://onlinejudge.u-aizu.ac.jp/api'
target=Path('/private/tmp/icpc-library-checker/aoj/session.cookies')
target.parent.mkdir(parents=True,exist_ok=True)
jar=http.cookiejar.MozillaCookieJar(str(target))
opener=urllib.request.build_opener(urllib.request.HTTPCookieProcessor(jar))
user=input('AOJ User ID: ').strip()
password=getpass.getpass('AOJ Password (not displayed or saved): ')
request=urllib.request.Request(API+'/session',data=json.dumps({'id':user,'password':password}).encode(),headers={'Content-Type':'application/json;charset=UTF-8'})
try:
    with opener.open(request,timeout=30) as response:
        result=json.load(response)
except urllib.error.HTTPError as error:
    raise SystemExit(f'Login failed: HTTP {error.code}. No credentials were saved.')
if not list(jar):
    raise SystemExit('Login did not return a session cookie; nothing saved.')
fd=os.open(target,os.O_WRONLY|os.O_CREAT|os.O_TRUNC,0o600);os.close(fd)
os.chmod(target,0o600)
jar.save(ignore_discard=True,ignore_expires=True)
(target.parent/'session_info.json').write_text(json.dumps({'id':result['id']}))
print('AOJ login completed. Session saved locally for the requested submissions.')
print('Return to the chat and say: AOJログイン完了')
