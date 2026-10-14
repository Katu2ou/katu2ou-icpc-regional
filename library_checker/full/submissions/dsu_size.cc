#include <bits/stdc++.h>
using namespace std;using ll=long long;


// BEGIN ORIGINAL dsu.cpp SHA256 9bd6258609e9d5fafcf11a5a0a03d7be840bef81bb469ef587240bcec72ca6c9
struct dsu{
    vector<int>p;
    dsu(int n):p(n,-1){}
    int leader(int x){return p[x]<0?x:p[x]=leader(p[x]);}
    int merge(int a,int b){
        a=leader(a);b=leader(b);
        if(a==b)return a;
        if(p[a]>p[b])swap(a,b);
        p[a]+=p[b];p[b]=a;return a;
    }
    bool same(int a,int b){return leader(a)==leader(b);}
    int size(int a){return -p[leader(a)];}
};
// END ORIGINAL dsu.cpp


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

int n,q;cin>>n>>q;dsu d(n);vector<int>counts(n,1);
while(q--){int t,u,v;cin>>t>>u>>v;if(t==0){int a=d.leader(u),b=d.leader(v);int expected=counts[a]+(a==b?0:counts[b]);int r=d.merge(u,v);counts[r]=expected;assert(d.size(u)==expected&&d.size(v)==expected);}
else cout<<d.same(u,v)<<'\n';}

}
