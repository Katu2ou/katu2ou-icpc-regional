/*
    - n頂点の木Tが与えられている

    - RerootingDP<E,V,merge,e,put_edge,put_vertex> g(n) : コンストラクタ O(n)
        E : 親のdpの計算をする際に用いる値の型
        V : 頂点のdpの値の型
        E put_edge (V v, int i) : 頂点vを根とする部分木のdpの値に辺iを追加した時に得られる値
        E merge(E e1, E e2) :  2つの子について求めたput_edgeの値を結合する
            mergeは可換モノイドによる結合を要求される
        E e() : mergeの単位元を返す関数
        V put_vertex(E e, int u) : 全ての子についてmergeした値から，頂点uのdp[u]を計算する
    - void add_edge(int u, int v, int idx, int xdi) : 頂点u->vの辺番号idxの辺と
        頂点v->uの辺番号xdiの辺を追加する (ちょうどn-1回呼ぶ．ならしO(1))
    - V bulid(int v): 頂点vを根とした時の，dp[v]の値を返す (1回までしか呼べない O(n))
    - vector<V> reroot(): 長さnの配列(i番目には，頂点iを根とした時の，dp[i]の値)を返す
        (先にbuildをちょうど1回呼んでいる必要あり O(n))
*/

template <class E, class V, E (*merge)(E, E), E (*e)(), E (*put_edge)(V, int), V (*put_vertex)(E, int)>
struct RerootingDP {
    struct edge {
        int to, idx, xdi;
    };
    RerootingDP(int n_ = 0) : n(n_), inner_edge_id(0) {
        es.resize(2*n-2);
        start.resize(2*n-2);
        if (n == 1) es_build();
    }
    void add_edge(int u, int v, int idx, int xdi){
        start[inner_edge_id] = u;
        es[inner_edge_id] = {v,idx,xdi};
        inner_edge_id++;
        start[inner_edge_id] = v;
        es[inner_edge_id] = {u,xdi,idx};
        inner_edge_id++;
        if (inner_edge_id == 2*n-2){
            es_build();
        }
    }
    vector<V> build(int root_ = 0){
        root = root_;
        vector<V> subdp(n); subdp[0] = put_vertex(e(),0);
        outs.resize(n);
        vector<int> geta(n+1,0);
        for (int i = 0; i < n; i++) geta[i+1] = start[i+1] - start[i] - 1;
        geta[root+1]++;
        for (int i = 0; i < n; i++) geta[i+1] += geta[i];
        auto dfs = [&](auto sfs, int v, int f) -> void {
            E val = e();
            for (int i = start[v]; i < start[v+1]; i++){
                if (es[i].to == f){
                    swap(es[start[v+1]-1],es[i]);
                }
                if (es[i].to == f) continue;
                sfs(sfs,es[i].to,v);
                E nval = put_edge(subdp[es[i].to],es[i].idx);
                outs[geta[v]++] = nval;
                val = merge(val,nval);
            }
            subdp[v] = put_vertex(val, v);
        };
        dfs(dfs,root,-1);
        return subdp;
    }
    vector<V> reroot(){
        vector<E> reverse_edge(n);
        reverse_edge[root] = e();
        vector<V> answers(n);
        auto dfs = [&](auto sfs, int v) -> void {
            int le = outs_start(v);
            int ri = outs_start(v+1);
            int siz = ri - le;
            vector<E> rui(siz+1);
            rui[siz] = e();
            for (int i = siz-1; i >= 0; i--){
                rui[i] = merge(outs[le+i],rui[i+1]);
            }
            answers[v] = put_vertex(merge(rui[0],reverse_edge[v]),v);
            E lui = e();
            for (int i = 0; i < siz; i++){
                V rdp = put_vertex(merge(merge(lui,rui[i+1]),reverse_edge[v]),v);
                reverse_edge[es[start[v]+i].to] = put_edge(rdp,es[start[v]+i].xdi);
                lui = merge(lui,outs[le+i]);
                sfs(sfs,es[start[v]+i].to);
            }
        };
        dfs(dfs,root);
        return answers;
    }
    private:
    int n, root, inner_edge_id;
    vector<E> outs;
    vector<edge> es;
    vector<int> start;
    int outs_start(int v){
        int res = start[v] - v;
        if (root < v) res++;
        return res;
    }
    void es_build(){
        vector<edge> nes(2*n-2);
        vector<int> nstart(n+2,0);
        for (int i = 0; i < 2*n-2; i++) nstart[start[i]+2]++;
        for (int i = 0; i < n; i++) nstart[i+1] += nstart[i];
        for (int i = 0; i < 2*n-2; i++) nes[nstart[start[i]+1]++] = es[i];
        swap(es,nes);
        swap(start,nstart);
    }
};
