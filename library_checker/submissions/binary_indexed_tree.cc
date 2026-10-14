#include <bits/stdc++.h>
using namespace std;
using ll = long long;


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

int n,q; cin>>n>>q; BIT<ll> bit(n);
for(int i=0;i<n;i++){ll x;cin>>x;bit.add(i,x);}
while(q--){int t,l;ll r;cin>>t>>l>>r;if(t==0)bit.add(l,r);else cout<<bit.query(l,(int)r)<<'\n';}

}
