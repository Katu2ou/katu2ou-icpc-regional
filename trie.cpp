template<int K,char B> struct trie{ //K文字種,Bを先頭文字とする文字列Trie
    struct N{
        int nx[K],end=0;
        N(){fill(nx,nx+K,-1);}
    };
    vector<N>t{N()};

    void insert(string s){ //文字列sを追加する
        int v=0;
        for(char c:s){
            int x=c-B; //文字cに対応する子番号 連続でない文字を取りたいならid関数を作る
            if(t[v].nx[x]<0)t[v].nx[x]=t.size(),t.emplace_back();
            v=t[v].nx[x];
        }
        t[v].end++;
    }

    int find(string s){ //文字列sに対応する頂点番号を返す(存在しなければ-1)
        int v=0;
        for(char c:s){
            v=t[v].nx[c-B];
            if(v<0)return -1;
        }
        return v;
    }
};