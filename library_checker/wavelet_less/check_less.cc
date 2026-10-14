// Differential verification of the unchanged original less() implementation.
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <cassert>
using namespace std;
#include "../../wavelet_matrix.cpp"

struct Stats { uint64_t arrays=0,queries=0,elements=0; } total;
template<class T> string value_string(T x) {
    if constexpr(is_signed_v<T>) return to_string((long long)x);
    else return to_string((unsigned long long)x);
}
template<class T> string type_name() {
    return string(is_signed_v<T>?"i":"u")+to_string(numeric_limits<make_unsigned_t<T>>::digits);
}
template<class T,int LOG> constexpr make_unsigned_t<T> data_max() {
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    static_assert(0<=LOG&&LOG<=W);
    U mask;
    if constexpr(LOG==W) mask=numeric_limits<U>::max();
    else mask=U((U(1)<<LOG)-1);
    return min(mask,U(numeric_limits<T>::max()));
}
template<class T,int LOG> void check(const WaveletMatrix<T,LOG>& w,const vector<T>& a,int l,int r,T x,const string& context) {
    ++total.queries;total.elements+=r-l;
    // Independent oracle: no wavelet queries, bit partitions, or prefix sums.
    int expected=0;for(int i=l;i<r;i++)expected+=a[i]<x;
    int actual=w.less(l,r,x);
    if(actual!=expected) {
        cerr<<"FAIL "<<type_name<T>()<<" LOG="<<LOG<<" "<<context<<" l="<<l<<" r="<<r
            <<" x="<<value_string(x)<<" expected="<<expected<<" actual="<<actual<<" n="<<a.size()<<" A=[";
        for(size_t i=0;i<min<size_t>(a.size(),1024);i++)cerr<<(i?",":"")<<value_string(a[i]);
        cerr<<"]\n";exit(1);
    }
}
template<class T,int LOG> vector<T> thresholds() {
    using U=make_unsigned_t<T>;
    constexpr int W=numeric_limits<U>::digits;
    const U M=numeric_limits<T>::max(), D=data_max<T,LOG>();
    vector<T> out{numeric_limits<T>::min(),numeric_limits<T>::max(),T(0),T(1)};
    if constexpr(is_signed_v<T>)out.push_back(T(-1));
    auto around=[&](U v){if(v>M)return;out.push_back(T(v));if(v)out.push_back(T(v-1));if(v<M)out.push_back(T(v+1));};
    around(0);around(D);around(M);
    for(int b=0;b<W;b++)around(U(U(1)<<b));
    sort(out.begin(),out.end());out.erase(unique(out.begin(),out.end()),out.end());return out;
}
template<class T,int LOG> vector<T> boundary_values() {
    vector<T> v;const auto D=data_max<T,LOG>();
    for(T x:thresholds<T,LOG>())if(x>=0&&make_unsigned_t<T>(x)<=D)v.push_back(x);
    sort(v.begin(),v.end());v.erase(unique(v.begin(),v.end()),v.end());return v;
}
void record(const string& phase,const string& type,int log,const Stats& before) {
    cout<<"{\"phase\":\""<<phase<<"\",\"type\":\""<<type<<"\",\"log\":"<<log
        <<",\"arrays\":"<<total.arrays-before.arrays<<",\"queries\":"<<total.queries-before.queries
        <<",\"oracle_elements\":"<<total.elements-before.elements<<"}"<<endl;
}

