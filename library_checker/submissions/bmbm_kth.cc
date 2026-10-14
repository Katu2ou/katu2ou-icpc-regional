#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL modint_convolution.cpp SHA256 92d6333c009329bccaa214bbc205c02750cc60094dddb0db1740182eb6ef5454
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
// END ORIGINAL modint_convolution.cpp

// BEGIN ORIGINAL bmbm.cpp SHA256 4170242b4c6a6adfe8738ea2ba31b896a5a00a6a146cec56dba204e16c6447ad
// modint-convolutionが前提
// 数列sから最短の線形漸化式
// s[n]=c[0]s[n-1]+...+c[d-1]s[n-d] を求める
vector<mint> BerlekampMassey(const vector<mint>&s){
    vector<mint>c{1},b{1}; int l=0,m=1; mint d0=1;
    for(int n=0;n<(int)s.size();n++){
        mint d=s[n];
        for(int i=1;i<=l;i++)d+=c[i]*s[n-i];
        if(!d.val()){m++;continue;}
        auto t=c; mint z=d/d0;
        if(c.size()<b.size()+m)c.resize(b.size()+m);
        for(int i=0;i<(int)b.size();i++)c[i+m]-=z*b[i];
        if(2*l<=n)l=n+1-l,b=t,d0=d,m=1;
        else m++;
    }
    c.erase(c.begin());
    for(auto&x:c)x=mint()-x;
    return c;
}
// 初項aと漸化式cから第n項をO(M(d)log n)で求める
mint BostanMori(vector<mint>a,vector<mint>c,long long n){
    if(n<(long long)a.size())return a[n];
    int d=c.size(); if(!d)return 0;
    a.resize(d);
    vector<mint>q(d+1); q[0]=1;
    for(int i=0;i<d;i++)q[i+1]=mint()-c[i];
    auto p=convolution(a,q); p.resize(d);
    while(n){
        auto qm=q;
        for(int i=1;i<(int)qm.size();i+=2)qm[i]=mint()-qm[i];
        auto x=convolution(p,qm),y=convolution(q,qm);
        vector<mint>np,nq;
        for(int i=n&1;i<(int)x.size();i+=2)np.push_back(x[i]);
        for(int i=0;i<(int)y.size();i+=2)nq.push_back(y[i]);
        p.swap(np); q.swap(nq); n>>=1;
    }
    return p[0]/q[0];
}
// 数列の先頭部分sから漸化式を推定し、第n項を求める
mint BMBM(const vector<mint>&s,long long n){
    if(n<(long long)s.size())return s[n];
    return BostanMori(s,BerlekampMassey(s),n);
}
// END ORIGINAL bmbm.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int d;ll k;cin>>d>>k;vector<mint>a(d),c(d);
for(auto&v:a){int x;cin>>x;v=x;}for(auto&v:c){int x;cin>>x;v=x;}
cout<<BostanMori(a,c,k).val()<<'\n';

}
