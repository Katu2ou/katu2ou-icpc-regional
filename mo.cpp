struct Mo{
    int n,l=0,r=0; // 現在の区間[l,r)
    vector<pair<int,int>>lr; //[l,r) addやeraseでは更新後のl,rを参照できる
    Mo(int N):n(N){} //i番目の答えが入る
    void add(int l,int r){ // [l,r) のクエリを追加
        lr.emplace_back(l,r);
    }
    template<class AL,class AR,class EL,class ER,class O>
    void build(const AL&al,const AR&ar,const EL&el,const ER&er,const O&out){
        int Q=lr.size(); if(!Q)return;
        int B=max(1,n/(int)sqrt(Q));
        vector<int>o(Q); iota(o.begin(),o.end(),0);
        sort(o.begin(),o.end(),[&](int a,int b){
            int x=lr[a].first/B,y=lr[b].first/B;
            return x!=y?x<y:
                (x&1?lr[a].second>lr[b].second:lr[a].second<lr[b].second);
        });
        l=r=0;
        for(int i:o){
            auto [L,R]=lr[i];
            while(l>L)al(--l);
            while(r<R)ar(r++);
            while(l<L)el(l++);
            while(r>R)er(--r);
            out(i);
        }
    }
    template<class A,class E,class O>
    void build(const A&add,const E&erase,const O&out){ //add(i):
        build(add,add,erase,erase,out);
    }
};
// auto add_left = [&](int i){}; 左側のi番目の要素を加える
// auto erase = [&](int i){};
// auto out = [&](int q){}; 現在のクエリ(q番目)の答えを求める
