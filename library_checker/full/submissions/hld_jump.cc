#include <bits/stdc++.h>
using namespace std;using ll=long long;


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


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

int n,q;cin>>n>>q;vector<vector<int>>g(n);vector<pair<int,int>>edges;
for(int i=1,u,v;i<n;i++){cin>>u>>v;g[u].push_back(v);g[v].push_back(u);edges.push_back({u,v});}
HeavyLightDecomposition<vector<vector<int>>>h(g);
for(auto[u,v]:edges){int child=h.par[u]==v?u:v;assert(h.get_edge(u,v)==h.id[child]);assert(h.get_edge(v,u)==h.id[child]);}
while(q--){int s,t,k;cin>>s>>t>>k;int l=h.lca(s,t),d=h.dist(s,t);if(k>d){cout<<-1<<'\n';continue;}
int z=k<=h.depth[s]-h.depth[l]?h.level_ancestor(s,h.depth[s]-k):h.level_ancestor(t,h.depth[t]-(d-k));cout<<z<<'\n';}

}
