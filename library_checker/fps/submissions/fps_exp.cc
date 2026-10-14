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



fps read_fps(int n){vector<mint>a(n);for(auto&x:a){long long y;cin>>y;x=mint(y);}return fps(move(a));}
void print_fps(const fps&a){for(int i=0;i<(int)a.size();i++)cout<<a[i].val()<<(i+1==(int)a.size()?'\n':' ');if(a.empty())cout<<'\n';}

int main(){ios::sync_with_stdio(false);cin.tie(nullptr);
int n;cin>>n;fps a=read_fps(n);print_fps(a.exp());
}
