#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
	m_NameFileResults = "../Results.txt";
}

Test::~Test(){}

float
Test::getFinalTime(std::clock_t start_s){
	return (std::clock() - start_s)/double(CLOCKS_PER_SEC);
}

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
		while((j < n) && (m_AllGestures[i].name.compare(m_AllGestures[j].name) == 0)) j++;
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
	std::ofstream fileCreate(m_NameFileResults.c_str());
	fileCreate.close();
	
	experiment1();
}

void
Test::saveResults(type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized){
	std::fstream fileOut;
	fileOut.open(m_NameFileResults.c_str(), ios::in | ios::out | ios::ate);
	if(fileOut.is_open()){
		fileOut<<gestureExecuted.name<<" "<<gestureExecuted.numHands<<" "<<gestureExecuted.handOne.positions.size()<<" ";
		fileOut<<gesturePredicted.name<<" "<<gesturePredicted.numHands<<" "<<gesturePredicted.handOne.positions.size()<<" ";
		fileOut<<timeExecution<<" "<<bestDistance<<" "<<isRecognized<<std::endl;
	}
	fileOut.close();
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
    clock_t start_s = clock(), isRecognized = 0;
    float distance = 0.0, bestDistance = 999999999;
    size_t n = m_GesturesFromFileOneHand.size();
    type_gesture gestureTemplate;
    std::vector<XnPoint3D> trajectoryHand, trajectoryComp;

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
            gestureTemplate = m_GesturesFromFileOneHand[i];
        }
    }
    
    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        isRecognized = 1;
    }

	saveResults(gesture, gestureTemplate, getFinalTime(start_s), bestDistance, isRecognized);
}

void
Test::recognizeTwoHands(const type_gesture gesture) {
    float start_s = clock(), isRecognized = 0;
    float distanceA = 0.0, distanceB = 0.0, bestDistanceB = 999999999, bestDistanceA = 999999999;
    size_t n = m_GesturesFromFileTwoHands.size();
	type_gesture gestureTemplate;
	std::vector<XnPoint3D> leftHandPoints, rightHandPoints, trajCompLeft, trajCompRight;

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

        if(bestDistanceA < MIN_DISTANCE_TRESHOLD && bestDistanceB < MIN_DISTANCE_TRESHOLD){
        	gestureTemplate = m_GesturesFromFileTwoHands[i];
        }
    }

	//Verify if the best distance is lower then the treshold
	if(bestDistanceA < MIN_DISTANCE_TRESHOLD && bestDistanceB < MIN_DISTANCE_TRESHOLD){
		isRecognized = 1;
	}

	//TODO: verificar qual bestdistance usar para salvar
	saveResults(gesture, gestureTemplate, getFinalTime(start_s), (bestDistanceB + bestDistanceB)/2 , isRecognized);
}

void
Test::generateMedianGesture(){
	size_t n = m_AllGestures.size(), i = 0, j = 0;
	generateGestureEqualSize();
	while(j < n){
		j = i + 1;
		while((j < n) && (m_AllGestures[i].name.compare(m_AllGestures[j].name) == 0)) j++;
		m_MedianGestures.push_back(getMeanGesture(i, j - 1));
		i = j;
	}
	saveMedianGestures();
}

void
Test::saveMedianGestures(){
	FileUtil& futil = FileUtil::getInstance();
	size_t n = m_MedianGestures.size();

	futil.mGesturesOneHand.clear();
	futil.mGesturesTwoHands.clear();

	for (int i = 0; i < n; i++){
		m_MedianGestures[i].handOne.positions = MathUtil::normCenterOrigin(m_MedianGestures[i].handOne.positions);
        m_MedianGestures[i].handTwo.positions = MathUtil::normCenterOrigin(m_MedianGestures[i].handTwo.positions);
		if(m_MedianGestures[i].numHands == 1){
			futil.mGesturesOneHand.push_back(m_MedianGestures[i]);
		} else {
			futil.mGesturesTwoHands.push_back(m_MedianGestures[i]);
		}
	}

	futil.saveAll();
}

void
Test::generateGestureEqualSize(){
	int n = m_AllGestures.size(), i = 0, j = 0, mean = 0, diff = 0;
	while(j < n){
		j = i + 1;
		while(m_AllGestures[i].name.compare(m_AllGestures[j].name) == 0) j++;
		mean = getMeanPoints(m_AllGestures, i, j - 1);
		for(int k = i; k < j - 1; k++){
			diff = mean - m_AllGestures[k].handOne.positions.size();
			if(diff > 0){
				MathUtil::insertPoints(&m_AllGestures[k].handOne.positions, diff);
				MathUtil::insertPoints(&m_AllGestures[k].handTwo.positions, diff);
			} else if(diff < 0) {
				MathUtil::removePoints(&m_AllGestures[k].handOne.positions, diff);
				MathUtil::removePoints(&m_AllGestures[k].handTwo.positions, diff);
			}
		}
		i = j;
	}
}

type_gesture
Test::getMeanGesture(const int k, const int p){
	const size_t n = m_AllGestures[k].handOne.positions.size();
	type_gesture medianGesture = m_AllGestures[k];

	for(int i = k + 1; i < p; i++){
		for(int j = 0; j < n; j++){
			medianGesture.handOne.positions[j] = MathUtil::sum(medianGesture.handOne.positions[j], m_AllGestures[i].handOne.positions[j]);
			medianGesture.handTwo.positions[j] = MathUtil::sum(medianGesture.handTwo.positions[j], m_AllGestures[i].handTwo.positions[j]);
		}
	}

	for(int i = 0; i < n; i++){
		medianGesture.handOne.positions[i].X = medianGesture.handOne.positions[i].X / n;
		medianGesture.handOne.positions[i].Y = medianGesture.handOne.positions[i].Y / n;
		medianGesture.handOne.positions[i].Z = medianGesture.handOne.positions[i].Z / n;
		
		medianGesture.handTwo.positions[i].X = medianGesture.handTwo.positions[i].X / n;
		medianGesture.handTwo.positions[i].Y = medianGesture.handTwo.positions[i].Y / n;
		medianGesture.handTwo.positions[i].Z = medianGesture.handTwo.positions[i].Z / n;
	}
	
	return medianGesture;
}

int
Test::getMeanPoints(vector<type_gesture> gestures, int i, int j){
    int max = 0, min = 99999999, n;
    for( ; i < j; i++){
    	n = gestures[i].handOne.positions.size();
        if(max < n){
        	max = n;
        }
        if(min > n){
        	min = n;
        }
    }
    return (max + min) / 2;
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
        //fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesOneHand[i].handOne.positions);
        //PRINT("A[" << i << "] - After : " << fileUtil.mGesturesOneHand[i].handOne.positions.size());
        fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesOneHand[i].handTwo.positions);
        //fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesOneHand[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << fileUtil.mGesturesOneHand[i].handTwo.positions.size());
    }
    
    for (int i = 0; i < n2; i++){
        //PRINT("B_1[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handOne.positions);
        //fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handOne.positions);
        //PRINT("B_1[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        //PRINT("B_2[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
        fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        //fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
    }

    fileUtil.saveAll();
}

void
Test::printGesture(const type_gesture gesture){
	PRINT(gesture.name);
	for (int i = 0; i < gesture.handOne.positions.size(); i++){
		PRINT(gesture.handOne.positions[i].X << " " << gesture.handOne.positions[i].Y <<" "<<gesture.handOne.positions[i].Z <<" "<<
		gesture.handTwo.positions[i].X << " " << gesture.handTwo.positions[i].Y <<" "<<gesture.handTwo.positions[i].Z);
	}
}
