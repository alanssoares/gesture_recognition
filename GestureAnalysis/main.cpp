//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "../Commons/commons.hpp"
#include "test/Test.h"

int main(int argc, char* argv[])
{
    Test test;

    test.init();
    
    test.experiment(1, "../result_experiment1.txt");
    test.experiment(2, "../result_experiment2.txt");
    test.experiment(3, "../result_experiment3.txt");
    test.experiment(4, "../result_experiment4.txt");
    test.experiment(5, "../result_experiment5.txt");
    test.experiment(6, "../result_experiment6.txt");
    test.experiment(7, "../result_experiment7.txt");
    test.experiment(8, "../result_experiment8.txt");
    test.experiment(9, "../result_experiment9.txt");
    test.experiment(10, "../result_experiment10.txt");
    test.experiment(11, "../result_experiment11.txt");
    test.experiment(12, "../result_experiment12.txt");

	return 0;
}