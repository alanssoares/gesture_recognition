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
    m_TwoHandsRecognized = false;
    m_PosCamera.X = m_PosCamera.Y = m_PosCamera.Z = 0.0;
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
Gesture::update(const int idHand, const XnPoint3D position) {
    updatePosition(idHand, position);
    updateState();
    updateRecognition();
}

void
Gesture::updatePosition(const int idHand, const XnPoint3D position) {
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
Gesture::addHand(const int idHand, const XnPoint3D position) {
    type_hand newHand;
    if(m_Hands.size() == 0){
        newHand.side_hand = RIGHT_HAND;
    } else if(getHand(LEFT_HAND).id_hand == NOT_HAND){
        newHand.side_hand = LEFT_HAND;
    } else {
        newHand.side_hand = RIGHT_HAND;
    }
    newHand.id_hand = idHand;
    newHand.positions.push_back(position);
    m_Hands.insert(pair<int, type_hand>(idHand, newHand));
    m_NumHands++;
}

void
Gesture::removeHand(const int idHand) {
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
    std::vector<XnPoint3D> trajectoryHand, trajectoryComp;
    double distance = 0.0, bestDistance = 999999999;
    type_gesture gestureTemplate;
    
    type_hand hand = getHand(RIGHT_HAND);
    if(hand.id_hand == NOT_HAND){
        hand = getHand(LEFT_HAND);
    }

    //Process the trajectory from user
    trajectoryHand = processTrajectory(hand.positions);
    //Find the best match trajectory using DTW
    for (int i = 0; i < m_GesturesFromFileOneHand.size(); i++) {
        //Process the trajectory template
        trajectoryComp = processTrajectory(m_GesturesFromFileOneHand[i].handOne.positions);
        //Compute the distance using dtw
        distance = computeDistanceBetweenTwoTrajectories(trajectoryComp, trajectoryHand);
        //Verify if the computed distance is lower that previous best
        if(distance < bestDistance){
            bestDistance = distance;
            gestureTemplate.name = m_GesturesFromFileOneHand[i].name;
            gestureTemplate.handOne.positions = m_GesturesFromFileOneHand[i].handOne.positions;
        }
    }
    //Verify if the best distance is lower then the treshold
    if(bestDistance < MIN_DISTANCE_TRESHOLD){
        m_NameGestureRecognized = gestureTemplate.name;
        m_GesturePerformedA = processTrajectory(hand.positions);
        m_GestureTemplateA = processTrajectory(gestureTemplate.handOne.positions);
        m_GesturePerformedProcessedA = processTrajectory(smooth(m_GesturePerformedA));
        m_TwoHandsRecognized = false;
    }
}

void
Gesture::recognizeTwoHands() {
    type_hand leftHand, rightHand;
    std::vector<XnPoint3D> leftHandPoints, rightHandPoints, trajCompLeft, trajCompRight;
    type_gesture gestureTemplate;
    double distanceA = 0.0, distanceB = 0.0, bestDistanceB = 999999999, bestDistanceA = 999999999;
    
    //Initialize the left and right hand according
    leftHand = getHand(LEFT_HAND);
    rightHand = getHand(RIGHT_HAND);

    //Process the trajectories
    rightHandPoints = processTrajectory(rightHand.positions);
    leftHandPoints = processTrajectory(leftHand.positions);
    
    //Find the best match trajectory using DTW
    for (int i = 0; i < m_GesturesFromFileTwoHands.size(); i++) {
        trajCompRight = processTrajectory(m_GesturesFromFileTwoHands[i].handOne.positions);
        distanceA = computeDistanceBetweenTwoTrajectories(trajCompRight, rightHandPoints);
        if (distanceA < bestDistanceA){
            bestDistanceA = distanceA;
            gestureTemplate.name = m_GesturesFromFileTwoHands[i].name;
            gestureTemplate.handOne.positions = m_GesturesFromFileTwoHands[i].handOne.positions;
        }
        trajCompLeft = processTrajectory(m_GesturesFromFileTwoHands[i].handTwo.positions);
        distanceB = computeDistanceBetweenTwoTrajectories(trajCompLeft, leftHandPoints);
        if (distanceB < bestDistanceB){
            bestDistanceB = distanceB;
            gestureTemplate.handTwo.positions = m_GesturesFromFileTwoHands[i].handTwo.positions;
        }
    }

    //Verify if the best distance is lower then the treshold
    if(bestDistanceA < MIN_DISTANCE_TRESHOLD &&
       bestDistanceB < MIN_DISTANCE_TRESHOLD){
        m_NameGestureRecognized = gestureTemplate.name;
        m_GestureTemplateA = gestureTemplate.handOne.positions;
        m_GesturePerformedA = rightHand.positions;
        m_GesturePerformedProcessedA = smooth(rightHand.positions);
        m_GestureTemplateB = gestureTemplate.handTwo.positions;
        m_GesturePerformedB = leftHand.positions;
        m_GesturePerformedProcessedB = smooth(leftHand.positions);
        m_TwoHandsRecognized = true;
    }
}

double
Gesture::computeDistanceBetweenTwoTrajectories(std::vector<XnPoint3D> trajectoryA, std::vector<XnPoint3D> trajectoryB){
    //Initialize the dynamic time warping
    m_Dtw.init();
    //Set sequences that will be computed
    m_Dtw.setSequences(trajectoryA, trajectoryB);
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
    //Smooth the trajectory
    return smooth(trajectory);
}

std::vector<XnPoint3D>
Gesture::smooth(std::vector<XnPoint3D> trajectory){
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
Gesture::setGesturesFromFile(const std::vector<type_gesture> oneHandGestures, const std::vector<type_gesture> twoHandsGestures) {
    m_GesturesFromFileOneHand = oneHandGestures;
    m_GesturesFromFileTwoHands = twoHandsGestures;
}

void
Gesture::clearHands(){
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it) {
        it->second.positions.clear();
    }
    m_LeftHandMoved = m_RightHandMoved = false;
    m_LeftHandStoped = m_RightHandStoped = true;
}

type_hand
Gesture::getHand(const int side_hand){
    type_hand hand;
    hand.id_hand = NOT_HAND;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it) {
        if(it->second.side_hand == side_hand){
            hand = it->second;
        }
    }
    return hand;
}