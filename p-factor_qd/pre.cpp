#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Data.cpp"

using namespace std;

vector<string> Ry_N;

void Simp(int a, int b) {
    int Ta = a, Tb = b;
    while(a != 0 && b != 0) {
        if(a > b) a %= b;
        else b %= a;
    }
    Ry_N.push_back(to_string(Ta/max(a, b)) + '/' + to_string(Tb/max(a, b)));
}

void Nface_Data(int N) {
	Ry_N.clear();
	vector<bool> A_N;
	int T = ceil(log2(N));
	for(int I = pow(2, ceil(log2(N) - 1)); I >= 1; I /= 2) {
    	if(N < I) A_N.push_back(0), Ry_N.push_back("0");
        else if(N >= I) {
            if(N > I) Simp(N - I, N);
            N -= I;
            A_N.push_back(1);
            if(N == 0) break;
        }
    }
    Pp_i.push(Ry_N);
}
