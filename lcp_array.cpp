template<class T> vector<int> lcp_array(const T&s,const vector<int>&sa){
    int n=s.size(),h=0; vector<int>r(n),l(n-1);
    for(int i=0;i<n;i++)r[sa[i]]=i;
    for(int i=0;i<n;i++)if(r[i]){
        int j=sa[r[i]-1];
        while(max(i,j)+h<n&&s[i+h]==s[j+h])h++;
        l[r[i]-1]=h; if(h)h--;
    }
    return l;
}