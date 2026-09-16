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