//
//  Testes.h
//  GestureTracking
//
//  Created by Alan Santos on 27/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__Testes__
#define __GestureTracking__Testes__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Hmm.h"

using namespace std;

class Testes {
private:
    
public:
    void trainHMM(char* fileProb, const char *fileOutPut, const char* stream, int maxIterations = 10);
    void generateSequence(char* fileProb, int seqs);
    
    void testeViterbi(char* fileProb);
};

#endif /* defined(__GestureTracking__Testes__) */
