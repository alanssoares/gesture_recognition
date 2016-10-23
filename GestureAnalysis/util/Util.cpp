#include "Util.h"

Util::Util(){
	m_CurvThreshold = 0.0001;
	m_DougThreshold = 0.01;
	m_isMedian = false;
}

Util::~Util(){}

void
Util::applyNormalization(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	XnPoint3D min, max;
	for (int i = 0; i < n; i++){
		max = MathUtil::findMaxFromTwo(gestures->at(i).handOne.positions, gestures->at(i).handTwo.positions);
		min = MathUtil::findMinFromTwo(gestures->at(i).handOne.positions, gestures->at(i).handTwo.positions);
		gestures->at(i).handOne.positions = MathUtil::normalizeTrajectory(gestures->at(i).handOne.positions, min, max);
		gestures->at(i).handTwo.positions = MathUtil::normalizeTrajectory(gestures->at(i).handTwo.positions, min, max);
	}
}

void
Util::applyLaplacian(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handOne.positions);
		gestures->at(i).handTwo.positions = MathUtil::smoothMeanNeighboring(gestures->at(i).handTwo.positions);
	}
}

void
Util::applyBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = BSpline::curvePoints(gestures->at(i).handOne.positions, NUM_STEP_BSPLINE);
		gestures->at(i).handTwo.positions = BSpline::curvePoints(gestures->at(i).handTwo.positions, NUM_STEP_BSPLINE);
	}
}

void
Util::applyUniformBSpline(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = BSpline::uniformFitting(gestures->at(i).handOne.positions);
		gestures->at(i).handTwo.positions = BSpline::uniformFitting(gestures->at(i).handTwo.positions);
	}
}

void
Util::applyCurvature(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::reduceByCurvature(gestures->at(i).handOne.positions, m_CurvThreshold);
		gestures->at(i).handTwo.positions = MathUtil::reduceByCurvature(gestures->at(i).handTwo.positions, m_CurvThreshold);
	}
}

void
Util::applyDouglasPeucker(std::vector<type_gesture>* gestures){
	size_t n = gestures->size();
	for (int i = 0; i < n; i++) {
		gestures->at(i).handOne.positions = MathUtil::simplify(gestures->at(i).handOne.positions, m_DougThreshold, false);
		gestures->at(i).handTwo.positions = MathUtil::simplify(gestures->at(i).handTwo.positions, m_DougThreshold, false);
	}
}

void
Util::generateGestureEqualSize(std::vector<type_gesture> *gestures){
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
Util::getMeanPoints(vector<type_gesture> gestures, int i, int j){
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
	fileUtil.saveAll();
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
    fileUtil.saveAll();
}

void
Util::generateMedianGestures(){
	std::vector<type_gesture> gestures, median;
	FileUtil& futil = FileUtil::getInstance();
	futil.loadGestures(NAME_FILE_DATA);

	gestures.reserve(futil.mGesturesOneHand.size() + futil.mGesturesTwoHands.size());
	gestures.insert( gestures.end(), futil.mGesturesOneHand.begin(), futil.mGesturesOneHand.end() );
	gestures.insert( gestures.end(), futil.mGesturesTwoHands.begin(), futil.mGesturesTwoHands.end() );
	std::sort(gestures.begin(), gestures.end(), sortByName);

	size_t n = gestures.size(), i = 0, j = 0, m, count;
	futil.clearHandGestures();

	/* Generate all gestures with equal length */
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

	futil.saveAll();
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
