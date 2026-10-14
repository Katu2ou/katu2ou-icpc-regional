#!/usr/bin/env python3
"""Additional function-level verification; original algorithm bodies are unchanged."""
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parent.parent
OUT=ROOT/'library_checker/full'
TASKS=[]

def add(key,problem,libraries,body,extra='',before='',functions=None,note=''):
    text='#include <bits/stdc++.h>\nusing namespace std;using ll=long long;\n'+before+'\n'
    sources=[]
    for name in libraries:
        raw=(ROOT/name).read_bytes();digest=hashlib.sha256(raw).hexdigest()
        text+=f'\n// BEGIN ORIGINAL {name} SHA256 {digest}\n'+raw.decode()+f'\n// END ORIGINAL {name}\n'
        sources.append({'file':name,'sha256':digest})
    text+='\n'+extra+'\nint main(){ios::sync_with_stdio(false);cin.tie(nullptr);\n'+body+'\n}\n'
    target=OUT/'submissions'/(key+'.cc');target.parent.mkdir(parents=True,exist_ok=True);target.write_text(text)
    TASKS.append(dict(key=key,problem=problem,libraries=sources,source=str(target.relative_to(OUT)),lang='cpp17',source_sha256=hashlib.sha256(text.encode()).hexdigest(),functions=functions or [],note=note))

