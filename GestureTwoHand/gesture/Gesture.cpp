//
//  GestureRecognize.cpp
//  GestureTracking
//
//  Created by Alan Santos on 14/02/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "Gesture.h"


//---------------------------------------------------------------------------
// Statics
//---------------------------------------------------------------------------
Gesture* Gesture::m_Instance = NULL;

Gesture::Gesture() {
    m_StateGesture = GESTURE_STOPED;
    m_NumHands = 0;
}

Gesture::~Gesture(){}

Gesture&
Gesture::getInstance() {
    if(m_Instance == NULL) {
        LOGGER->Log("Instance Gesture Created");
        return *(m_Instance = new Gesture());
    }
    return *m_Instance;
}

void
Gesture::updatePosition(const int idHand, XnPoint3D position) {
    m_Hands.at(idHand).positions.push_back(position);
    LOGGER->Log("Position add");
}

void
Gesture::addHand(int idHand, XnPoint3D position) {
    type_hand newHand;
    //Garantir que a primeira mão a ser detectada é a direita
    //Para poder identificar as posteriores
    if(m_Hands.size() == 0){
        newHand.side_hand = RIGHT_HAND;
    } else {
        it = m_Hands.begin();
        if(it->second.side_hand == LEFT_HAND){
            newHand.side_hand = RIGHT_HAND;
        } else {
            newHand.side_hand = LEFT_HAND;
        }
    }
    newHand.id_hand = idHand;
    newHand.positions.push_back(position);
    m_Hands.insert(pair<int, type_hand>(idHand, newHand));
    m_NumHands++;
    LOGGER->Log("Hand add");
}

void
Gesture::removeHand(int idHand) {
    m_Hands.erase(idHand);
    m_NumHands--;
    LOGGER->Log("Hand removed");
}

void
Gesture::update(const int idHand, XnPoint3D position) {
    updatePosition(idHand, position);
    updateState();
    updateRecognition();
    LOGGER->Log("Update");
}

void
Gesture::updateRecognition() {
    if (isGesturePerformed()) {
        if(isTwoHands()){
            recognizeTwoHands();
        } else {
            recognizeOndeHand();
        }
        clearHands();
    }
    LOGGER->Log("Update recognition ");
}

bool
Gesture::isGesturePerformed() {
    if (m_StateGesturePrev == GESTURE_DOING && m_StateGesture == GESTURE_STOPED) {
        for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
            if(it->second.positions.size() >= MIN_CONTROL_POINTS){
                LOGGER->Log("Gesture performed");
                return true;
            }
        }
    }
    LOGGER->Log("Gesture not performed");
    return false;
}

void
Gesture::updateState() {
    std::vector<double> diffs = calcDiffsTracking();
    m_Diff = MathUtil::getMaxValue(diffs);
    //cout<<"A "<<m_Diff<<endl;
    m_StateGesturePrev = m_StateGesture;
    m_StateGesture = m_Diff >= MIN_DIFF_LENGTH ? GESTURE_DOING : GESTURE_STOPED;
    LOGGER->Log("State updated");
}

bool
Gesture::isTwoHands() {
    if(m_NumHands == 1) return false;
    std::vector<double> diffs = calcDiffsTracking();
    //cout<<"A "<<diffs[0]<<" B "<<diffs[1]<<endl;
    //TODO: adicionar constante e otimizar valores
    if(diffs[0] >= 0.5 && diffs[1] >= 0.5) return true;
    return false;
}

vector<double>
Gesture::calcDiffsTracking(){
    std::vector<double> diffs;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            diffs.push_back(MathUtil::getSumDiff(MathUtil::normalizeTrajectory(MathUtil::translateToOrigin(it->second.positions))));
        }
    }
    return diffs;
}

void
Gesture::recognizeOndeHand() {
    LOGGER->Log("Init DTW");
    std::vector<XnPoint3D> trajectoryHand, trajectoryComp;
    double distance = 0.0, bestDistance = 999999999;
    type_gesture gestureTemplate, gesturePerformed;
    type_hand hand = m_Hands.begin()->second;

    //Process the trajectory from user
    trajectoryHand = processTrajectory(hand.positions);

    //Find the best match trajectory using DTW
    for (int i = 0; i < mGesturesFromFileOneHand.size(); i++) {
        //Process the trajectory template
        trajectoryComp = processTrajectory(mGesturesFromFileOneHand[i].handOne.positions);
        //Compute the distance using dtw
        distance = computeDistanceBetweenTwoTrajectories(trajectoryComp, trajectoryHand);
        //Verify if the computed distance is lower that previous best
        if(distance < bestDistance){
            bestDistance = distance;
            gestureTemplate.name = mGesturesFromFileOneHand[i].name;
            gestureTemplate.handOne.positions = mGesturesFromFileOneHand[i].handOne.positions;
            gesturePerformed.handOne.positions = hand.positions;
        }
    }
    
    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        cout<< "Gesture "<<gestureTemplate.name<<" recognized with cost distance "<<bestDistance<<endl;
        m_gesturePerformed = gesturePerformed.handOne.positions;
        m_gesturePerformedProcessed = MathUtil::simplify(m_gesturePerformed, DPR_FACTOR_SIMPLIFY, DPR_HIGH_QUALITY);
        m_gesturePerformedProcessed = MathUtil::smoothMeanNeighboring(m_gesturePerformedProcessed, NUMBER_SMOOTH_NB);
        m_gestureTemplate = gestureTemplate.handOne.positions;
    }
    LOGGER->Log("End DTW");
}

