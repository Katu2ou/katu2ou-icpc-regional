struct two_sat{
    int n; vector<vector<int>>g,rg; vector<bool>a;
    two_sat(int N):n(N),g(2*N),rg(2*N),a(N){}

    void add(int x,int y){g[x].push_back(y);rg[y].push_back(x);}
    void add_clause(int i,bool f,int j,bool h){
        add(2*i+!f,2*j+h); add(2*j+!h,2*i+f);
    }

    bool satisfiable(){
        int N=2*n; vector<int>v(N),o,c(N);
        auto dfs=[&](auto&&self,int x)->void{
            v[x]=1;
            for(int y:g[x])if(!v[y])self(self,y);
            o.push_back(x);
        };
        for(int i=0;i<N;i++)if(!v[i])dfs(dfs,i);

        fill(v.begin(),v.end(),0); reverse(o.begin(),o.end());
        int k=0;
        auto rdfs=[&](auto&&self,int x)->void{
            v[x]=1;c[x]=k;
            for(int y:rg[x])if(!v[y])self(self,y);
        };
        for(int x:o)if(!v[x])rdfs(rdfs,x),k++;

        for(int i=0;i<n;i++){
            if(c[2*i]==c[2*i+1])return false;
            a[i]=c[2*i]<c[2*i+1];
        }
        return true;
    }

    vector<bool> answer(){return a;}
};