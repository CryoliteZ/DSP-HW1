#include "hmm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
using namespace std;

#define STATE_NUM 6
#define MAX_TIME 50
#define MODEL_NUM 5
#define SAMPLE_SIZE 12000

// int *forwarding(HMM hmm);

class Forwarding{
    public:
        double alpha[MAX_TIME+2][STATE_NUM];
        Forwarding();
    private:
        int time_length;
        int state_length;

};

string observation[MODEL_NUM][SAMPLE_SIZE][MAX_TIME];
void loadSeq(string str, int id);

int main(){
    
    HMM hmm;
	loadHMM( &hmm, "model_init.txt" );
	dumpHMM( stderr, &hmm);
    for(int i = 1; i <= 5; ++i){
        char buffer[10];
        sprintf(buffer,"%d",i);
        string filename = "seq_model_0" + string(buffer) + ".txt";
        // cout << filename;
        loadSeq(filename, i-1);
    }
    
    

}

void loadSeq(string file, int id){
    ifstream inFile(file.c_str());
    string line;
    int t = 0;
    while(getline(inFile, line)){
        for(int i = 0; i < line.length(); ++i){      
            observation[id][t][i] = line[i];
        }
        t++;
    }
    return;
}



Forwarding::Forwarding(){
    
}