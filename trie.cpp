template<int K,char B> struct trie{ //文字種類数 / 最初の文字
    struct N{
        int nx[K],end=0;
        N(){fill(nx,nx+K,-1);}
    };
    vector<N>t{N()};
    void insert(string s){
        int v=0;
        for(char c:s){
            int x=c-B; //xがidにあたる 連続でないならid(c)に変更
            if(t[v].nx[x]<0)t[v].nx[x]=t.size(),t.emplace_back();
            v=t[v].nx[x];
        }
        t[v].end++;
    }
    int find(string s){
        int v=0;
        for(char c:s){
            v=t[v].nx[c-B];
            if(v<0)return -1;
        }
        return v;
    }
};