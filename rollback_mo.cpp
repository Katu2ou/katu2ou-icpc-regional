struct MoRollBack{
    int w,l=0,r=0; // 現在の区間 [l,r)
    vector<pair<int,int>>q;
    vector<int>ord;
    MoRollBack(int N,int Q):w(max(1,(int)sqrt(max(1,N)))),ord(Q){
        iota(ord.begin(),ord.end(),0);
    }
    void add(int L,int R){ // クエリ[L,R)を追加
        q.push_back({L,R});
    }
    template<class A,class REM,class RS,class S,class B>
    void run(const A&add,const REM&rem,const RS&reset,
             const S&snapshot,const B&rollback){ // 全クエリを処理
        assert(q.size()==ord.size());
        sort(ord.begin(),ord.end(),[&](int i,int j){
            int a=q[i].first/w,b=q[j].first/w;
            return a!=b?a<b:q[i].second<q[j].second;
        });
        l=r=0; reset();
        for(int i:ord)if(q[i].second-q[i].first<w){
            auto [L,R]=q[i];
            l=r=L;
            while(r<R){
                int j=r++;
                add(j); // add後の区間 [l,r) を参照できる
            }
            rem(i);     // [l,r) はクエリ区間そのもの
            l=r=0;
            rollback(); // rollback後は空区間 [0,0)
        }
        int last=-1;
        for(int i:ord)if(q[i].second-q[i].first>=w){
            auto [L,R]=q[i];
            int b=L/w,base=(b+1)*w;
            if(b!=last){
                last=b;
                l=r=base;
                reset(); // 空区間 [base,base)
            }
            while(r<R){
                int j=r++;
                add(j); // 右に追加後
            }
            snapshot();
            while(l>L)
                add(--l); // 左に追加後
            rem(i); // [l,r)=[L,R)
            l=base;
            rollback(); // [base,r) まで戻す
        }
    }
};
/* 区間内の異なる値の個数
int N,Q;
cin>>N>>Q;
vector<int>A(N);
for(int &x:A)cin>>x;
MoRollBack mo(N,Q);
for(int i=0;i<Q;i++){
    int l,r;
    cin>>l>>r;
    mo.add(l,r);
}
vector<int>cnt(1000001),ans(Q);
vector<int>hist;
int distinct=0,snap=0;
auto undo=[&](){
    int i=hist.back();
    hist.pop_back();

    if(--cnt[A[i]]==0)
        distinct--;
};
auto add=[&](int i){ // iを追加
    hist.push_back(i);

    if(cnt[A[i]]++==0)
        distinct++;

    cout<<"add "<<i<<" -> ["
        <<mo.l<<","<<mo.r<<")\n";
};
auto rem=[&](int q){ // q番目の答えを記録
    ans[q]=distinct;

    cout<<"query "<<q<<" = ["
        <<mo.l<<","<<mo.r<<")\n";
};
auto reset=[&](){ // 状態を空にする
    while(!hist.empty())undo();
    snap=0;
};
auto snapshot=[&](){ // 現在位置を保存
    snap=hist.size();
};
auto rollback=[&](){ // snapshotまで巻き戻す
    while((int)hist.size()>snap)
        undo();
};
mo.run(add,rem,reset,snapshot,rollback);
for(int x:ans)
    cout<<x<<"\n";
*/