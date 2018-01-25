#include "Util.h"

Util::Util(){
	m_CurvThreshold = 0.001;
	m_DougThreshold = 0.01;
	m_isMedian = false;
}

Util::~Util(){}

void
Util::applyUniformByArcLength(type_gesture *gesture, double factor){
	MathUtil::uniformCurveByArcLength(&gesture->handOne.positions, factor);
	MathUtil::uniformCurveByArcLength(&gesture->handTwo.positions, factor);
}

void
Util::applyUniformByArcLength(std::vector<type_gesture> *gestures, double factor){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++){
		applyUniformByArcLength(&gestures->at(i), factor);
	}
}

void
Util::applyNormalization(type_gesture* gesture){
	XnPoint3D min, max;
	max = MathUtil::findMaxFromTwo(gesture->handOne.positions, gesture->handTwo.positions);
	min = MathUtil::findMinFromTwo(gesture->handOne.positions, gesture->handTwo.positions);
	gesture->handOne.positions = MathUtil::normalizeTrajectory(gesture->handOne.positions, min, max);
	gesture->handTwo.positions = MathUtil::normalizeTrajectory(gesture->handTwo.positions, min, max);
}

void
Util::applyNormalization(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++){
		applyNormalization(&gestures->at(i));
	}
}

void
Util::applyLaplacian(type_gesture* gesture){
	gesture->handOne.positions = MathUtil::smoothMeanNeighboring(gesture->handOne.positions);
	gesture->handTwo.positions = MathUtil::smoothMeanNeighboring(gesture->handTwo.positions);
}

void
Util::applyLaplacian(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		applyLaplacian(&gestures->at(i));
	}
}

void
Util::applyBSpline(type_gesture* gesture){
	gesture->handOne.positions = BSpline::curvePoints(gesture->handOne.positions, NUM_STEP_BSPLINE);
	gesture->handTwo.positions = BSpline::curvePoints(gesture->handTwo.positions, NUM_STEP_BSPLINE);
}

void
Util::applyBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		applyBSpline(&gestures->at(i));
	}
}

void
Util::applyUniformBSpline(type_gesture* gesture){
	gesture->handOne.positions = BSpline::uniformFitting(gesture->handOne.positions);
	gesture->handTwo.positions = BSpline::uniformFitting(gesture->handTwo.positions);
}

void
Util::applyUniformBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		applyUniformBSpline(&gestures->at(i));
	}
}

void
Util::applyCurvature(type_gesture* gesture){
	gesture->handOne.positions = MathUtil::reduceByCurvature(gesture->handOne.positions, m_CurvThreshold);
	gesture->handTwo.positions = MathUtil::reduceByCurvature(gesture->handTwo.positions, m_CurvThreshold);
}

void
Util::applyCurvature(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		applyCurvature(&gestures->at(i));
	}
}

void
Util::applyDouglasPeucker(type_gesture* gesture){
	gesture->handOne.positions = MathUtil::simplify(gesture->handOne.positions, m_DougThreshold, false);
	gesture->handTwo.positions = MathUtil::simplify(gesture->handTwo.positions, m_DougThreshold, false);
}

type_gesture
Util::cleanZeroPositions(type_gesture gesture) {
	type_gesture newGesture = gesture;
	size_t n = gesture.handOne.positions.size();
	newGesture.handOne.positions.clear();
	newGesture.handTwo.positions.clear();
	for (int i = 0; i < n; i++) {
		if (gesture.handOne.positions[i].X == 0 && gesture.handOne.positions[i].Y == 0 && gesture.handOne.positions[i].Z == 0 &&
				gesture.handTwo.positions[i].X == 0 && gesture.handTwo.positions[i].Y == 0 && gesture.handTwo.positions[i].Z == 0) {
			continue;
		} else {
			newGesture.handOne.positions.push_back(gesture.handOne.positions[i]);
			newGesture.handTwo.positions.push_back(gesture.handTwo.positions[i]);
		}
	}
	return newGesture;
}

void
Util::applyDouglasPeucker(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		applyDouglasPeucker(&gestures->at(i));
	}
}

