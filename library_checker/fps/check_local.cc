#include <bits/stdc++.h>
using namespace std;
using ll=long long;

// BEGIN UNCHANGED modint_convolution.cpp
struct mint{
    static const int M=998244353;
    int x;
    mint(long long v=0):x((v%M+M)%M){}
    int val()const{return x;}
    static int mod(){return M;}
    mint& operator+=(mint a){if((x+=a.x)>=M)x-=M;return *this;}
    mint& operator-=(mint a){if((x-=a.x)<0)x+=M;return *this;}
    mint& operator*=(mint a){x=(long long)x*a.x%M;return *this;}
    mint pow(long long n)const{
        mint a=*this,r=1;
        for(;n;n>>=1,a*=a)if(n&1)r*=a;
        return r;
    }
    mint inv()const{return pow(M-2);}
    mint& operator/=(mint a){return *this*=a.inv();}
    friend mint operator+(mint a,mint b){return a+=b;}
    friend mint operator-(mint a,mint b){return a-=b;}
    friend mint operator*(mint a,mint b){return a*=b;}
    friend mint operator/(mint a,mint b){return a/=b;}
};
using modint998244353=mint;
void ntt(vector<mint>&a,bool inv=0){
    int n=a.size();
    for(int i=1,j=0;i<n;i++){
        int b=n>>1; for(;j&b;b>>=1)j^=b; j^=b;
        if(i<j)swap(a[i],a[j]);
    }
    for(int l=2;l<=n;l<<=1){
        mint w=mint(3).pow((mint::M-1)/l);
        if(inv)w=w.inv();
        for(int i=0;i<n;i+=l){
            mint z=1;
            for(int j=0;j<l/2;j++){
                mint x=a[i+j],y=a[i+j+l/2]*z;
                a[i+j]=x+y; a[i+j+l/2]=x-y; z*=w;
            }
        }
    }
    if(inv){
        mint z=mint(n).inv();
        for(auto&x:a)x*=z;
    }
}
vector<mint> convolution(vector<mint>a,vector<mint>b){
    if(a.empty()||b.empty())return {};
    int n=a.size()+b.size()-1,z=1;
    while(z<n)z<<=1;
    a.resize(z); b.resize(z); ntt(a); ntt(b);
    for(int i=0;i<z;i++)a[i]*=b[i];
    ntt(a,1); a.resize(n);
    return a;
}
// END UNCHANGED modint_convolution.cpp

// BEGIN UNCHANGED fps.cpp
struct fps:vector<mint>{
    using vector<mint>::vector;
    fps(vector<mint>a):vector<mint>(move(a)){}

    fps pre(int n)const{
        return fps(begin(),begin()+min(n,(int)size()));
    }

    friend fps operator+(fps a,const fps&b){
        if(a.size()<b.size())a.resize(b.size());
        for(int i=0;i<(int)b.size();i++)a[i]+=b[i];
        return a;
    }
    friend fps operator-(fps a,const fps&b){
        if(a.size()<b.size())a.resize(b.size());
        for(int i=0;i<(int)b.size();i++)a[i]-=b[i];
        return a;
    }
    friend fps operator*(const fps&a,const fps&b){
        return convolution(a,b);
    }

    fps& operator*=(mint c){
        for(auto&x:*this)x*=c;
        return *this;
    }
    friend fps operator*(fps a,mint c){return a*=c;}
    friend fps operator*(mint c,fps a){return a*=c;}

    fps diff()const{
        fps r(max(0,(int)size()-1));
        for(int i=1;i<(int)size();i++)r[i-1]=(*this)[i]*i;
        return r;
    }

    fps integral()const{
        fps r(size()+1);
        static vector<mint>iv{0,1};
        while(iv.size()<=size()){
            int i=iv.size();
            iv.push_back(mint(mint::M-mint::M/i)*iv[mint::M%i]);
        }
        for(int i=0;i<(int)size();i++)r[i+1]=(*this)[i]*iv[i+1];
        return r;
    }

    // f[0] != 0
    fps inv(int n=-1)const{
        if(n<0)n=size();
        assert(!empty()&&(*this)[0].val());
        fps g{(*this)[0].inv()};
        for(int m=1;m<n;m<<=1){
            int k=min(n,m<<1);
            g=(g+g-g*g*pre(k)).pre(k);
        }
        g.resize(n);
        return g;
    }

