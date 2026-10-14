#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL trie.cpp SHA256 edc9ae30944fc4b46446e45266c819967464a4b5dbbde70c49b01c36839be26c
template<int K,char B> struct trie{ //K文字種,Bを先頭文字とする文字列Trie
    struct N{
        int nx[K],end=0;
        N(){fill(nx,nx+K,-1);}
    };
    vector<N>t{N()};

    void insert(string s){ //文字列sを追加する
        int v=0;
        for(char c:s){
            int x=c-B; //文字cに対応する子番号 連続でない文字を取りたいならid関数を作る
            if(t[v].nx[x]<0)t[v].nx[x]=t.size(),t.emplace_back();
            v=t[v].nx[x];
        }
        t[v].end++;
    }

    int find(string s){ //文字列sに対応する頂点番号を返す(存在しなければ-1)
        int v=0;
        for(char c:s){
            v=t[v].nx[c-B];
            if(v<0)return -1;
        }
        return v;
    }
};
// END ORIGINAL trie.cpp


int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int q;cin>>q;trie<10,'0'>tr;unordered_map<int,ll>values;
while(q--){int type;ll k;cin>>type>>k;string s=to_string(k);int v=tr.find(s);
if(type==0){ll value;cin>>value;if(v<0||!tr.t[v].end){tr.insert(s);v=tr.find(s);}values[v]=value;}
else{if(v<0||!tr.t[v].end)cout<<0<<'\n';else cout<<values[v]<<'\n';}}

}
