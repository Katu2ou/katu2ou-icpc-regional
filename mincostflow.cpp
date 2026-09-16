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