    // f[0] = 1
    fps log(int n=-1)const{
        if(n<0)n=size();
        assert(!empty()&&(*this)[0].val()==1);
        fps r=(diff()*inv(n)).pre(max(0,n-1)).integral();
        r.resize(n);
        return r;
    }

    // f[0] = 0
    fps exp(int n=-1)const{
        if(n<0)n=size();
        assert(empty()||(*this)[0].val()==0);
        if(!n)return {};
        fps g{1};
        for(int m=1;m<n;m<<=1){
            int k=min(n,m<<1);
            fps q=pre(k)-g.log(k);
            q[0]+=1;
            g=(g*q).pre(k);
        }
        return g;
    }

    // k >= 0
    fps pow(long long k,int n=-1)const{
        assert(k>=0);
        if(n<0)n=size();
        if(!n)return {};
        if(!k){
            fps r(n);
            r[0]=1;
            return r;
        }

        int s=0;
        while(s<(int)size()&&!(*this)[s].val())s++;

        if(s==(int)size()||s>(n-1)/k)return fps(n);

        int sh=s*k;
        mint c=(*this)[s];

        fps f(begin()+s,end());
        f*=c.inv();

        f=(f.log(n-sh)*mint(k)).exp(n-sh);
        f*=c.pow(k);

        fps r(sh);
        r.insert(r.end(),f.begin(),f.end());
        r.resize(n);
        return r;
    }
};
// END UNCHANGED fps.cpp

