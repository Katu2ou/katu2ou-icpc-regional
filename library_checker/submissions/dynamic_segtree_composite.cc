#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// BEGIN ORIGINAL modint_convolution.cpp SHA256 92d6333c009329bccaa214bbc205c02750cc60094dddb0db1740182eb6ef5454
struct mint{
    static const int M=998244353;
    int x;
    mint(long long v=0):x((v%M+M)%M){}
    int val()const{return x;}
    static int mod(){return M;}
    mint& operator+=(mint a){if((x+=a.x)>=M)x-=M;return *this;}
    mint& operator-=(mint a){if((x-=a.x)<0)x+=M;return *this;}
    mint& operator*=(mint a){x=(long long)x*a.x%M;return *this;}
    mint pow(long long n)const{
        mint a=*this,r=1;
        for(;n;n>>=1,a*=a)if(n&1)r*=a;
        return r;
    }
    mint inv()const{return pow(M-2);}
    mint& operator/=(mint a){return *this*=a.inv();}
    friend mint operator+(mint a,mint b){return a+=b;}
    friend mint operator-(mint a,mint b){return a-=b;}
    friend mint operator*(mint a,mint b){return a*=b;}
    friend mint operator/(mint a,mint b){return a/=b;}
};
using modint998244353=mint;
void ntt(vector<mint>&a,bool inv=0){
    int n=a.size();
    for(int i=1,j=0;i<n;i++){
        int b=n>>1; for(;j&b;b>>=1)j^=b; j^=b;
        if(i<j)swap(a[i],a[j]);
    }
    for(int l=2;l<=n;l<<=1){
        mint w=mint(3).pow((mint::M-1)/l);
        if(inv)w=w.inv();
        for(int i=0;i<n;i+=l){
            mint z=1;
            for(int j=0;j<l/2;j++){
                mint x=a[i+j],y=a[i+j+l/2]*z;
                a[i+j]=x+y; a[i+j+l/2]=x-y; z*=w;
            }
        }
    }
    if(inv){
        mint z=mint(n).inv();
        for(auto&x:a)x*=z;
    }
}
vector<mint> convolution(vector<mint>a,vector<mint>b){
    if(a.empty()||b.empty())return {};
    int n=a.size()+b.size()-1,z=1;
    while(z<n)z<<=1;
    a.resize(z); b.resize(z); ntt(a); ntt(b);
    for(int i=0;i<z;i++)a[i]*=b[i];
    ntt(a,1); a.resize(n);
    return a;
}
// END ORIGINAL modint_convolution.cpp

// BEGIN ORIGINAL dynamic_segtree.cpp SHA256 7e11fd649e9dedc3643a14954010547bb9b96337125fe496202538928f139025
template <class S, auto op, auto e> class dynamic_segtree {
	private:
	struct node;
	using node_ptr=unique_ptr<node>;
	struct node{
		long long idx;
		S value,product;
		node_ptr left,right;
		node(long long idx,S value)
			: idx(idx),
			value(value),
			product(value),
			left(nullptr),
			right(nullptr) {}

		void update(){
			product=op(op(left?left->product:e(),value),right?right->product:e());
		}
	};
	const long long n;
	node_ptr root;

  	public:
    dynamic_segtree() : dynamic_segtree(0) {}
	dynamic_segtree(long long n):n(n),root(nullptr){}

	void set(long long p,S x){
		assert(0<=p&&p<n);
		set(root,0,n,p,x);
	}
	void set(node_ptr& t,long long a,long long b,long long p,S x){
		if(!t){
			t=make_unique<node>(p,x);
			return;
		}
		if(t->idx==p){
			t->value=x;
			t->update();
			return;
		}
		long long c=(a+b)/2;
		if(p<c){
			if(t->idx<p)swap(t->idx,p),swap(t->value,x);
			set(t->left,a,c,p,x);
		}
		else{
			if(p<t->idx)swap(p,t->idx),swap(x,t->value);
			set(t->right,c,b,p,x);
		}
		t->update();
	}

	S get(long long p) const{
		assert(0<=p&&p<n);
		return get(root,0,n,p);
	}

	S get(const node_ptr& t,long long a,long long b,long long p) const{
		if(!t)return e();
		if(t->idx==p)return t->value;
		long long c=(a+b)/2;
		if(p<c)return get(t->left,a,c,p);
		else return get(t->right,c,b,p);
	}

	S prod(long long l,long long r){
		assert(0<=l&&l<=r&&r<=n);
		return prod(root,0,n,l,r);
	}

	S prod(const node_ptr& t,long long a,long long b,long long l,long long r) const{
		if(!t||b<=l||r<=a)return e();
		if(l<=a&&b<=r)return t->product;
		long long c=(a+b)/2;
		S result=prod(t->left,a,c,l,r);
		if(l<=t->idx&&t->idx<r)result=op(result,t->value);
		return op(result,prod(t->right,c,b,l,r));
	}

	S all_prod() const{
		return root?root->product:e();
	}

};
// END ORIGINAL dynamic_segtree.cpp


struct S{mint a,b;};S op(S l,S r){return {r.a*l.a,r.a*l.b+r.b};}S e(){return {1,0};}

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;dynamic_segtree<S,op,e>seg(n);for(int i=0,x,y;i<n;i++){cin>>x>>y;seg.set(i,{x,y});}
while(q--){int t,l,r;ll x;cin>>t;if(t==0){cin>>l>>r>>x;seg.set(l,{r,x});}else{cin>>l>>r>>x;auto f=seg.prod(l,r);cout<<(f.a*mint(x)+f.b).val()<<'\n';}}

}
