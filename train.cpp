#include "hmm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define STATE_NUM 6

int *forwarding(HMM hmm);


int main(){
    HMM hmm;
	loadHMM( &hmm, "model_init.txt" );
	dumpHMM( stderr, &hmm);

    int *y = copy();
    for(int i = 0; i < 10; ++i){
        cout << y[i];
    }
    

}

int forwarding(hmm hmm){
    int alpha[STATE_NUM];
    // initialization
    for(int i = 0; i < STATE_NUM; ++i){
        alpha[i] = hmm.initial[i] * 
    }

}