template<class T,int LOG> void special() {
    Stats before=total;auto xs=thresholds<T,LOG>();auto vs=boundary_values<T,LOG>();
    const auto D=data_max<T,LOG>();mt19937_64 rng(923521);
    auto test=[&](const vector<T>& a,const string& name){
        WaveletMatrix<T,LOG>w(a);++total.arrays;int n=a.size();vector<pair<int,int>>ranges;
        if(n<=8)for(int l=0;l<=n;l++)for(int r=l;r<=n;r++)ranges.emplace_back(l,r);
        else{
            ranges={{0,0},{n,n},{0,n},{0,1},{n-1,n},{n/2,n/2},{0,n/2},{n/2,n},{n/2-1,n/2+1}};
            for(int i=0;i<16;i++){int l=rng()%(n+1),r=rng()%(n+1);if(l>r)swap(l,r);ranges.emplace_back(l,r);}
        }
        for(auto[l,r]:ranges)for(T x:xs)check(w,a,l,r,x,name);
    };
    test({},"empty");
    for(T v:vs)test({v},"singleton");
    for(int n:{1,2,3,4,7,8,9,15,16,17,31,32,33,63,64,65,127}){
        test(vector<T>(n,0),"all_zero");test(vector<T>(n,T(D)),"all_maximum");
        vector<T>a(n);for(int i=0;i<n;i++)a[i]=i%2?T(D):T(0);test(a,"alternating_extremes");
        fill(a.begin(),a.end(),0);a[n/2]=T(D);test(a,"one_maximum");
        fill(a.begin(),a.end(),T(D));a[n/2]=0;test(a,"one_zero");
        for(int i=0;i<n;i++)a[i]=vs[(i/3)%vs.size()];test(a,"duplicate_blocks");
    }
    test(vs,"ascending_bit_boundaries");reverse(vs.begin(),vs.end());test(vs,"descending_bit_boundaries");
    record("special",type_name<T>(),LOG,before);
}

constexpr array<uint64_t,6> SEEDS={0,1,0xDEADBEEF,0x123456789ABCDEF0ULL,0xFFFFFFFFFFFFFFFFULL,20260921};
template<class T> T arbitrary_threshold(mt19937_64& rng) {
    using U=make_unsigned_t<T>;
    if constexpr(is_signed_v<T>){
        T magnitude=T(U(rng())&U(numeric_limits<T>::max()));
        return rng()&1?T(-1-magnitude):magnitude;
    } else return T(rng());
}
template<class T,int LOG> void randomized() {
    using U=make_unsigned_t<T>;
    Stats before=total;auto xs=thresholds<T,LOG>();auto vs=boundary_values<T,LOG>();const U D=data_max<T,LOG>();
    for(uint64_t seed:SEEDS){mt19937_64 rng(seed);
        for(int trial=0;trial<200;trial++){
            int n=trial%137==0?257+rng()%257:rng()%129;
            vector<T>a(n);for(T&x:a)x=T(U(rng())&D);
            switch(trial%10){
                case 0:fill(a.begin(),a.end(),T(U(rng())&D));break;
                case 1:for(T&x:a)x=vs[rng()%vs.size()];break;
                case 2:sort(a.begin(),a.end());break;
                case 3:sort(a.rbegin(),a.rend());break;
                case 4:for(T&x:a)x=T(U(rng()%4)&D);break;
                case 5:for(int i=0;i<n;i++)a[i]=i%2?T(D):T(0);break;
                case 6:fill(a.begin(),a.end(),0);if(n)a[rng()%n]=T(D);break;
                case 7:fill(a.begin(),a.end(),T(D));if(n)a[rng()%n]=0;break;
            }
            WaveletMatrix<T,LOG>w(a);++total.arrays;
            string context="random seed="+to_string(seed)+" trial="+to_string(trial);
            for(int query=0;query<96;query++){
                int l=rng()%(n+1),r=rng()%(n+1);if(l>r)swap(l,r);
                if(query%16==0)l=0,r=n;
                if(query%16==1)r=l;
                T x=query%3==0?arbitrary_threshold<T>(rng):xs[rng()%xs.size()];
                if(query%8==0&&n)x=a[rng()%n];
                check(w,a,l,r,x,context);
            }
        }
    }
    record("random",type_name<T>(),LOG,before);
}
template<class T,int LOG> void configuration(){special<T,LOG>();randomized<T,LOG>();}
template<class T,int... L> void configurations(integer_sequence<int,L...>){(configuration<T,L>(),...);}

