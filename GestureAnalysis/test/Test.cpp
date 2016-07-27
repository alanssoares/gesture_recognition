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
}

void
Test::initializeGestureTemplates(){
	size_t n = m_GesturesTemplate.size();
	for (int i = 0; i < n; i++){
		if(m_GesturesTemplate[i].numHands == 1){
			m_GesturesFromFileOneHand.push_back(m_GesturesTemplate[i]);
		} else {
			m_GesturesFromFileTwoHands.push_back(m_GesturesTemplate[i]);
		}
	}
}

void 
Test::execute(){
	experiment1();
}

void
Test::saveResults(){

}

void 
Test::experiment1(){
	size_t n = m_GesturesTest.size();
	for (int i = 0; i < n; i++){
		if(m_GesturesTest[i].numHands == 1){
			recognizeOneHand(m_GesturesTest[i]);
		} else {
			recognizeTwoHands(m_GesturesTest[i]);
		}
	}
}

void 
Test::experiment2(){

}

void
Test::recognizeOneHand(const type_gesture gesture) {
    int start_s = clock();

    std::vector<XnPoint3D> trajectoryHand, trajectoryComp;
    double distance = 0.0, bestDistance = 999999999;
    type_gesture gestureTemplate;
    size_t n = m_GesturesFromFileOneHand.size();

    //Process the trajectory from user
    trajectoryHand = MathUtil::smoothAndReduce(gesture.handTwo.positions);
    //Find the best match trajectory using DTW
    for (int i = 0; i < n; i++) {
        //Process the trajectory template
        trajectoryComp = MathUtil::smoothAndReduce(m_GesturesFromFileOneHand[i].handTwo.positions);
        //Compute the distance using dtw
        distance = MathUtil::computeDistanceBetweenTwoTrajectories(trajectoryComp, trajectoryHand);
        //Verify if the computed distance is lower that previous best
        if(distance < bestDistance){
            bestDistance = distance;
            gestureTemplate.name = m_GesturesFromFileOneHand[i].name;
            gestureTemplate.handTwo.positions = m_GesturesFromFileOneHand[i].handTwo.positions;
        }
    }
    
    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        m_NameGestureRecognized = gestureTemplate.name;
    }

    TIME_METHOD_EXEC("recognizeOneHand", start_s, clock());
}

void
Test::recognizeTwoHands(const type_gesture gesture) {
    int start_s = clock();
    
    std::vector<XnPoint3D> leftHandPoints, rightHandPoints, trajCompLeft, trajCompRight;
    double distanceA = 0.0, distanceB = 0.0, bestDistanceB = 999999999, bestDistanceA = 999999999;
    size_t n = m_GesturesFromFileTwoHands.size();

    //Process the trajectories
    leftHandPoints = MathUtil::smoothAndReduce(gesture.handOne.positions);
    rightHandPoints = MathUtil::smoothAndReduce(gesture.handTwo.positions);
    
    //Find the best match trajectory using DTW
    for (int i = 0; i < n; i++) {
	
	trajCompLeft = MathUtil::smoothAndReduce(m_GesturesFromFileTwoHands[i].handOne.positions);
        distanceA = MathUtil::computeDistanceBetweenTwoTrajectories(trajCompLeft, leftHandPoints);
        if (distanceA < bestDistanceA){
            bestDistanceA = distanceA;
        }
        
        trajCompRight = MathUtil::smoothAndReduce(m_GesturesFromFileTwoHands[i].handTwo.positions);
        distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(trajCompRight, rightHandPoints);
        if (distanceB < bestDistanceB){
            bestDistanceB = distanceB;
        }
        
	//Verify if the best distance is lower then the treshold
	if(bestDistanceA < MIN_DISTANCE_TRESHOLD && bestDistanceB < MIN_DISTANCE_TRESHOLD){
		m_NameGestureRecognized = m_GesturesFromFileTwoHands[i].name;
	}
    }
    
    TIME_METHOD_EXEC("recognizeTwoHands", start_s, clock());
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

void
Test::generateMedianGestures(){
	size_t n = m_AllGestures.size();
	int j = 0, i = 0, numTests = 0;
	while(i < n){
		j = i + 1;
		while(m_AllGestures[i].name.compare(m_AllGestures[j].name) == 0) j++;
		m_MedianGestures.push_back(resampleGesture(i, j));
		i = j;
	}
}

type_gesture
Test::resampleGesture(const int initGesture, const int endGesture){
	type_gesture medianGesture = m_AllGestures[initGesture];
	int n = medianGesture.handOne.positions.size();
	int diff = 0;
	
	for(int i = initGesture + 1; i < endGesture; i++){
		diff = n - m_AllGestures[i].handOne.positions.size();
		if(diff > 0){
			m_AllGestures[i].handOne.positions = inserePositions(m_AllGestures[i].handOne.positions, diff);
			m_AllGestures[i].handTwo.positions = inserePositions(m_AllGestures[i].handTwo.positions, diff);
		} else if(diff < 0) {
			m_AllGestures[i].handOne.positions = removePositions(m_AllGestures[i].handOne.positions, diff);
			m_AllGestures[i].handTwo.positions = removePositions(m_AllGestures[i].handTwo.positions, diff);
		}
		for(int j = 0; j < n; j++){
			medianGesture.handOne.positions[j] = MathUtil::sum(medianGesture.handOne.positions[j], m_AllGestures[j].handOne.positions);
			medianGesture.handTwo.positions[j] = MathUtil::sum(medianGesture.handTwo.positions[j], m_AllGestures[i].handTwo.positions);
		}
	}
	
	return medianGesture;
}
