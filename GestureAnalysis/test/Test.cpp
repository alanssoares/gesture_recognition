#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
	m_Param = 0.0005;
}

Test::~Test(){}

float
Test::getFinalTime(std::clock_t start_s){
	return (std::clock() - start_s)/double(CLOCKS_PER_SEC);
}

void
Test::clearSamples(){
	m_GesturesTemplate.clear();
	m_GesturesTest.clear();
}

void
Test::loadAll(){
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA_NORMALIZED);
	m_AllGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
}

void 
Test::init(){
	loadAll();
	std::sort(m_AllGestures.begin(), m_AllGestures.end(), sortByName);
	initAllSamples();
}

void
Test::initAllSamples(){
	splitDataset();
	initGestureTemplates();
}

void
Test::process1(){
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
}

void
Test::process2(){
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
}

void
Test::process3(){
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
	applyMedian();
}

void
Test::process4(){
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
	applyMedian();
}

void
Test::process5(){
	applyCurvature(&m_GesturesTest);
	applyCurvature(&m_GesturesTemplate);
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
}

void
Test::process6(){
	applyCurvature(&m_GesturesTest);
	applyCurvature(&m_GesturesTemplate);
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
}

void
Test::process7(){
	applyCurvature(&m_GesturesTest);
	applyCurvature(&m_GesturesTemplate);
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
	applyMedian();
}

void
Test::process8(){
	applyCurvature(&m_GesturesTest);
	applyCurvature(&m_GesturesTemplate);
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
	applyMedian();
}

void
Test::process9(){
	applyDouglasPeucker(&m_GesturesTest);
	applyDouglasPeucker(&m_GesturesTemplate);
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
}

void
Test::process10(){
	applyDouglasPeucker(&m_GesturesTest);
	applyDouglasPeucker(&m_GesturesTemplate);
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
}

void
Test::process11(){
	applyDouglasPeucker(&m_GesturesTest);
	applyDouglasPeucker(&m_GesturesTemplate);
	applyLaplacian(&m_GesturesTest);
	applyLaplacian(&m_GesturesTemplate);
	applyMedian();
}

void
Test::process12(){
	applyDouglasPeucker(&m_GesturesTest);
	applyDouglasPeucker(&m_GesturesTemplate);
	applyBSpline(&m_GesturesTest);
	applyBSpline(&m_GesturesTemplate);
	applyMedian();
}

void
Test::applyMedian(){
	generateMedianGesture(m_GesturesTemplate);
	m_GesturesTemplate = m_MedianGestures;
	initGestureTemplates();
}

void
Test::splitDataset(){
	size_t n = m_AllGestures.size();
	int j = 0, i = 0, numTests = 0;
	clearSamples();
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
Test::initGestureTemplates(){
	size_t n = m_GesturesTemplate.size();
	m_GesturesOneHand.clear();
	m_GesturesTwoHands.clear();
	for (int i = 0; i < n; i++){
		if(m_GesturesTemplate[i].numHands == 1){
			m_GesturesOneHand.push_back(m_GesturesTemplate[i]);
		} else {
			m_GesturesTwoHands.push_back(m_GesturesTemplate[i]);
		}
	}
}

void
Test::saveResults(std::string nameFile, type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized){
	std::fstream fileOut;
	fileOut.open(nameFile.c_str(), ios::in | ios::out | ios::ate);
	if(fileOut.is_open()){
		fileOut<<gestureExecuted.name<<" "<<gestureExecuted.numHands<<" "<<gestureExecuted.handOne.positions.size()<<" ";
		fileOut<<gesturePredicted.name<<" "<<gesturePredicted.numHands<<" "<<gesturePredicted.handOne.positions.size()<<" ";
		fileOut<<timeExecution<<" "<<bestDistance<<" "<<isRecognized<<std::endl;
	}
	fileOut.close();
}

void
Test::applyLaplacian(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handOne.positions, NUMBER_SMOOTH_NB);
		gestures->at(i).handTwo.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handTwo.positions, NUMBER_SMOOTH_NB);
	}
}

