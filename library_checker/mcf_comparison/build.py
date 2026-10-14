#!/usr/bin/env python3
"""Build isolated MCF comparisons without changing the paper library."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
original = (ROOT / 'mincostflow.cpp').read_text()
upstream = HERE / 'upstream/ac-library/atcoder'
acl = (upstream / 'mincostflow.hpp').read_text()
for header in ['internal_csr', 'internal_queue']:
    acl = acl.replace(f'#include "atcoder/{header}"', (upstream / (header + '.hpp')).read_text())
acl += '\nusing atcoder::mcf_graph;\n'

early = original.replace('vector<int>pv(N),pe(N);', 'vector<int>pv(N),pe(N);\n        vector<char>done(N);')
early = early.replace('d[s]=0;', 'd[s]=0;\n            fill(done.begin(),done.end(),false);')
early = early.replace('if(dv!=d[v])continue;', 'if(dv!=d[v])continue;\n                done[v]=true;\n                if(v==t)break;')
early = early.replace('if(d[v]<INF)h[v]+=d[v];', 'if(done[v])h[v]+=d[v]-d[t];')
(HERE / 'early_exit_mincostflow.cpp').write_text(early)

main_global = (HERE.parent / 'submissions/mincostflow.cc').read_text().split('int main() {', 1)[1]
main_global = 'int main() {' + main_global
main_row = r'''
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<ll>> a(n,vector<ll>(n));
    vector<ll> low(n); ll offset=0;
    for(int i=0;i<n;i++) {
        for(auto &x:a[i]) cin >> x;
        low[i]=*min_element(a[i].begin(),a[i].end()); offset+=low[i];
    }
    int s=2*n,t=s+1; mcf_graph<int,ll> g(t+1);
    for(int i=0;i<n;i++) { g.add_edge(s,i,1,0); g.add_edge(n+i,t,1,0); }
    vector<vector<int>> ids(n,vector<int>(n));
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) ids[i][j]=g.add_edge(i,n+j,1,a[i][j]-low[i]);
    auto curve=g.slope(s,t,n); assert(curve.back().first==n);
    cout << curve.back().second+offset << '\n';
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(g.get_edge(ids[i][j]).flow) cout << j << ' ';
    cout << '\n';
}
'''
prefix = '#include <bits/stdc++.h>\nusing namespace std;\nusing ll=long long;\n'
variants = {
    'original_global': (original, main_global),
    'original_row': (original, main_row),
    'acl_global': (acl, main_global),
    'acl_row': (acl, main_row),
    'early_exit_row': (early, main_row),
}
(HERE / 'submissions').mkdir(exist_ok=True)
(HERE / 'checks').mkdir(exist_ok=True)
tasks = []
for key, (lib, main) in variants.items():
    source = prefix + lib + '\n' + main
    path = HERE / 'submissions' / (key + '.cc')
    path.write_text(source)
    if key.startswith('original_'):
        continue  # Already have original-library online verdicts.
    tasks.append({
        'key': key, 'problem': 'assignment', 'lang': 'cpp17',
        'source': 'submissions/' + key + '.cc',
        'source_sha256': hashlib.sha256(source.encode()).hexdigest(),
        'libraries': [],
        'note': 'Isolated comparison; original mincostflow.cpp is unchanged. See upstream.json and build.py for provenance.'
    })
(HERE / 'manifest.json').write_text(json.dumps(tasks,indent=2)+'\n')

oracle_src = (HERE.parent / 'aoj/submissions/mincost_slope.cc').read_text()
oracle = oracle_src[oracle_src.index('struct InputEdge'):oracle_src.rindex('int main()')]
for key, lib in [('original',original),('acl',acl),('early_exit',early)]:
    (HERE / 'checks' / (key+'.cc')).write_text(prefix+lib+'\n'+oracle+'\nint main(){flow_checks();}\n')
print('Built', ', '.join(variants))
