// F_2上の線形方程式 Ax=b を解く
// a[i][0..W-1] : Aのi行目, a[i][W] : b[i]
// 返り値 : 解なしなら {}
//          解ありなら {v,w1,...,wk}
//          全解は x=v+c1*w1+...+ck*wk (ci in F_2)
//   v      : 特殊解
//   w1..wk : Ker(A)の基底 O(HW^2/64)
template<int MAX>
vector<bitset<MAX>> LinearEquation(vector<bitset<MAX+1>>a,int W){
    int H=a.size(),r=0;
    vector<int>p(W,-1);
    for(int j=0;j<W;j++){
        int q=r; while(q<H&&!a[q][j])q++;
        if(q==H)continue;
        swap(a[r],a[q]); p[j]=r;
        for(int i=0;i<H;i++)if(i!=r&&a[i][j])a[i]^=a[r];
        r++;
    }
    for(int i=r;i<H;i++)if(a[i][W])return {};

    vector<bitset<MAX>>res(1);
    for(int j=0;j<W;j++)if(p[j]>=0)
        res[0][j]=a[p[j]][W];

    for(int j=0;j<W;j++)if(p[j]<0){
        bitset<MAX>x; x[j]=1;
        for(int k=0;k<W;k++)if(p[k]>=0&&a[p[k]][j])x[k]=1;
        res.push_back(x);
    }
    return res;
}