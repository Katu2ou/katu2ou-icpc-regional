template<class S,S op(S,S),S e()>
struct segtree{
    int n;
    vector<S> d;
    segtree(int n):n(n),d(2*n,e()){}
    segtree(vector<S> a):segtree(a.size()){
        copy(a.begin(),a.end(),d.begin()+n);
        for(int i=n-1;i;i--) d[i]=op(d[i*2],d[i*2+1]);
    }
    void set(int p,S x){
        for(d[p+=n]=x;p/=2;)
            d[p]=op(d[p*2],d[p*2+1]);
    }
    S get(int p){return d[p+n];}
    S prod(int l,int r){
        S x=e(),y=e();
        for(l+=n,r+=n;l<r;l/=2,r/=2){
            if(l&1)x=op(x,d[l++]);
            if(r&1)y=op(d[--r],y);
        }
        return op(x,y);
    }
    S all_prod(){return d[1];}
};