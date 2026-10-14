#!/usr/bin/env python3
"""Download official statements and examples from the public judge bucket."""
from concurrent.futures import ThreadPoolExecutor
import json
from pathlib import Path
import urllib.request
import sys
try:
    import tomllib
except ImportError:
    import toml as tomllib

HERE = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path(__file__).resolve().parent
WORK = Path('/private/tmp/icpc-library-checker/problems')
API = 'https://v3.api.judge.yosupo.jp'
BUCKET = 'https://storage.googleapis.com/v2-prod-library-checker-data-public'


def get(url):
    with urllib.request.urlopen(url, timeout=60) as response:
        return response.read()


def fetch(problem):
    directory = WORK / problem
    directory.mkdir(parents=True, exist_ok=True)
    if (directory / 'metadata.json').exists() and (directory / 'example_00.in').exists():
        print(problem, 'cached', flush=True)
        return
    metadata = json.loads(get(f'{API}/problems/{problem}'))
    version = metadata['overall_version']
    cases_version = metadata['testcases_version']
    file_base = f'{BUCKET}/v4/files/{problem}/{version}/{problem}'
    for name in ('task.md', 'info.toml'):
        (directory / name).write_bytes(get(f'{file_base}/{name}'))
    info = tomllib.loads((directory / 'info.toml').read_text())
    count = next(test['number'] for test in info['tests'] if test['name'] == 'example.in')
    for i in range(count):
        for extension in ('in', 'out'):
            name = f'example_{i:02d}.{extension}'
            data = get(f'{BUCKET}/v4/examples/{problem}/{cases_version}/{extension}/{name}')
            (directory / name).write_bytes(data)
    (directory / 'metadata.json').write_text(json.dumps(metadata, ensure_ascii=False, indent=2) + '\n')
    print(problem, count, 'examples', flush=True)


if __name__ == '__main__':
    tasks = json.loads((HERE / 'manifest.json').read_text())
    problems = sorted({task['problem'] for task in tasks})
    with ThreadPoolExecutor(max_workers=3) as pool:
        list(pool.map(fetch, problems))
