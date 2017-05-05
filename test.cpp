#include "hmm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;
#define STATE_NUM 6
#define OBSERV_NUM 6
#define MAX_TIME 50
#define MODEL_NUM 5
#define TEST_SAMPLE_SIZE 2500

void loadSeq(char*);
void loadAns(string);
int char2int(char);
double viterbi(HMM, int[]);



int observation[TEST_SAMPLE_SIZE][MAX_TIME];
int answer[TEST_SAMPLE_SIZE];
int main(int argc, char* argv[]){
    HMM hmms[5];
	load_models( argv[1], hmms, 5);
    

    // dump_models(hmms, 5);
    loadSeq(argv[2]);
    
   
    int guess[TEST_SAMPLE_SIZE];
    double guessP[TEST_SAMPLE_SIZE];
    for(int i = 0; i < TEST_SAMPLE_SIZE; ++i){
        // double p[MODEL_NUM] = {0.0};
        double p = -1.0;
        int targetID = 0;
        // cout << "SAMPLE " << i << " ";
        for(int id = 0; id < MODEL_NUM; ++id){
            double myp = viterbi(hmms[id], observation[i]);           
            if(myp > p){
                p = myp;
                targetID = id;
            }           
            
        }  
        // cout << endl; 
        guess[i] = targetID;  
        guessP[i] = p;
         
    }
    
    char *output = NULL;
    output = strstr (argv[2],"1");

    if(output){

        // compute test_data_1 acc.
        string str = "testing_answer.txt";
        loadAns(str);
        double p = 0;
        for(int i = 0; i < TEST_SAMPLE_SIZE; ++i){       
            if(guess[i] == answer[i])
                p++;
        }
        p /= TEST_SAMPLE_SIZE;
        cout << p;

        // output acc.
        ofstream  fsAcc;
        fsAcc.open("acc.txt" , ios::out);
        fsAcc << p;
        fsAcc.close();
    }
   
   // output file result.txt
   ofstream  fs;
   fs.open(argv[3],  ios::out);
   for(int i = 0; i < TEST_SAMPLE_SIZE; ++i){
       fs << "model_0" << guess[i] + 1 << ".txt";
       fs << " ";
       fs << guessP[i];
       fs << "\n";
   }
   fs.close();

 

    
	
}

double viterbi(HMM hmm, int o[]){ 
    // cout << o << endl;
    // for(int i = 0; i < MAX_TIME; ++i){
    //     cout << o[i];
    // }
    // cout << endl;
    double delta[MAX_TIME][STATE_NUM] = {{0.0}};
    // init
    for(int i = 0; i < STATE_NUM; ++i){
        delta[0][i] = hmm.initial[i] *  hmm.observation[o[0]][i];
    }
    for(int t = 0; t < MAX_TIME-1; ++t){        
        // compute state j's delta prob at time(t+1)     
        for(int j = 0; j < STATE_NUM; ++j){
            double maxP = -1.0; 
            for(int i = 0; i < STATE_NUM; ++i){
                double p  = delta[t][i] * hmm.transition[i][j]; 
                if(p > maxP){
                    maxP = p;                    
                }
            }
            delta[t+1][j] = maxP * hmm.observation[o[t+1]][j];;
            assert(maxP >= -0.5);
        }
    }
    double P = -1;
    for(int i = 0; i <STATE_NUM; ++i){
        if(delta[MAX_TIME-1][i] > P)
            P = delta[MAX_TIME-1][i];
    }
    
//    cout << P << " ";
   return P;
}

void loadSeq(char* file){
    ifstream inFile(file);
    string line;
    int t = 0;
    while(getline(inFile, line)){
        for(int i = 0; i < line.length(); ++i){        
            observation[t][i] = char2int(line[i]);
        }
        t++;
    }
    return;
}


void loadAns(string file){
    ifstream inFile(file.c_str());
    string line;
    int i = 0;
    while(getline(inFile, line)){        
        char c = line[7];
       
        int a = c - '1';
        // cout << a << endl;
        answer[i] = a ;
        i++;
    }
    return;
}

int char2int(char s){ 
    int x = s - 65;
    return x;
}

