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

    size_t n1, n2;

    n1 = m_Hands.begin()->second.positions.size();
    n2 = m_Hands.end()->second.positions.size();

    if(n1 > 0 && n2 > 0){
        double dist = MathUtil::getDistancePointToPoint(m_Hands.begin()->second.positions[0], m_Hands.end()->second.positions[0]);
        cout<< "Threshold between two hands - "<<dist<<endl;
        if(dist < MIN_DISTANCE_TRESHOLD_TWO_HANDS){
            LOGGER->Log("Is Two Hands!");
            return true;
        } else if(n1 >= MIN_CONTROL_POINTS && n2 >= MIN_CONTROL_POINTS){
            LOGGER->Log("Is Two Hands!");
            return true;
        }
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
            trajectoryComp = processTrajectory(mGesturesFromFileOneHand[i].handOne.positions);
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
                gestureTemplate.handOne.positions = mGesturesFromFileOneHand[i].handOne.positions;
                gesturePerformed.handOne.positions = it->second.positions;
            }
        }
    }
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
    cout<<"Not implemented yet"<<endl;
    //Como saber qual é a mão direita e qual é a esquerda?
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