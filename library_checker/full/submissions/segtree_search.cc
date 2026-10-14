#include <bits/stdc++.h>
using namespace std;using ll=long long;


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

int op(int a,int b){return a+b;}int e(){return 0;}
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

int n,q;string s;cin>>n>>q>>s;segtree<int,op,e>seg(n);segtree<int,op,e>empty;assert(empty.all_prod()==0);
for(int i=0;i<n;i++)seg.set(i,s[i]-'0');
while(q--){int t,k;cin>>t>>k;if(t<2)seg.set(k,t==0);else if(t==2)cout<<seg.get(k)<<'\n';
else if(t==3){int r=seg.max_right(k,[](int x){return x==0;});cout<<(r==n?-1:r)<<'\n';}
else cout<<seg.min_left(k+1,[](int x){return x==0;})-1<<'\n';}
assert(seg.all_prod()==seg.prod(0,n));

}
