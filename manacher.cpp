template<class T> vector<int> manacher(const T&s){
    int n=s.size();vector<int>R(n);
	int i=0,j=0;
	while (i < s.size()) {
	while (i-j >= 0 && i+j < s.size() && s[i-j] == s[i+j]) ++j;
	R[i] = j; int k = 1;
	while (i-k >= 0 && k+R[i-k] < j) R[i+k] = R[i-k], ++k;
	i += k; j -= k;
	}
	return R;
}