void
Util::generateGestureEqualSize(std::vector<type_gesture> *gestures){
	int n = gestures->size(), i = 0, j = 0, mean = 0, diff = 0;
	while(j < n){
		j = i + 1;
		while((j < n) && (gestures->at(i).name.compare(gestures->at(j).name) == 0)) j++;
		mean = getMeanPoints(*gestures, i, j - 1);
		for(int k = i; k < j; k++){
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

void
Util::equalizeDatasetFromMax(std::vector<type_gesture> *gestures, std::string name){
	FileUtil& futil = FileUtil::getInstance();
	int n = gestures->size(), max, diff = 0;
	max = getMaxPoints(*gestures, 0, n - 1);

	futil.clearHandGestures();

	for(int i = 0; i < n; i++){
		diff = max - gestures->at(i).handOne.positions.size();
		if(diff > 0){
			MathUtil::insertPoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::insertPoints(&gestures->at(i).handTwo.positions, diff);
		} else if (diff < 0) {
			MathUtil::removePoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::removePoints(&gestures->at(i).handTwo.positions, diff);
		}
	}

	for (int i = 0; i < n; i++){
		if(gestures->at(i).numHands == 1){
			futil.mGesturesOneHand.push_back(gestures->at(i));
		} else {
			futil.mGesturesTwoHands.push_back(gestures->at(i));
		}
	}

	futil.saveAll(name);
}

void
Util::equalizeDatasetFromMin(std::vector<type_gesture> *gestures, std::string name){
	FileUtil& futil = FileUtil::getInstance();
	int n = gestures->size(), min = 0, diff = 0;
	min = getMinPoints(*gestures, 0, n - 1);

	futil.clearHandGestures();

	for(int i = 0; i < n; i++){
		diff = min - gestures->at(i).handOne.positions.size();
		if(diff > 0){
			MathUtil::insertPoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::insertPoints(&gestures->at(i).handTwo.positions, diff);
		} else if(diff < 0) {
			MathUtil::removePoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::removePoints(&gestures->at(i).handTwo.positions, diff);
		}
	}

	for (int i = 0; i < n; i++){
		if(gestures->at(i).numHands == 1){
			futil.mGesturesOneHand.push_back(gestures->at(i));
		} else {
			futil.mGesturesTwoHands.push_back(gestures->at(i));
		}
	}

	futil.saveAll(name);
}

void
Util::equalizeDatasetFromMean(std::vector<type_gesture> *gestures, std::string name){
	FileUtil& futil = FileUtil::getInstance();
	int n = gestures->size(), min = 0, diff = 0;
	min = getMeanPoints(*gestures, 0, n - 1);

	futil.clearHandGestures();

	for(int i = 0; i < n; i++){
		diff = min - gestures->at(i).handOne.positions.size();
		if(diff > 0){
			MathUtil::insertPoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::insertPoints(&gestures->at(i).handTwo.positions, diff);
		} else if(diff < 0) {
			MathUtil::removePoints(&gestures->at(i).handOne.positions, diff);
			MathUtil::removePoints(&gestures->at(i).handTwo.positions, diff);
		}
	}

	for (int i = 0; i < n; i++){
		if(gestures->at(i).numHands == 1){
			futil.mGesturesOneHand.push_back(gestures->at(i));
		} else {
			futil.mGesturesTwoHands.push_back(gestures->at(i));
		}
	}

	futil.saveAll(name);
}

int
Util::getMinPoints(vector<type_gesture> gestures, int i, int j) {
    int min = 99999999, n;
    for( ; i < j; i++) {
    	n = gestures[i].handOne.positions.size();
      if(min > n){
      	min = n;
      }
    }
    return min;
}

int
Util::getMaxPoints(vector<type_gesture> gestures, int i, int j) {
    int max = 0, n;
    for( ; i < j; i++) {
    	n = gestures[i].handOne.positions.size();
      if(max < n){
      	max = n;
      }
    }
    return max;
}

int
Util::getMeanPoints(vector<type_gesture> gestures, int i, int j) {
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
Util::scaleGestures(){
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
	fileUtil.saveAll("scaled");
}

void
Util::centerOriginGestures(){
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
    fileUtil.saveAll("origin");
}

void
Util::generateMedianGestures(){
	std::vector<type_gesture> gestures, median;
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA_NORMALIZED);

	gestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	gestures.insert( gestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	gestures.insert( gestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(gestures.begin(), gestures.end(), MathUtil::sortByName);

	size_t n = gestures.size(), i = 0, j = 0, m1, m2, count;
	futil.clearHandGestures();

	/* Generate all gestures with equal length */
	// applyUniformByArcLength(&gestures);
	generateGestureEqualSize(&gestures);

	while(j < n){

		j = i + 1;
		count = 0;
		m1 = gestures[i].handOne.positions.size();
		m2 = gestures[i].handTwo.positions.size();

		while((j < n) && (gestures[i].name.compare(gestures[j].name) == 0)) j++;

		for(int k = i + 1; k < j - 1; k++){
			for(int t = 0; t < m1; t++){
				gestures[i].handOne.positions[t].X += gestures[k].handOne.positions[t].X;
				gestures[i].handOne.positions[t].Y += gestures[k].handOne.positions[t].Y;
				gestures[i].handOne.positions[t].Z += gestures[k].handOne.positions[t].Z;
			}
			for(int t = 0; t < m2; t++){
				gestures[i].handTwo.positions[t].X += gestures[k].handTwo.positions[t].X;
				gestures[i].handTwo.positions[t].Y += gestures[k].handTwo.positions[t].Y;
				gestures[i].handTwo.positions[t].Z += gestures[k].handTwo.positions[t].Z;
			}
			count++;
		}

		for(int k = 0; k < m1; k++){
			gestures[i].handOne.positions[k].X = gestures[i].handOne.positions[k].X / count;
			gestures[i].handOne.positions[k].Y = gestures[i].handOne.positions[k].Y / count;
			gestures[i].handOne.positions[k].Z = gestures[i].handOne.positions[k].Z / count;
		}
		for(int k = 0; k < m2; k++){
			gestures[i].handTwo.positions[k].X = gestures[i].handTwo.positions[k].X / count;
			gestures[i].handTwo.positions[k].Y = gestures[i].handTwo.positions[k].Y / count;
			gestures[i].handTwo.positions[k].Z = gestures[i].handTwo.positions[k].Z / count;
		}

		median.push_back(gestures[i]);
		i = j;
	}

	for (int i = 0; i < median.size(); i++){
		if(median[i].numHands == 1){
			futil.mGesturesOneHand.push_back(median[i]);
		} else {
			futil.mGesturesTwoHands.push_back(median[i]);
		}
	}

	futil.saveAll("median");
}

void
Util::printGesture(const type_gesture gesture){
	for (int i = 0; i < gesture.handOne.positions.size(); i++){
		PRINT(gesture.handOne.positions[i].X << " " << gesture.handOne.positions[i].Y <<" "<<gesture.handOne.positions[i].Z <<" "<<
		gesture.handTwo.positions[i].X << " " << gesture.handTwo.positions[i].Y <<" "<<gesture.handTwo.positions[i].Z);
	}
}

void
Util::normCenterOriginGesture(type_gesture *gesture){
	gesture->handOne.positions = MathUtil::normCenterOrigin(gesture->handOne.positions);
	gesture->handTwo.positions = MathUtil::normCenterOrigin(gesture->handTwo.positions);
}
