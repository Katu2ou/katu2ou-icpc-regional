#include <bits/stdc++.h>
using namespace std;using ll=long long;


// BEGIN ORIGINAL maxflow.cpp SHA256 ce8402f77805182da0c9fe639700bd7442cd7c37baddb67d662245fa9222eb0b
template<class Cap> struct mf_graph{
    struct E{int to,rev; Cap cap;};
    struct edge{int from,to; Cap cap,flow;};
    int N; vector<vector<E>>g; vector<pair<int,int>>pos;

    mf_graph(int n=0):N(n),g(n){}

    int add_edge(int s,int t,Cap c){
        int a=g[s].size(),b=g[t].size()+(s==t);
        pos.push_back({s,a});
        g[s].push_back({t,b,c}); g[t].push_back({s,a,0});
        return (int)pos.size()-1;
    }

    edge get_edge(int i){
        auto [v,j]=pos[i]; auto &e=g[v][j],&r=g[e.to][e.rev];
        return {v,e.to,e.cap+r.cap,r.cap};
    }

    vector<edge> edges(){
        vector<edge>r;
        for(int i=0;i<(int)pos.size();i++)r.push_back(get_edge(i));
        return r;
    }

    void change_edge(int i,Cap c,Cap f){
        auto [v,j]=pos[i]; auto &e=g[v][j],&r=g[e.to][e.rev];
        e.cap=c-f; r.cap=f;
    }

    Cap flow(int s,int t){
        return flow(s,t,numeric_limits<Cap>::max());
    }

    Cap flow(int s,int t,Cap lim){
        Cap ans=0; vector<int>lv(N),it(N);
        while(ans<lim){
            fill(lv.begin(),lv.end(),-1);
            queue<int>q; q.push(s); lv[s]=0;
            while(q.size()){
                int v=q.front();q.pop();
                for(auto&e:g[v])
                    if(e.cap&&lv[e.to]<0)
                        lv[e.to]=lv[v]+1,q.push(e.to);
            }
            if(lv[t]<0)break;
            fill(it.begin(),it.end(),0);

            auto dfs=[&](auto&&self,int v,Cap f)->Cap{
                if(v==t)return f;
                for(int&i=it[v];i<(int)g[v].size();i++){
                    E&e=g[v][i];
                    if(e.cap&&lv[e.to]==lv[v]+1)
                        if(Cap x=self(self,e.to,min(f,e.cap))){
                            e.cap-=x;
                            g[e.to][e.rev].cap+=x;
                            return x;
                        }
                }
                return 0;
            };

            while(ans<lim){
                Cap x=dfs(dfs,s,lim-ans);
                if(!x)break;
                ans+=x;
            }
        }
        return ans;
    }

    vector<bool> min_cut(int s){
        vector<bool>z(N);
        queue<int>q; q.push(s); z[s]=1;
        while(q.size()){
            int v=q.front();q.pop();
            for(auto&e:g[v])
                if(e.cap&&!z[e.to])
                    z[e.to]=1,q.push(e.to);
        }
        return z;
    }
};
// END ORIGINAL maxflow.cpp


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);
{mf_graph<int>probe(2);int i=probe.add_edge(0,1,5);probe.change_edge(i,9,4);auto x=probe.get_edge(i);assert(x.cap==9&&x.flow==4);assert(probe.flow(0,1,3)==3);x=probe.get_edge(i);assert(x.cap==9&&x.flow==7);probe.change_edge(i,10,7);assert(probe.flow(0,1)==3);auto cut=probe.min_cut(0);assert(cut[0]&&!cut[1]);}

int l,r,m;cin>>l>>r>>m;int s=l+r,t=s+1;mf_graph<int>g(t+1),empty;assert(empty.edges().empty());
auto add=[&](int u,int v){int id=g.add_edge(u,v,0);g.change_edge(id,1,0);return id;};
for(int i=0;i<l;i++)add(s,i);for(int i=0;i<r;i++)add(l+i,t);
vector<int>ids;for(int i=0,u,v;i<m;i++){cin>>u>>v;ids.push_back(add(u,l+v));}
int f=g.flow(s,t,1);f+=g.flow(s,t);auto cut=g.min_cut(s);auto es=g.edges();int cap=0;vector<int>bal(t+1);
for(int i=0;i<(int)es.size();i++){auto e=es[i],z=g.get_edge(i);assert(e.from==z.from&&e.to==z.to&&e.cap==z.cap&&e.flow==z.flow);
assert(0<=e.flow&&e.flow<=e.cap);bal[e.from]-=e.flow;bal[e.to]+=e.flow;if(cut[e.from]&&!cut[e.to])cap+=e.cap;}
assert(cap==f&&bal[s]==-f&&bal[t]==f);for(int i=0;i<s;i++)assert(bal[i]==0);
cout<<f<<'\n';for(int id:ids){auto e=g.get_edge(id);if(e.flow)cout<<e.from<<' '<<e.to-l<<'\n';}

}
