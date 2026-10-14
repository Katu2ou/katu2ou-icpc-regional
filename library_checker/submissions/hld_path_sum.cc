#include <bits/stdc++.h>
using namespace std;
using ll = long long;


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

// BEGIN ORIGINAL binary_indexed_tree.cpp SHA256 cfa80d0f4b90eb57ce2c4b07534ad1233508e649657a02fa5b00225448d52354
#pragma once
#include <bits/stdc++.h>
using namespace std;
template<typename T>
class BIT {
	int size;
	vector<T>dat;
public:
	BIT() {}
	BIT(int n) { init(n); }
	void init(int n) { size = n; dat.assign(size + 1, 0); }
	void add(int i, T x) {
		i++;
		while (i <= size) { dat[i] += x; i += i & -i; }
	}
	//[l,r)
	void add(int l, int r, T x) { add(l, x); add(r, -x); }
	//[0,i]
	T query(int i) {
		i++; T sum = 0;
		while (i > 0) { sum += dat[i]; i -= i & -i; }
		return sum;
	}
	//[l,r)
	T query(int l, int r) { return query(r - 1) - query(l - 1); }
	//[0,x] >= w
	int lower_bound(T w) {
		if (w <= 0) return 0;
		int x = 0, k = 1;
		while ((k << 1) < size) k <<= 1;
		for (; k > 0; k >>= 1) {
			if (x + k <= size && dat[x + k] < w) { w -= dat[x + k]; x += k; }
		}
		return x;
	}
};//size, 0-indexed

// END ORIGINAL binary_indexed_tree.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;vector<ll>a(n);for(auto&x:a)cin>>x;vector<vector<int>>g(n);
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
HeavyLightDecomposition<vector<vector<int>>> h(g);BIT<ll> bit(n);for(int i=0;i<n;i++)bit.add(h.id[i],a[i]);
while(q--){int t,u;ll v;cin>>t>>u>>v;if(t==0)bit.add(h.id[u],v);else{ll ans=0;h.path_query(u,(int)v,true,[&](int l,int r){ans+=bit.query(l,r);});cout<<ans<<'\n';}}

}