// Independent coefficient oracles. These checks remain active with NDEBUG.
namespace fps_verification {
constexpr int MOD=998244353;
using V=vector<int>;
long long checks=0;
void require(bool ok,const char* what){
    ++checks;
    if(!ok){cerr<<"FPS CHECK FAILED: "<<what<<'\n';abort();}
}
int add(int a,int b){int s=a+b;return s>=MOD?s-MOD:s;}
int sub(int a,int b){int d=a-b;return d<0?d+MOD:d;}
int mul(int a,int b){return (long long)a*b%MOD;}
int power(int a,long long k){int r=1;while(k){if(k&1)r=mul(r,a);a=mul(a,a);k>>=1;}return r;}
fps make(const V&a){vector<mint>v;for(int x:a)v.emplace_back(x);return fps(move(v));}
V values(const fps&a){V v;for(auto x:a)v.push_back(x.val());return v;}
void equal_to(const fps&a,const V&b,const char* what){require(values(a)==b,what);}
V product(const V&a,const V&b,int n){
    V c(n);for(int i=0;i<(int)a.size()&&i<n;i++)for(int j=0;j<(int)b.size()&&i+j<n;j++)c[i+j]=add(c[i+j],mul(a[i],b[j]));return c;
}
V inverse(const V&a,int n){
    V b(n);if(!n)return b;b[0]=power(a[0],MOD-2);
    for(int i=1;i<n;i++){int s=0;for(int j=1;j<=i&&j<(int)a.size();j++)s=add(s,mul(a[j],b[i-j]));b[i]=sub(0,mul(b[0],s));}return b;
}
V logarithm(const V&a,int n){
    V b(n);if(!n)return b;V d;
    for(int i=1;i<(int)a.size();i++)d.push_back(mul(a[i],i));
    V q=product(d,inverse(a,n),n-1);
    for(int i=1;i<n;i++)b[i]=mul(q[i-1],power(i,MOD-2));return b;
}
V exponential(const V&a,int n){
    V b(n);if(!n)return b;b[0]=1;
    for(int i=1;i<n;i++){int s=0;for(int j=1;j<=i&&j<(int)a.size();j++)s=add(s,mul(mul(j,a[j]),b[i-j]));b[i]=mul(s,power(i,MOD-2));}return b;
}
V polynomial_power(V a,long long k,int n){
    V b(n);if(!n)return b;b[0]=1;a.resize(n);
    while(k){if(k&1)b=product(b,a,n);a=product(a,a,n);k>>=1;}return b;
}
void helpers(const fps&a,const fps&b){
    V av=values(a),bv=values(b);
    equal_to(make(av),av,"vector constructor");
    int n=a.size(),m=b.size();
    for(int k:vector<int>{0,1,n/2,n,n+3,INT_MAX}){
        V expected(av.begin(),av.begin()+min(k,n));
        equal_to(a.pre(k),expected,"pre");
    }
    V sum(max(n,m)),difference(max(n,m));
    for(int i=0;i<(int)sum.size();i++){
        int x=i<n?av[i]:0,y=i<m?bv[i]:0;
        sum[i]=add(x,y);difference[i]=sub(x,y);
    }
    equal_to(a+b,sum,"operator+ unequal lengths");
    equal_to(a-b,difference,"operator- unequal lengths");
    for(int k:vector<int>{0,1,MOD-1,123456789}){
        V expected=av;for(int&x:expected)x=mul(x,k);
        equal_to(a*mint(k),expected,"fps * mint");
        equal_to(mint(k)*a,expected,"mint * fps");
        fps copy=a;fps&ref=(copy*=mint(k));
        require(&ref==&copy,"operator*= reference");
        equal_to(copy,expected,"operator*=");
    }
    V derivative(max(0,n-1));for(int i=1;i<n;i++)derivative[i-1]=mul(av[i],i);
    equal_to(a.diff(),derivative,"diff");
    fps integrated=a.integral();
    require((int)integrated.size()==n+1&&integrated[0].val()==0,"integral size and constant");
    // Division by i is unique modulo MOD since 1 <= i < MOD.
    for(int i=0;i<n;i++)require(mul(integrated[i+1].val(),i+1)==av[i],"integral coefficient");
    equal_to(a,av,"input a unchanged");equal_to(b,bv,"input b unchanged");
}
void small_checks(){
    vector<V> cases{{},{0},{1},{MOD-1},{0,0},{0,1},{0,0,1},{1,0,0,0},{MOD-1,MOD-1,MOD-1}};
    for(int n:vector<int>{3,4,7,8,9,15,16,17,31,32,33,63,64,65}){
        cases.emplace_back(n,0);cases.emplace_back(n,1);
    }
    mt19937_64 rng(20260921);
    for(int t=0;t<64;t++){
        V a(rng()%35);for(int&x:a)x=rng()%MOD;
        if(t%3==0)for(int i=0;i<min((int)a.size(),t%8);i++)a[i]=0;
        cases.push_back(move(a));
    }
    vector<long long> exponents{0,1,2,5,31,998244353LL,998244354LL,1000000000000000000LL,LLONG_MAX};
    for(int t=0;t<(int)cases.size();t++){
        const V&a=cases[t];V b=cases[(t*17+7)%cases.size()];fps f=make(a),g=make(b);
        helpers(f,g);helpers(g,f);
        int full=a.empty()||b.empty()?0:a.size()+b.size()-1;
        equal_to(f*g,product(a,b,full),"polynomial multiplication");
        V ai=a,al=a,ae=a;
        if(ai.empty())ai.push_back(1);else if(!ai[0])ai[0]=1234567;
        if(al.empty())al.push_back(1);else al[0]=1;
        if(!ae.empty())ae[0]=0;
        fps fi=make(ai),fl=make(al),fe=make(ae);
        equal_to(fi.inv(),inverse(ai,ai.size()),"inv default precision");
        equal_to(fl.log(),logarithm(al,al.size()),"log default precision");
        equal_to(fe.exp(),exponential(ae,ae.size()),"exp default precision");
        long long k=exponents[t%exponents.size()];
        equal_to(f.pow(k),polynomial_power(a,k,a.size()),"pow default precision");
        for(int n:vector<int>{0,1,(int)a.size()/2,(int)a.size()+3}){
            equal_to(fi.inv(n),inverse(ai,n),"inv explicit precision");
            equal_to(fl.log(n),logarithm(al,n),"log explicit precision");
            equal_to(fe.exp(n),exponential(ae,n),"exp explicit precision");
            equal_to(f.pow(k,n),polynomial_power(a,k,n),"pow explicit precision");
            equal_to(f.pow(0,n),polynomial_power(a,0,n),"pow exponent zero");
        }
    }
    // Exhaust all short binary polynomials, including zero and delayed terms.
    for(int len=0;len<=5;len++)for(int mask=0;mask<(1<<len);mask++){
        V a(len);for(int i=0;i<len;i++)a[i]=(mask>>i)&1;fps f=make(a);
        for(long long k:vector<long long>{0,1,2,3,LLONG_MAX})
            equal_to(f.pow(k,8),polynomial_power(a,k,8),"pow exhaustive binary inputs");
    }
}
} // namespace fps_verification

int main(){fps_verification::small_checks();cout<<"PASS "<<fps_verification::checks<<" checks\n";}
