#include "Test.h"

Test::Test(){
	m_PercentTest = 0.3;
	m_RecThreshold = 0.5;
	m_isMedian = false;
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
	std::sort(m_AllGestures.begin(), m_AllGestures.end(), Util::sortByName);
}

void
Test::loadMedian(){
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA_MEDIAN);
	m_MedianGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(m_MedianGestures.begin(), m_MedianGestures.end(), Util::sortByName);
}

void
Test::init(){
	loadAll();
	loadMedian();
}

void
Test::process1(type_gesture* gesture){
	m_Util.applyLaplacian(gesture);
}

void
Test::process2(type_gesture* gesture){
	m_Util.applyCurvature(gesture);
}

void
Test::process3(type_gesture* gesture){
	m_Util.applyDouglasPeucker(gesture);
}

void
Test::process4(type_gesture* gesture){
	process1(gesture);
	process2(gesture);
}

void
Test::process5(type_gesture* gesture){
	process1(gesture);
	process3(gesture);
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
	std::sort(m_GesturesTest.begin(), m_GesturesTest.end(), Util::sortByName);
	std::sort(m_GesturesTemplate.begin(), m_GesturesTemplate.end(), Util::sortByName);
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
Test::recognize(const int env, const type_gesture gesture, const std::string nameFile) {
    float isRecognized = 0;
    double distanceA, distanceB, bestDistanceB = 999999999, bestDistanceA = 999999999;
    size_t n = m_GesturesTemplate.size();
    type_gesture gestureTemplate, gcompare;

    for (int i = 0; i < n; i++) {
				gcompare = m_GesturesTemplate[i];
				applyProcess(env, &gcompare);
        distanceA = MathUtil::computeDistanceBetweenTwoTrajectories(gcompare.handOne.positions, gesture.handOne.positions);
        distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(gcompare.handTwo.positions, gesture.handTwo.positions);
        if (distanceA < bestDistanceA && distanceB < bestDistanceB){
            bestDistanceA = distanceA;
            bestDistanceB = distanceB;
            gestureTemplate = m_GesturesTemplate[i];
        }
    }

    if(bestDistanceA < m_RecThreshold && bestDistanceB < m_RecThreshold){
    	isRecognized = 1;
    }

    saveResults(nameFile, gesture, gestureTemplate, getFinalTime(m_Start_s), (bestDistanceA + bestDistanceB)/2, isRecognized);
}

void
Test::executeAll(){
	std::string baseFolder = "../results/result_experiment_";
  //Load samples
  init();
	//Normalize all gestures
	m_Util.applyNormalization(&m_AllGestures);
	m_Util.applyNormalization(&m_MedianGestures);
  //Execute experiments using normal gestures
	executeNormal(baseFolder + "normal_");
	//Execute experiments using gestures with equal number of points
	executeWithEqualSize(baseFolder + "equal_");
	//Execute experiments using median gestures as template
	executeWithMedian(baseFolder + "median_");
}

void
Test::executeNormal(std::string folder){
	//Execute all experiments
	execute(folder);
}

void
Test::executeWithEqualSize(std::string folder){
	//Resample all templates to equal size according with the type
	m_Util.generateGestureEqualSize(&m_AllGestures);
	//Execute all experiments
	execute(folder);
}

void
Test::executeWithMedian(std::string folder){
	//Preenche o vetor com os gestos médios
	m_GesturesTemplate.clear();
	m_GesturesTemplate.reserve(m_MedianGestures.size());
	m_GesturesTemplate.insert(m_GesturesTemplate.begin(), m_MedianGestures.begin(), m_MedianGestures.end());
	m_isMedian = true;
	//Execute all experiments
	execute(folder);
}

void
Test::execute(std::string folder){
	experiment(0, folder + MathUtil::intToString(0) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(1, folder + MathUtil::intToString(1) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(2, folder + MathUtil::intToString(2) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(3, folder + MathUtil::intToString(3) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(4, folder + MathUtil::intToString(4) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(5, folder + MathUtil::intToString(5) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
}

void
Test::experiment(int env, std::string nameFile){
	FileUtil::getInstance().createFile(nameFile);
	type_gesture gesture;
	//Evitar sobrescrever os gestos médios
	if(!m_isMedian){
		//Split Dataset in test and templates
		splitDataset();
	}
	//Executa o teste de reconhecimento de cada gesto
	for (int i = 0; i < m_GesturesTest.size(); i++){
		gesture = m_GesturesTest[i];
		//Inicializa a contagem do tempo para realizar o pré-proc e classificação
		m_Start_s = clock();
		//Aplica o processamento de acordo com o cenário
		applyProcess(env, &gesture);
		//Classifica o gesto
		recognize(env, gesture, nameFile);
	}
}

void
Test::applyProcess(int env, type_gesture* gesture){
	switch(env){
		case 1:
			process1(gesture);
			break;
		case 2:
			process2(gesture);
			break;
		case 3:
			process3(gesture);
			break;
		case 4:
			process4(gesture);
			break;
		case 5:
			process5(gesture);
			break;
		default:
			break;
	}
}
