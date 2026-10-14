#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL scc.cpp SHA256 04ef8d63ea4eca2d30becc10cf2cd7bf3e4762d4cfd9c0a5762ab88b8952e478
struct scc_graph{
    int n;
    vector<vector<int>>g,rg;
    scc_graph(int n):n(n),g(n),rg(n){}
    void add_edge(int a,int b){
        g[a].push_back(b); rg[b].push_back(a);
    }
    vector<vector<int>> scc(){
        vector<int>vis(n),ord;
        for(int s=0;s<n;s++)if(!vis[s]){
            stack<pair<int,int>>q;
            q.push({s,0}); vis[s]=1;
            while(q.size()){
                auto &[v,i]=q.top();
                if(i<(int)g[v].size()){
                    int u=g[v][i++];
                    if(!vis[u])vis[u]=1,q.push({u,0});
                }else ord.push_back(v),q.pop();
            }
        }
        fill(vis.begin(),vis.end(),0);
        reverse(ord.begin(),ord.end());
        vector<vector<int>>res;
        for(int s:ord)if(!vis[s]){
            res.push_back({});
            stack<int>q; q.push(s); vis[s]=1;
            while(q.size()){
                int v=q.top();q.pop();
                res.back().push_back(v);
                for(int u:rg[v])
                    if(!vis[u])vis[u]=1,q.push(u);
            }
        }
        return res;
    }
};
// END ORIGINAL scc.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,m;cin>>n>>m;scc_graph g(n);for(int i=0,u,v;i<m;i++){cin>>u>>v;g.add_edge(u,v);}auto components=g.scc();cout<<components.size()<<'\n';for(auto&c:components){cout<<c.size();for(int v:c)cout<<' '<<v;cout<<'\n';}

}
