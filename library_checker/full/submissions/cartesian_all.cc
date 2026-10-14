#include <bits/stdc++.h>
using namespace std;using ll=long long;


// BEGIN ORIGINAL cartesiantree.cpp SHA256 3feb3f7a2361dbca6ac98f6264df277b79b8390069f015b58dbb7e4bfb25d6b9
template<class T> struct CartesianTree{
    int root=0; //最小値のうちindex最小が根
    vector<int> par,left,right;

    CartesianTree(){}
    CartesianTree(const vector<T>&a) // aのCartesian TreeをO(n)で構築
        :par(a.size(),-1),left(a.size(),-1),right(a.size(),-1){
        vector<int>st;
        for(int i=0;i<(int)a.size();i++){
            int p=-1;
            while(!st.empty()&&a[st.back()]>a[i])
                p=st.back(),st.pop_back();
            if(!st.empty())
                par[i]=st.back(),right[st.back()]=i;
            if(p>=0)
                par[p]=i,left[i]=p;
            st.push_back(i);
        }
        if(!st.empty())root=st[0];
    }
};
// END ORIGINAL cartesiantree.cpp


int main(){ios::sync_with_stdio(false);cin.tie(nullptr);

CartesianTree<int>empty;assert(empty.par.empty()&&empty.left.empty()&&empty.right.empty());int n;cin>>n;vector<int>a(n);for(auto&x:a)cin>>x;CartesianTree<int>t(a);
for(int i=0;i<n;i++){for(int v:{t.left[i],t.right[i]})if(v>=0){assert(t.par[v]==i&&a[i]<=a[v]);}if(t.left[i]>=0)assert(t.left[i]<i);if(t.right[i]>=0)assert(t.right[i]>i);}
assert(t.par[t.root]==-1);for(int i=0;i<n;i++)cout<<(t.par[i]<0?i:t.par[i])<<' ';cout<<'\n';

}
