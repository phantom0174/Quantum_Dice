int T = ceil(log2(N));


short CCXcount, U = 0; //Used qubit count

string ry(string ry, int c) {
    return "circuit.ry(2*asin(sqrt(" + ry + ")), " + to_string(c) + ")";
}

string cry(string ry, int c1, int c2) {
    return "circuit.cry(2*asin(sqrt(" + ry + ")), " + to_string(c1) + ", " + to_string(c2) + ")";
}

string CircuitMeasure(short QB) {
    string D;
    for(short i = 0; i < QB; i++) D.append(to_string(i + U)), D.append(", ");
    D.erase(D.end() - 2, D.end());
    return "circuit.measure([" + D + "], [" + D + "])\n";
}

void Make_N_fdice() {
    Ry.clear();
    CCXcount = 0;
    int N, T;
    vector<short> A;

    cin >> N;
    T = ceil(log2(N));

    if(log2(N) != int(log2(N))) {
    }
    else A.push_back(-1);

    for(int i = 0; i < A.size(); i++) cout << A[i] << endl;
    for(int i = 0; i < Ry.size(); i++) cout << Ry[i] << endl;

    int SH; //Stable_H
    

    int pAC = 0, dq; //pre - Active_Count & Differential_qubit

    if(A[0] == -1) goto skip;

    for(int i = 0; i < A.size(); i++) if(A[i] == 1) pAC++;
    if(pAC == 1) dq = 0;
    else if(pAC == 2) dq = 1;
    else if(pAC >= 3) dq = 2;
    
    File << "from math import *\n";
    File << "circuit = QuantumCircuit(" << T + dq << ", " << T << ")\n";
	
	int LA, AC = 0; //Last_active & Active_count
    for(int i = 0; i < A.size() - 1; i++) {
        if(A[i] == 1) {
            AC++;
            if(i == 0) {
                File << ry(Ry[0], T - 1) << "\n";
                File << "circuit.barrier()\n";
                File << "circuit.x(" << T - 1 << ")\n";
                for(int j = T - 2; j >= 0; j--) File << "circuit.ch(" << T - 1 << ", " << j << ")\n";
                File << "circuit.x(" << T - 1 << ")\n";
                File << "circuit.barrier()\n";
            }
            else {
                File << cry(Ry[i], LA, T - 1 - i) << "\n";
                File << "circuit.barrier()\n";
                
                if(AC >= 4) File << "circuit.reset(" << T + (AC % 2) << ")\n";
                File << "circuit.x(" << T - 1 - i << ")\n";
                File << "circuit.ccx(" << T - 1 - i << ", " << LA << ", " << T + (AC % 2) << ")\n";
                File << "circuit.x(" << T - 1 - i << ")\n";
                File << "circuit.barrier()\n";
                for(int j = T - 2 - i; j >= 0; j--) File << "circuit.ch(" << T + (AC % 2) << ", " << j << ")\n";
                File << "circuit.reset(" << T + (AC % 2) << ")\n";
                File << "circuit.barrier()\n";
                
                if(i < CCXcount - 1) {
                    File << "circuit.ccx(" << T - 1 - i << ", " << LA << ", " << T + (AC % 2) << ")\n";
                    File << "circuit.barrier()\n";
                }
            }

            if(i == 0) LA = T - 1;
            else if(A[i] == 1 && i != 0) LA = T + (AC % 2);
        }
    }

    skip:
        if(A[0] == -1) {
            for(int i = 0; i < T; i++) File << "circuit.h(" << i << ")\n";
            File << "circuit.barrier()\n";
        }

    File << CircuitMeasure(T);
    
    File.close();
}
