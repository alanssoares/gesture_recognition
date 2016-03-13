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

Gesture::Gesture()
{
    m_StateGesture = GESTURE_STOPED;
}

Gesture::~Gesture(){}

Gesture&
Gesture::getInstance()
{
    if(m_Instance == NULL)
    {
        LOGGER->Log("Instance Gesture Created");
        return *(m_Instance = new Gesture());
    }

    return *m_Instance;
}

void
Gesture::updatePosition(const int idHand, XnPoint3D position)
{
    m_Hands.at(idHand).positions.push_back(position);
}

void
Gesture::addHand(int idHand, XnPoint3D position)
{
    type_hand newHand;
    
    newHand.id_hand = idHand;
    newHand.positions.push_back(position);
    
    m_Hands.insert(pair<int, type_hand>(idHand, newHand));
    
    LOGGER->Log("Hand add");
}

void
Gesture::removeHand(int idHand)
{
    m_Hands.erase(idHand);
    
    LOGGER->Log("Hand removed");
}

void
Gesture::update(const int idHand, XnPoint3D position)
{
    updatePosition(idHand, position);
    
    updateState();
    
    updateRecognition();
}

void
Gesture::updateRecognition()
{
    if (isGesturePerformed())
    {
        recognizeDTW();
        
        clearHands();
    }
}

bool
Gesture::isGesturePerformed()
{
    if (m_StateGesturePrev == GESTURE_DOING && m_StateGesture == GESTURE_STOPED)
    {
        for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
            if (it->second.positions.size() >= MIN_CONTROL_POINTS) {
                LOGGER->Log("Gesture performed");
                return true;
            }
        }
    }
    
    return false;
}

void
Gesture::updateState()
{
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

void
Gesture::recognizeDTW(){
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
        for (int i = 0; i < mGesturesFromFile.size(); i++) {
            //Process the trajectory template
            trajectoryComp = processTrajectory(mGesturesFromFile[i].positions);
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
                gestureTemplate.name = mGesturesFromFile[i].name;
                gestureTemplate.positions = mGesturesFromFile[i].positions;
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

std::vector<XnPoint3D> 
Gesture::processTrajectory(std::vector<XnPoint3D> trajectory){
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
Gesture::setGesturesFromFile(std::vector<type_gesture> gestures){
    mGesturesFromFile = gestures;
}

void
Gesture::clearHands(){
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        it->second.positions.clear();
    }
}