add('wavelet_all','range_kth_smallest',['wavelet_matrix.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;WaveletMatrix<int,30>w(a);
for(int i=0;i<n;i++)assert(w.access(i)==a[i]);
for(int i=0;i<q;i++){int l,r,k;cin>>l>>r>>k;int z=w.kth_largest(l,r,r-l-1-k);
assert(w.kth(l,r,k)==z);assert(w.less(l,r,z)<=k&&k<w.less(l,r,z+1));
assert(w.rank(z,l,r)==w.rank(z,r)-w.rank(z,l));
assert(w.range_freq(l,r,z,z+1)==w.rank(z,l,r));
assert(w.prev_value(l,r,z+1)==optional<int>(z));assert(w.next_value(l,r,z)==optional<int>(z));
if(i<100&&r-l<=10000){vector<int>b(a.begin()+l,a.begin()+r);sort(b.begin(),b.end());
for(int x:{-1,0,z,z+1,1000000001,1073741824}){
auto lo=lower_bound(b.begin(),b.end(),x);assert(w.less(l,r,x)==lo-b.begin());
assert(w.rank(x,l,r)==upper_bound(b.begin(),b.end(),x)-lo);
optional<int>p,v;if(lo!=b.begin())p=*prev(lo);if(lo!=b.end())v=*lo;
assert(w.prev_value(l,r,x)==p);assert(w.next_value(l,r,x)==v);
}}
cout<<z<<'\n';}
''',functions=['WaveletMatrix(vector)','access','rank(x,l,r)','rank(x,r)','kth','kth_largest','less','range_freq','prev_value','next_value'],note='Official kth answers plus boundary and independent sorted-vector checks of every query method.')

add('wavelet_frequency','static_range_frequency',['wavelet_matrix.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;WaveletMatrix<int,30>w(a);
while(q--){int l,r,x;cin>>l>>r>>x;int ans=w.range_freq(l,r,x,x+1);assert(ans==w.rank(x,r)-w.rank(x,l));cout<<ans<<'\n';}
''',functions=['rank(x,r)','less','range_freq'])

add('segtree_search','predecessor_problem',['segtree.cpp'],r'''
int n,q;string s;cin>>n>>q>>s;segtree<int,op,e>seg(n);segtree<int,op,e>empty;assert(empty.all_prod()==0);
for(int i=0;i<n;i++)seg.set(i,s[i]-'0');
while(q--){int t,k;cin>>t>>k;if(t<2)seg.set(k,t==0);else if(t==2)cout<<seg.get(k)<<'\n';
else if(t==3){int r=seg.max_right(k,[](int x){return x==0;});cout<<(r==n?-1:r)<<'\n';}
else cout<<seg.min_left(k+1,[](int x){return x==0;})-1<<'\n';}
assert(seg.all_prod()==seg.prod(0,n));
''',extra='int op(int a,int b){return a+b;}int e(){return 0;}',functions=['segtree(int)','set','get','prod','all_prod','max_right','mr','min_left','ml'])

add('lazy_segtree_search','predecessor_problem',['lazy_segtree.cpp'],r'''
int n,q;string s;cin>>n>>q>>s;vector<S>a(n);for(int i=0;i<n;i++)a[i]={s[i]-'0',1};lazy_segtree<S,op,e,int,mapping,composition,id>seg(a),empty;
assert(empty.all_prod().sum==0);
while(q--){int t,k;cin>>t>>k;if(t<2){if(q%3==0)seg.set(k,{t==0,1});else if(q%3==1)seg.apply(k,t==0);else seg.apply(k,k+1,t==0);}
else if(t==2)cout<<seg.get(k).sum<<'\n';
else if(t==3){int r=seg.max_right(k,[](S x){return x.sum==0;});cout<<(r==n?-1:r)<<'\n';}
else cout<<seg.min_left(k+1,[](S x){return x.sum==0;})-1<<'\n';}
assert(seg.all_prod().sum==seg.prod(0,n).sum);
''',extra='''struct S{int sum,len;};S op(S a,S b){return {a.sum+b.sum,a.len+b.len};}S e(){return {0,0};}
S mapping(int f,S s){return f<0?s:S{f*s.len,s.len};}int composition(int f,int g){return f<0?g:f;}int id(){return -1;}
''',functions=['lazy_segtree(int)','lazy_segtree(vector)','set(public/recursive)','get','prod(public/recursive)','all_prod','apply(point/range/recursive)','all','push','max_right','mr','min_left','ml'])

add('lazy_segtree_range_search','range_affine_range_sum',['modint_convolution.cpp','lazy_segtree.cpp'],r'''
int n,q;cin>>n>>q;vector<S>a(n);for(auto&v:a){int x;cin>>x;v={x,1};}lazy_segtree<S,op,e,F,mapping,composition,id>seg(a);
while(q--){int t,l,r;cin>>t>>l>>r;if(t==0){int b,c;cin>>b>>c;seg.apply(l,r,{b,c});}
else{mint ans=0;auto pred=[&](S x){if(x.len<=r-l){ans=x.sum;return true;}return false;};
if(q%2)assert(seg.max_right(l,pred)==r);else assert(seg.min_left(r,pred)==l);cout<<ans.val()<<'\n';}}
''',extra='''struct S{mint sum;int len;};struct F{mint a,b;};S op(S a,S b){return {a.sum+b.sum,a.len+b.len};}S e(){return {0,0};}
S mapping(F f,S x){return {f.a*x.sum+f.b*mint(x.len),x.len};}F composition(F f,F g){return {f.a*g.a,f.a*g.b+f.b};}F id(){return {1,0};}
''',functions=['max_right','min_left','mr','ml','apply(range)','push'],note='Search by accumulated length; accepted search prefixes/suffixes supply the actual range sum, including after lazy affine updates.')

add('binarytrie_max','set_xor_min',['binarytrie.cpp'],r'''
int q;cin>>q;binary_trie<30>tr;unordered_set<ll>s;const ll mask=(1LL<<30)-1;
while(q--){int t;ll x;cin>>t>>x;if(t==0){if(s.insert(x).second)tr.add(x,1);}else if(t==1){if(s.erase(x))tr.add(x,-1);}
else cout<<(mask^tr.max_xor(mask^x))<<'\n';}
''',functions=['add','max_xor'],note='Complementing all 30 query bits turns max-XOR into the official min-XOR query.')

add('bit_lower_bound','predecessor_problem',['binary_indexed_tree.cpp'],r'''
int n,q;string s;cin>>n>>q>>s;BIT<int>b;b.init(n);vector<int>a(n);for(int i=0;i<n;i++){a[i]=s[i]-'0';b.add(i,a[i]);}
while(q--){int t,k;cin>>t>>k;if(t<2){int v=t==0;b.add(k,v-a[k]);a[k]=v;}else if(t==2)cout<<b.query(k,k+1)<<'\n';
else if(t==3){int target=b.query(k-1)+1;cout<<(target>b.query(n-1)?-1:b.lower_bound(target))<<'\n';}
else{int target=b.query(k);cout<<(target==0?-1:b.lower_bound(target))<<'\n';}}
''',functions=['BIT()','init','lower_bound'],note='Rank/select implements predecessor and successor; out-of-total targets are guarded for this problem.')

add('dsu_size','unionfind',['dsu.cpp'],r'''
int n,q;cin>>n>>q;dsu d(n);vector<int>counts(n,1);
while(q--){int t,u,v;cin>>t>>u>>v;if(t==0){int a=d.leader(u),b=d.leader(v);int expected=counts[a]+(a==b?0:counts[b]);int r=d.merge(u,v);counts[r]=expected;assert(d.size(u)==expected&&d.size(v)==expected);}
else cout<<d.same(u,v)<<'\n';}
''',functions=['size','leader','merge'])

add('suffix_array_integer','suffixarray',['suffix_array.cpp'],r'''
string s;cin>>s;vector<int>a(s.begin(),s.end());for(int x:suffix_array(a,256))cout<<x<<' ';cout<<'\n';
''',functions=['suffix_array(vector<int>,int)','suffix_array<T>(vector<T>)'])

add('bit_range_add','static_range_sum',['binary_indexed_tree.cpp'],r'''
int n,q;cin>>n>>q;BIT<ll>b(n);for(int i=0;i<n;i++){ll x;cin>>x;b.add(i,n,x);}
while(q--){int l,r;cin>>l>>r;cout<<b.query(r-1)-b.query(l-1)<<'\n';}
''',functions=['add(l,r,x)','query(i)'])

add('hld_jump','jump_on_tree',['hld.cpp'],r'''
int n,q;cin>>n>>q;vector<vector<int>>g(n);vector<pair<int,int>>edges;
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);edges.push_back({u,v});}
HeavyLightDecomposition<vector<vector<int>>>h(g);
for(auto[u,v]:edges){int child=h.par[u]==v?u:v;assert(h.get_edge(u,v)==h.id[child]);assert(h.get_edge(v,u)==h.id[child]);}
while(q--){int s,t,k;cin>>s>>t>>k;int l=h.lca(s,t),d=h.dist(s,t);if(k>d){cout<<-1<<'\n';continue;}
int z=k<=h.depth[s]-h.depth[l]?h.level_ancestor(s,h.depth[s]-k):h.level_ancestor(t,h.depth[t]-(d-k));cout<<z<<'\n';}
''',functions=['level_ancestor','dist','get_edge','lca'])

add('maxflow_helpers','bipartitematching',['maxflow.cpp'],r'''
int l,r,m;cin>>l>>r>>m;int s=l+r,t=s+1;mf_graph<int>g(t+1),empty;assert(empty.edges().empty());
auto add=[&](int u,int v){int id=g.add_edge(u,v,0);g.change_edge(id,1,0);return id;};
for(int i=0;i<l;i++)add(s,i);for(int i=0;i<r;i++)add(l+i,t);
vector<int>ids;for(int i=0,u,v;i<m;i++){cin>>u>>v;ids.push_back(add(u,l+v));}
int f=g.flow(s,t,1);f+=g.flow(s,t);auto cut=g.min_cut(s);auto es=g.edges();int cap=0;vector<int>bal(t+1);
for(int i=0;i<(int)es.size();i++){auto e=es[i],z=g.get_edge(i);assert(e.from==z.from&&e.to==z.to&&e.cap==z.cap&&e.flow==z.flow);
assert(0<=e.flow&&e.flow<=e.cap);bal[e.from]-=e.flow;bal[e.to]+=e.flow;if(cut[e.from]&&!cut[e.to])cap+=e.cap;}
assert(cap==f&&bal[s]==-f&&bal[t]==f);for(int i=0;i<s;i++)assert(bal[i]==0);
cout<<f<<'\n';for(int id:ids){auto e=g.get_edge(id);if(e.flow)cout<<e.from<<' '<<e.to-l<<'\n';}
''',functions=['mf_graph()','edges','get_edge','change_edge','flow(s,t,limit)','flow(s,t)','min_cut'])

add('bmbm_wrapper','find_linear_recurrence',['modint_convolution.cpp','bmbm.cpp'],r'''
int n;cin>>n;vector<mint>a(n);for(auto&v:a){int x;cin>>x;v=x;}auto c=BerlekampMassey(a);auto seq=a;
for(int i=0;i<min(n,3);i++)assert(BMBM(a,i).val()==a[i].val());
for(int k=0;k<4;k++){mint z=0;for(int j=0;j<(int)c.size();j++)z+=c[j]*seq[seq.size()-j-1];seq.push_back(z);assert(BMBM(a,n+k).val()==z.val());}
cout<<c.size()<<'\n';for(auto x:c)cout<<x.val()<<' ';cout<<'\n';
''',functions=['BMBM'],note='The recurrence is judged officially; BMBM predictions beyond the supplied prefix are checked against direct recurrence evaluation.')

add('dynamic_all_prod','point_set_range_composite',['modint_convolution.cpp','dynamic_segtree.cpp'],r'''
int n,q;cin>>n>>q;dynamic_segtree<S,op,e>seg(n),empty;assert(empty.all_prod().a.val()==1&&empty.all_prod().b.val()==0);
for(int i=0,x,y;i<n;i++){cin>>x>>y;seg.set(i,{x,y});auto z=seg.get(i);assert(z.a.val()==x&&z.b.val()==y);}
while(q--){int t,l,r;ll x;cin>>t;if(t==0){cin>>l>>r>>x;seg.set(l,{r,x});}
else{cin>>l>>r>>x;auto z=(l==0&&r==n)?seg.all_prod():seg.prod(l,r);cout<<(z.a*mint(x)+z.b).val()<<'\n';}}
auto f=seg.all_prod(),g=seg.prod(0,n);assert(f.a.val()==g.a.val()&&f.b.val()==g.b.val());
''',extra='struct S{mint a,b;};S op(S l,S r){return {r.a*l.a,r.a*l.b+r.b};}S e(){return {1,0};}',functions=['dynamic_segtree()','all_prod','get(public/recursive)'])

add('mint_all','convolution_mod',['modint_convolution.cpp'],r'''
assert(mint::mod()==998244353);mt19937_64 rng(7142);for(int i=0;i<2000;i++){ll a=(ll)(rng()%4000000000ULL)-2000000000LL,b=rng()%998244352+1;
atcoder::modint998244353 x=a,y=b;mint u=a,v=b;
assert(u.val()==x.val());assert((u+v).val()==(x+y).val());assert((u-v).val()==(x-y).val());assert((u*v).val()==(x*y).val());assert((u/v).val()==(x/y).val());
ll e=rng()%1000000000000000000LL;assert(u.pow(e).val()==x.pow(e).val());assert(v.inv().val()==y.inv().val());
mint c=u;c+=v;assert(c.val()==(x+y).val());c=u;c-=v;assert(c.val()==(x-y).val());c=u;c*=v;assert(c.val()==(x*y).val());c=u;c/=v;assert(c.val()==(x/y).val());}
int n,m;cin>>n>>m;vector<mint>a(n),b(m);for(auto&v:a){ll x;cin>>x;v=x;}for(auto&v:b){ll x;cin>>x;v=x;}for(auto x:convolution(a,b))cout<<x.val()<<' ';cout<<'\n';
''',before='#include <atcoder/modint>\n',functions=['mint','val','mod','operator+=','operator-=','operator*=','operator/=','operator+','operator-','operator*','operator/','pow','inv','ntt','convolution'],note='Convolution is judged officially; all modular operations also have fixed-seed differential checks against ACL.')

add('cartesian_all','cartesian_tree',['cartesiantree.cpp'],r'''
CartesianTree<int>empty;assert(empty.par.empty()&&empty.left.empty()&&empty.right.empty());int n;cin>>n;vector<int>a(n);for(auto&x:a)cin>>x;CartesianTree<int>t(a);
for(int i=0;i<n;i++){for(int v:{t.left[i],t.right[i]})if(v>=0){assert(t.par[v]==i&&a[i]<=a[v]);}if(t.left[i]>=0)assert(t.left[i]<i);if(t.right[i]>=0)assert(t.right[i]>i);}
assert(t.par[t.root]==-1);for(int i=0;i<n;i++)cout<<(t.par[i]<0?i:t.par[i])<<' ';cout<<'\n';
''',functions=['CartesianTree()','CartesianTree(vector)'])

add('mo_directional','static_range_count_distinct',['mo.cpp'],r'''
mt19937 rng(9284);for(int rep=0;rep<30;rep++){int n=1+rng()%30;vector<int>a(n);for(auto&v:a)v=rng()%11;vector<pair<int,int>>qs;Mo mo(n);for(int i=0;i<100;i++){int l=rng()%(n+1),r=rng()%(n+1);if(l>r)swap(l,r);qs.push_back({l,r});mo.add(l,r);}
vector<int>cnt(11);ll inversions=0;auto al=[&](int i){for(int x=0;x<a[i];x++)inversions+=cnt[x];cnt[a[i]]++;};auto ar=[&](int i){for(int x=a[i]+1;x<=10;x++)inversions+=cnt[x];cnt[a[i]]++;};
auto el=[&](int i){cnt[a[i]]--;for(int x=0;x<a[i];x++)inversions-=cnt[x];};auto er=[&](int i){cnt[a[i]]--;for(int x=a[i]+1;x<=10;x++)inversions-=cnt[x];};
mo.build(al,ar,el,er,[&](int i){auto[l,r]=qs[i];ll expected=0;for(int u=l;u<r;u++)for(int v=u+1;v<r;v++)expected+=a[u]>a[v];assert(inversions==expected);});}
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;auto vs=a;sort(vs.begin(),vs.end());vs.erase(unique(vs.begin(),vs.end()),vs.end());for(auto&x:a)x=lower_bound(vs.begin(),vs.end(),x)-vs.begin();
Mo mo(n);for(int i=0,l,r;i<q;i++){cin>>l>>r;mo.add(l,r);}vector<int>cnt(vs.size()),ans(q);int d=0;mo.build([&](int i){d+=cnt[a[i]]++==0;},[&](int i){d-=--cnt[a[i]]==0;},[&](int i){ans[i]=d;});for(int x:ans)cout<<x<<'\n';
''',functions=['Mo','add','build(AL,AR,EL,ER,out)','build(add,erase,out)'],note='Direction-sensitive callbacks are independently checked by counting inversions on fixed-seed small cases; distinct-count answers use official input.')

base=json.loads((ROOT/'library_checker/manifest.json').read_text())
task=next(t for t in base if t['key']=='rerooting');source=(ROOT/'library_checker'/task['source']).read_text();assert source.count('dp.build();')==1
source=source.replace('dp.build();','dp.build(n-1);');target=OUT/'submissions/rerooting_nonzero_root.cc';target.write_text(source)
TASKS.append({**task,'key':'rerooting_nonzero_root','source':'submissions/rerooting_nonzero_root.cc','source_sha256':hashlib.sha256(source.encode()).hexdigest(),'functions':['build(nonzero_root)','reroot','es_build','outs_start'],'note':'The adapter chooses n-1 as the build root; the original rerooting implementation is unchanged.'})

original=next(t for t in TASKS if t['key']=='maxflow_helpers');source=(OUT/original['source']).read_text()
source=source.replace('cin.tie(nullptr);\n','''cin.tie(nullptr);
{mf_graph<int>probe(2);int i=probe.add_edge(0,1,5);probe.change_edge(i,9,4);auto x=probe.get_edge(i);assert(x.cap==9&&x.flow==4);assert(probe.flow(0,1,3)==3);x=probe.get_edge(i);assert(x.cap==9&&x.flow==7);probe.change_edge(i,10,7);assert(probe.flow(0,1)==3);auto cut=probe.min_cut(0);assert(cut[0]&&!cut[1]);}
''',1)
target=OUT/'submissions/maxflow_nonzero_change.cc';target.write_text(source)
TASKS.append({**original,'key':'maxflow_nonzero_change','source':'submissions/maxflow_nonzero_change.cc','source_sha256':hashlib.sha256(source.encode()).hexdigest(),'functions':['change_edge(nonzero_flow)','flow(repeated)']})

# The original template has an empty solve(). Submitting its real entry point
# records that behavior honestly instead of substituting an implementation.
raw=(ROOT/'template.cpp').read_bytes();target=OUT/'submissions/template_entry.cc';target.write_bytes(raw)
TASKS.append(dict(key='template_entry',problem='aplusb',libraries=[dict(file='template.cpp',sha256=hashlib.sha256(raw).hexdigest())],source='submissions/template_entry.cc',lang='cpp17',source_sha256=hashlib.sha256(raw).hexdigest(),functions=['main','solve'],note='Original entry point unchanged. Empty solve() is expected to produce WA for A+B; this is a template placeholder, not an implemented solver.'))

(OUT/'manifest.json').write_text(json.dumps(TASKS,ensure_ascii=False,indent=2)+'\n')
print('Generated',len(TASKS),'function-level adapters')
