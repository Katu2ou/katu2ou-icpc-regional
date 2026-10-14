#!/usr/bin/env python3
"""Submit the exact saved FPS and modint code, with independent test adapters."""
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent.parent
sha=lambda b:hashlib.sha256(b).hexdigest()
libraries=[]
prefix='#include <bits/stdc++.h>\nusing namespace std;\nusing ll=long long;\n'
for name in ['modint_convolution.cpp','fps.cpp']:
    raw=(ROOT/name).read_bytes()
    assert raw,name+' is empty'
    libraries.append({'file':name,'sha256':sha(raw)})
    prefix+='\n// BEGIN UNCHANGED '+name+'\n'+raw.decode()+'\n// END UNCHANGED '+name+'\n'

io=r'''
fps read_fps(int n){vector<mint>a(n);for(auto&x:a){long long y;cin>>y;x=mint(y);}return fps(move(a));}
void print_fps(const fps&a){for(int i=0;i<(int)a.size();i++)cout<<a[i].val()<<(i+1==(int)a.size()?'\n':' ');if(a.empty())cout<<'\n';}
'''
functions=[
    ('fps(vector<mint>)',3,'fps_helpers'),('pre(int)',5,'fps_helpers'),
    ('operator+(fps,const fps&)',9,'fps_helpers'),('operator-(fps,const fps&)',14,'fps_helpers'),
    ('operator*(const fps&,const fps&)',19,'fps_multiply'),('operator*=(mint)',23,'fps_helpers'),
    ('operator*(fps,mint)',27,'fps_helpers'),('operator*(mint,fps)',28,'fps_helpers'),
    ('diff()',30,'fps_helpers'),('integral()',36,'fps_helpers'),
    ('inv(int=-1)',48,'fps_inv'),('log(int=-1)',61,'fps_log'),
    ('exp(int=-1)',70,'fps_exp'),('pow(long long,int=-1)',85,'fps_pow'),
]
tasks=[]
(HERE/'submissions').mkdir(parents=True,exist_ok=True)
def emit(key,problem,body,checks='',note=''):
    source=prefix+'\n'+checks+'\n'+io+'\nint main(){ios::sync_with_stdio(false);cin.tie(nullptr);\n'+body+'\n}\n'
    path=HERE/'submissions'/(key+'.cc');path.write_text(source)
    tasks.append({'key':key,'problem':problem,'lang':'cpp17','libraries':libraries,
                  'source':'submissions/'+key+'.cc','source_sha256':sha(source.encode()),
                  'functions':[name for name,_,task in functions if task==key], 'note':note})
emit('fps_multiply','convolution_mod','int n,m;cin>>n>>m;fps a=read_fps(n),b=read_fps(m);print_fps(a*b);')
for name in ['inv','log','exp']:
    emit('fps_'+name,name+'_of_formal_power_series',f'int n;cin>>n;fps a=read_fps(n);print_fps(a.{name}());')
emit('fps_pow','pow_of_formal_power_series','int n;long long k;cin>>n>>k;fps a=read_fps(n);print_fps(a.pow(k));')
emit('fps_helpers','convolution_mod',
     'fps_verification::small_checks();int n,m;cin>>n>>m;fps a=read_fps(n),b=read_fps(m);fps_verification::helpers(a,b);fps_verification::helpers(b,a);print_fps(a*b);',
     (HERE/'checks.hpp').read_text(),
     'Every helper is compared against an independent coefficient oracle on official input arrays; fixed random and exhaustive small inputs additionally exercise all 14 functions, explicit/default precision, empty inputs, and large exponents. The checks abort on failure even with NDEBUG.')
(HERE/'manifest.json').write_text(json.dumps(tasks,ensure_ascii=False,indent=2)+'\n')
(HERE/'function_inventory.json').write_text(json.dumps([{'function':f,'line':line,'primary_submission':task}for f,line,task in functions],ensure_ascii=False,indent=2)+'\n')
(HERE/'check_local.cc').write_text(prefix+'\n'+(HERE/'checks.hpp').read_text()+'\nint main(){fps_verification::small_checks();cout<<"PASS "<<fps_verification::checks<<" checks\\n";}\n')
print('Built',len(tasks),'submissions for',len(functions),'functions; libraries:',libraries)
