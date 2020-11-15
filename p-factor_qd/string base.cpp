#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

int main() {
	long long int N;
	cin >> N;
	stack<short> N_2;
	while(N >= 2) {
		N_2.push(N%2);
		N = (N-(N%2))/2;
	}
	N_2.push(N);
	vector<short> N2;
	N2.push_back(0);
	while(!N_2.empty()) {
		N2.push_back(N_2.top());
		N_2.pop();
	}
	vector<short> coor;
	vector<short> value;
	for(int i = N2.size()-2; i >= 1; i--) {
		if(N2[i] == 1 && N2[i-1] == 0) {
			coor.push_back(i);
			short sum_pN = 0;
			for(int j = N2.size()-1; j >= i; j--) if(N2[j] == 1) sum_pN++;
			value.push_back(2*(sum_pN-1)-(N2.size()-i));
		}
	}
	for(auto i : N2) cout << i << ", ";
	cout << "\n";
	for(auto i : coor) cout << i << ", ";
	cout << "\n";
	for(auto i : value) cout << i << ", ";
	cout << "\n";
	
	cout << "As for the best results: ";
	short best = *max_element(value.begin(), value.end());
	cout << best << " string shorter\n";
	cout << "In coor: ";
	for(int i = 0; i < value.size(); i++) if(value[i] == best) cout << coor[i] << ", ";
	
	
	//full - sequence
	
	
	return 0;
}
