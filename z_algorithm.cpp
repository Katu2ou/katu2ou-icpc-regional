template<class T> vector<int> z_algorithm(const T&s){
    int n=s.size(); vector<int>z(n);
    for(int i=1,j=0;i<n;i++){
        if(i<j+z[j])z[i]=min(j+z[j]-i,z[i-j]);
        while(i+z[i]<n&&s[z[i]]==s[i+z[i]])z[i]++;
        if(i+z[i]>j+z[j])j=i;
    }
    if(n)z[0]=n;
    return z;
}