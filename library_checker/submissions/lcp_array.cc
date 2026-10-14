#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL suffix_array.cpp SHA256 8f5f09d7155bd06123e9b61524c3e08f74f506eb2b12c634517f4a2b20accb95
template<class T> vector<int> suffix_array(vector<T>s){
    int n=s.size(); if(!n)return {};
    int N=n+1; vector<int>p(N),c(N),q(N),nc(N),cnt(N);
    p[0]=n; iota(p.begin()+1,p.end(),0);
    sort(p.begin()+1,p.end(),[&](int i,int j){return s[i]<s[j];});
    for(int i=1;i<N;i++)c[p[i]]=i==1?1:c[p[i-1]]+(s[p[i-1]]!=s[p[i]]);
    for(int k=1;k<N;k*=2){
        for(int i=0;i<N;i++)q[i]=(p[i]-k+N)%N;
        fill(cnt.begin(),cnt.end(),0);
        for(int x:q)cnt[c[x]]++;
        for(int i=1;i<N;i++)cnt[i]+=cnt[i-1];
        for(int i=N-1;i>=0;i--)p[--cnt[c[q[i]]]]=q[i];
        nc[p[0]]=0;
        for(int i=1;i<N;i++)nc[p[i]]=nc[p[i-1]]+
            (c[p[i-1]]!=c[p[i]]||c[(p[i-1]+k)%N]!=c[(p[i]+k)%N]);
        c.swap(nc);
    }
    return {p.begin()+1,p.end()};
}
vector<int> suffix_array(string s){
    return suffix_array(vector<unsigned char>(s.begin(),s.end()));
}
vector<int> suffix_array(vector<int>s,int){
    return suffix_array<int>(move(s));
}
// END ORIGINAL suffix_array.cpp

// BEGIN ORIGINAL lcp_array.cpp SHA256 80307d9a7e20a8e172b0b66c5fa89a6b613536daeb111e85dbc6963810e1dd83
template<class T> vector<int> lcp_array(const T&s,const vector<int>&sa){
    int n=s.size(),h=0; vector<int>r(n),l(n-1);
    for(int i=0;i<n;i++)r[sa[i]]=i;
    for(int i=0;i<n;i++)if(r[i]){
        int j=sa[r[i]-1];
        while(max(i,j)+h<n&&s[i+h]==s[j+h])h++;
        l[r[i]-1]=h; if(h)h--;
    }
    return l;
}
// END ORIGINAL lcp_array.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

string s;cin>>s;auto sa=suffix_array(s);auto lcp=lcp_array(s,sa);ll ans=ll(s.size())*(s.size()+1)/2;for(int x:lcp)ans-=x;cout<<ans<<'\n';

}
