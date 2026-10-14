#!/usr/bin/env python3
"""Sequential local timings on matrices from the pinned official generators."""
import hashlib
import json
from pathlib import Path
import subprocess
import time

HERE = Path(__file__).resolve().parent
WORK = Path('/private/tmp/icpc-mcf-comparison')
(WORK / 'inputs').mkdir(parents=True, exist_ok=True)
n = 500
cases = [
    ('hand_minus_00', lambda i,j: -(i*(i+1)+j*(j+1)), -2*(n-1)*n*(n+1)//3),
    ('multiplication_table_00', lambda i,j: (i+1)*(j+1), n*(n+1)*(n+2)//6),
]
records = []
for name, value, expected in cases:
    matrix = [[value(i,j) for j in range(n)] for i in range(n)]
    data = (str(n)+'\n'+'\n'.join(' '.join(map(str,row)) for row in matrix)+'\n').encode()
    (WORK / 'inputs' / (name+'.in')).write_bytes(data)
    for key in ['original_global','acl_global','original_row','early_exit_row','acl_row']:
        start = time.monotonic()
        record = {'case':name,'variant':key,'n':n,'input_sha256':hashlib.sha256(data).hexdigest()}
        try:
            result = subprocess.run([str(WORK/'bin'/('submissions_'+key))],input=data,capture_output=True,timeout=45)
            record['seconds']=time.monotonic()-start
            assert result.returncode==0, result.stderr.decode()
            numbers=list(map(int,result.stdout.split()))
            cost, permutation=numbers[0],numbers[1:]
            assert len(permutation)==n and sorted(permutation)==list(range(n))
            assert cost==sum(matrix[i][permutation[i]] for i in range(n))==expected
            record.update(status='PASS',cost=cost)
        except subprocess.TimeoutExpired:
            record.update(status='TIMEOUT',seconds=45)
        records.append(record)
        (HERE/'benchmark_results.json').write_text(json.dumps(records,indent=2)+'\n')
        print(record,flush=True)
