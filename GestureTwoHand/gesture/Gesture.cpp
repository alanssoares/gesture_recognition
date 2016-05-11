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
    m_NumHands = 0;
    m_LeftHandMoved = m_RightHandMoved = false;
    m_LeftHandStoped = m_RightHandStoped = true;
}

Gesture::~Gesture(){}

Gesture&
Gesture::getInstance() {
    if(m_Instance == NULL) {
        return *(m_Instance = new Gesture());
    }
    return *m_Instance;
}

void
Gesture::update(const int idHand, XnPoint3D position) {
    updatePosition(idHand, position);
    updateState();
    updateRecognition();
}

void
Gesture::updatePosition(const int idHand, XnPoint3D position) {
    m_Hands.at(idHand).positions.push_back(position);
}

void
Gesture::updateState() {
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it) {
        if(MathUtil::isGestureDoing(it->second.positions)){
            if(it->second.side_hand == LEFT_HAND){
                m_LeftHandMoved = true;
                m_LeftHandStoped = false;
            } else {
                m_RightHandMoved = true;
                m_RightHandStoped = false;
            }
        } else if(it->second.side_hand == LEFT_HAND){
            m_LeftHandStoped = true;
        } else {
            m_RightHandStoped = true;
        }
    }
}

void
Gesture::updateRecognition() {
    if(isLeftHandMoved() && isRightHandMoved()){
        recognizeTwoHands();
        clearHands();
    } if((isLeftHandMoved() && !m_RightHandMoved) || 
        (isRightHandMoved() && !m_LeftHandMoved)){
        recognizeOneHand();
        clearHands();
    }
}

//Ensure that the first hand to be detected is the right 
//to be able to identify later
void
Gesture::addHand(int idHand, XnPoint3D position) {
    type_hand newHand;
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
}

void
Gesture::removeHand(int idHand) {
    if(m_Hands.at(idHand).side_hand == LEFT_HAND){
        m_LeftHandMoved = false;
        m_LeftHandStoped = true;
    } else {
        m_RightHandMoved = false;
        m_RightHandStoped = true;
    }
    m_Hands.erase(idHand);
    m_NumHands--;
}

void
Gesture::recognizeOneHand() {
    cout<<"OneHand"<<endl;
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
}

void
Gesture::recognizeTwoHands() {
    cout<<"TwoHand"<<endl;
    type_hand leftHand, rightHand;
    std::vector<XnPoint3D> leftHandPoints, rightHandPoints, trajCompLeft, trajCompRight;
    type_gesture gestureTemplate, gesturePerformed;
    double distanceA = 0.0, distanceB = 0.0, distanceC = 0.0, bestDistance = 999999999;
    
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

    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        cout<< "Gesture "<<gestureTemplate.name<<" recognized with cost distance "<<bestDistance<<endl;
    }
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
    m_LeftHandMoved = m_RightHandMoved = false;
    m_LeftHandStoped = m_RightHandStoped = true;
}

bool
Gesture::isLeftHandMoved(){
    return m_LeftHandMoved && m_LeftHandStoped;
}

bool
Gesture::isRightHandMoved(){
    return m_RightHandMoved && m_RightHandStoped;
}