#!/usr/bin/env python3
"""Submit unchanged library adapters anonymously; save IDs before polling.

Re-running resumes known submissions. An interrupted POST with no saved ID is
never automatically retried, because the server may already have accepted it.
"""
import hashlib
import json
from pathlib import Path
import time
from datetime import datetime, timezone
import urllib.request
import sys

ROOT = Path(__file__).resolve().parent.parent
HERE = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path(__file__).resolve().parent
API = 'https://v3.api.judge.yosupo.jp'
RESULTS = HERE / 'results.json'
FINAL = {'AC', 'WA', 'RE', 'TLE', 'MLE', 'CE', 'OLE', 'IE'}


def now():
    return datetime.now(timezone.utc).isoformat()


def save(records):
    temp = RESULTS.with_suffix('.tmp')
    temp.write_text(json.dumps(records, ensure_ascii=False, indent=2) + '\n')
    temp.replace(RESULTS)


def request(path, payload=None):
    data = None if payload is None else json.dumps(payload).encode()
    req = urllib.request.Request(API + path, data=data, headers={
        'Content-Type': 'application/json',
        'User-Agent': 'library-checker-library-verification/1.0',
    })
    with urllib.request.urlopen(req, timeout=60) as response:
        return json.load(response)


def main():
    tasks = json.loads((HERE / 'manifest.json').read_text())
    records = json.loads(RESULTS.read_text()) if RESULTS.exists() else []
    pending = []
    queue = []
    for task in tasks:
        source = (HERE / task['source']).read_bytes()
        assert hashlib.sha256(source).hexdigest() == task['source_sha256']
        for lib in task['libraries']:
            raw = (ROOT / lib['file']).read_bytes()
            assert hashlib.sha256(raw).hexdigest() == lib['sha256'], lib['file']
            if 'parts' in lib:
                for part in lib['parts']:
                    fragment=raw[part['start']:part['end']]
                    assert hashlib.sha256(fragment).hexdigest()==part['sha256']
                    assert fragment in source
            else:
                assert raw in source, lib['file']
        previous = [r for r in records if r['key'] == task['key'] and
                    r['source_sha256'] == task['source_sha256']]
        if previous:
            record = previous[-1]
            if 'id' not in record:
                raise RuntimeError(f"Uncertain earlier POST: {task['key']}; inspect before retry")
            if record.get('status') not in FINAL:
                pending.append(record)
        else:
            queue.append(task)

    (HERE / 'judge').mkdir(exist_ok=True)
    while queue or pending:
        while queue and len(pending) < 2:
            task = queue.pop(0)
            record = {**task, 'started_at': now(), 'status': 'SUBMITTING', 'anonymous': True}
            records.append(record)
            save(records)
            try:
                answer = request('/submit', {
                    'problem': task['problem'], 'lang': task['lang'],
                    'source': (HERE / task['source']).read_text(), 'tle_knockout': True,
                })
                record['id'] = answer['id']
            except Exception as error:
                record['status'] = 'SUBMISSION_UNKNOWN'
                record['error'] = str(error)
                save(records)
                raise
            record.update(status='WJ', url=f"https://judge.yosupo.jp/submission/{record['id']}")
            save(records)
            pending.append(record)
            print('SUBMITTED', task['key'], record['url'], flush=True)
            time.sleep(1)
        for record in pending[:]:
            try:
                result = request(f"/submissions/{record['id']}")
            except Exception as error:
                print('POLL_RETRY', record['id'], str(error), flush=True)
                continue
            received = result['source'].encode()
            if hashlib.sha256(received).hexdigest() != record['source_sha256']:
                raise RuntimeError(f"Source mismatch for {record['id']}")
            record['source_verified'] = True
            record['overview'] = result['overview']
            record['status'] = result['overview']['status']
            record['checked_at'] = now()
            (HERE / 'judge' / f"{record['id']}.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
            save(records)
            if record['status'] in FINAL:
                print('RESULT', record['key'], record['status'], record['url'], flush=True)
                pending.remove(record)
        if pending:
            time.sleep(8)
    print('ALL_DONE', len(records), flush=True)


if __name__ == '__main__':
    main()
