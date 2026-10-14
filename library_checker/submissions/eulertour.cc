#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL eulertour.cpp SHA256 5e273cfd2b94f79a448e1fad990e91d7674a0f0bfdc960e4a1436d34ea438067
struct EulerTour{
    int N;
    vector<int>vs,depth,id,id_depth,id_rev;
    vector<vector<int>> G;
    EulerTour(int n,vector<vector<int>>&g){
        N=n; G=g;
        vs.resize(2*N-1);//DFSでの訪問順
        depth.resize(2*N-1);// 訪問順での根からの深さ
        id.resize(N); //id[i]:iが初めて現れるindex
        id_depth.resize(N);//頂点iの深さ
		id_rev.resize(N); //iが最後に現れるindex
        int k=0; dfs(0,-1,0,k);
    }
    void dfs(int now,int par,int d,int& k){
        id[now]=k;
		id_rev[now]=k;
        id_depth[now]=d;
        vs[k]=now;
        depth[k++]=d;
        for(auto e:G[now]){
            if(e==par)continue;
            dfs(e,now,d+1,k);
			id_rev[now]=k;
            vs[k]=now;
            depth[k++]=d;
        }
    }
};
//int lca(int u,int v){ return vs[seg.prod(min(id[u],id[v]),max(id[u],id[v])+1).idx];}
// END ORIGINAL eulertour.cpp

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

using S=pair<int,int>; S op(S a,S b){return min(a,b);} S e(){return {INT_MAX,INT_MAX};}
int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;vector<vector<int>>g(n);
for(int i=1,p;i<n;i++){cin>>p;g[p].push_back(i);g[i].push_back(p);}
EulerTour tour(n,g);vector<S>a(2*n-1);for(int i=0;i<2*n-1;i++)a[i]={tour.depth[i],tour.vs[i]};segtree<S,op,e> seg(a);
while(q--){int u,v;cin>>u>>v;int l=tour.id[u],r=tour.id[v];if(l>r)swap(l,r);cout<<seg.prod(l,r+1).second<<'\n';}

}
