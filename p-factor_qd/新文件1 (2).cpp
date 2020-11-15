#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int main() {
	vector<int> fac = {2, 3};
	vector<short> fac_c = {2, 2};
	string N;
	cin >> N;
	vector<int> query;
	for(short i = 0; i < fac.size(); i++) {
		for(short j = 0; j < fac_c[i]; j++) {
			short T = ceil(log2(fac[i])), num = 0;
			for(short k = 0; k < T; k++) num += pow(2, T - k - 1)*(int(N[k]) - 48);
			num++;
			query.push_back(num);
			N.erase(N.begin(), N.begin()+T);
		}
	}
	long long int res = 1;
	for(auto i : query) res *= i;
	cout << res << "\n";
	return 0;
}
