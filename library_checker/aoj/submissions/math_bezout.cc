#include <bits/stdc++.h>
using namespace std;using ll=long long;
using ll=long long;
using ull=unsigned long long;

ll smod(ll x,ll m){return (x%=m)<0?x+m:x;}

pair<ll,ll> igcd(ll a,ll b){
    a=smod(a,b); if(!a)return {b,0};
    ll s=b,t=a,x=0,y=1;
    while(t){
        ll q=s/t;
        s-=t*q; x-=y*q;
        swap(s,t); swap(x,y);
    }
    if(x<0)x+=b/s;
    return {s,x};
}

ll pow_mod(ll x,ll n,int m){
    ll r=1%m; x=smod(x,m);
    for(;n;n>>=1,x=x*x%m)
        if(n&1)r=r*x%m;
    return r;
}

ll inv_mod(ll x,ll m){
    return igcd(x,m).second;
}

pair<ll,ll> crt(const vector<ll>&r,const vector<ll>&m){
    ll R=0,M=1;
    for(int i=0;i<(int)r.size();i++){
        ll x=smod(r[i],m[i]);
        auto [g,im]=igcd(M,m[i]);
        ll u=m[i]/g;
        if((x-R)%g)return {0,0};
        ll t=(__int128)smod((x-R)/g,u)*im%u;
        R+=M*t; M*=u; R=smod(R,M);
    }
    return {R,M};
}

ll floor_sum(ll n,ll m,ll a,ll b){
    ull z=0;
    if(a<0){
        ull x=smod(a,m);
        z-=1ULL*n*(n-1)/2*((x-a)/m); a=x;
    }
    if(b<0){
        ull x=smod(b,m);
        z-=1ULL*n*((x-b)/m); b=x;
    }
    for(;;){
        if(a>=m)z+=1ULL*n*(n-1)/2*(a/m),a%=m;
        if(b>=m)z+=1ULL*n*(b/m),b%=m;
        ull y=(ull)a*n+b;
        if(y<(ull)m)return (ll)z;
        n=y/m; b=y%m; swap(a,m);
    }
}

void math_checks(){
for(ll m=1;m<=12;m++)for(ll a=-15;a<=15;a++){
ll rem=(a%m+m)%m;assert(smod(a,m)==rem);auto[g,x]=igcd(a,m);assert(g==gcd(a,m)&&smod(a*x-g,m)==0);
if(g==1)assert(smod(a*inv_mod(a,m),m)==1%m);
ll p=1%m;for(int n=0;n<9;n++){assert(pow_mod(a,n,m)==p);p=p*rem%m;}}
for(ll m=1;m<=8;m++)for(ll k=1;k<=8;k++)for(ll a=-3;a<=8;a++)for(ll b=-3;b<=8;b++){
ll L=lcm(m,k),want=-1;for(ll x=0;x<L;x++)if(x%m==(a%m+m)%m&&x%k==(b%k+k)%k){want=x;break;}
auto[r,z]=crt({a,b},{m,k});if(want<0)assert(z==0);else assert(r==want&&z==L);}
for(ll n=0;n<=6;n++)for(ll m=1;m<=6;m++)for(ll a=-6;a<=6;a++)for(ll b=-6;b<=6;b++){
ll z=0;for(ll i=0;i<n;i++){ll v=a*i+b,q=v/m;if(v%m<0)q--;z+=q;}assert(floor_sum(n,m,a,b)==z);}
}
pair<ll,ll> normalize_bezout(ll a,ll b,ll g,ll x){
pair<ll,ll>best;ll score=LLONG_MAX;for(ll k=-2;k<=2;k++){ll u=x+k*(b/g),v=(g-a*u)/b;ll z=abs(u)+abs(v);if(z<score||(z==score&&u<=v&&best.first>best.second)){score=z;best={u,v};}}return best;
}

int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

ll a,b;cin>>a>>b;auto[g,x]=igcd(a,b);if(g==1)x=inv_mod(a,b);auto[u,v]=normalize_bezout(a,b,g,x);cout<<u<<' '<<v<<'\n';

}
