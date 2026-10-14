#include <bits/stdc++.h>
using namespace std;using ll=long long;


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


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

int n,q;string s;cin>>n>>q>>s;BIT<int>b;b.init(n);vector<int>a(n);for(int i=0;i<n;i++){a[i]=s[i]-'0';b.add(i,a[i]);}
while(q--){int t,k;cin>>t>>k;if(t<2){int v=t==0;b.add(k,v-a[k]);a[k]=v;}else if(t==2)cout<<b.query(k,k+1)<<'\n';
else if(t==3){int target=b.query(k-1)+1;cout<<(target>b.query(n-1)?-1:b.lower_bound(target))<<'\n';}
else{int target=b.query(k);cout<<(target==0?-1:b.lower_bound(target))<<'\n';}}

}
