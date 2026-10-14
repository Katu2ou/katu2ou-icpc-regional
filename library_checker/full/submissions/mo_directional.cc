#include <bits/stdc++.h>
using namespace std;using ll=long long;


// BEGIN ORIGINAL mo.cpp SHA256 d151507fa0fa4187fd55e9c3f3572018ab55707bd59e59930556ca1c05538421
struct Mo{
    int n,l=0,r=0; // 現在の区間[l,r)
    vector<pair<int,int>>lr; //[l,r) addやeraseでは更新後のl,rを参照できる
    Mo(int N):n(N){} //i番目の答えが入る
    void add(int l,int r){ // [l,r) のクエリを追加
        lr.emplace_back(l,r);
    }
    template<class AL,class AR,class EL,class ER,class O>
    void build(const AL&al,const AR&ar,const EL&el,const ER&er,const O&out){
        int Q=lr.size(); if(!Q)return;
        int B=max(1,n/(int)sqrt(Q));
        vector<int>o(Q); iota(o.begin(),o.end(),0);
        sort(o.begin(),o.end(),[&](int a,int b){
            int x=lr[a].first/B,y=lr[b].first/B;
            return x!=y?x<y:
                (x&1?lr[a].second>lr[b].second:lr[a].second<lr[b].second);
        });
        l=r=0;
        for(int i:o){
            auto [L,R]=lr[i];
            while(l>L)al(--l);
            while(r<R)ar(r++);
            while(l<L)el(l++);
            while(r>R)er(--r);
            out(i);
        }
    }
    template<class A,class E,class O>
    void build(const A&add,const E&erase,const O&out){ //add(i):
        build(add,add,erase,erase,out);
    }
};
// auto add_left = [&](int i){}; 左側のi番目の要素を加える
// auto erase = [&](int i){};
// auto out = [&](int q){}; 現在のクエリ(q番目)の答えを求める

// END ORIGINAL mo.cpp


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

mt19937 rng(9284);for(int rep=0;rep<30;rep++){int n=1+rng()%30;vector<int>a(n);for(auto&v:a)v=rng()%11;vector<pair<int,int>>qs;Mo mo(n);for(int i=0;i<100;i++){int l=rng()%(n+1),r=rng()%(n+1);if(l>r)swap(l,r);qs.push_back({l,r});mo.add(l,r);}
vector<int>cnt(11);ll inversions=0;auto al=[&](int i){for(int x=0;x<a[i];x++)inversions+=cnt[x];cnt[a[i]]++;};auto ar=[&](int i){for(int x=a[i]+1;x<=10;x++)inversions+=cnt[x];cnt[a[i]]++;};
auto el=[&](int i){cnt[a[i]]--;for(int x=0;x<a[i];x++)inversions-=cnt[x];};auto er=[&](int i){cnt[a[i]]--;for(int x=a[i]+1;x<=10;x++)inversions-=cnt[x];};
mo.build(al,ar,el,er,[&](int i){auto[l,r]=qs[i];ll expected=0;for(int u=l;u<r;u++)for(int v=u+1;v<r;v++)expected+=a[u]>a[v];assert(inversions==expected);});}
int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;auto vs=a;sort(vs.begin(),vs.end());vs.erase(unique(vs.begin(),vs.end()),vs.end());for(auto&x:a)x=lower_bound(vs.begin(),vs.end(),x)-vs.begin();
Mo mo(n);for(int i=0,l,r;i<q;i++){cin>>l>>r;mo.add(l,r);}vector<int>cnt(vs.size()),ans(q);int d=0;mo.build([&](int i){d+=cnt[a[i]]++==0;},[&](int i){d-=--cnt[a[i]]==0;},[&](int i){ans[i]=d;});for(int x:ans)cout<<x<<'\n';

}
