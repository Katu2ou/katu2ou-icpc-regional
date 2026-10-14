#include <bits/stdc++.h>
using namespace std;using ll=long long;
template<class Cap,class Cost> struct mcf_graph{
    struct E{int to,rev; Cap cap; Cost cost;};
    struct edge{int from,to; Cap cap,flow; Cost cost;};
    int N; vector<vector<E>>g; vector<pair<int,int>>pos;

    mcf_graph(int n=0):N(n),g(n){}

    int add_edge(int s,int t,Cap c,Cost w){
        int a=g[s].size(),b=g[t].size()+(s==t);
        pos.push_back({s,a});
        g[s].push_back({t,b,c,w});
        g[t].push_back({s,a,0,-w});
        return (int)pos.size()-1;
    }

    edge get_edge(int i){
        auto [v,j]=pos[i]; auto &e=g[v][j],&r=g[e.to][e.rev];
        return {v,e.to,e.cap+r.cap,r.cap,e.cost};
    }

    vector<edge> edges(){
        vector<edge>r;
        for(int i=0;i<(int)pos.size();i++)r.push_back(get_edge(i));
        return r;
    }

    pair<Cap,Cost> flow(int s,int t){
        return flow(s,t,numeric_limits<Cap>::max());
    }

    pair<Cap,Cost> flow(int s,int t,Cap lim){
        return slope(s,t,lim).back();
    }

    vector<pair<Cap,Cost>> slope(int s,int t){
        return slope(s,t,numeric_limits<Cap>::max());
    }

    vector<pair<Cap,Cost>> slope(int s,int t,Cap lim){
        const Cost INF=numeric_limits<Cost>::max();
        vector<Cost>h(N),d(N);
        vector<int>pv(N),pe(N);

        Cap f=0;
        Cost c=0,last=-1;
        vector<pair<Cap,Cost>>res{{0,0}};

        while(f<lim){
            fill(d.begin(),d.end(),INF);
            d[s]=0;

            priority_queue<
                pair<Cost,int>,
                vector<pair<Cost,int>>,
                greater<pair<Cost,int>>
            >q;
            q.push({0,s});

            while(q.size()){
                auto [dv,v]=q.top();q.pop();
                if(dv!=d[v])continue;

                for(int i=0;i<(int)g[v].size();i++){
                    E&e=g[v][i];
                    if(!e.cap)continue;

                    __int128 nd=(__int128)dv+e.cost+h[v]-h[e.to];
                    if(nd<d[e.to]){
                        d[e.to]=(Cost)nd;
                        pv[e.to]=v; pe[e.to]=i;
                        q.push({(Cost)nd,e.to});
                    }
                }
            }

            if(d[t]==INF)break;

            for(int v=0;v<N;v++)
                if(d[v]<INF)h[v]+=d[v];

            Cap x=lim-f;
            for(int v=t;v!=s;v=pv[v])
                x=min(x,g[pv[v]][pe[v]].cap);

            for(int v=t;v!=s;v=pv[v]){
                E&e=g[pv[v]][pe[v]];
                e.cap-=x;
                g[v][e.rev].cap+=x;
            }

            Cost w=h[t]-h[s];
            f+=x;
            c+=x*w;

            if(last==w)res.pop_back();
            res.push_back({f,c});
            last=w;
        }

        return res;
    }
};

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


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);flow_checks();
int n;cin>>n;vector<vector<ll>>a(n,vector<ll>(n));vector<ll>low(n);ll offset=0;
for(int i=0;i<n;i++){for(auto&x:a[i])cin>>x;low[i]=*min_element(a[i].begin(),a[i].end());offset+=low[i];}
int s=2*n,t=s+1;mcf_graph<int,ll>g(t+1);for(int i=0;i<n;i++){g.add_edge(s,i,1,0);g.add_edge(n+i,t,1,0);}
vector<vector<int>>ids(n,vector<int>(n));for(int i=0;i<n;i++)for(int j=0;j<n;j++)ids[i][j]=g.add_edge(i,n+j,1,a[i][j]-low[i]);
auto curve=g.slope(s,t);assert(curve.back().first==n);cout<<at_flow(curve,n)+offset<<'\n';
for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(g.get_edge(ids[i][j]).flow)cout<<j<<' ';cout<<'\n';
}
