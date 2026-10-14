#!/usr/bin/env python3
"""Compile adapters and check official samples before online submission."""
import concurrent.futures
import json
from pathlib import Path
import subprocess
import sys

HERE=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else Path(__file__).resolve().parent
WORK=Path('/private/tmp/icpc-library-checker')
(WORK/'bin').mkdir(exist_ok=True)
(HERE/'logs').mkdir(exist_ok=True)


def matrix_rank(a, mod):
    a=[r[:] for r in a]
    if not a:return 0
    rank=0
    for j in range(len(a[0])):
        p=next((i for i in range(rank,len(a)) if a[i][j]%mod),None)
        if p is None:continue
        a[p],a[rank]=a[rank],a[p]
        inv=pow(a[rank][j]%mod,-1,mod)
        a[rank]=[x*inv%mod for x in a[rank]]
        for i in range(rank+1,len(a)):
            k=a[i][j]%mod
            a[i]=[(x-k*y)%mod for x,y in zip(a[i],a[rank])]
        rank+=1
        if rank==len(a):break
    return rank


def check(problem, inp, out, expected):
    if out.split()==expected.split():return True
    try:
        x=inp.split();z=out.split();e=expected.split()
        if problem=='find_linear_recurrence':
            n=int(x[0]);a=list(map(int,x[1:]));d=int(z[0]);c=list(map(int,z[1:]))
            return d==int(e[0]) and len(c)==d and all((a[i]-sum(c[j]*a[i-j-1] for j in range(d)))%998244353==0 for i in range(d,n))
        if problem=='assignment':
            n=int(x[0]);a=list(map(int,x[1:]));p=list(map(int,z[1:]));cost=int(z[0])
            return cost==int(e[0]) and sorted(p)==list(range(n)) and sum(a[i*n+p[i]] for i in range(n))==cost
        if problem=='bipartitematching':
            l,r,m=map(int,x[:3]);edges=set(zip(map(int,x[3::2]),map(int,x[4::2])))
            k=int(z[0]);pairs=list(zip(map(int,z[1::2]),map(int,z[2::2])))
            return k==int(e[0]) and len(pairs)==k and len(z)==1+2*k and all(p in edges for p in pairs) and len({u for u,v in pairs})==k and len({v for u,v in pairs})==k
        if problem in ('system_of_linear_equations','system_of_linear_equations_mod_2'):
            n,m=map(int,x[:2]);d=int(z[0]);mod=2 if problem.endswith('mod_2') else 998244353
            if d!=int(e[0]):return False
            if d==-1:return len(z)==1
            if mod==2:
                a=[list(map(int,s)) for s in x[2:2+n]];b=list(map(int,x[2+n]));sol=[list(map(int,s)) for s in z[1:]]
            else:
                vals=list(map(int,x[2:]));a=[vals[i*m:(i+1)*m] for i in range(n)];b=vals[n*m:]
                vals=list(map(int,z[1:]));sol=[vals[i*m:(i+1)*m] for i in range(d+1)]
                if len(vals)!=(d+1)*m:return False
            if len(sol)!=d+1 or any(len(v)!=m for v in sol):return False
            if any(sum(v*w for v,w in zip(row,sol[0]))%mod!=b[i] for i,row in enumerate(a)):return False
            if any(sum(v*w for v,w in zip(row,s))%mod for s in sol[1:] for row in a):return False
            return matrix_rank(sol[1:],mod)==d
        if problem=='two_sat':
            if z[:2]!=e[:2]:return False
            if z[1]=='UNSATISFIABLE':return True
            n,m=map(int,x[2:4]);vals=list(map(int,z[3:-1]));assignment={abs(v):v>0 for v in vals}
            return len(assignment)==n and all(assignment[abs(int(x[4+3*i]))]==(int(x[4+3*i])>0) or assignment[abs(int(x[5+3*i]))]==(int(x[5+3*i])>0) for i in range(m))
        if problem=='scc':
            def parse(vals):
                vals=list(map(int,vals));count=vals[0];i=1;groups=[]
                for _ in range(count):
                    k=vals[i];groups.append(vals[i+1:i+1+k]);i+=k+1
                assert i==len(vals)
                return groups
            g=parse(z);ref=parse(e)
            if {tuple(sorted(c)) for c in g}!={tuple(sorted(c)) for c in ref}:return False
            ids={v:i for i,c in enumerate(g) for v in c};edges=list(map(int,x[2:]))
            return all(ids[u]<=ids[v] for u,v in zip(edges[::2],edges[1::2]))
    except (ValueError,IndexError,KeyError,AssertionError):
        return False
    return False


def run(task):
    key=task['key'];binary=WORK/'bin'/(HERE.name+'__'+key)
    c=subprocess.run(['g++','-std=c++17','-O2','-I/opt/homebrew/include/ac-library',str(HERE/task['source']),'-o',str(binary)],capture_output=True,text=True)
    (HERE/'logs'/f'{key}.compile.txt').write_text(c.stderr)
    result={'key':key,'compile':c.returncode,'samples':[]}
    if c.returncode:
        print(key,'COMPILE_ERROR',flush=True);return result
    directory=WORK/'problems'/task['problem']
    for inp in sorted(directory.glob('example_*.in')):
        expected=inp.with_suffix('.out').read_text()
        try:
            p=subprocess.run([str(binary)],input=inp.read_text(),capture_output=True,text=True,timeout=15)
            ok=p.returncode==0 and check(task['problem'],inp.read_text(),p.stdout,expected)
            result['samples'].append({'name':inp.name,'ok':ok,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
        except subprocess.TimeoutExpired:
            result['samples'].append({'name':inp.name,'ok':False,'timeout':True})
    print(key, 'SAMPLES_OK' if result['samples'] and all(s['ok'] for s in result['samples']) else 'SAMPLE_FAILURE',flush=True)
    return result


if __name__=='__main__':
    tasks=json.loads((HERE/'manifest.json').read_text())
    if len(sys.argv)>2:
        keys=set(sys.argv[2:]);tasks=[t for t in tasks if t['key'] in keys]
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
        results=list(pool.map(run,tasks))
    if len(sys.argv)>2 and (HERE/'local_results.json').exists():
        previous=json.loads((HERE/'local_results.json').read_text())
        results=[r for r in previous if r['key'] not in keys]+results
    (HERE/'local_results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
    print('Checked',len(results),'adapters')