template<class T,int LOG,int ALPHABET,int MAX_N> void exhaustive() {
    Stats before=total;vector<T>xs;for(int i=0;i<=ALPHABET+1;i++)xs.push_back(T(i));
    if constexpr(is_signed_v<T>){xs.push_back(T(-1));xs.push_back(numeric_limits<T>::min());}
    xs.push_back(numeric_limits<T>::max());
    uint64_t count=1;
    for(int n=0;n<=MAX_N;n++,count*=ALPHABET)for(uint64_t code=0;code<count;code++){
        vector<T>a(n);uint64_t tmp=code;for(auto&v:a){v=T(tmp%ALPHABET);tmp/=ALPHABET;}
        WaveletMatrix<T,LOG>w(a);++total.arrays;
        for(int l=0;l<=n;l++)for(int r=l;r<=n;r++)for(T x:xs)check(w,a,l,r,x,"exhaustive");
    }
    record("exhaustive",type_name<T>(),LOG,before);
}
template<class T,int LOG> void large(int n) {
    using U=make_unsigned_t<T>;Stats before=total;mt19937_64 rng(982451653);const U D=data_max<T,LOG>();
    vector<T>a(n);for(int i=0;i<n;i++)a[i]=i%7==0?T(0):i%7==1?T(D):T(U(rng())&D);
    WaveletMatrix<T,LOG>w(a);++total.arrays;auto xs=thresholds<T,LOG>();
    for(T x:xs){check(w,a,0,n,x,"large_full");check(w,a,0,0,x,"large_empty_left");check(w,a,n,n,x,"large_empty_right");}
    for(int i=0;i<512;i++){int l=rng()%(n+1),r=rng()%(n+1);if(l>r)swap(l,r);check(w,a,l,r,arbitrary_threshold<T>(rng),"large_random");}
    record("large",type_name<T>(),LOG,before);
}

int main(int argc,char**argv) {
    if(argc>1&&string(argv[1])=="--insufficient-log"){
        vector<int>a{4};WaveletMatrix<int,2>w(a);int actual=w.less(0,1,1);
        cout<<"{\"probe\":\"insufficient_log\",\"array\":[4],\"log\":2,\"threshold\":1,\"expected_mathematical_count\":0,\"actual\":"<<actual<<"}"<<endl;
        return 0;
    }
    if(argc>1&&string(argv[1])=="--oversized-log"){
        WaveletMatrix<uint64_t,65>w(vector<uint64_t>{1});cout<<w.less(0,1,2)<<endl;return 0;
    }
    auto start=chrono::steady_clock::now();
    exhaustive<int8_t,1,2,8>();exhaustive<uint8_t,1,2,8>();
    exhaustive<int32_t,2,4,6>();exhaustive<uint32_t,2,4,6>();
    exhaustive<int64_t,3,8,4>();exhaustive<uint64_t,3,8,4>();
    exhaustive<int32_t,32,2,6>();exhaustive<uint64_t,64,2,6>();
    configurations<int8_t>(make_integer_sequence<int,9>{});configurations<uint8_t>(make_integer_sequence<int,9>{});
    using L16=integer_sequence<int,0,1,2,3,7,8,9,15,16>;
    configurations<int16_t>(L16{});configurations<uint16_t>(L16{});
    using L32=integer_sequence<int,0,1,2,3,7,8,10,15,16,17,30,31,32>;
    configurations<int32_t>(L32{});configurations<uint32_t>(L32{});
    using L64=integer_sequence<int,0,1,2,3,7,8,15,16,31,32,33,62,63,64>;
    configurations<int64_t>(L64{});configurations<uint64_t>(L64{});
    large<int32_t,30>(200000);large<uint64_t,64>(131073);
    double elapsed=chrono::duration<double>(chrono::steady_clock::now()-start).count();
    cout<<"{\"phase\":\"total\",\"arrays\":"<<total.arrays<<",\"queries\":"<<total.queries<<",\"oracle_elements\":"<<total.elements<<",\"seconds\":"<<elapsed<<",\"status\":\"PASS\"}"<<endl;
}
