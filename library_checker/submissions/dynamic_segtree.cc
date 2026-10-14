#include <bits/stdc++.h>
using namespace std;
using ll = long long;


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

ll op(ll a,ll b){return a+b;} ll e(){return 0;}
int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

int n,q;cin>>n>>q;dynamic_segtree<ll,op,e> seg(n);
for(int i=0;i<n;i++){ll x;cin>>x;seg.set(i,x);}
while(q--){int t,l;ll r;cin>>t>>l>>r;if(t==0)seg.set(l,seg.get(l)+r);else cout<<seg.prod(l,r)<<'\n';}

}
