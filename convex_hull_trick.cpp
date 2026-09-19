template<class T,const T id> struct convex_hull_trick{
    struct L{
        T a=0,b=id;
        T f(T x)const{return a*x+b;} // 直線ax+bのxでの値
    };

    int n;
    vector<T>x;
    vector<L>t;
    vector<char>u;

    convex_hull_trick(const vector<T>&X) // 使用するx座標を渡して構築
        :n(X.size()),x(X),t(4*n),u(4*n){}

    void insert(T a,T b){ // 直線 ax+b を追加
        add(1,0,n-1,{a,b});
    }

    T get(T X)const{ // x=Xでの追加済み直線の最大値
        int p=lower_bound(x.begin(),x.end(),X)-x.begin();
        assert(p<n&&x[p]==X);
        return qry(1,0,n-1,p);
    }

private:
    void add(int k,int l,int r,L z){ // Li Chao Treeに直線zを追加
        if(!u[k]){t[k]=z;u[k]=1;return;}
        int m=(l+r)/2;
        bool a=z.f(x[l])>t[k].f(x[l]);
        bool b=z.f(x[m])>t[k].f(x[m]);
        if(b)swap(z,t[k]);
        if(l==r)return;
        if(a!=b)add(2*k,l,m,z);
        else add(2*k+1,m+1,r,z);
    }

    T qry(int k,int l,int r,int p)const{ // x[p]での最大値を求める
        if(!u[k])return id;
        T y=t[k].f(x[p]);
        if(l==r)return y;
        int m=(l+r)/2;
        return max(y,p<=m?qry(2*k,l,m,p)
                         :qry(2*k+1,m+1,r,p));
    }
};