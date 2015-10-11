//
//  Testes.cpp
//  GestureTracking
//
//  Created by Alan Santos on 27/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "Testes.h"


/*
    USAGE: trainhmm INIT-HMM RESULT-HMM DATA [MAX-ITERATIONS]
 */
void
Testes::trainHMM(char* fileProb, const char *fileOutPut, const char* stream, int maxIterations)
{
    Hmm hmm;
    vector<vector<unsigned long>*> trainingSequences;
    
    hmm.loadProbs(fileProb);
    ifstream istrm(stream);
    hmm.readSeqs(istrm, trainingSequences);
    hmm.baumWelch(trainingSequences, maxIterations);
    hmm.saveProbs(fileOutPut);

}

/*
    USAGE: genseq NAME N" generates N observation sequences using the HMM with the given NAME
 */
void
Testes::generateSequence(char* fileProb, int seqs)
{
    Hmm hmm;
    hmm.loadProbs(fileProb);
    hmm.genSeqs(cout, seqs);
}

void
Testes::testeViterbi(char* fileProb)
{
    Hmm hmm;
    
    hmm.loadProbs(fileProb);
    vector<vector<unsigned long>*> seqs;
    hmm.readSeqs(cin, seqs);
    
    for (unsigned int i = 0; i<seqs.size(); i++) {
        vector<unsigned long>& seq = *seqs[i];
        for (unsigned int j =0; j<seq.size(); j++) {
            hmm.addObservation(seq[j]);
        }
        vector<Transition*> path;
        double jointProb = hmm.viterbi(path);
        cout << "P(path)=" << exp(jointProb-hmm.obsProb()) << endl << "path: " << endl;
        for (unsigned int i = 0; i<path.size(); i++) {
            Transition* trans = path[i];
            if (trans==0) continue;
            cout << hmm.getStr(trans->_obs) << '\t'
            << hmm.getStr(trans->_to->state()) << endl;
        }
        hmm.reset();
    }
}