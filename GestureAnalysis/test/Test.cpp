#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
	m_RecThreshold = 0.5;
	m_CurvThreshold = 0.0001;
	m_DougThreshold = 0.01;
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
	futil.loadGestures(NAME_FILE_DATA);
	m_AllGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(m_AllGestures.begin(), m_AllGestures.end(), sortByName);
}

void
Test::loadMedian(){
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA_MEDIAN);
	m_MedianGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(m_MedianGestures.begin(), m_MedianGestures.end(), sortByName);
}

void
Test::init(){
	loadAll();
	loadMedian();
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
	applyCurvature(&m_GesturesTest);
	applyCurvature(&m_GesturesTemplate);
}

void
Test::process4(){
	process1();
	process3();
}

void
Test::process5(){
	process2();
	process3();
}

void
Test::process6(){
	applyDouglasPeucker(&m_GesturesTest);
	applyDouglasPeucker(&m_GesturesTemplate);
}

void
Test::process7(){
	process1();
	process6();
}

void
Test::process8(){
	process2();
	process6();
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
	std::sort(m_GesturesTest.begin(), m_GesturesTest.end(), sortByName);
	std::sort(m_GesturesTemplate.begin(), m_GesturesTemplate.end(), sortByName);
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
		gestures->at(i).handOne.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handOne.positions);
		gestures->at(i).handTwo.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handTwo.positions);
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
Test::applyUniformBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = BSpline::uniformFitting(gestures->at(i).handOne.positions);
		gestures->at(i).handTwo.positions = BSpline::uniformFitting(gestures->at(i).handTwo.positions);
	}
}

void
Test::applyCurvature(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::reduceByCurvature(gestures->at(i).handOne.positions, m_CurvThreshold);
		gestures->at(i).handTwo.positions = MathUtil::reduceByCurvature(gestures->at(i).handTwo.positions, m_CurvThreshold);
	}
}

void
Test::applyDouglasPeucker(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::simplify(gestures->at(i).handOne.positions, m_DougThreshold, false);
		gestures->at(i).handTwo.positions = MathUtil::simplify(gestures->at(i).handTwo.positions, m_DougThreshold, false);
	}
}

void
Test::experiment(int env, std::string nameFile){
	FileUtil::getInstance().createFile(nameFile);
	applyProcess(env);
	for (int i = 0; i < m_GesturesTest.size(); i++){
		recognize(m_GesturesTest[i], nameFile);
	}
	splitDataset();
}

void
Test::recognize(type_gesture gesture, const std::string nameFile) {
    float start_s = clock(), isRecognized = 0;
    double distanceA, distanceB, bestDistanceB = 999999999, bestDistanceA = 999999999;
    size_t n = m_GesturesTemplate.size();
    type_gesture gestureTemplate;

    for (int i = 0; i < n; i++) {
        distanceA = MathUtil::computeDistanceBetweenTwoTrajectories(m_GesturesTemplate[i].handOne.positions, gesture.handOne.positions);
        distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(m_GesturesTemplate[i].handTwo.positions, gesture.handTwo.positions);
        if (distanceA < bestDistanceA && distanceB < bestDistanceB){
            bestDistanceA = distanceA;
            bestDistanceB = distanceB;
            gestureTemplate = m_GesturesTemplate[i];
        }
    }

    if(bestDistanceA < m_RecThreshold && bestDistanceB < m_RecThreshold){
    	isRecognized = 1;
    }

    saveResults(nameFile, gesture, gestureTemplate, getFinalTime(start_s), (bestDistanceA + bestDistanceB)/2, isRecognized);
}

