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

// BEGIN ORIGINAL hld.cpp SHA256 35d59446461568951b4ce0ad275e03b08660a023647fc3f9286285f3ad5be528
template <typename Graph>
struct HeavyLightDecomposition{
    //Edge query:: seg.set(hld.get_edge(u[i],v[i]),w[i]);
    //auto f=[&](ll u,ll v){ans+=seg.prod(u,v);return;};
    //hld.path_query<function<void(ll,ll)>>(uc,vc,false,f);
private:
	void dfs_sz(int now,int p){
		// 1st dfs
		par[now]=p;
		depth[now]=(p==-1?0:depth[p]+1);
		size[now]=1;

		if(G[now].size()&&G[now][0]==p)swap(G[now][0],G[now].back());
		for(auto &e:G[now]){
			if(e==p)continue;
			dfs_sz(e,now);
			size[now]+=size[e];
			if(size[e]>size[G[now][0]]){
				swap(G[now][0],e);
			}
		}
	}
	void dfs_hld(int now,int p){
		id[now]=id_k;
		vertex[id_k++]=now;
		for(auto &e:G[now]){
			if(e==p)continue;
			bool heavy=(e==G[now][0]);
			head[e]=(heavy?head[now]:e);
			dfs_hld(e,now);
		}
	}
	vector<pair<int,int>>ascend(int u,int v)const{
		vector<pair<int,int>>res;
		while(head[u]!=head[v]){
			res.push_back({id[u],id[head[u]]});
			u=par[head[u]];
		}
		if(u!=v)res.push_back({id[u],id[v]+1});
		return res;
	}
	vector<pair<int,int>>descend(int u,int v)const{
		if(u==v)return {};
		if(head[u]==head[v])return {{id[u]+1,id[v]}};
		auto res=descend(u,par[head[v]]);
		res.push_back({id[head[v]],id[v]});
		return res;
	}

public:
	Graph& G;
	int root,id_k;
	vector<int>size,par,depth,head,id,vertex;
	//head[v]:=vを通るheavy pathのうち最も根側にある頂点
	//id[v]:=vを行きがけ順で何番目に通るか
	//vertex[i]:=行きがけ順でi番目に通る頂点番号
	HeavyLightDecomposition(Graph& _g,int _root=0):
		G(_g),
		root(_root),
		id_k(0),
		size(G.size(),0),
		par(G.size(),root),
		depth(G.size(),0),
		head(G.size(),0),
		id(G.size(),0),
		vertex(G.size(),0){
			dfs_sz(root,-1);
			dfs_hld(root,-1);
		}
	
		template<typename F>
		void path_query(int u,int v,bool vertex,const F& f){
			int l=lca(u,v);
			for(auto [l,r]:ascend(u,l)){
				int s=l+1,t=r;
				s>t?f(t,s):f(s,t);
			}
			if(vertex)f(id[l],id[l]+1);
			for(auto [l,r]:descend(l,v)){
				int s=l,t=r+1;
				s>t?f(t,s):f(s,t);
			}
		}

		template<typename F>
		void path_noncommutative_query(int u,int v,bool vertex,const F& f){
			int l=lca(u,v);
			for(auto [l,r]:ascend(u,l))f(l+1,r);
			if(vertex)f(id[l],id[l]+1);
			for(auto [l,r]:descend(l,v))f(l,r+1);
		}

		int level_ancestor(int v,int d){
			//vの祖先で深さdにあるものは?
			assert(depth[v]>=d);
			while(depth[head[v]]>d){
				v=par[head[v]];
			}
			return vertex[id[v]-(depth[v]-d)];
		}

		int lca(int u,int v){
			while(head[u]!=head[v]){
				if(id[u]>id[v])swap(u,v);
				v=par[head[v]];
			}
			return (id[u]<id[v]?u:v);
		}

		int dist(int u,int v){
			return depth[u]+depth[v]-2*depth[lca(u,v)];
		}
		int get_edge(int u,int v){
			if(abs(depth[u]-depth[v])!=1)return -1;
			if(depth[u]>depth[v])return id[u];
			return id[v];
		}
};

// END ORIGINAL hld.cpp

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


struct A{mint a,b;};A compose(A l,A r){return {r.a*l.a,r.a*l.b+r.b};}
struct S{A forward,backward;};S op(S l,S r){return {compose(l.forward,r.forward),compose(r.backward,l.backward)};}
S e(){return {{1,0},{1,0}};}

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;vector<A>a(n);for(auto&f:a){int x,y;cin>>x>>y;f={x,y};}vector<vector<int>>g(n);
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
HeavyLightDecomposition<vector<vector<int>>>h(g);vector<S>init(n);for(int i=0;i<n;i++)init[h.id[i]]={a[i],a[i]};segtree<S,op,e>seg(init);
while(q--){int t,u,v;ll x;cin>>t>>u>>v>>x;if(t==0){A a={v,x};seg.set(h.id[u],{a,a});}
else{A total={1,0};h.path_noncommutative_query(u,v,true,[&](int l,int r){A f=l<r?seg.prod(l,r).forward:seg.prod(r,l).backward;total=compose(total,f);});cout<<(total.a*mint(x)+total.b).val()<<'\n';}}

}
