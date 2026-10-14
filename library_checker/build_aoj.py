#!/usr/bin/env python3
"""AOJ adapters; extract unchanged geometry except its uncompilable function."""
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parent.parent
OUT=ROOT/'library_checker/aoj'
TASKS=[]
GEO=(ROOT/'geometry.cpp').read_bytes()
START=GEO.index(b'Circle circumCircle(')
END=GEO.index(b'Point projection(',START)
FORWARD='int ccw(const complex<long double>&,complex<long double>,complex<long double>);\nstruct Line;\ncomplex<long double> crossPoint(const Line&,const Line&);\n'

GEO_HELPERS=r'''
void helper_checks(){
auto near=[](ld a,ld b){return fabsl(a-b)<1e-8L;};
assert(equal(0,EPS/2)&&!equal(0,2*EPS));assert(sgn(-2*EPS)==-1&&sgn(0)==0&&sgn(2*EPS)==1);
Line d;d.a={1,2};d.b={4,6};Line coeff(2,3,7),vertical(2,0,8);Circle c;c.p={1,2};c.r=3;assert(c.r==3);
for(auto z:{coeff.a,coeff.b})assert(near(2*z.real()+3*z.imag(),7));assert(near(vertical.a.real(),4)&&near(vertical.b.real(),4));
for(int x=-3;x<=3;x++)for(int y=-3;y<=3;y++)if(x||y){Point p(x,y),q(y+2,x-1);
assert(near(dot(p,q),x*(y+2)+y*(x-1)));assert(near(cross(p,q),x*(x-1)-y*(y+2)));
assert(abs(normalVector(p)-Point(-y,x))<1e-8L);assert(near(abs(unitVector(p)),1));
ld a=degreeToRadian(37);assert(near(radianToDegree(a),37));auto z=rotate(p,a);
assert(abs(z-Point(x*cosl(a)-y*sinl(a),x*sinl(a)+y*cosl(a)))<1e-8L);
}
}
Point read_point(){ld x,y;cin>>x>>y;return {x,y};}
Circle read_circle(){auto p=read_point();ld r;cin>>r;return {p,r};}
void print_point(Point p){cout<<p.real()<<' '<<p.imag();}
void sort_points(vector<Point>&p){sort(p.begin(),p.end(),[](Point a,Point b){return a.real()!=b.real()?a.real()<b.real():a.imag()<b.imag();});}
'''

def add(key,problem,library,body,extra='',functions=None,whole_geometry=False,judge='aoj'):
    raw=(ROOT/library).read_bytes();lib=dict(file=library,sha256=hashlib.sha256(raw).hexdigest())
    text='#include <bits/stdc++.h>\nusing namespace std;using ll=long long;\n'
    if library=='geometry.cpp':
        text+=FORWARD
        if whole_geometry:
            text+=raw.decode()
        else:
            parts=[(0,START),(END,len(raw))]
            lib['parts']=[dict(start=a,end=b,sha256=hashlib.sha256(raw[a:b]).hexdigest()) for a,b in parts]
            text+='\n// Unchanged geometry fragments; circumCircle is submitted separately.\n'
            text+='\n'.join(raw[a:b].decode() for a,b in parts)
        extra=GEO_HELPERS+extra
        body='helper_checks();cout<<fixed<<setprecision(15);\n'+body
    else:text+=raw.decode()
    text+='\n'+extra+'\nint main(){ios::sync_with_stdio(false);cin.tie(nullptr);\n'+body+'\n}\n'
    directory=OUT if judge=='aoj' else ROOT/'library_checker/full_geometry'
    target=directory/'submissions'/(key+'.cc');target.parent.mkdir(parents=True,exist_ok=True);target.write_text(text)
    task=dict(key=key,problem=problem,libraries=[lib],source=str(target.relative_to(directory)),lang='C++17' if judge=='aoj' else 'cpp17',source_sha256=hashlib.sha256(text.encode()).hexdigest(),functions=functions or [],judge=judge)
    if judge=='aoj':TASKS.append(task)
    else:(directory/'manifest.json').write_text(json.dumps([task],ensure_ascii=False,indent=2)+'\n')

