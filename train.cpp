#include "hmm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

#define STATE_NUM 6
#define MAX_TIME 50
#define MODEL_NUM 5
#define SAMPLE_SIZE 12000

// int *forwarding(HMM hmm);

class MyClass{
    private:
        int time_length;
        int state_length;

    public:
        double alpha[MAX_TIME+2][STATE_NUM];
        double beta[MAX_TIME+2][STATE_NUM];
        double gamma[MAX_TIME+2][STATE_NUM];
        int forward(HMM hmm, int o[MAX_TIME]);
        int backward(HMM hmm, int o[MAX_TIME]);
        int computeGamma();
        int display();
        
        MyClass(int,int);
        
    
        
        

};

int observation[MODEL_NUM][SAMPLE_SIZE][MAX_TIME];
void loadSeq(string str, int id);
int char2int(char);

int main(){
    
    HMM hmm;
	loadHMM( &hmm, "model_init.txt" );
	// dumpHMM( stderr, &hmm);
    for(int i = 1; i <= 5; ++i){
        char buffer[10];
        sprintf(buffer,"%d",i);
        string filename = "seq_model_0" + string(buffer) + ".txt";
        // cout << filename;
        loadSeq(filename, i-1);
    }
    MyClass f = MyClass(MAX_TIME, 6);
    f.forward(hmm, observation[0][0]);
    f.backward(hmm, observation[0][0]);
    f.computeGamma();
    // f.display();
    

    
    

}

void loadSeq(string file, int id){
    ifstream inFile(file.c_str());
    string line;
    int t = 0;
    while(getline(inFile, line)){
        for(int i = 0; i < line.length(); ++i){           
            observation[id][t][i] = char2int(line[i]);
        }
        t++;
    }
    return;
}

int char2int(char s){ 
    int x = s - 65;
    return x;
}



MyClass::MyClass(int t,int s){
    this->time_length = t;
    this->state_length = s;
}

int MyClass::forward(HMM hmm, int o[MAX_TIME]){
    // init
    for(int i = 0; i < this->state_length; ++i){
        // i is now state
        alpha[0][i] = hmm.initial[i] * hmm.observation[o[0]][i];
    }
    // induction
    for(int t = 1; t < this->time_length; ++t){
        for(int j = 0; j < this->state_length; ++j){
            // current state j
            double p = 0.0;
            for(int i = 0; i < this->state_length; ++i){
                p += alpha[t-1][i] * hmm.transition[i][j];
            }
            alpha[t][j] = p * hmm.observation[o[t]][j];
        }
    }
    /* compute P(O|lambda), optional */
    double p = 0.0;
    for(int i = 0; i < this->state_length; ++i){
        p += alpha[this->time_length-1][i];
    }
    cout << "Forward p " << p << endl;

}

int MyClass::backward(HMM hmm, int o[MAX_TIME]){
    // init
    for(int i = 0; i < this->state_length; ++i){
        // i is now state
        beta[this->time_length-1 ][i] = 1.0;
    }
    // induction
    for(int t = this->time_length-2; t >= 0 ; --t){
        for(int i = 0; i < this->state_length; ++i){
            // current state i
            double p = 0.0;
            for(int j = 0; j < this->state_length; ++j){
                p += hmm.transition[i][j] * hmm.observation[o[t+1]][j];
            }
            beta[t][i] = p;
        }
    }
    /* compute P(O|lambda), optional */
    double p = 0.0;
    for(int j = 0; j < this->state_length; ++j){
        p += hmm.initial[j] * hmm.observation[o[0]][j] * beta[0][j];
    }
     cout << "Backward p " << p << endl;
}

int MyClass::computeGamma(){
    for (int t = 0; t < this->time_length; ++t){
        double p = 0.0;
        for(int i = 0; i < this->state_length; ++i){
            p += alpha[t][i] * beta[t][i];
            cout << t << " " << i << " " << p << endl;
        }           
       
        assert(p != 0); 
        for (int i = 0; i < this->state_length; ++i)
            gamma[t][i] = alpha[t][i] * beta[t][i] / p;
    }
}

int MyClass::display(){
    for(int t = 0; t < this->time_length; ++t){
        for(int i = 0; i < this->state_length; ++i){
            cout << t << " " << alpha[t][i] << endl;
            cout << t << " " << beta[t][i] << endl;
        }
        cout << endl;
    }
}