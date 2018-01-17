#include "Test.h"
#include "../../Commons/cluster/KMeans.h"

Test::Test(){
	m_PercentTest = 0.7;
	m_RecThreshold = 0.5;
	m_isMedianTest = false;
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
	m_AllGestures.clear();
	m_AllGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_AllGestures.insert( m_AllGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(m_AllGestures.begin(), m_AllGestures.end(), MathUtil::sortByName);
}

void
Test::loadMedian(){
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA_MEDIAN);
	m_MedianGestures.clear();
	m_MedianGestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	m_MedianGestures.insert( m_MedianGestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(m_MedianGestures.begin(), m_MedianGestures.end(), MathUtil::sortByName);
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
	std::sort(m_GesturesTest.begin(), m_GesturesTest.end(), MathUtil::sortByName);
	std::sort(m_GesturesTemplate.begin(), m_GesturesTemplate.end(), MathUtil::sortByName);
	//Verifica e preenche o vetor com os gestos médios
	if(m_isMedianTest){
		addMedianTemplates();
	}
}

void
Test::saveResults(std::string nameFile, type_gesture gestureExecuted, type_gesture gesturePredicted, float timeExecution, float bestDistance, int isRecognized){
	std::fstream fileOut;
	fileOut.open(nameFile.c_str(), ios::in | ios::out | ios::ate);
	if(fileOut.is_open()){
		fileOut<<gestureExecuted.name<<" "<<gestureExecuted.numHands<<" "<<gestureExecuted.handTwo.positions.size()<<" ";
		fileOut<<gesturePredicted.name<<" "<<gesturePredicted.numHands<<" "<<gesturePredicted.handTwo.positions.size()<<" ";
		fileOut<<timeExecution<<" "<<bestDistance<<" "<<isRecognized<<std::endl;
	}
	fileOut.close();
}

void
Test::recognize(const int env, const type_gesture gesture, const std::string nameFile) {
    float isRecognized = 0;
    double distanceA, distanceB, bestDistanceB = 999999999, bestDistanceA = 999999999, bestDistance = 0;
    size_t n = m_GesturesTemplate.size();
    type_gesture gestureTemplate, gcompare;

    for (int i = 0; i < n; i++) {
				gcompare = m_GesturesTemplate[i];
				applyProcess(env, &gcompare);
				if(gesture.numHands == 1){
					distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(gcompare.handTwo.positions, gesture.handTwo.positions);
					if (distanceB < bestDistanceB){
							bestDistanceB = distanceB;
							gestureTemplate = m_GesturesTemplate[i];
					}
				} else if(gesture.numHands == 2){
					distanceA = MathUtil::computeDistanceBetweenTwoTrajectories(gcompare.handOne.positions, gesture.handOne.positions);
					distanceB = MathUtil::computeDistanceBetweenTwoTrajectories(gcompare.handTwo.positions, gesture.handTwo.positions);
					if (distanceA < bestDistanceA && distanceB < bestDistanceB){
							bestDistanceA = distanceA;
							bestDistanceB = distanceB;
							gestureTemplate = m_GesturesTemplate[i];
					}
				}
    }

		if(gesture.numHands == 1){
			bestDistance = bestDistanceB;
			// if(bestDistanceB < m_RecThreshold){
			// 	isRecognized = 1;
			// }
		} else if(gesture.numHands == 2){
			bestDistance = (bestDistanceA + bestDistanceB) / 2;
			// if(bestDistanceA < m_RecThreshold && bestDistanceB < m_RecThreshold){
			// 		isRecognized = 1;
			// }
    }

		if (gestureTemplate.name == gesture.name) {
			isRecognized = 1;
		}

    saveResults(nameFile, gesture, gestureTemplate, getFinalTime(m_Start_s), bestDistance, isRecognized);
}

void
Test::executeAll(){
	std::string baseFolder = "../results/result_experiment_";

	//Load samples
  init();

	//Normalize all gestures
	//m_Util.applyNormalization(&m_AllGestures);
	//m_Util.applyNormalization(&m_MedianGestures);

  //Execute experiments using normal gestures
	executeNormal(baseFolder + "normal_");

	//Execute experiments using gestures with equal number of points
	// executeWithEqualSize(baseFolder + "equal_");

	//Execute experiments using median gestures as template
	// executeWithMedian(baseFolder + "median_");
}

void
Test::executeNormal(std::string folder){
	execute(folder);
}

void
Test::executeWithEqualSize(std::string folder){
	//Resample all templates to equal size according with the type
	m_Util.applyUniformByArcLength(&m_AllGestures);
	m_Util.generateGestureEqualSize(&m_AllGestures);
	//Execute all experiments
	execute(folder);
}

void
Test::execute(std::string folder){
	experiment(0, folder + MathUtil::intToString(0) + "_" + MathUtil::floatToString(m_RecThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(1, folder + MathUtil::intToString(1) + "_" + MathUtil::floatToString(m_RecThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(3, folder + MathUtil::intToString(3) + "_" + MathUtil::floatToString(m_Util.m_DougThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(2, folder + MathUtil::intToString(2) + "_" + MathUtil::floatToString(m_Util.m_CurvThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(4, folder + MathUtil::intToString(4) + "_" + MathUtil::floatToString(m_Util.m_CurvThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
	experiment(5, folder + MathUtil::intToString(5) + "_" + MathUtil::floatToString(m_Util.m_DougThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
}

void
Test::executeWithMedian(std::string folder){
	//Enable the load of median gestures in template vector
	m_isMedianTest = true;
	//Execute all experiments
	execute(folder);
}

void
Test::addMedianTemplates(){
	m_GesturesTemplate.clear();
	m_GesturesTemplate.reserve(m_MedianGestures.size());
	m_GesturesTemplate.insert(m_GesturesTemplate.begin(), m_MedianGestures.begin(), m_MedianGestures.end());
}

void
Test::executeCrossValidation(std::string folder){
	//Cross validation using 5 parameters
	for (float i = 0.1; i <= 0.5; i+= 0.1) {
		m_RecThreshold = i;
		experiment(0, folder + MathUtil::intToString(0) + "_" + MathUtil::floatToString(i) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
		experiment(1, folder + MathUtil::intToString(1) + "_" + MathUtil::floatToString(i) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
		//Param of the curvature using 10 folds
		for (float j = 0.0001; j <= 0.001; j+= 0.0001) {
			m_Util.m_CurvThreshold = j;
			experiment(2, folder + MathUtil::intToString(2) + "_" + MathUtil::floatToString(m_Util.m_CurvThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
			experiment(4, folder + MathUtil::intToString(4) + "_" + MathUtil::floatToString(m_Util.m_CurvThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
		}
		//Param of the douglas peucker using 10 folds
		for (float j = 0.01; j <= 0.1; j+= 0.01) {
			m_Util.m_DougThreshold = j;
			experiment(3, folder + MathUtil::intToString(3) + "_" + MathUtil::floatToString(m_Util.m_DougThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
			experiment(5, folder + MathUtil::intToString(5) + "_" + MathUtil::floatToString(m_Util.m_DougThreshold) + "_" + MathUtil::floatToString(m_RecThreshold) + ".txt");
		}
	}
}

void
Test::experiment(int env, std::string nameFile){
	FileUtil::getInstance().createFile(nameFile);
	type_gesture gesture;
	//Split Dataset in test and templates
	splitDataset();
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

void
Test::train(int algorithm) {
	// Load samples
  init();
	// Split train and template
	splitDataset();
	// Create K-means
	CKmeans kmeans;
	kmeans.createClusters(m_GesturesTemplate);
	// Test all
	for (size_t i = 0; i < m_GesturesTest.size(); i++) {
		kmeans.createClusters(m_GesturesTemplate);
		kmeans.classify(m_GesturesTest[i]);
	}
}

void
Test::saveFeatureFormat() {
	CKmeans kmeans;
	// std:string nameDataset = "msr_action_3d_series.grt";
	// std:string nameDataset = "msrc_12_series.grt";
	// std:string nameDataset = "utkinect_series.grt";
	// std:string nameDataset = "grufba_series.grt";
	std:string nameDataset = "asl_series.grt";

	FileUtil& fileUtil = FileUtil::getInstance();
	// Load samples
  init();
	// Create K-means
	kmeans.createClusters(m_AllGestures);
	// Save files
	for (size_t i = 0; i < kmeans.mClusters.size(); i++) {
		// std::cout << "G " << kmeans.mClusters[i].mCollection[0].name << " N " << kmeans.mClusters[i].mCollection.size() << " S " << kmeans.mClusters[i].mCollection[0].handTwo.positions.size() << std::endl;
		// std::cout << "fileOut << \"" << kmeans.mClusters[i].mCollection[0].name << " " << kmeans.mClusters[i].mCollection.size() << " NOT_SET\" << std::endl; " << std::endl;
		// fileUtil.saveFeatureGestures(kmeans.mClusters[i].mCollection, "feature_" + kmeans.mClusters[i].mCollection[0].name + ".txt");
		size_t mean = 0;
		for (size_t j = 0; j < kmeans.mClusters[i].mCollection.size(); j++) {
			mean += kmeans.mClusters[i].mCollection[j].handTwo.positions.size();
		}
		mean /= kmeans.mClusters[i].mCollection.size();
		std::cout << kmeans.mClusters[i].mCollection[0].name << " " << mean << '\n';
	}
	// Save file with features grt nickgillian
	// fileUtil.saveFeauresToToolkit(m_AllGestures, nameDataset, true);
}

void
Test::createDatasetEqualizedFromMin(std::string name) {
	loadAll();
	m_Util.equalizeDatasetFromMin(&m_AllGestures, name);
}

void
Test::createDatasetEqualizedFromMax(std::string name) {
	loadAll();
	m_Util.equalizeDatasetFromMax(&m_AllGestures, name);
}

void
Test::createDatasetEqualizedFromMean(std::string name) {
	loadAll();
	m_Util.equalizeDatasetFromMean(&m_AllGestures, name);
}

void
Test::saveToFile(std::vector<type_gesture> gestures, std::string name) {
	FileUtil& fileUtil = FileUtil::getInstance();
	fileUtil.clearHandGestures();

	size_t n = gestures.size();
	for (int i = 0; i < n; i++){
		if(gestures[i].numHands == 1){
			fileUtil.mGesturesOneHand.push_back(gestures[i]);
		} else {
			fileUtil.mGesturesTwoHands.push_back(gestures[i]);
		}
	}

	fileUtil.saveAll(name);
}

void
Test::createFeatureDataset(std::string name, int method) {
	FileUtil& fileUtil = FileUtil::getInstance();

	loadAll();
	for(size_t i = 0; i < m_AllGestures.size(); i++){
		applyProcess(method, &m_AllGestures[i]);
	}
	saveToFile(m_AllGestures, name);
	fileUtil.saveDescriptorFeauresToolkit(m_AllGestures, name + ".grt");
}

void
Test::createGroupsToTest(int method) {
	FileUtil& fileUtil = FileUtil::getInstance();
	std::vector<type_gesture> as1, as2, as3;

	loadAll();

	for(size_t i = 0; i < m_AllGestures.size(); i++){
		applyProcess(method, &m_AllGestures[i]);
	}

	for (size_t i = 0; i < m_AllGestures.size(); i++){
		if (m_AllGestures[i].name == "a02" || m_AllGestures[i].name == "a03" ||
			m_AllGestures[i].name == "a05" || m_AllGestures[i].name == "a06" ||
			m_AllGestures[i].name == "a10" || m_AllGestures[i].name == "a13" ||
			m_AllGestures[i].name == "a20") {
				as1.push_back(m_AllGestures[i]);
		}
		if (m_AllGestures[i].name == "a01" || m_AllGestures[i].name == "a04" ||
			m_AllGestures[i].name == "a07" || m_AllGestures[i].name == "a08" ||
			m_AllGestures[i].name == "a09" || m_AllGestures[i].name == "a11" ||
			m_AllGestures[i].name == "a12") {
				as2.push_back(m_AllGestures[i]);
		}
		if (m_AllGestures[i].name == "a06" || m_AllGestures[i].name == "a14" ||
			m_AllGestures[i].name == "a15" || m_AllGestures[i].name == "a16" ||
			m_AllGestures[i].name == "a17" || m_AllGestures[i].name == "a18" ||
			m_AllGestures[i].name == "a20"){
				as3.push_back(m_AllGestures[i]);
		}
	}

	saveToFile(as1, "msr3d_action_origin_as1");
	saveToFile(as2, "msr3d_action_origin_as2");
	saveToFile(as3, "msr3d_action_origin_as3");

}

void
Test::reduceGesturesByCurvature(std::string name) {
	FileUtil& fileUtil = FileUtil::getInstance();
	size_t n1, n2;

	loadAll();

	for (size_t i = 0; i < m_AllGestures.size(); i++) {
		m_AllGestures[i].handOne.positions = MathUtil::reduceByCurvature(m_AllGestures[i].handOne.positions, 0.025);
		m_AllGestures[i].handTwo.positions = MathUtil::reduceByCurvature(m_AllGestures[i].handTwo.positions, 0.025);
		n1 = m_AllGestures[i].handOne.positions.size();
		n2 = m_AllGestures[i].handTwo.positions.size();
		if (n1 < n2) {
			MathUtil::insertPoints(&m_AllGestures[i].handOne.positions, n2 - n1);
		} else if (n1 > n2){
			MathUtil::insertPoints(&m_AllGestures[i].handTwo.positions, n1 - n2);
		}
	}

	saveToFile(m_AllGestures, name);
}
