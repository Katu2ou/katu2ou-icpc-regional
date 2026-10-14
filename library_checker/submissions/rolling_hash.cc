#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL rolling_hash.cpp SHA256 a930917443c03ab6b3e359237dda5c02e322e09e64a40318bd1ce0ad63f3cf85
struct rolling_hash{
    using ull=unsigned long long; using u128=__uint128_t;
    static constexpr ull M=(1ULL<<61)-1;
    static ull mul(ull a,ull b){
        u128 x=(u128)a*b;
        ull y=(x>>61)+(x&M);
        return y>=M?y-M:y;
    }
    inline static ull B=
        chrono::steady_clock::now().time_since_epoch().count()%(M-256)+256;
    vector<ull>h,p;
    rolling_hash(const string&s):h(s.size()+1),p(s.size()+1,1){
        for(int i=0;i<(int)s.size();i++)
            p[i+1]=mul(p[i],B),
            h[i+1]=(mul(h[i],B)+(unsigned char)s[i]+1)%M;
    }
    ull get(int l,int r)const{
        ull x=h[r]+M-mul(h[l],p[r-l]);
        return x>=M?x-M:x;
    }
};
// END ORIGINAL rolling_hash.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

string s;cin>>s;int n=s.size();rolling_hash h(s);for(int i=0;i<n;i++){int l=0,r=n-i+1;while(r-l>1){int m=(l+r)/2;if(h.get(0,m)==h.get(i,i+m))l=m;else r=m;}cout<<l<<' ';}cout<<'\n';

}
