#include <iostream>
#include <cmath>
#include <vector>
#include "pre.cpp"

using namespace std;

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
