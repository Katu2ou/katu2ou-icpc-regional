template<class T> vector<int> suffix_array(vector<T>s){
    int n=s.size(); if(!n)return {};
    int N=n+1; vector<int>p(N),c(N),q(N),nc(N),cnt(N);
    p[0]=n; iota(p.begin()+1,p.end(),0);
    sort(p.begin()+1,p.end(),[&](int i,int j){return s[i]<s[j];});
    for(int i=1;i<N;i++)c[p[i]]=i==1?1:c[p[i-1]]+(s[p[i-1]]!=s[p[i]]);
    for(int k=1;k<N;k*=2){
        for(int i=0;i<N;i++)q[i]=(p[i]-k+N)%N;
        fill(cnt.begin(),cnt.end(),0);
        for(int x:q)cnt[c[x]]++;
        for(int i=1;i<N;i++)cnt[i]+=cnt[i-1];
        for(int i=N-1;i>=0;i--)p[--cnt[c[q[i]]]]=q[i];
        nc[p[0]]=0;
        for(int i=1;i<N;i++)nc[p[i]]=nc[p[i-1]]+
            (c[p[i-1]]!=c[p[i]]||c[(p[i-1]+k)%N]!=c[(p[i]+k)%N]);
        c.swap(nc);
    }
    return {p.begin()+1,p.end()};
}
vector<int> suffix_array(string s){
    return suffix_array(vector<unsigned char>(s.begin(),s.end()));
}
vector<int> suffix_array(vector<int>s,int){
    return suffix_array<int>(move(s));
}