void
Test::applyBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = BSpline::curvePoints(gestures->at(i).handOne.positions, NUM_STEP_BSPLINE);
		gestures->at(i).handTwo.positions = BSpline::curvePoints(gestures->at(i).handTwo.positions, NUM_STEP_BSPLINE);
	}
}

void
Test::applyCurvature(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::reduceByCurvature(gestures->at(i).handOne.positions, m_Param);
		gestures->at(i).handTwo.positions = MathUtil::reduceByCurvature(gestures->at(i).handTwo.positions, m_Param);
	}
}

void
Test::applyDouglasPeucker(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::simplify(gestures->at(i).handOne.positions, m_Param, false);
		gestures->at(i).handTwo.positions = MathUtil::simplify(gestures->at(i).handTwo.positions, m_Param, false);
	}
}

void 
Test::experiment(int env, std::string nameFile){
	FileUtil::getInstance().createFile(nameFile);
	applyProcess(env);
	for (int i = 0; i < m_GesturesTest.size(); i++){
		if(m_GesturesTest[i].numHands == 1){
			recognizeOneHand(m_GesturesTest[i], nameFile);
		} else {
			recognizeTwoHands(m_GesturesTest[i], nameFile);
		}
	}
	initAllSamples();
}

void
Test::recognizeOneHand(const type_gesture gesture, const std::string nameFile) {
    clock_t start_s = clock(), isRecognized = 0;
    float distance = 0.0, bestDistance = 1.0;
    size_t n = m_GesturesOneHand.size();
    type_gesture gestureTemplate;

    for (int i = 0; i < n; i++) {
        distance = MathUtil::computeDistanceBetweenTwoTrajectories(m_GesturesOneHand[i].handTwo.positions, gesture.handTwo.positions);
        if(distance < bestDistance){
            bestDistance = distance;
            gestureTemplate = m_GesturesOneHand[i];
        }
    }
    
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        isRecognized = 1;
    }

	saveResults(nameFile, gesture, gestureTemplate, getFinalTime(start_s), bestDistance, isRecognized);
}

void
Test::recognizeTwoHands(const type_gesture gesture, const std::string nameFile) {
    float start_s = clock(), isRecognized = 0;
    float distanceA, distanceB, bestDistanceB = 1.0, bestDistanceA = 1.0;
    size_t n = m_GesturesTwoHands.size();
    type_gesture gestureTemplate;
    
    for (int i = 0; i < n; i++) {

        distanceA = MathUtil::computeDistanceBetweenTwoTrajectories(m_GesturesTwoHands[i].handOne.positions, gesture.handOne.positions);
        if (distanceA < bestDistanceA){
            bestDistanceA = distanceA;
        }
        
        distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(m_GesturesTwoHands[i].handTwo.positions, gesture.handTwo.positions);
        if (distanceB < bestDistanceB){
            bestDistanceB = distanceB;
        }

        if(bestDistanceA < MIN_DISTANCE_TRESHOLD && bestDistanceB < MIN_DISTANCE_TRESHOLD){
        	isRecognized = 1;
        	gestureTemplate = m_GesturesTwoHands[i];
        }
    }

    saveResults(nameFile, gesture, gestureTemplate, getFinalTime(start_s), bestDistanceA, isRecognized);
}

void
Test::generateMedianGesture(std::vector<type_gesture> gestures){
	size_t n = gestures.size(), i = 0, j = 0, m, count;
	
	generateGestureEqualSize(&gestures);
	
	while(j < n){
		
		j = i + 1;
		count = 0;
		m = gestures[i].handOne.positions.size();
		
		while((j < n) && (gestures[i].name.compare(gestures[j].name) == 0)) j++;
		
		for(int k = i + 1; k < j - 1; k++){
			for(int t = 0; t < m; t++){
				gestures[i].handOne.positions[t].X += gestures[k].handOne.positions[t].X;
				gestures[i].handOne.positions[t].Y += gestures[k].handOne.positions[t].Y;
				gestures[i].handOne.positions[t].Z += gestures[k].handOne.positions[t].Z;

				gestures[i].handTwo.positions[t].X += gestures[k].handTwo.positions[t].X;
				gestures[i].handTwo.positions[t].Y += gestures[k].handTwo.positions[t].Y;
				gestures[i].handTwo.positions[t].Z += gestures[k].handTwo.positions[t].Z;
			}
			count++;
		}
		
		for(int k = 0; k < m; k++){
			gestures[i].handOne.positions[k].X = gestures[i].handOne.positions[k].X / count;
			gestures[i].handOne.positions[k].Y = gestures[i].handOne.positions[k].Y / count;
			gestures[i].handOne.positions[k].Z = gestures[i].handOne.positions[k].Z / count;
			
			gestures[i].handTwo.positions[k].X = gestures[i].handTwo.positions[k].X / count;
			gestures[i].handTwo.positions[k].Y = gestures[i].handTwo.positions[k].Y / count;
			gestures[i].handTwo.positions[k].Z = gestures[i].handTwo.positions[k].Z / count;
		}
		
		m_MedianGestures.push_back(gestures[i]);
		i = j;
	}
}

