#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
}

Test::~Test(){}

float
Test::getFinalTime(std::clock_t start_s){
	return (std::clock() - start_s)/double(CLOCKS_PER_SEC);
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
Test::init(){
	//Load all samples
	FileUtil::getInstance().loadGestures(NAME_FILE_DATA_NORMALIZED);
	//Merge all gestures
	loadAll();
	//Split the samples in templates and test, after Initialize the gestures template
	initAllSamples();
}

void
Test::splitDataset(){
	size_t n = m_AllGestures.size();
	int j = 0, i = 0, numTests = 0;

	m_GesturesTemplate.clear();
	m_GesturesTest.clear();

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

	PRINT("-------------------------------------------")
	PRINT("Nº Test - " << m_GesturesTest.size());
	PRINT("Nº Samples - " << m_GesturesTemplate.size());
	PRINT("-------------------------------------------")
}

void
Test::initAllSamples(){
	splitDataset();
	initGestureTemplates();
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

	PRINT("-------------------------------------------")
	PRINT("Nº One Hand - " << m_GesturesOneHand.size());
	PRINT("Nº Two Hand - " << m_GesturesTwoHands.size());
	PRINT("-------------------------------------------")
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
Test::applyLaplacian(){
	size_t n1 = m_GesturesTest.size();
	for (int i = 0; i < n1; i++) {
		m_GesturesTest[i].handOne.positions = MathUtil::smoothMeanNeighboring(m_GesturesTest[i].handOne.positions, NUMBER_SMOOTH_NB);
		m_GesturesTest[i].handTwo.positions = MathUtil::smoothMeanNeighboring(m_GesturesTest[i].handTwo.positions, NUMBER_SMOOTH_NB);
	}

	size_t n2 = m_GesturesTemplate.size();
	for (int i = 0; i < n2; i++) {
		m_GesturesTemplate[i].handOne.positions =  MathUtil::smoothMeanNeighboring(m_GesturesTemplate[i].handOne.positions, NUMBER_SMOOTH_NB);
		m_GesturesTemplate[i].handTwo.positions =  MathUtil::smoothMeanNeighboring(m_GesturesTemplate[i].handTwo.positions, NUMBER_SMOOTH_NB);
	}
}

void
Test::applyBSpline(){
	size_t n1 = m_GesturesTest.size();
	for (int i = 0; i < n1; i++) {
		m_GesturesTest[i].handOne.positions = MathUtil::applyCubicBSpline(m_GesturesTest[i].handOne.positions);
		m_GesturesTest[i].handTwo.positions = MathUtil::applyCubicBSpline(m_GesturesTest[i].handTwo.positions);
	}

	size_t n2 = m_GesturesTemplate.size();
	for (int i = 0; i < n2; i++) {
		m_GesturesTemplate[i].handOne.positions =  MathUtil::applyCubicBSpline(m_GesturesTemplate[i].handOne.positions);
		m_GesturesTemplate[i].handTwo.positions =  MathUtil::applyCubicBSpline(m_GesturesTemplate[i].handTwo.positions);
	}
}

void
Test::applyCurvature(){
	size_t n1 = m_GesturesTest.size();
	for (int i = 0; i < n1; i++) {
		m_GesturesTest[i].handOne.positions = MathUtil::reduceByCurvature(m_GesturesTest[i].handOne.positions);
		m_GesturesTest[i].handTwo.positions = MathUtil::reduceByCurvature(m_GesturesTest[i].handTwo.positions);
	}

	size_t n2 = m_GesturesTemplate.size();
	for (int i = 0; i < n2; i++) {
		m_GesturesTemplate[i].handOne.positions =  MathUtil::reduceByCurvature(m_GesturesTemplate[i].handOne.positions);
		m_GesturesTemplate[i].handTwo.positions =  MathUtil::reduceByCurvature(m_GesturesTemplate[i].handTwo.positions);
	}
}

void
Test::applyDouglasPeucker(){
	size_t n1 = m_GesturesTest.size();
	for (int i = 0; i < n1; i++) {
		m_GesturesTest[i].handOne.positions = MathUtil::simplify(m_GesturesTest[i].handOne.positions, 0.01, false);
		m_GesturesTest[i].handTwo.positions = MathUtil::simplify(m_GesturesTest[i].handTwo.positions, 0.01, false);
	}

	size_t n2 = m_GesturesTemplate.size();
	for (int i = 0; i < n2; i++) {
		m_GesturesTemplate[i].handOne.positions =  MathUtil::simplify(m_GesturesTemplate[i].handOne.positions, 0.01, false);
		m_GesturesTemplate[i].handTwo.positions =  MathUtil::simplify(m_GesturesTemplate[i].handTwo.positions, 0.01, false);
	}
}
void
Test::applyMedian(){
	generateMedianGesture(m_GesturesTemplate);
	m_GesturesTemplate = m_MedianGestures;
	initGestureTemplates();
}

void
Test::process1(){
	applyLaplacian();
}

void
Test::process2(){
	applyBSpline();
}

void
Test::process3(){
	applyLaplacian();
	applyMedian();
}

void
Test::process4(){
	applyBSpline();
	applyMedian();
}

void
Test::process5(){
	applyLaplacian();
	applyCurvature();
}

void
Test::process6(){
	applyBSpline();
	applyCurvature();
}

void
Test::process7(){
	applyLaplacian();
	applyMedian();
	applyCurvature();
}

void
Test::process8(){
	applyBSpline();
	applyMedian();
	applyCurvature();
}

void
Test::process9(){
	applyLaplacian();
	applyDouglasPeucker();
}

void
Test::process10(){
	applyBSpline();
	applyDouglasPeucker();
}

void
Test::process11(){
	applyLaplacian();
	applyMedian();
	applyDouglasPeucker();
}

void
Test::process12(){
	applyBSpline();
	applyMedian();
	applyDouglasPeucker();
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

void 
Test::experiment(int env, std::string nameFile){
	size_t n = m_GesturesTest.size();
	
	FileUtil::getInstance().createFile(nameFile);

	applyProcess(env);

	for (int i = 0; i < n; i++){
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
	size_t n = gestures.size(), i = 0, j = 0;
	gestures = generateGestureEqualSize(gestures);
	while(j < n){
		j = i + 1;
		while((j < n) && (gestures[i].name.compare(gestures[j].name) == 0)) j++;
		type_gesture median = getMeanGesture(i, j - 1, gestures);
		median = normCenterOriginGesture(median);
		m_MedianGestures.push_back(median);
		i = j;
	}
}

void
Test::saveMedianGestures(){
	FileUtil& futil = FileUtil::getInstance();
	size_t n = m_MedianGestures.size();

	futil.mGesturesOneHand.clear();
	futil.mGesturesTwoHands.clear();

	for (int i = 0; i < n; i++){
		if(m_MedianGestures[i].numHands == 1){
			futil.mGesturesOneHand.push_back(m_MedianGestures[i]);
		} else {
			futil.mGesturesTwoHands.push_back(m_MedianGestures[i]);
		}
	}

	futil.saveAll();
}

std::vector<type_gesture>
Test::generateGestureEqualSize(std::vector<type_gesture> gestures){
	int n = gestures.size(), i = 0, j = 0, mean = 0, diff = 0;
	while(j < n){
		j = i + 1;
		while((j < n) && (gestures[i].name.compare(gestures[j].name) == 0)) j++;
		mean = getMeanPoints(gestures, i, j - 1);
		for(int k = i; k < j - 1; k++){
			diff = mean - gestures[k].handOne.positions.size();
			if(diff > 0){
				MathUtil::insertPoints(&gestures[k].handOne.positions, diff);
				MathUtil::insertPoints(&gestures[k].handTwo.positions, diff);
			} else if(diff < 0) {
				MathUtil::removePoints(&gestures[k].handOne.positions, diff);
				MathUtil::removePoints(&gestures[k].handTwo.positions, diff);
			}
		}
		i = j;
	}
	return gestures;
}

type_gesture
Test::getMeanGesture(const int k, const int p, std::vector<type_gesture> gestures){
	const size_t n = gestures[k].handOne.positions.size();
	type_gesture medianGesture = gestures[k];

	for(int i = k + 1; i < p; i++){
		for(int j = 0; j < n; j++){
			medianGesture.handOne.positions[j] = MathUtil::sum(medianGesture.handOne.positions[j], gestures[i].handOne.positions[j]);
			medianGesture.handTwo.positions[j] = MathUtil::sum(medianGesture.handTwo.positions[j], gestures[i].handTwo.positions[j]);
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
    
    fileUtil.loadGestures(NAME_FILE_DATA);

    size_t n1 = fileUtil.mGesturesOneHand.size();
    size_t n2 = fileUtil.mGesturesTwoHands.size();
    
    for (int i = 0; i < n1; i++){
    	fileUtil.mGesturesOneHand[i] = normCenterOriginGesture(fileUtil.mGesturesOneHand[i]);
    }
    
    for (int i = 0; i < n2; i++){
    	fileUtil.mGesturesTwoHands[i] = normCenterOriginGesture(fileUtil.mGesturesTwoHands[i]);
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

type_gesture
Test::normCenterOriginGesture(type_gesture gesture){
	gesture.handOne.positions = MathUtil::normCenterOrigin(gesture.handOne.positions);
	gesture.handTwo.positions = MathUtil::normCenterOrigin(gesture.handTwo.positions);
	return gesture;
}
