template<class S,S op(S,S),S e(),class F,
         S mapping(F,S),F composition(F,F),F id()>
struct lazy_segtree{
    int n; vector<S>d; vector<F>lz;
    lazy_segtree(int m=0):n(1){
        while(n<m)n*=2;
        d.assign(2*n,e()); lz.assign(2*n,id());
    }
    lazy_segtree(vector<S>a):lazy_segtree(a.size()){
        copy(a.begin(),a.end(),d.begin()+n);
        for(int i=n-1;i;i--)d[i]=op(d[2*i],d[2*i+1]);
    }
    void all(int k,F f){
        d[k]=mapping(f,d[k]);
        lz[k]=composition(f,lz[k]);
    }
    void push(int k){
        all(2*k,lz[k]); all(2*k+1,lz[k]); lz[k]=id();
    }
    void set(int p,S x){set(p,x,1,0,n);}
    void set(int p,S x,int k,int l,int r){
        if(r-l==1){d[k]=x;lz[k]=id();return;}
        push(k); int m=(l+r)/2;
        p<m?set(p,x,2*k,l,m):set(p,x,2*k+1,m,r);
        d[k]=op(d[2*k],d[2*k+1]);
    }
    S get(int p){return prod(p,p+1);}
    S prod(int a,int b){return prod(a,b,1,0,n);}
    S prod(int a,int b,int k,int l,int r){
        if(r<=a||b<=l)return e();
        if(a<=l&&r<=b)return d[k];
        push(k); int m=(l+r)/2;
        return op(prod(a,b,2*k,l,m),prod(a,b,2*k+1,m,r));
    }
    S all_prod(){return d[1];}
    void apply(int p,F f){apply(p,p+1,f);}
    void apply(int a,int b,F f){apply(a,b,f,1,0,n);}
    void apply(int a,int b,F f,int k,int l,int r){
        if(r<=a||b<=l)return;
        if(a<=l&&r<=b){all(k,f);return;}
        push(k); int m=(l+r)/2;
        apply(a,b,f,2*k,l,m); apply(a,b,f,2*k+1,m,r);
        d[k]=op(d[2*k],d[2*k+1]);
    }
};