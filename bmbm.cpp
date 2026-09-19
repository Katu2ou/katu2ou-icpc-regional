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