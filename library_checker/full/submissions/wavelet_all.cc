#include <bits/stdc++.h>
using namespace std;using ll=long long;


// BEGIN ORIGINAL wavelet_matrix.cpp SHA256 189730cd8f8f10c48fb39d4f31c6cee56fa50cbd98720bec127f02858bc3bc3a
template<class T,int LOG=numeric_limits<make_unsigned_t<T>>::digits> //LOGは log_2 A_i より大きい値を選択 
struct WaveletMatrix{ //WaveletMatrix<int,10> など
    using U=make_unsigned_t<T>;
    int n; array<int,LOG>mid{}; array<vector<int>,LOG>s;
    WaveletMatrix(const vector<T>&A):n(A.size()){ // 非負整数列Aから構築
        vector<U>a(n),b(n);
        for(int i=0;i<n;i++){
            if constexpr(is_signed_v<T>) assert(A[i]>=0);
            a[i]=(U)A[i];
        }
        for(int h=LOG-1;h>=0;h--){
            s[h].assign(n+1,0);
            for(int i=0;i<n;i++)s[h][i+1]=s[h][i]+(a[i]>>h&1);
            mid[h]=n-s[h][n];
            int p=0,q=mid[h];
            for(U x:a)((x>>h&1)?b[q++]:b[p++])=x;
            a.swap(b);
        }
    }
    T access(int k)const{ // 元配列のA[k]を返す
        U x=0;
        for(int h=LOG-1;h>=0;h--){
            int o=s[h][k],b=s[h][k+1]-o;
            if(b)x|=U(1)<<h,k=mid[h]+o;
            else k-=o;
        }
        return (T)x;
    }
    int rank(T x,int l,int r)const{ // [l,r)にあるxの個数
        if constexpr(is_signed_v<T>)if(x<0)return 0;
        U y=(U)x;
        if constexpr(LOG<numeric_limits<U>::digits)if(y>>LOG)return 0;
        for(int h=LOG-1;h>=0;h--){
            int L=s[h][l],R=s[h][r];
            if(y>>h&1)l=mid[h]+L,r=mid[h]+R;
            else l-=L,r-=R;
        }
        return r-l;
    }
    int rank(T x,int r)const{ // [0,r)にあるxの個数
        return rank(x,0,r);
    }
    T kth(int l,int r,int k)const{ // [l,r)のk番目に小さい値 (0-indexed)
        U x=0;
        for(int h=LOG-1;h>=0;h--){
            int L=s[h][l],R=s[h][r],z=r-l-(R-L);
            if(k<z)l-=L,r-=R;
            else x|=U(1)<<h,k-=z,l=mid[h]+L,r=mid[h]+R;
        }
        return (T)x;
    }
    T kth_largest(int l,int r,int k)const{ // [l,r)のk番目に大きい値
        return kth(l,r,r-l-1-k);
    }
    int less(int l,int r,T x)const{ // [l,r)にあるx未満の値の個数
        if constexpr(is_signed_v<T>)if(x<=0)return 0;
        U y=(U)x;
        if constexpr(LOG<numeric_limits<U>::digits)if(y>>LOG)return r-l;
        int c=0;
        for(int h=LOG-1;h>=0;h--){
            int L=s[h][l],R=s[h][r],z=r-l-(R-L);
            if(y>>h&1)c+=z,l=mid[h]+L,r=mid[h]+R;
            else l-=L,r-=R;
        }
        return c;
    }
    int range_freq(int l,int r,T lo,T hi)const{ // [l,r)でlo<=A[i]<hiの個数
        return less(l,r,hi)-less(l,r,lo);
    }
    optional<T> prev_value(int l,int r,T x)const{ // [l,r)のx未満の最大値
        int k=less(l,r,x);
        if(!k)return nullopt;
        return kth(l,r,k-1);
    }
    optional<T> next_value(int l,int r,T x)const{ // [l,r)のx以上の最小値
        int k=less(l,r,x);
        if(k==r-l)return nullopt;
        return kth(l,r,k);
    }
};
// END ORIGINAL wavelet_matrix.cpp


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

int n,q;cin>>n>>q;vector<int>a(n);for(auto&x:a)cin>>x;WaveletMatrix<int,30>w(a);
for(int i=0;i<n;i++)assert(w.access(i)==a[i]);
for(int i=0;i<q;i++){int l,r,k;cin>>l>>r>>k;int z=w.kth_largest(l,r,r-l-1-k);
assert(w.kth(l,r,k)==z);assert(w.less(l,r,z)<=k&&k<w.less(l,r,z+1));
assert(w.rank(z,l,r)==w.rank(z,r)-w.rank(z,l));
assert(w.range_freq(l,r,z,z+1)==w.rank(z,l,r));
assert(w.prev_value(l,r,z+1)==optional<int>(z));assert(w.next_value(l,r,z)==optional<int>(z));
if(i<100&&r-l<=10000){vector<int>b(a.begin()+l,a.begin()+r);sort(b.begin(),b.end());
for(int x:{-1,0,z,z+1,1000000001,1073741824}){
auto lo=lower_bound(b.begin(),b.end(),x);assert(w.less(l,r,x)==lo-b.begin());
assert(w.rank(x,l,r)==upper_bound(b.begin(),b.end(),x)-lo);
optional<int>p,v;if(lo!=b.begin())p=*prev(lo);if(lo!=b.end())v=*lo;
assert(w.prev_value(l,r,x)==p);assert(w.next_value(l,r,x)==v);
}}
cout<<z<<'\n';}

}
