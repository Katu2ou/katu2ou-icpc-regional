// 行列aをガウス消去する
// 返り値 : {rank, det}
//   rank : pivot_end列目までを係数部分としたときのrank
//   det  : 消去中のpivotの積×行交換の符号
//          正方行列aに対して全列をpivot対象にした場合は det(a)
// diagonalize=trueなら簡約行階段形にする
template<class T>
pair<int,T> GaussElimination(vector<vector<T>>&a,int pe=-1,bool diag=false){
    if(a.empty())return {0,T(1)};
    int h=a.size(),w=a[0].size(),r=0;
    if(pe<0)pe=w;
    T det=1;
    for(int j=0;j<pe;j++){
        int p=r;
        while(p<h&&a[p][j]==T(0))p++;
        if(p==h){det=0;continue;}
        if(p!=r)swap(a[p],a[r]),det=-det;
        det*=a[r][j];
        if(diag){
            T x=T(1)/a[r][j];
            for(int k=j;k<w;k++)a[r][k]*=x;
        }
        for(int i=diag?0:r+1;i<h;i++)if(i!=r&&a[i][j]!=T(0)){
            T x=a[i][j]/a[r][j];
            for(int k=j;k<w;k++)a[i][k]-=a[r][k]*x;
        }
        r++;
    }
    return {r,det};
}

// 線形方程式 Ax=b を解く
// 返り値 : 解なしなら {}
//          解ありなら {v,w1,...,wk}
//          ただし全ての解は
//              x = v + c1*w1 + ... + ck*wk
//          と表される
//   v       : 特殊解の1つ
//   w1..wk  : Ker(A)の基底（自由変数に対応）
//   k       : W-rank(A) = 解空間の次元
template<class T>
vector<vector<T>> LinearEquation(vector<vector<T>>a,vector<T>b){
    int h=a.size(),w=a[0].size();
    for(int i=0;i<h;i++)a[i].push_back(b[i]);
    int r=GaussElimination(a,w,true).first;
    for(int i=r;i<h;i++)if(a[i][w]!=T(0))return {};

    vector<vector<T>>res(1,vector<T>(w));
    vector<int>p(w,-1);
    for(int i=0,j=0;i<r;i++){
        while(a[i][j]==T(0))j++;
        p[j]=i; res[0][j]=a[i][w];
    }
    for(int j=0;j<w;j++)if(p[j]<0){
        vector<T>x(w); x[j]=1;
        for(int k=0;k<j;k++)if(p[k]>=0)x[k]=-a[p[k]][j];
        res.push_back(x);
    }
    return res;
}