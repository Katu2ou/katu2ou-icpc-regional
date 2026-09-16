struct dsu{
    vector<int>p;
    dsu(int n):p(n,-1){}
    int leader(int x){return p[x]<0?x:p[x]=leader(p[x]);}
    int merge(int a,int b){
        a=leader(a);b=leader(b);
        if(a==b)return a;
        if(p[a]>p[b])swap(a,b);
        p[a]+=p[b];p[b]=a;return a;
    }
    bool same(int a,int b){return leader(a)==leader(b);}
    int size(int a){return -p[leader(a)];}
};