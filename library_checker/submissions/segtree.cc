#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL modint_convolution.cpp SHA256 92d6333c009329bccaa214bbc205c02750cc60094dddb0db1740182eb6ef5454
struct mint{
    static const int M=998244353;
    int x;
    mint(long long v=0):x((v%M+M)%M){}
    int val()const{return x;}
    static int mod(){return M;}
    mint& operator+=(mint a){if((x+=a.x)>=M)x-=M;return *this;}
    mint& operator-=(mint a){if((x-=a.x)<0)x+=M;return *this;}
    mint& operator*=(mint a){x=(long long)x*a.x%M;return *this;}
    mint pow(long long n)const{
        mint a=*this,r=1;
        for(;n;n>>=1,a*=a)if(n&1)r*=a;
        return r;
    }
    mint inv()const{return pow(M-2);}
    mint& operator/=(mint a){return *this*=a.inv();}
    friend mint operator+(mint a,mint b){return a+=b;}
    friend mint operator-(mint a,mint b){return a-=b;}
    friend mint operator*(mint a,mint b){return a*=b;}
    friend mint operator/(mint a,mint b){return a/=b;}
};
using modint998244353=mint;
void ntt(vector<mint>&a,bool inv=0){
    int n=a.size();
    for(int i=1,j=0;i<n;i++){
        int b=n>>1; for(;j&b;b>>=1)j^=b; j^=b;
        if(i<j)swap(a[i],a[j]);
    }
    for(int l=2;l<=n;l<<=1){
        mint w=mint(3).pow((mint::M-1)/l);
        if(inv)w=w.inv();
        for(int i=0;i<n;i+=l){
            mint z=1;
            for(int j=0;j<l/2;j++){
                mint x=a[i+j],y=a[i+j+l/2]*z;
                a[i+j]=x+y; a[i+j+l/2]=x-y; z*=w;
            }
        }
    }
    if(inv){
        mint z=mint(n).inv();
        for(auto&x:a)x*=z;
    }
}
vector<mint> convolution(vector<mint>a,vector<mint>b){
    if(a.empty()||b.empty())return {};
    int n=a.size()+b.size()-1,z=1;
    while(z<n)z<<=1;
    a.resize(z); b.resize(z); ntt(a); ntt(b);
    for(int i=0;i<z;i++)a[i]*=b[i];
    ntt(a,1); a.resize(n);
    return a;
}
// END ORIGINAL modint_convolution.cpp

// BEGIN ORIGINAL segtree.cpp SHA256 551be17d2a6920acafff5be0d18e2e630b8517299503abbbf50305b59c0796f4
template<class S,S op(S,S),S e()>
struct segtree{
    int n,sz; vector<S>d;
    segtree(int m=0):n(1),sz(m){
        while(n<m)n*=2;
        d.assign(2*n,e());
    }
    segtree(vector<S>a):segtree(a.size()){
        copy(a.begin(),a.end(),d.begin()+n);
        for(int i=n-1;i;i--)d[i]=op(d[2*i],d[2*i+1]);
    }
    void set(int p,S x){
        for(d[p+=n]=x;p/=2;)d[p]=op(d[2*p],d[2*p+1]);
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

    template<class G> int max_right(int l,G g){
        assert(g(e())); S x=e();
        int r=mr(l,g,x,1,0,n); return r<0?sz:r;
    }
    template<class G> int mr(int a,G&g,S&x,int k,int l,int r){
        if(r<=a||sz<=l)return -1;
        if(a<=l&&r<=sz){
            S y=op(x,d[k]);
            if(g(y)){x=y;return -1;}
            if(r-l==1)return l;
        }
        int m=(l+r)/2,q=mr(a,g,x,2*k,l,m);
        return q<0?mr(a,g,x,2*k+1,m,r):q;
    }
    template<class G> int min_left(int r,G g){
        assert(g(e())); S x=e();
        int l=ml(r,g,x,1,0,n); return l<0?0:l;
    }
    template<class G> int ml(int b,G&g,S&x,int k,int l,int r){
        if(b<=l)return -1;
        if(r<=b){
            S y=op(d[k],x);
            if(g(y)){x=y;return -1;}
            if(r-l==1)return r;
        }
        int m=(l+r)/2,q=ml(b,g,x,2*k+1,m,r);
        return q<0?ml(b,g,x,2*k,l,m):q;
    }
};
// END ORIGINAL segtree.cpp


struct S{mint a,b;};S op(S l,S r){return {r.a*l.a,r.a*l.b+r.b};}S e(){return {1,0};}

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;vector<S>a(n);for(auto&v:a){int x,y;cin>>x>>y;v={x,y};}segtree<S,op,e> seg(a);
while(q--){int t,l,r;ll x;cin>>t;if(t==0){cin>>l>>r>>x;seg.set(l,{r,x});}else{cin>>l>>r>>x;auto f=seg.prod(l,r);cout<<(f.a*mint(x)+f.b).val()<<'\n';}}

}
