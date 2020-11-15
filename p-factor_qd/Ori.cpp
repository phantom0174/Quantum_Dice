// 可印出N面量子骰程式的程式 //
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<std::string> Ry;
short CCXcount;

// 使用輾轉相除法化簡分數 a/b
void Simp(int a, int b) {
    int Ta = a, Tb = b;
    while(a != 0 && b != 0) {
        if(a > b) a %= b;
        else b %= a;
    }
    CCXcount++;
    Ry.push_back(to_string(Ta/max(a, b)) + '/' + to_string(Tb/max(a, b)));//將化簡完的分數儲存於 Ry 旋轉紀錄矩陣中
}

string ry(string ry, int c) {
    return "circuit.ry(2*asin(sqrt(" + ry + ")), " + to_string(c) + ")";
}

string cry(string ry, int c1, int c2) {
    return "circuit.cry(2*asin(sqrt(" + ry + ")), " + to_string(c1) + ", " + to_string(c2) + ")";
}

string CircuitMeasure(short QB) {
    string D;
    for(short i = 0; i < QB; i++) D.append(to_string(i)), D.append(", ");
    D.erase(D.end() - 2, D.end());
    return "circuit.measure([" + D + "], [" + D + "])\n";
}

int main() {
    int N, T;
    vector<short> A;

    cin >> N;
    T = ceil(log2(N));

    if(log2(N) != int(log2(N))) { //判別N是否為2^n
        for(int I = pow(2, ceil(log2(N) - 1)); I >= 1; I /= 2) {
            if(N < I) A.push_back(0), Ry.push_back("0");
            else if(N >= I) {
                if(N > I) Simp(N - I, N);
                N -= I;
                A.push_back(1);
                if(N == 0) break;
            }
        }
    }
    else A.push_back(-1);

    std::fstream File;
    File.open("D:\\QDC.txt", std::ios::out | std::ios::trunc);

    int SH; //Stable_H
    int LA, AC = 0; //Last_active & Active_count

    int pAC, dq; //pre - Active_Count & Differential_qubit

    if(A[0] == -1) goto skip;

    for(int i = 0; i < A.size(); i++) if(A[i] == 1) pAC++;
    if(pAC == 1) dq = 0;
    else if(pAC == 2) dq = 1;
    else if(pAC >= 3) dq = 2;
    
    File << "from math import *\n";
    File << "circuit = QuantumCircuit(" << T + dq << ", " << T << ")\n";

    SH = T - A.size();
    for(int i = 0; i < A.size() - 1; i++) {
        if(A[i] == 1) {
            AC++;
            if(i == 0) {
                File << ry(Ry[0], T - 1) << "\n";
                File << "circuit.barrier()\n";
                File << "circuit.x(" << T - 1 << ")\n";
                for(int j = T - 2; j >= SH; j--) File << "circuit.ch(" << T - 1 << ", " << j << ")\n";
                File << "circuit.x(" << T - 1 << ")\n";
                File << "circuit.barrier()\n";
            }
            else {
                File << cry(Ry[i], LA, T - 1 - i) << "\n";
                File << "circuit.barrier()\n";
                //判別 0的 ccX
                if(AC >= 4) File << "circuit.reset(" << T + (AC % 2) << ")\n";
                File << "circuit.x(" << T - 1 - i << ")\n";
                File << "circuit.ccx(" << T - 1 - i << ", " << LA << ", " << T + (AC % 2) << ")\n";
                File << "circuit.x(" << T - 1 - i << ")\n";
                File << "circuit.barrier()\n";
                for(int j = T - 2 - i; j >= SH; j--) File << "circuit.ch(" << T + (AC % 2) << ", " << j << ")\n";
                File << "circuit.reset(" << T + (AC % 2) << ")\n";
                File << "circuit.barrier()\n";
                //判別 1的 ccX
                if(i < CCXcount - 1) {
                    File << "circuit.ccx(" << T - 1 - i << ", " << LA << ", " << T + (AC % 2) << ")\n";
                    File << "circuit.barrier()\n";
                }
            }

            if(i == 0) LA = T - 1;
            else if(A[i] == 1 && i != 0) LA = T + (AC % 2);
        }
    }
    for(int j = 0; j < SH; j++) File << "circuit.h(" << j << ")\n";
    File << "circuit.barrier()\n";

    skip:
        if(A[0] == -1) {
            for(int i = 0; i < T; i++) File << "circuit.h(" << i << ")\n";
            File << "circuit.barrier()\n";
        }

    File << CircuitMeasure(T);
    
    File.close();
}