add('geo_projection','CGL_1_A','geometry.cpp',r'''
Point a=read_point(),b=read_point();Line l(a,b);ld A=a.imag()-b.imag(),B=b.real()-a.real(),C=A*a.real()+B*a.imag();Line coeff(A,B,C);
int q;cin>>q;while(q--){Point p=read_point();auto z=projection(coeff,p);assert(abs(z-projection(l,p))<1e-6L);print_point(z);cout<<'\n';}
''',functions=['Line()','Line(Point,Point)','Line(A,B,C)','projection','equal','dot','unitVector','normalVector','cross','rotate','sgn','degreeToRadian','radianToDegree','Circle()'])
add('geo_reflection','CGL_1_B','geometry.cpp',r'''
Point a=read_point(),b=read_point();Line l(a,b);int q;cin>>q;while(q--){print_point(reflection(l,read_point()));cout<<'\n';}
''',functions=['reflection'])
add('geo_orientation','CGL_1_C','geometry.cpp',r'''
Point a=read_point(),b=read_point();Line l(a,b);int q;cin>>q;while(q--){Point p=read_point();int c=ccw(a,b,p);
if(onSegment(l,p))cout<<"ON_SEGMENT\n";else if(onLine(l,p))cout<<(c==2?"ONLINE_BACK":"ONLINE_FRONT")<<'\n';else cout<<(c==1?"COUNTER_CLOCKWISE":"CLOCKWISE")<<'\n';}
''',functions=['ccw','onLine','onSegment'])
add('geo_parallel','CGL_2_A','geometry.cpp',r'''
int q;cin>>q;while(q--){Point a=read_point(),b=read_point(),c=read_point(),d=read_point();Line l(a,b),m(c,d);cout<<(isParallel(l,m)?2:isOrthogonal(l,m)?1:0)<<'\n';}
''',functions=['isParallel','isOrthogonal'])
add('geo_segments_intersect','CGL_2_B','geometry.cpp',r'''
int q;cin>>q;while(q--){Point a=read_point(),b=read_point(),c=read_point(),d=read_point();cout<<isIntersect(Line(a,b),Line(c,d))<<'\n';}
''',functions=['isIntersect(Line,Line)'])
add('geo_line_cross','CGL_2_C','geometry.cpp',r'''
int q;cin>>q;while(q--){Point a=read_point(),b=read_point(),c=read_point(),d=read_point();print_point(crossPoint(Line(a,b),Line(c,d)));cout<<'\n';}
''',functions=['crossPoint(Line,Line)'])
add('geo_distance','CGL_2_D','geometry.cpp',r'''
int q;cin>>q;while(q--){Point a=read_point(),b=read_point(),c=read_point(),d=read_point();cout<<distanceBetweenSegments(Line(a,b),Line(c,d))<<'\n';}
''',functions=['distanceBetweenSegments','distanceBetweenSegmentAndPoint'])
add('geo_area','CGL_3_A','geometry.cpp',r'''
int n;cin>>n;vector<Point>p(n);for(auto&x:p)x=read_point();ld signed_area=SignedPolygonArea(p),area=PolygonArea(p);reverse(p.begin(),p.end());assert(fabsl(SignedPolygonArea(p)+signed_area)<1e-6L);cout<<area<<'\n';
''',functions=['SignedPolygonArea','PolygonArea'])
add('geo_convex','CGL_3_B','geometry.cpp',r'''
int n;cin>>n;vector<Point>p(n);for(auto&x:p)x=read_point();cout<<isConvex(p)<<'\n';
''',functions=['isConvex'])
add('geo_contained','CGL_3_C','geometry.cpp',r'''
int n;cin>>n;vector<Point>p(n);for(auto&x:p)x=read_point();int q;cin>>q;while(q--)cout<<isContained(p,read_point())<<'\n';
''',functions=['isContained'])
add('geo_hull_boundary','CGL_4_A','geometry.cpp',r'''
int n;cin>>n;vector<Point>p(n);for(auto&x:p)x=read_point();auto h=ConvexHull(p,true);int s=min_element(h.begin(),h.end(),[](Point a,Point b){return a.imag()!=b.imag()?a.imag()<b.imag():a.real()<b.real();})-h.begin();rotate(h.begin(),h.begin()+s,h.end());cout<<h.size()<<'\n';for(auto z:h)cout<<llround(z.real())<<' '<<llround(z.imag())<<'\n';
''',functions=['ConvexHull(col=true)'])
add('geo_hull','static_convex_hull','geometry.cpp',r'''
int t;cin>>t;while(t--){int n;cin>>n;vector<Point>p(n);for(auto&x:p)x=read_point();auto h=ConvexHull(p);cout<<h.size()<<'\n';for(auto z:h)cout<<llround(z.real())<<' '<<llround(z.imag())<<'\n';}
''',functions=['ConvexHull(col=false)'],judge='library_checker')
add('geo_circle_relation','CGL_7_A','geometry.cpp',r'''
Circle a=read_circle(),b=read_circle();cout<<isIntersect(a,b)<<'\n';
''',functions=['Circle(Point,r)','isIntersect(Circle,Circle)'])
add('geo_incircle','CGL_7_B','geometry.cpp',r'''
Point a=read_point(),b=read_point(),c=read_point();auto z=inCircle(a,b,c);print_point(z.p);cout<<' '<<z.r<<'\n';
''',functions=['inCircle','distanceBetweenLineAndPoint'])
add('geo_circumcircle','CGL_7_C','geometry.cpp',r'''
Point a=read_point(),b=read_point(),c=read_point();auto z=circumCircle(a,b,c);print_point(z.p);cout<<' '<<z.r<<'\n';
''',functions=['circumCircle'],whole_geometry=True)
add('geo_circle_line','CGL_7_D','geometry.cpp',r'''
Circle c=read_circle();int q;cin>>q;while(q--){Point a=read_point(),b=read_point();auto p=crossPoint(c,Line(a,b));sort_points(p);if(p.size()==1)p.push_back(p[0]);print_point(p[0]);cout<<' ';print_point(p[1]);cout<<'\n';}
''',functions=['crossPoint(Circle,Line)'])
add('geo_circle_cross','CGL_7_E','geometry.cpp',r'''
Circle a=read_circle(),b=read_circle();auto p=crossPoint(a,b);sort_points(p);if(p.size()==1)p.push_back(p[0]);print_point(p[0]);cout<<' ';print_point(p[1]);cout<<'\n';
''',functions=['crossPoint(Circle,Circle)'])
add('geo_tangent_point','CGL_7_F','geometry.cpp',r'''
Point p=read_point();Circle c=read_circle();auto z=tangentToCircle(p,c);sort_points(z);if(z.size()==1)z.push_back(z[0]);for(auto p:z){print_point(p);cout<<'\n';}
''',functions=['tangentToCircle'])
add('geo_tangents','CGL_7_G','geometry.cpp',r'''
Circle a=read_circle(),b=read_circle();auto lines=tangent(a,b);vector<Point>p;for(auto l:lines)p.push_back(l.a);sort_points(p);for(auto z:p){print_point(z);cout<<'\n';}
''',functions=['tangent'])
add('geo_angles','ITP1_10_B','geometry.cpp',r'''
ld a,b,C;cin>>a>>b>>C;Point p(a,0),q=rotate(Point(b,0),degreeToRadian(C));cout<<fabsl(cross(p,q))/2<<'\n'<<a+b+abs(p-q)<<'\n'<<fabsl(q.imag())<<'\n';
''',functions=['rotate','degreeToRadian','radianToDegree'])

