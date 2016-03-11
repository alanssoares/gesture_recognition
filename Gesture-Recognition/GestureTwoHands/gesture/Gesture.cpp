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
    vector<double> diff;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            diff.push_back(MathUtil::getSumDiff(it->second.positions));
        }
    }
    m_Diff = MathUtil::getMaxValue(diff);
    m_StateGesturePrev = m_StateGesture;
    m_StateGesture = m_Diff > MIN_DIFF_LENGTH ? GESTURE_DOING : GESTURE_STOPED;
    LOGGER->Log("State updated");
}

bool
Gesture::isTwoHands() {
    if(m_NumHands == 1) {
        LOGGER->Log("Is One Hand!");
        return false;
    }
    double dist = MathUtil::getDistancePointToPoint(m_Hands.begin()->second.positions[0], m_Hands.end()->second.positions[0]);
    if(dist < MIN_DISTANCE_TRESHOLD_TWO_HANDS){
        LOGGER->Log("Is Two Hands!");
        return true;
    } else if(m_Hands.begin()->second.positions.size() >= MIN_CONTROL_POINTS && 
        m_Hands.end()->second.positions.size() >= MIN_CONTROL_POINTS){
        LOGGER->Log("Is Two Hands!");
        return true;
    }
    LOGGER->Log("Is One Hand!");
    return false;
}

void
Gesture::recognizeOndeHand() {
    LOGGER->Log("Init DTW");
    DTW2 dtw;
    std::vector<XnPoint3D> trajectoryHand;
    std::vector<XnPoint3D> trajectoryComp;
    double distance = 0.0;
    double bestDistance = 999999999;
    type_gesture gestureTemplate;
    type_gesture gesturePerformed;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        //Process the trajectory from user
        trajectoryHand = processTrajectory(it->second.positions);
        for (int i = 0; i < mGesturesFromFileOneHand.size(); i++) {
            //Process the trajectory template
            trajectoryComp = processTrajectory(mGesturesFromFileOneHand[i].positions);
            //Initialize the dynamic time warping
            dtw.init();
            //Set sequences that will be computed
            dtw.setSequences(trajectoryHand, trajectoryComp);
            //Calc dtw distance between two trajectories
            dtw.compute();
            //Get the best cost distance computed by dtw
            distance = dtw.getDistance();
            //Verify if the computed distance is lower that previous best
            if(distance < bestDistance){
                bestDistance = distance;
                gestureTemplate.name = mGesturesFromFileOneHand[i].name;
                gestureTemplate.positions = mGesturesFromFileOneHand[i].positions;
                gesturePerformed.positions = it->second.positions;
            }
        }
    }
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        cout<< "Gesture "<<gestureTemplate.name<<" recognized with cost distance "<<bestDistance<<endl;
        m_gesturePerformed = gesturePerformed.positions;
        m_gesturePerformedProcessed = MathUtil::simplify(m_gesturePerformed, 0.01, false);
        m_gesturePerformedProcessed = MathUtil::smoothMeanNeighboring(m_gesturePerformedProcessed, 1);
        m_gestureTemplate = gestureTemplate.positions;
    }
    LOGGER->Log("End DTW");    
}

void
Gesture::recognizeTwoHands() {

}

std::vector<XnPoint3D> 
Gesture::processTrajectory(std::vector<XnPoint3D> trajectory) {
    //Translate the hand trajectory to origin
    trajectory = MathUtil::translateToOrigin(trajectory);
    //Normalize between the interval -1 to 1
    trajectory = MathUtil::normalizeTrajectory(trajectory);
    //Resample the trajectory using the tolerance distance between points
    trajectory = MathUtil::simplify(trajectory, 0.01, false);
    //Smooth the trajectory according the method choosed
    switch(TYPE_SMOOTH){
        case MEAN_NEIGHBORING:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory, 1);
            break;
        case CUBIC_B_SPLINE:
            trajectory = MathUtil::applyCubicBSpline(trajectory);
            break;
        case CUBIC_BEZIER:
            trajectory = MathUtil::applyCubicBezier(trajectory);
            break;
        default:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory, 1);
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