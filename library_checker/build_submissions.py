#!/usr/bin/env python3
"""Create Library Checker adapters, embedding the original libraries verbatim."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = Path(__file__).resolve().parent
TASKS = []
PRELUDE = '#include <bits/stdc++.h>\nusing namespace std;\nusing ll = long long;\n'


def add(key, problem, libraries, body, extra='', before='', note='', namespace=False):
    text = PRELUDE + before + '\n'
    sources = []
    for name in libraries:
        raw = (ROOT / name).read_bytes()
        source = raw.decode('utf-8')
        digest = hashlib.sha256(raw).hexdigest()
        sources.append({'file': name, 'sha256': digest})
        text += f'\n// BEGIN ORIGINAL {name} SHA256 {digest}\n'
        if namespace:
            text += 'namespace original_template {\n'
        text += source
        if namespace:
            text += '\n}\n'
        text += f'\n// END ORIGINAL {name}\n'
        assert source in text
    text += '\n' + extra + '\nint main() {\n  ios::sync_with_stdio(false); cin.tie(nullptr);\n'
    text += body + '\n}\n'
    target = OUT / 'submissions' / (key + '.cc')
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(text, encoding='utf-8')
    TASKS.append({'key': key, 'problem': problem, 'libraries': sources,
                  'source': str(target.relative_to(OUT)), 'lang': 'cpp17',
                  'source_sha256': hashlib.sha256(text.encode()).hexdigest(), 'note': note})


add('binary_indexed_tree', 'point_add_range_sum', ['binary_indexed_tree.cpp'], r'''
int n,q; cin>>n>>q; BIT<ll> bit(n);
for(int i=0;i<n;i++){ll x;cin>>x;bit.add(i,x);}
while(q--){int t,l;ll r;cin>>t>>l>>r;if(t==0)bit.add(l,r);else cout<<bit.query(l,(int)r)<<'\n';}
''')

add('binarytrie', 'set_xor_min', ['binarytrie.cpp'], r'''
int q;cin>>q;binary_trie<30> tr;unordered_set<ll> present;
while(q--){int t;ll x;cin>>t>>x;if(t==0){if(present.insert(x).second)tr.insert(x);}
else if(t==1){if(present.erase(x))tr.erase(x);}else cout<<tr.min_xor(x)<<'\n';}
''', note='The adapter enforces set semantics (duplicate insert / absent erase are ignored).')

add('bmbm_recurrence', 'find_linear_recurrence', ['modint_convolution.cpp','bmbm.cpp'], r'''
int n;cin>>n;vector<mint>a(n);for(auto&v:a){int x;cin>>x;v=x;}
auto c=BerlekampMassey(a);cout<<c.size()<<'\n';for(auto x:c)cout<<x.val()<<' ';cout<<'\n';
''')
add('bmbm_kth', 'kth_term_of_linearly_recurrent_sequence', ['modint_convolution.cpp','bmbm.cpp'], r'''
int d;ll k;cin>>d>>k;vector<mint>a(d),c(d);
for(auto&v:a){int x;cin>>x;v=x;}for(auto&v:c){int x;cin>>x;v=x;}
cout<<BostanMori(a,c,k).val()<<'\n';
''')
add('cartesiantree', 'cartesian_tree', ['cartesiantree.cpp'], r'''
int n;cin>>n;vector<int>a(n);for(auto&x:a)cin>>x;CartesianTree<int> t(a);
for(int i=0;i<n;i++)cout<<(t.par[i]<0?i:t.par[i])<<' ';cout<<'\n';
''')
add('convex_hull_trick', 'line_add_get_min', ['convex_hull_trick.cpp'], r'''
int n,q;cin>>n>>q;vector<pair<ll,ll>> lines(n);for(auto&[a,b]:lines)cin>>a>>b;
struct Q{int t;ll a,b;};vector<Q> queries;vector<ll> xs;
for(int i=0;i<q;i++){int t;ll a,b=0;cin>>t>>a;if(t==0)cin>>b;else xs.push_back(a);queries.push_back({t,a,b});}
sort(xs.begin(),xs.end());xs.erase(unique(xs.begin(),xs.end()),xs.end());if(xs.empty())return 0;
convex_hull_trick<ll,LLONG_MIN> tr(xs);for(auto[a,b]:lines)tr.insert(-a,-b);
for(auto query:queries)if(query.t==0)tr.insert(-query.a,-query.b);else cout<<-tr.get(query.a)<<'\n';
''', note='Negating lines converts the library maximum query to the problem minimum query.')
add('dsu', 'unionfind', ['dsu.cpp'], r'''
int n,q;cin>>n>>q;dsu d(n);while(q--){int t,u,v;cin>>t>>u>>v;if(t==0)d.merge(u,v);else cout<<d.same(u,v)<<'\n';}
''')
add('dynamic_segtree', 'point_add_range_sum', ['dynamic_segtree.cpp'], r'''
int n,q;cin>>n>>q;dynamic_segtree<ll,op,e> seg(n);
for(int i=0;i<n;i++){ll x;cin>>x;seg.set(i,x);}
while(q--){int t,l;ll r;cin>>t>>l>>r;if(t==0)seg.set(l,seg.get(l)+r);else cout<<seg.prod(l,r)<<'\n';}
''', extra='ll op(ll a,ll b){return a+b;} ll e(){return 0;}')
add('eulertour', 'lca', ['eulertour.cpp','segtree.cpp'], r'''
int n,q;cin>>n>>q;vector<vector<int>>g(n);
for(int i=1,p;i<n;i++){cin>>p;g[p].push_back(i);g[i].push_back(p);}
EulerTour tour(n,g);vector<S>a(2*n-1);for(int i=0;i<2*n-1;i++)a[i]={tour.depth[i],tour.vs[i]};segtree<S,op,e> seg(a);
while(q--){int u,v;cin>>u>>v;int l=tour.id[u],r=tour.id[v];if(l>r)swap(l,r);cout<<seg.prod(l,r+1).second<<'\n';}
''', extra='using S=pair<int,int>; S op(S a,S b){return min(a,b);} S e(){return {INT_MAX,INT_MAX};}',
    note='Euler tour plus the repository segtree for range minimum queries.')
add('geometry', 'static_convex_hull', ['geometry.cpp'], r'''
int t;cin>>t;while(t--){int n;cin>>n;vector<Point>p(n);for(auto&z:p){ll x,y;cin>>x>>y;z=Point(x,y);}
auto h=ConvexHull(p);cout<<h.size()<<'\n';for(auto z:h)cout<<llround(z.real())<<' '<<llround(z.imag())<<'\n';}
''', note='The entire geometry file is included unchanged; compilation errors are reported as judge results.')
add('hld_path_sum', 'vertex_add_path_sum', ['hld.cpp','binary_indexed_tree.cpp'], r'''
int n,q;cin>>n>>q;vector<ll>a(n);for(auto&x:a)cin>>x;vector<vector<int>>g(n);
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
HeavyLightDecomposition<vector<vector<int>>> h(g);BIT<ll> bit(n);for(int i=0;i<n;i++)bit.add(h.id[i],a[i]);
while(q--){int t,u;ll v;cin>>t>>u>>v;if(t==0)bit.add(h.id[u],v);else{ll ans=0;h.path_query(u,(int)v,true,[&](int l,int r){ans+=bit.query(l,r);});cout<<ans<<'\n';}}
''')
add('hld_short', 'vertex_add_subtree_sum', ['hld_short.cpp','binary_indexed_tree.cpp'], r'''
int n,q;cin>>n>>q;vector<ll>a(n);for(auto&x:a)cin>>x;g.resize(n);sz.resize(n);in.resize(n);out.resize(n);nxt.resize(n);
for(int i=1,p;i<n;i++){cin>>p;g[p].push_back(i);}dfs_sz();dfs_hld();BIT<ll>bit(n);for(int i=0;i<n;i++)bit.add(in[i],a[i]);
while(q--){int type,u;cin>>type>>u;if(type==0){ll x;cin>>x;bit.add(in[u],x);}else cout<<bit.query(in[u],out[u])<<'\n';}
''', note='The short HLD expects a directed children-only tree; the adapter supplies that representation.')
add('lazy_segtree', 'range_affine_range_sum', ['modint_convolution.cpp','lazy_segtree.cpp'], r'''
int n,q;cin>>n>>q;vector<S>a(n);for(auto&x:a){int v;cin>>v;x={v,1};}lazy_segtree<S,op,e,F,mapping,composition,id> seg(a);
while(q--){int t,l,r;cin>>t>>l>>r;if(t==0){int b,c;cin>>b>>c;seg.apply(l,r,{b,c});}else cout<<seg.prod(l,r).sum.val()<<'\n';}
''', extra='''
struct S{mint sum,len;};struct F{mint a,b;};
S op(S a,S b){return {a.sum+b.sum,a.len+b.len};} S e(){return {0,0};}
S mapping(F f,S s){return {f.a*s.sum+f.b*s.len,s.len};}
F composition(F f,F g){return {f.a*g.a,f.a*g.b+f.b};}F id(){return {1,0};}
''')
add('lcp_array', 'number_of_substrings', ['suffix_array.cpp','lcp_array.cpp'], r'''
string s;cin>>s;auto sa=suffix_array(s);auto lcp=lcp_array(s,sa);ll ans=ll(s.size())*(s.size()+1)/2;for(int x:lcp)ans-=x;cout<<ans<<'\n';
''')
for key,problem,body in [
    ('linear_equation','system_of_linear_equations',r'''
int n,m;cin>>n>>m;vector<vector<Fp>>a(n,vector<Fp>(m));vector<Fp>b(n);
for(auto&row:a)for(auto&v:row){int x;cin>>x;v=x;}for(auto&v:b){int x;cin>>x;v=x;}
auto sol=LinearEquation(a,b);if(sol.empty()){cout<<-1<<'\n';return 0;}cout<<sol.size()-1<<'\n';for(auto&row:sol){for(auto x:row)cout<<x.val()<<' ';cout<<'\n';}
'''),
    ('linear_equation_det','matrix_det',r'''
int n;cin>>n;vector<vector<Fp>>a(n,vector<Fp>(n));for(auto&row:a)for(auto&v:row){int x;cin>>x;v=x;}
cout<<GaussElimination(a).second.val()<<'\n';
'''),
    ('linear_equation_rank','matrix_rank',r'''
int n,m;cin>>n>>m;vector<vector<Fp>>a(n,vector<Fp>(m));for(auto&row:a)for(auto&v:row){int x;cin>>x;v=x;}
cout<<GaussElimination(a).first<<'\n';
'''),
]:
    add(key,problem,['linear_equation.cpp'],body,
        before='#include <atcoder/modint>\nusing Fp=atcoder::modint998244353;\n',
        note='Uses AC Library modint as the generic coefficient type; linear_equation.cpp is unchanged.')
add('linear_equation_bitset','system_of_linear_equations_mod_2',['linear_equation_bitset.cpp'],r'''
int n,m;cin>>n>>m;vector<bitset<4097>>a(n);string s;
for(int i=0;i<n;i++){cin>>s;for(int j=0;j<m;j++)a[i][j]=s[j]=='1';}
cin>>s;for(int i=0;i<n;i++)a[i][m]=s[i]=='1';auto sol=LinearEquation<4096>(a,m);
if(sol.empty()){cout<<-1<<'\n';return 0;}cout<<sol.size()-1<<'\n';for(auto&row:sol){for(int j=0;j<m;j++)cout<<row[j];cout<<'\n';}
''')
add('manacher','enumerate_palindromes',['manacher.cpp'],r'''
string s;cin>>s;string t="#";for(char c:s){t+=c;t+='#';}auto r=manacher(t);
for(int i=1;i+1<(int)t.size();i++)cout<<r[i]-1<<' ';cout<<'\n';
''',note='Separator insertion converts odd-radius output to odd and even palindrome lengths.')
add('math','sum_of_floor_of_linear',['math.cpp'],r'''
int t;cin>>t;while(t--){ll n,m,a,b;cin>>n>>m>>a>>b;cout<<floor_sum(n,m,a,b)<<'\n';}
''',note='Tests floor_sum; the other functions in math.cpp are included but not exercised by this problem.')
add('maxflow','bipartitematching',['maxflow.cpp'],r'''
int l,r,m;cin>>l>>r>>m;int source=l+r,sink=source+1;mf_graph<int>g(sink+1);
for(int i=0;i<l;i++)g.add_edge(source,i,1);for(int i=0;i<r;i++)g.add_edge(l+i,sink,1);
vector<int>ids;for(int i=0,u,v;i<m;i++){cin>>u>>v;ids.push_back(g.add_edge(u,l+v,1));}
cout<<g.flow(source,sink)<<'\n';for(int id:ids){auto e=g.get_edge(id);if(e.flow)cout<<e.from<<' '<<e.to-l<<'\n';}
''')
add('mincostflow','assignment',['mincostflow.cpp'],r'''
int n;cin>>n;vector<vector<ll>>a(n,vector<ll>(n));ll low=0;for(auto&row:a)for(auto&x:row){cin>>x;low=min(low,x);}
int source=2*n,sink=source+1;mcf_graph<int,ll>g(sink+1);for(int i=0;i<n;i++){g.add_edge(source,i,1,0);g.add_edge(n+i,sink,1,0);}
vector<vector<int>>id(n,vector<int>(n));for(int i=0;i<n;i++)for(int j=0;j<n;j++)id[i][j]=g.add_edge(i,n+j,1,a[i][j]-low);
auto [flow,cost]=g.flow(source,sink,n);cout<<cost+flow*low<<'\n';
for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(g.get_edge(id[i][j]).flow)cout<<j<<' ';cout<<'\n';
''',note='Costs are shifted by a common constant to provide nonnegative initial edge costs.')
add('mo','static_range_count_distinct',['mo.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;auto vals=a;sort(vals.begin(),vals.end());vals.erase(unique(vals.begin(),vals.end()),vals.end());
for(auto&x:a)x=lower_bound(vals.begin(),vals.end(),x)-vals.begin();Mo mo(n);for(int i=0,l,r;i<q;i++){cin>>l>>r;mo.add(l,r);}
vector<int>cnt(vals.size()),ans(q);int distinct=0;auto add=[&](int i){if(cnt[a[i]]++==0)distinct++;};auto erase=[&](int i){if(--cnt[a[i]]==0)distinct--;};
mo.build(add,erase,[&](int i){ans[i]=distinct;});for(int x:ans)cout<<x<<'\n';
''')
add('modint_convolution','convolution_mod',['modint_convolution.cpp'],r'''
int n,m;cin>>n>>m;vector<mint>a(n),b(m);for(auto&v:a){int x;cin>>x;v=x;}for(auto&v:b){int x;cin>>x;v=x;}
for(auto x:convolution(a,b))cout<<x.val()<<' ';cout<<'\n';
''')
add('rollback_mo','static_range_count_distinct',['rollback_mo.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;auto vals=a;sort(vals.begin(),vals.end());vals.erase(unique(vals.begin(),vals.end()),vals.end());
for(auto&x:a)x=lower_bound(vals.begin(),vals.end(),x)-vals.begin();MoRollBack mo(n,q);for(int i=0,l,r;i<q;i++){cin>>l>>r;mo.add(l,r);}
vector<int>cnt(vals.size()),ans(q),hist;int distinct=0,snap=0;
auto undo=[&](){int i=hist.back();hist.pop_back();if(--cnt[a[i]]==0)distinct--;};
auto add=[&](int i){hist.push_back(i);if(cnt[a[i]]++==0)distinct++;};
auto reset=[&](){while(!hist.empty())undo();snap=0;};
auto snapshot=[&](){snap=hist.size();};auto rollback=[&](){while((int)hist.size()>snap)undo();};
mo.run(add,[&](int i){ans[i]=distinct;},reset,snapshot,rollback);for(int x:ans)cout<<x<<'\n';
''')
add('rolling_hash','zalgorithm',['rolling_hash.cpp'],r'''
string s;cin>>s;int n=s.size();rolling_hash h(s);for(int i=0;i<n;i++){int l=0,r=n-i+1;while(r-l>1){int m=(l+r)/2;if(h.get(0,m)==h.get(i,i+m))l=m;else r=m;}cout<<l<<' ';}cout<<'\n';
''',note='Computes each suffix/prefix LCP by binary search over the library hashes.')
add('scc','scc',['scc.cpp'],r'''
int n,m;cin>>n>>m;scc_graph g(n);for(int i=0,u,v;i<m;i++){cin>>u>>v;g.add_edge(u,v);}auto components=g.scc();cout<<components.size()<<'\n';for(auto&c:components){cout<<c.size();for(int v:c)cout<<' '<<v;cout<<'\n';}
''')
COMPOSITE_EXTRA='''
struct S{mint a,b;};S op(S l,S r){return {r.a*l.a,r.a*l.b+r.b};}S e(){return {1,0};}
'''
COMPOSITE_BODY=r'''
int n,q;cin>>n>>q;vector<S>a(n);for(auto&v:a){int x,y;cin>>x>>y;v={x,y};}segtree<S,op,e> seg(a);
while(q--){int t,l,r;ll x;cin>>t;if(t==0){cin>>l>>r>>x;seg.set(l,{r,x});}else{cin>>l>>r>>x;auto f=seg.prod(l,r);cout<<(f.a*mint(x)+f.b).val()<<'\n';}}
'''
add('segtree','point_set_range_composite',['modint_convolution.cpp','segtree.cpp'],COMPOSITE_BODY,extra=COMPOSITE_EXTRA)
add('suffix_array','suffixarray',['suffix_array.cpp'],r'''
string s;cin>>s;for(int x:suffix_array(s))cout<<x<<' ';cout<<'\n';
''')
add('template','aplusb',['template.cpp'],r'''
ll a,b;cin>>a>>b;ll lo=a,hi=a;original_template::chmin(lo,b);original_template::chmax(hi,b);cout<<lo+hi<<'\n';
''',namespace=True,note='The unchanged template is namespaced to avoid a second entry point. A+B exercises chmin/chmax; the empty solve() and rep macro are not validated.')
add('trie','associative_array',['trie.cpp'],r'''
int q;cin>>q;trie<10,'0'>tr;unordered_map<int,ll>values;
while(q--){int type;ll k;cin>>type>>k;string s=to_string(k);int v=tr.find(s);
if(type==0){ll value;cin>>value;if(v<0||!tr.t[v].end){tr.insert(s);v=tr.find(s);}values[v]=value;}
else{if(v<0||!tr.t[v].end)cout<<0<<'\n';else cout<<values[v]<<'\n';}}
''',note='Decimal strings encode keys; the trie resolves string keys to node IDs, with values stored separately by node ID.')
add('twosat','two_sat',['twosat.cpp'],r'''
string p,cnf;int n,m;cin>>p>>cnf>>n>>m;two_sat sat(n);for(int i=0,a,b,z;i<m;i++){cin>>a>>b>>z;sat.add_clause(abs(a)-1,a>0,abs(b)-1,b>0);}
if(!sat.satisfiable()){cout<<"s UNSATISFIABLE\n";return 0;}cout<<"s SATISFIABLE\nv ";auto a=sat.answer();for(int i=0;i<n;i++)cout<<(a[i]?i+1:-i-1)<<' ';cout<<"0\n";
''')
add('wavelet_matrix_kth','range_kth_smallest',['wavelet_matrix.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;WaveletMatrix<int,30>w(a);while(q--){int l,r,k;cin>>l>>r>>k;cout<<w.kth(l,r,k)<<'\n';}
''')
add('wavelet_matrix_rank','static_range_frequency',['wavelet_matrix.cpp'],r'''
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;WaveletMatrix<int,30>w(a);while(q--){int l,r,x;cin>>l>>r>>x;cout<<w.rank(x,l,r)<<'\n';}
''')
add('z_algorithm','zalgorithm',['z_algorithm.cpp'],r'''
string s;cin>>s;for(int x:z_algorithm(s))cout<<x<<' ';cout<<'\n';
''')

add('rerooting','tree_path_composite_sum',['modint_convolution.cpp','rerooting.cpp'],r'''
int n;cin>>n;vertex_value.resize(n);for(auto&v:vertex_value){int x;cin>>x;v=x;}
edge_a.resize(n-1);edge_b.resize(n-1);RerootingDP<D,D,merge_dp,empty_dp,put_edge_dp,put_vertex_dp> dp(n);
for(int i=0,u,v,b,c;i<n-1;i++){cin>>u>>v>>b>>c;edge_a[i]=b;edge_b[i]=c;dp.add_edge(u,v,i,i);}
dp.build();auto ans=dp.reroot();for(auto x:ans)cout<<x.sum.val()<<' ';cout<<'\n';
''',extra='''
struct D{mint sum,count;};vector<mint>vertex_value,edge_a,edge_b;
D merge_dp(D a,D b){return {a.sum+b.sum,a.count+b.count};}
D empty_dp(){return {0,0};}
D put_edge_dp(D a,int i){return {a.sum*edge_a[i]+a.count*edge_b[i],a.count};}
D put_vertex_dp(D a,int i){return {a.sum+vertex_value[i],a.count+mint(1)};}
''')
add('dynamic_segtree_composite','point_set_range_composite',
    ['modint_convolution.cpp','dynamic_segtree.cpp'],r'''
int n,q;cin>>n>>q;dynamic_segtree<S,op,e>seg(n);for(int i=0,x,y;i<n;i++){cin>>x>>y;seg.set(i,{x,y});}
while(q--){int t,l,r;ll x;cin>>t;if(t==0){cin>>l>>r>>x;seg.set(l,{r,x});}else{cin>>l>>r>>x;auto f=seg.prod(l,r);cout<<(f.a*mint(x)+f.b).val()<<'\n';}}
''',extra=COMPOSITE_EXTRA)
add('hld_composite','vertex_set_path_composite',['modint_convolution.cpp','hld.cpp','segtree.cpp'],r'''
int n,q;cin>>n>>q;vector<A>a(n);for(auto&f:a){int x,y;cin>>x>>y;f={x,y};}vector<vector<int>>g(n);
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
HeavyLightDecomposition<vector<vector<int>>>h(g);vector<S>init(n);for(int i=0;i<n;i++)init[h.id[i]]={a[i],a[i]};segtree<S,op,e>seg(init);
while(q--){int t,u,v;ll x;cin>>t>>u>>v>>x;if(t==0){A a={v,x};seg.set(h.id[u],{a,a});}
else{A total={1,0};h.path_noncommutative_query(u,v,true,[&](int l,int r){A f=l<r?seg.prod(l,r).forward:seg.prod(r,l).backward;total=compose(total,f);});cout<<(total.a*mint(x)+total.b).val()<<'\n';}}
''',extra='''
struct A{mint a,b;};A compose(A l,A r){return {r.a*l.a,r.a*l.b+r.b};}
struct S{A forward,backward;};S op(S l,S r){return {compose(l.forward,r.forward),compose(r.backward,l.backward)};}
S e(){return {{1,0},{1,0}};}
''')

(OUT / 'manifest.json').write_text(json.dumps(TASKS,ensure_ascii=False,indent=2)+'\n')
print(f'Generated {len(TASKS)} submissions in {OUT / "submissions"}')
covered={s['file'] for task in TASKS for s in task['libraries']}
missing={p.name for p in ROOT.glob('*.cpp')}-covered
print('Libraries without an adapter:', sorted(missing))