MATH_CHECKS=r'''
void math_checks(){
for(ll m=1;m<=12;m++)for(ll a=-15;a<=15;a++){
ll rem=(a%m+m)%m;assert(smod(a,m)==rem);auto[g,x]=igcd(a,m);assert(g==gcd(a,m)&&smod(a*x-g,m)==0);
if(g==1)assert(smod(a*inv_mod(a,m),m)==1%m);
ll p=1%m;for(int n=0;n<9;n++){assert(pow_mod(a,n,m)==p);p=p*rem%m;}}
for(ll m=1;m<=8;m++)for(ll k=1;k<=8;k++)for(ll a=-3;a<=8;a++)for(ll b=-3;b<=8;b++){
ll L=lcm(m,k),want=-1;for(ll x=0;x<L;x++)if(x%m==(a%m+m)%m&&x%k==(b%k+k)%k){want=x;break;}
auto[r,z]=crt({a,b},{m,k});if(want<0)assert(z==0);else assert(r==want&&z==L);}
for(ll n=0;n<=6;n++)for(ll m=1;m<=6;m++)for(ll a=-6;a<=6;a++)for(ll b=-6;b<=6;b++){
ll z=0;for(ll i=0;i<n;i++){ll v=a*i+b,q=v/m;if(v%m<0)q--;z+=q;}assert(floor_sum(n,m,a,b)==z);}
}
pair<ll,ll> normalize_bezout(ll a,ll b,ll g,ll x){
pair<ll,ll>best;ll score=LLONG_MAX;for(ll k=-2;k<=2;k++){ll u=x+k*(b/g),v=(g-a*u)/b;ll z=abs(u)+abs(v);if(z<score||(z==score&&u<=v&&best.first>best.second)){score=z;best={u,v};}}return best;
}
'''
add('math_power','NTL_1_B','math.cpp',r'''
math_checks();ll m,n;cin>>m>>n;cout<<pow_mod(m,n,1000000007)<<'\n';
''',extra=MATH_CHECKS,functions=['smod','igcd','pow_mod','inv_mod','crt','floor_sum'])
add('math_bezout','NTL_1_E','math.cpp',r'''
ll a,b;cin>>a>>b;auto[g,x]=igcd(a,b);if(g==1)x=inv_mod(a,b);auto[u,v]=normalize_bezout(a,b,g,x);cout<<u<<' '<<v<<'\n';
''',extra=MATH_CHECKS,functions=['igcd','inv_mod'])
# CRT computes a multiple of a congruent to gcd(a,b) modulo b, yielding Bezout coefficients.
add('math_crt','NTL_1_E','math.cpp',r'''
ll a,b;cin>>a>>b;ll g=gcd(a,b);auto[R,M]=crt({0,g},{a,b});assert(M==lcm(a,b)&&R%a==0);auto[u,v]=normalize_bezout(a,b,g,R/a);cout<<u<<' '<<v<<'\n';
''',extra=MATH_CHECKS,functions=['crt'])

