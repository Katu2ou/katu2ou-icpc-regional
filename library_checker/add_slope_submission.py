#!/usr/bin/env python3
"""Use the full original min-cost-flow and independent slope oracle on LC."""
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
tasks=json.loads((HERE/'full/manifest.json').read_text())
aoj=json.loads((HERE/'aoj/manifest.json').read_text())
base=next(t for t in aoj if t['key']=='mincost_slope')
source=(HERE/'aoj'/base['source']).read_text()
source=source[:source.rfind('int main()')]+r'''
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);flow_checks();
int n;cin>>n;vector<vector<ll>>a(n,vector<ll>(n));vector<ll>low(n);ll offset=0;
for(int i=0;i<n;i++){for(auto&x:a[i])cin>>x;low[i]=*min_element(a[i].begin(),a[i].end());offset+=low[i];}
int s=2*n,t=s+1;mcf_graph<int,ll>g(t+1);for(int i=0;i<n;i++){g.add_edge(s,i,1,0);g.add_edge(n+i,t,1,0);}
vector<vector<int>>ids(n,vector<int>(n));for(int i=0;i<n;i++)for(int j=0;j<n;j++)ids[i][j]=g.add_edge(i,n+j,1,a[i][j]-low[i]);
auto curve=g.slope(s,t);assert(curve.back().first==n);cout<<at_flow(curve,n)+offset<<'\n';
for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(g.get_edge(ids[i][j]).flow)cout<<j<<' ';cout<<'\n';
}
'''
key='mincost_slope_assignment';path=HERE/'full/submissions'/(key+'.cc');path.write_text(source)
task={**base,'key':key,'problem':'assignment','judge':'library_checker','lang':'cpp17','source':'submissions/'+key+'.cc','source_sha256':hashlib.sha256(source.encode()).hexdigest(),'note':'Each row minimum is subtracted so initial costs are nonnegative. The original slope implementation supplies the judged assignment; an exhaustive small-graph oracle checks every curve segment and all flow/slope overloads.'}
tasks=[t for t in tasks if t['key']!=key]+[task]
(HERE/'full/manifest.json').write_text(json.dumps(tasks,ensure_ascii=False,indent=2)+'\n')
print('Added',key)