void
Test::generateMedianGesture(std::vector<type_gesture> gestures){
	size_t n = gestures.size(), i = 0, j = 0, m, count;
	FileUtil& futil = FileUtil::getInstance();
	futil.clearHandGestures();

	/* Generate all gestures with equal length */
	generateGestureEqualSize(&gestures);
	/* Apply a Uniform B-Spline to generate uniform probability distribution */
	//applyUniformBSpline(&gestures);

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
Test::generateMedians(){
	//Load all gestures and store in the vector m_AllGestures
	loadAll();
	//Generate a new file with median gestures
  generateMedianGesture(m_AllGestures);
}

void
Test::generateGestureEqualSize(std::vector<type_gesture> *gestures){
	int n = gestures->size(), i = 0, j = 0, mean = 0, diff = 0;
	while(j < n){
		j = i + 1;
		while((j < n) && (gestures->at(i).name.compare(gestures->at(j).name) == 0)) j++;
		mean = getMeanPoints(*gestures, i, j);
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
Test::scaleGestures(){
	FileUtil& fileUtil = FileUtil::getInstance();
	XnPoint3D max, min;
	fileUtil.loadGestures(NAME_FILE_DATA);
	for (int i = 0; i < fileUtil.mGesturesOneHand.size(); i++){
		max = MathUtil::findMaxFromTwo(fileUtil.mGesturesOneHand[i].handOne.positions, fileUtil.mGesturesOneHand[i].handTwo.positions);
		min = MathUtil::findMinFromTwo(fileUtil.mGesturesOneHand[i].handOne.positions, fileUtil.mGesturesOneHand[i].handTwo.positions);
		fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::normalizeTrajectory(fileUtil.mGesturesOneHand[i].handOne.positions, min, max);
		fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::normalizeTrajectory(fileUtil.mGesturesOneHand[i].handTwo.positions, min, max);
	}
	for (int i = 0; i < fileUtil.mGesturesTwoHands.size(); i++){
		max = MathUtil::findMaxFromTwo(fileUtil.mGesturesTwoHands[i].handOne.positions, fileUtil.mGesturesTwoHands[i].handTwo.positions);
		min = MathUtil::findMinFromTwo(fileUtil.mGesturesTwoHands[i].handOne.positions, fileUtil.mGesturesTwoHands[i].handTwo.positions);
		fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::normalizeTrajectory(fileUtil.mGesturesTwoHands[i].handOne.positions, min, max);
		fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::normalizeTrajectory(fileUtil.mGesturesTwoHands[i].handTwo.positions, min, max);
	}
	fileUtil.saveAll();
}

void
Test::improveGestures(){
    FileUtil& fileUtil = FileUtil::getInstance();
    fileUtil.loadGestures(NAME_FILE_DATA);
    for (int i = 0; i < fileUtil.mGesturesOneHand.size(); i++){
    	fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::translateToOrigin(fileUtil.mGesturesOneHand[i].handOne.positions);
			fileUtil.mGesturesOneHand[i].handTwo.positions = MathUtil::translateToOrigin(fileUtil.mGesturesOneHand[i].handTwo.positions);
    }
    for (int i = 0; i < fileUtil.mGesturesTwoHands.size(); i++){
    	fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::translateToOrigin(fileUtil.mGesturesTwoHands[i].handOne.positions);
			fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::translateToOrigin(fileUtil.mGesturesTwoHands[i].handTwo.positions);
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
	std::string folder = "../results/result_experiment_";
  //Load samples
  init();
	//Split Dataset in test and templates
	splitDataset();
  //Execute experiments using different parameters
	experiment(0, folder + MathUtil::intToString(0) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(1, folder + MathUtil::intToString(1) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(2, folder + MathUtil::intToString(2) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(3, folder + MathUtil::intToString(3) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(4, folder + MathUtil::intToString(4) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(5, folder + MathUtil::intToString(5) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(6, folder + MathUtil::intToString(6) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(7, folder + MathUtil::intToString(7) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(8, folder + MathUtil::intToString(8) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
}

void
Test::transformAllToEqualSize() {
	//Resample all templates to equal size according with the type
	generateGestureEqualSize(&m_AllGestures);
	//Splie again, now with equal size
	splitDataset();
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
		default:
			break;
	}
}
