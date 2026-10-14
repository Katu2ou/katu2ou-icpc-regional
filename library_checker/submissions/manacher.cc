#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL manacher.cpp SHA256 bd451b6e7c694dbdba1a540afc10749c6cbcefea8141e9882d3028530ceff029
template<class T> vector<int> manacher(const T&s){
    int n=s.size();vector<int>R(n);
	int i=0,j=0;
	while (i < s.size()) {
	while (i-j >= 0 && i+j < s.size() && s[i-j] == s[i+j]) ++j;
	R[i] = j; int k = 1;
	while (i-k >= 0 && k+R[i-k] < j) R[i+k] = R[i-k], ++k;
	i += k; j -= k;
	}
	return R;
}
// END ORIGINAL manacher.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

string s;cin>>s;string t="#";for(char c:s){t+=c;t+='#';}auto r=manacher(t);
for(int i=1;i+1<(int)t.size();i++)cout<<r[i]-1<<' ';cout<<'\n';

}