void
Test::saveMedianGestures(){
	FileUtil& futil = FileUtil::getInstance();
	futil.clearHandGestures();
	for (int i = 0; i < m_MedianGestures.size(); i++){
		if(m_MedianGestures[i].numHands == 1){
			futil.mGesturesOneHand.push_back(m_MedianGestures[i]);
		} else {
			futil.mGesturesTwoHands.push_back(m_MedianGestures[i]);
		}
	}
	futil.saveAll();
}

void
Test::generateGestureEqualSize(std::vector<type_gesture> *gestures){
	int n = gestures->size(), i = 0, j = 0, mean = 0, diff = 0;
	while(j < n){
		j = i + 1;
		while((j < n) && (gestures->at(i).name.compare(gestures->at(j).name) == 0)) j++;
		mean = getMeanPoints(*gestures, i, j - 1);
		for(int k = i; k < j - 1; k++){
			diff = mean - gestures->at(k).handOne.positions.size();
			if(diff > 0){
				MathUtil::insertPoints(&gestures->at(k).handOne.positions, diff);
				MathUtil::insertPoints(&gestures->at(k).handTwo.positions, diff);
			} else if(diff < 0) {
				MathUtil::removePoints(&gestures->at(k).handOne.positions, diff);
				MathUtil::removePoints(&gestures->at(k).handTwo.positions, diff);
			}
		}
		i = j;
	}
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
    fileUtil.loadGestures(NAME_FILE_DATA);
    for (int i = 0; i < fileUtil.mGesturesOneHand.size(); i++){
    	normCenterOriginGesture(&fileUtil.mGesturesOneHand[i]);
    }
    for (int i = 0; i < fileUtil.mGesturesTwoHands.size(); i++){
    	normCenterOriginGesture(&fileUtil.mGesturesTwoHands[i]);
    }
    fileUtil.saveAll();
}

void
Test::printGesture(const type_gesture gesture){
	for (int i = 0; i < gesture.handOne.positions.size(); i++){
		PRINT(gesture.handOne.positions[i].X << " " << gesture.handOne.positions[i].Y <<" "<<gesture.handOne.positions[i].Z <<" "<<
		gesture.handTwo.positions[i].X << " " << gesture.handTwo.positions[i].Y <<" "<<gesture.handTwo.positions[i].Z);
	}
}

void
Test::normCenterOriginGesture(type_gesture *gesture){
	gesture->handOne.positions = MathUtil::normCenterOrigin(gesture->handOne.positions);
	gesture->handTwo.positions = MathUtil::normCenterOrigin(gesture->handTwo.positions);
}

void
Test::executeAll(){
    loadAll();
    for (int i = 1; i < 8; i++){
        experiment(i, "../result_experiment" + MathUtil::intToString(i) + ".txt");
    }
}

void
Test::applyProcess(int env){
	switch(env){
		case 1:
			process1();
			break;
		case 2:
			process2();
			break;
		case 3:
			process3();
			break;
		case 4:
			process4();
			break;
		case 5:
			process5();
			break;
		case 6:
			process6();
			break;
		case 7:
			process7();
			break;
		case 8:
			process8();
			break;
		case 9:
			process9();
			break;
		case 10:
			process10();
			break;
		case 11:
			process11();
			break;
		case 12:
			process12();
			break;
		default:
			break;
	}
}