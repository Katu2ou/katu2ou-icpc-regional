#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL hld_short.cpp SHA256 202d35f1d43d34ceb4578492f0a1f1fc072c2f707c95e627767610d27edef85d
#include <bits/stdc++.h>
using namespace std;
vector<vector<int>> g;
vector<int> sz, in, out, nxt;
int t;
void dfs_sz(int v = 0) {
    sz[v] = 1;
    for(auto &u: g[v]) {
        dfs_sz(u); sz[v] += sz[u];
        if(sz[u] > sz[g[v][0]]) { swap(u, g[v][0]); }
    }
}
void dfs_hld(int v = 0) {
    in[v] = t++;
    for(auto u: g[v]) {
        nxt[u] = (u == g[v][0] ? nxt[v] : u);
        dfs_hld(u);
    }
    out[v] = t;
}

// END ORIGINAL hld_short.cpp

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

int n,q;cin>>n>>q;vector<ll>a(n);for(auto&x:a)cin>>x;g.resize(n);sz.resize(n);in.resize(n);out.resize(n);nxt.resize(n);
for(int i=1,p;i<n;i++){cin>>p;g[p].push_back(i);}dfs_sz();dfs_hld();BIT<ll>bit(n);for(int i=0;i<n;i++)bit.add(in[i],a[i]);
while(q--){int type,u;cin>>type>>u;if(type==0){ll x;cin>>x;bit.add(in[u],x);}else cout<<bit.query(in[u],out[u])<<'\n';}

}