void
Gesture::recognizeTwoHands() {
    LOGGER->Log("Init DTW");
    type_hand leftHand, rightHand;
    std::vector<XnPoint3D> leftHandPoints, rightHandPoints, trajCompLeft, trajCompRight;
    type_gesture gestureTemplate, gesturePerformed;
    double distanceA = 0.0, distanceB = 0.0, distanceC = 0.0;
    double bestDistance = 999999999;

    //Initialize the left and right hand according
    if(m_Hands.begin()->second.side_hand == LEFT_HAND){
        leftHand = m_Hands.begin()->second;
        rightHand = m_Hands.end()->second;
    } else {
        rightHand = m_Hands.begin()->second;
        leftHand = m_Hands.end()->second;
    }

    rightHandPoints = processTrajectory(rightHand.positions);
    leftHandPoints = processTrajectory(leftHand.positions);

    //Find the best match trajectory using DTW
    for (int i = 0; i < mGesturesFromFileTwoHands.size(); i++) {
        //Process the trajectory template
        trajCompRight = processTrajectory(mGesturesFromFileTwoHands[i].handOne.positions);
        trajCompLeft = processTrajectory(mGesturesFromFileTwoHands[i].handTwo.positions);
         //Compute the distance using dtw
        distanceA = computeDistanceBetweenTwoTrajectories(trajCompRight, rightHandPoints);
        distanceB = computeDistanceBetweenTwoTrajectories(trajCompLeft, leftHandPoints);
        //Sum the two distances
        distanceC = distanceA + distanceB;
        if(distanceC < bestDistance){
            bestDistance = distanceC;
            gestureTemplate.name = mGesturesFromFileTwoHands[i].name;
            gestureTemplate.handOne.positions = mGesturesFromFileTwoHands[i].handOne.positions;
            gestureTemplate.handTwo.positions = mGesturesFromFileTwoHands[i].handTwo.positions;
            gesturePerformed.handOne.positions = rightHand.positions;
            gesturePerformed.handTwo.positions = leftHand.positions;
        }
    }

    cout<<"Best distance: "<<bestDistance<<endl;
    
    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        cout<< "Gesture "<<gestureTemplate.name<<" recognized with cost distance "<<bestDistance<<endl;
    }
    LOGGER->Log("End DTW");
}

double
Gesture::computeDistanceBetweenTwoTrajectories(std::vector<XnPoint3D> A, std::vector<XnPoint3D> B){
    //Initialize the dynamic time warping
    m_Dtw.init();
    //Set sequences that will be computed
    m_Dtw.setSequences(A, B);
    //Calc dtw distance between two trajectories
    m_Dtw.compute();
    //Get the best cost distance computed by dtw
    return m_Dtw.getDistance();
}

std::vector<XnPoint3D> 
Gesture::processTrajectory(std::vector<XnPoint3D> trajectory) {
    //Translate the hand trajectory to origin
    trajectory = MathUtil::translateToOrigin(trajectory);
    //Normalize between the interval -1 to 1
    trajectory = MathUtil::normalizeTrajectory(trajectory);
    //Resample the trajectory using the tolerance distance between points
    trajectory = MathUtil::simplify(trajectory, DPR_FACTOR_SIMPLIFY, DPR_HIGH_QUALITY);
    //Smooth the trajectory according the method choosed
    switch(TYPE_SMOOTH){
        case MEAN_NEIGHBORING:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory, NUMBER_SMOOTH_NB);
            break;
        case CUBIC_B_SPLINE:
            trajectory = MathUtil::applyCubicBSpline(trajectory);
            break;
        case CUBIC_BEZIER:
            trajectory = MathUtil::applyCubicBezier(trajectory);
            break;
        default:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory, NUMBER_SMOOTH_NB);
            break;
    }
    return trajectory;
}

void
Gesture::setGesturesFromFile(std::vector<type_gesture> oneHandGestures, std::vector<type_gesture> twoHandsGestures) {
    mGesturesFromFileOneHand = oneHandGestures;
    mGesturesFromFileTwoHands = twoHandsGestures;
}

void
Gesture::clearHands(){
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it) {
        it->second.positions.clear();
    }
}