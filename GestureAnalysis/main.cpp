//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "../Commons/commons.hpp"
#include "test/Test.h"

int main(int argc, char* argv[])
{
    Test test;
    
    test.init();
    //test.execute();
    //test.saveResults();
    test.generateMedianGesture();
    //test.improveGestures();
    
	return 0;
}