#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

int S_H = 0;
vector<int> N_Fac;
vector<int> N_sFac;
vector<int> N_sFac_c;

vector<int> N_CCXcount;

vector<vector<short>> Ap_i;
vector<vector<string>> Pp_i;

bool is_prime(int k) {
    for(int i = 2; i <= sqrt(k); i++) {
        if(k%i == 0 && k != i) return false;
    }
    return true;
}

int fac_N(int N) {
    int i = 2;
    while(1) {
        if(is_prime(N)) break;
        else if(is_prime(i) && N%i == 0) {
            N_Fac.push_back(i);
            N/=i;
        }
        else i++;
    }
    N_Fac.push_back(N);
}

int Cc; //CCXcount
vector<string> Ry_N;

void Simp(int a, int b) {
    int Ta = a, Tb = b;
    while(a != 0 && b != 0) {
        if(a > b) a %= b;
        else b %= a;
    }
    Cc++;
    Ry_N.push_back(to_string(Ta/max(a, b)) + '/' + to_string(Tb/max(a, b)));
}

void Nface_Data(int N) {
	Cc = 0;
	Ry_N.clear();
	vector<short> A_N;
	for(int I = pow(2, ceil(log2(N) - 1)); I >= 1; I /= 2) {
    	if(N < I) A_N.push_back(0), Ry_N.push_back("0");
        else {
            if(N > I) Simp(N - I, N);
            N -= I;
            A_N.push_back(1);
            if(N == 0) break;
        }
    }
    Pp_i.push_back(Ry_N);
    Ap_i.push_back(A_N);
    N_CCXcount.push_back(Cc);
}

void simpN_Fac() {
	int i = 0;
	while(i < N_Fac.size()) {
		int l_pi = 1;
		if(i != N_Fac.size() - 1) {
			int j = i+1;
			while(N_Fac[i] == N_Fac[j]) l_pi++, j++;
		}
		N_sFac.push_back(N_Fac[i]);
		N_sFac_c.push_back(l_pi);
		i += l_pi;
	}
}

void H_pre_simp() {
	S_H += N_sFac_c[0];
	N_sFac.erase(N_sFac.begin(), N_sFac.begin() + 1);
	N_sFac_c.erase(N_sFac_c.begin(), N_sFac_c.begin() + 1);
}

vector<string> Measure_qubits;

vector<short> N_sneeded_qcount;
short _needed_bcount;

void qubit_need_measure() {
	short loc = S_H;
	for(int i = 0; i < Ap_i.size(); i++) {
		int T = ceil(log2(N_sFac[i]));
		
		int pre_AC = 0, data_qc = 0; //pre activecount & data qubit count
		for(int j = 0; j < Ap_i[i].size(); j++) if(Ap_i[i][j] == 1) pre_AC++;
		if(pre_AC <= 2) data_qc = 0;
	    else if(pre_AC == 3) data_qc = 1;
	    else if(pre_AC >= 4) data_qc = 2;
	    
		N_sneeded_qcount.push_back(T+data_qc);
	    _needed_bcount += T*N_sFac_c[i];
	    
	    //load uti-measure qubit
	    for(int k = 0; k < N_sFac_c[i]; k++) {
	    	for(int l = loc; l < T+loc; l++) Measure_qubits.push_back(to_string(l));
	    	loc += T+data_qc;
		}
	}
}

string ry(string ry, int c) {
    return "circuit.ry(2*asin(sqrt(" + ry + ")), " + to_string(c) + ")";
}

string cry(string ry, int c1, int c2) {
    return "circuit.cry(2*asin(sqrt(" + ry + ")), " + to_string(c1) + ", " + to_string(c2) + ")";
}

string CircuitMeasure() {
	string Q = "circuit.measure([";
	for(int i = 0; i < S_H; i++) Q.append(to_string(i) + ", ");
	for(int i = 0; i < Measure_qubits.size(); i++) Q.append(Measure_qubits[i] + ", ");
	Q.erase(Q.end() - 2, Q.end());
	Q.append("], [");
	for(int i = 0; i < _needed_bcount + S_H; i++) Q.append(to_string(i) + ", ");
	Q.erase(Q.end() - 2, Q.end());
	Q.append("])");
	return Q;
}

string Barrier(short ini, short end) {
	string Q = "circuit.barrier([";
	for(int i = ini; i >= end; i--) Q.append(to_string(i) + ", ");
	Q.erase(Q.end() - 2, Q.end());
	Q.append("])");
	return Q;
}

