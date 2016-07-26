//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "../Commons/commons.hpp"
#include "test/Test.h"

int main(int argc, char* argv[])
{
    Test test;
    
    test.init();
    test.execute();
    test.saveResults();

	return 0;
}