FLOW_CHECKS=r'''
struct InputEdge{int u,v,cap,cost;};
ll at_flow(const vector<pair<int,ll>>&s,int f){
if(f>s.back().first)return -1;for(int i=1;i<(int)s.size();i++)if(f<=s[i].first){auto[a,b]=s[i-1];auto[c,d]=s[i];return b+(d-b)/(c-a)*(f-a);}return 0;
}
void flow_checks(){
mt19937 rng(41982);mcf_graph<int,ll>empty;assert(empty.edges().empty());
for(int tt=0;tt<150;tt++){int n=2+rng()%4,m=1+rng()%7;vector<InputEdge>es; mcf_graph<int,ll>g(n);
for(int i=0;i<m;i++){int u=rng()%n,v=rng()%n,c=rng()%3,w=rng()%5;es.push_back({u,v,c,w});assert(g.add_edge(u,v,c,w)==i);}
map<int,ll>best;vector<int>balance(n);auto enumerate=[&](auto&&self,int i,ll cost)->void{
if(i==m){for(int v=1;v<n-1;v++)if(balance[v])return;int f=-balance[0];if(f<0||balance[n-1]!=f)return;auto it=best.find(f);if(it==best.end()||cost<it->second)best[f]=cost;return;}
auto e=es[i];for(int f=0;f<=e.cap;f++){balance[e.u]-=f;balance[e.v]+=f;self(self,i+1,cost+ll(f)*e.cost);balance[e.u]+=f;balance[e.v]-=f;}};
enumerate(enumerate,0,0);auto copy=g;auto s=copy.slope(0,n-1);assert(s.front()==make_pair(0,0LL));assert(s.back().first==best.rbegin()->first);
ll prev=-1;for(int i=1;i<(int)s.size();i++){int df=s[i].first-s[i-1].first;ll dc=s[i].second-s[i-1].second;assert(df>0&&dc%df==0&&dc/df>prev);prev=dc/df;}
for(auto[f,c]:best)assert(at_flow(s,f)==c);
auto flowed=copy.edges();ll cost=0;vector<int>bal(n);for(int i=0;i<m;i++){auto e=flowed[i],z=copy.get_edge(i);assert(e.from==es[i].u&&e.to==es[i].v&&e.cap==es[i].cap&&e.cost==es[i].cost&&e.flow==z.flow);assert(e.flow>=0&&e.flow<=e.cap);cost+=e.cost*e.flow;bal[e.from]-=e.flow;bal[e.to]+=e.flow;}
assert(cost==s.back().second&&bal[0]==-s.back().first&&bal[n-1]==s.back().first);for(int i=1;i<n-1;i++)assert(bal[i]==0);
int lim=rng()%8;copy=g;auto limited=copy.slope(0,n-1,lim);assert(limited.back().first==min(lim,s.back().first));for(int f=0;f<=limited.back().first;f++)assert(at_flow(limited,f)==best[f]);
copy=g;assert(copy.flow(0,n-1)==s.back());copy=g;assert(copy.flow(0,n-1,lim)==limited.back());
}}
'''
for unlimited in (False,True):
    add('mincost_slope'+('_unlimited' if unlimited else ''),'GRL_6_B','mincostflow.cpp',r'''
flow_checks();int n,m,f;cin>>n>>m>>f;mcf_graph<int,ll>g(n);for(int i=0,u,v,c,w;i<m;i++){cin>>u>>v>>c>>w;g.add_edge(u,v,c,w);}
'''+('auto s=g.slope(0,n-1);' if unlimited else 'auto s=g.slope(0,n-1,f);')+r'''
cout<<at_flow(s,f)<<'\n';
''',extra=FLOW_CHECKS,functions=['mcf_graph','add_edge','get_edge','edges','flow(s,t)','flow(s,t,limit)','slope(s,t)','slope(s,t,limit)'])

(OUT/'manifest.json').write_text(json.dumps(TASKS,ensure_ascii=False,indent=2)+'\n')
print('Generated',len(TASKS),'AOJ adapters and one Library Checker geometry adapter')
