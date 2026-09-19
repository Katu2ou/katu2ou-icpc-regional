template<int B=30> struct binary_trie{ //0以上2^B未満の整数を管理するBinary Trie
    struct N{int nx[2]={-1,-1},cnt=0;};
    vector<N>t{N()};

    void add(long long x,int d=1){ //xの個数をdだけ増減する
        int v=0;t[v].cnt+=d;
        for(int i=B-1;i>=0;i--){
            int b=x>>i&1;
            if(t[v].nx[b]<0)t[v].nx[b]=t.size(),t.emplace_back();
            v=t[v].nx[b]; t[v].cnt+=d;
        }
    }

    void insert(long long x){ //xを1個追加する
        add(x,1);
    }

    void erase(long long x){ //xを1個削除する
        add(x,-1);
    }

    long long min_xor(long long x){ //集合内のyについてx xor yの最小値を返す
        int v=0; long long r=0;
        for(int i=B-1;i>=0;i--){
            int b=x>>i&1,u=t[v].nx[b];
            if(u<0||!t[u].cnt)b^=1,r|=1LL<<i;
            v=t[v].nx[b];
        }
        return r;
    }

    long long max_xor(long long x){ //集合内のyについてx xor yの最大値を返す
        int v=0; long long r=0;
        for(int i=B-1;i>=0;i--){
            int b=x>>i&1,u=t[v].nx[b^1];
            if(u>=0&&t[u].cnt)b^=1,r|=1LL<<i;
            v=t[v].nx[b];
        }
        return r;
    }
};