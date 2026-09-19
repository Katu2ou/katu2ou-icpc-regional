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