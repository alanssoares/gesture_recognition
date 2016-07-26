#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
}

Test::~Test(){}

void 
Test::init(){
	//Load all samples
	FileUtil::getInstance().loadGestures();
	//Clear all gestures template and test
	clearAll();
	//Merge all gestures
	loadAll();
	//Split the samples in templates and test
	splitDataset();
	//Initialize the gestures template
	initializeGestureTemplates();
}

void
Test::loadAll(){
	FileUtil& futil = FileUtil::getInstance();
	for (int i = 0; i < futil.mGesturesOneHand.size(); i++){
		m_AllGestures.push_back(futil.mGesturesOneHand[i]);
	}
	for (int i = 0; i < futil.mGesturesTwoHands.size(); i++){
		m_AllGestures.push_back(futil.mGesturesTwoHands[i]);
	}
	//Sort by gesture name
	std::sort(m_AllGestures.begin(), m_AllGestures.end(), sortByName);
}

void
Test::clearAll(){
	m_GesturesTemplate.clear();
	m_GesturesTest.clear();
}

void
Test::splitDataset(){
	size_t n = m_AllGestures.size();
	int j = 0, i = 0, numTests = 0;
	while(i < n){
		j = i + 1;
		while(m_AllGestures[i].name.compare(m_AllGestures[j].name) == 0) j++;
		numTests = (j - i) * m_PercentTest;
		while(numTests > 0) {
			m_GesturesTest.push_back(m_AllGestures[i++]);
			numTests--;
		}
		while(i < j) {
			m_GesturesTemplate.push_back(m_AllGestures[i++]);
		}
	}

	PRINT("Tests: " << m_GesturesTest.size());
	PRINT("Templ: " << m_GesturesTemplate.size());
}

void
Test::initializeGestureTemplates(){
	std::vector<type_gesture> oneHand, twoHands;
	size_t n = m_GesturesTemplate.size();
	for (int i = 0; i < n; i++){
		if(m_GesturesTemplate[i].numHands == 1){
			oneHand.push_back(m_GesturesTemplate[i]);
		} else {
			twoHands.push_back(m_GesturesTemplate[i]);
		}
	}
	Gesture::getInstance().setGesturesFromFile(oneHand, twoHands);
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

void
Test::improveGestures(){  
    FileUtil& fileUtil = FileUtil::getInstance();
    
    fileUtil.loadGestures();

    size_t n1 = fileUtil.mGesturesOneHand.size();
    size_t n2 = fileUtil.mGesturesTwoHands.size();
    
    for (int i = 0; i < n1; i++){
        //PRINT("A[" << i << "] - Before : " << fileUtil.mGesturesOneHand[i].handOne.positions.size());
        fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesOneHand[i].handOne.positions);
        fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesOneHand[i].handOne.positions);
        //PRINT("A[" << i << "] - After : " << fileUtil.mGesturesOneHand[i].handOne.positions.size());
        fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesOneHand[i].handTwo.positions);
        fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesOneHand[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << fileUtil.mGesturesOneHand[i].handTwo.positions.size());
    }
    
    for (int i = 0; i < n2; i++){
        //PRINT("B_1[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handOne.positions);
        fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handOne.positions);
        //PRINT("B_1[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        //PRINT("B_2[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
        fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
    }

    fileUtil.saveAll();
}