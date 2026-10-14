#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL z_algorithm.cpp SHA256 c1778879548e8f4dcfee817fa76584528c7b2792ce3f2024e901cce8ef83eb69
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
// END ORIGINAL z_algorithm.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

string s;cin>>s;for(int x:z_algorithm(s))cout<<x<<' ';cout<<'\n';

}
