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