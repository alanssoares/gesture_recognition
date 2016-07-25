#include "Test.h"

void 
Test::init(){
	FileUtil& futil = FileUtil::getInstance();

	m_GesturesTemplate.clear();
	m_GesturesTest.clear();

	for (int i = 0; i < futil.mGesturesOneHand.size(); i++){
		m_AllGestures.push_back(futil.mGesturesOneHand[i]);
	}
	for (int i = 0; i < futil.mGesturesTwoHands.size(); i++){
		m_AllGestures.push_back(futil.mGesturesTwoHands[i]);
	}

	std::sort(m_AllGestures.begin(), m_AllGestures.end(), sortByName);
}

void 
Test::execute(){

}

void
Test::saveResults(){

}

void 
Test::experiment1(){

}

void 
Test::experiment2(){

}