int main() {
	//input N
	int N;
	cin >> N;
	
	fac_N(N); //factorize N
	simpN_Fac(); //simp the facs if N
	N_Fac.clear(); //discard the old fac_vector
	
	if(N_sFac[0] == 2) H_pre_simp(); //pre-count H
	for(int i = 0; i < N_sFac.size(); i++) Nface_Data(N_sFac[i]); //make infor of N
	
	//setup file
	std::fstream File;
	File.open("D:\\QDC.txt", std::ios::out | std::ios::trunc);
	File << "from math import *\n";

	//pre_count all needed qubit
	qubit_need_measure();
	short _needed_qcount = 0;
	for(int i = 0; i < N_sFac.size(); i++) _needed_qcount += N_sneeded_qcount[i]*N_sFac_c[i];
	File << "circuit = QuantumCircuit(" << _needed_qcount + S_H << ", " << _needed_bcount + S_H << ")\n";
	File << "\n";
	
	if(S_H != 0) {
		File << "#2-facedice\n" << endl;
		for(int i = 0; i < S_H; i++) File << "circuit.h(" << i << ")\n";
		File << "\n";
	}
	
	short used_qc = S_H;
	for(int i = 0; i < N_sFac.size(); i++) { //i is the Ni face
		File << "#" << N_sFac[i] << "-facedice\n";
		for(int j = 0; j < N_sFac_c[i]; j++) { //j is Ni^j
			int T = ceil(log2(N_sFac[i]));
			File << "\n";
			int LA, AC = 0; //Last_active & Active_count
		    for(int k = 0; k < Ap_i[i].size() - 1; k++) { //k is the element in A_Ni
		        if(Ap_i[i][k] == 1) {
		            AC++;
		            if(k == 0) {
		                File << ry(Pp_i[i][0], T - 1 + used_qc) << "\n";
						
						File << Barrier(T - 1 + used_qc, used_qc) << "\n";
						
		                File << "circuit.x(" << T - 1 + used_qc << ")\n";
		                for(int l = T - 2; l >= 0; l--) File << "circuit.ch(" << T - 1 + used_qc << ", " << l + used_qc << ")\n";
		                File << "circuit.x(" << T - 1 + used_qc << ")\n";
		                
						File << Barrier(T - 1 + used_qc, used_qc) << "\n";
		            }
		            else {
		                File << cry(Pp_i[i][k], LA, T - 1 - k + used_qc) << "\n";
		                
		                File << Barrier(T - 1 + used_qc, used_qc) << "\n";
		                
		                if(AC >= 4) File << "circuit.reset(" << T + (AC % 2) + used_qc << ")\n";
		                File << "circuit.x(" << T - 1 - k + used_qc << ")\n";
		                File << "circuit.ccx(" << T - 1 - k + used_qc << ", " << LA << ", " << T + (AC % 2) + used_qc << ")\n";
		                File << "circuit.x(" << T - 1 - k + used_qc << ")\n";
		                
						File << Barrier(T - 1 + used_qc, used_qc) << "\n";
		                
						for(int l = T - 2 - k; l >= 0; l--) File << "circuit.ch(" << T + (AC % 2) + used_qc << ", " << l + used_qc << ")\n";
		                if(AC >= 4) File << "circuit.reset(" << T + (AC % 2) + used_qc << ")\n";
		                
						File << Barrier(T - 1 + used_qc, used_qc) << "\n";
		                
		                if(k < N_CCXcount[i] - 1) {
		                    File << "circuit.ccx(" << T - 1 - k + used_qc << ", " << LA << ", " << T + (AC % 2) + used_qc << ")\n";
		                    
							File << Barrier(T - 1 + used_qc, used_qc) << "\n";
		                }
		            }
		
		            if(k == 0) LA = T - 1 + used_qc;
		            else if(Ap_i[i][k] == 1 && k != 0) LA = T + (AC % 2) + used_qc;
		        }
		    }
		    used_qc += N_sneeded_qcount[i];
		}
		File << "\n";
	}
	
	File << "circuit.barrier()\n";
	File << CircuitMeasure(); 

	File.close();
	
	//f^-1 feature
	string S;
	cin >> S;
	vector<int> query;
	for(short i = 0; i < N_sFac.size(); i++) {
		for(short j = 0; j < N_sFac_c[i]; j++) {
			short T = ceil(log2(N_sFac[i])), num = 0;
			for(short k = 0; k < T; k++) num += pow(2, T - k - 1)*(int(S[k]) - 48);
			num++;
			query.push_back(num);
			S.erase(S.begin(), S.begin()+T);
		}
	}
	long long int res = 1;
	for(auto i : query) res *= i;
	cout << res << "\n";
	
	return 0;
}
