#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL template.cpp SHA256 c8101c0f16a17523ba4d8b00917b91632527cbf6504cae13e031c952daeac82a
namespace original_template {
#include<bits/stdc++.h>
using namespace std;
#define rep(i,n)
#define all(a) begin(a), end(a)
using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
template<class T>bool chmax(T& a, const T& b) { if (a < b) { a = b; return 1; } return 0; }
template<class T>bool chmin(T& a, const T& b) { if (b < a) { a = b; return 1; } return 0; }

void solve() {

}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	int TT; TT = 1; //cin >> TT;
	solve();
}
}

// END ORIGINAL template.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

ll a,b;cin>>a>>b;ll lo=a,hi=a;original_template::chmin(lo,b);original_template::chmax(hi,b);cout<<lo+hi<<'\n';

}
