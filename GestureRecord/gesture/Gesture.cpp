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
    LOGGER->Log("Update");
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

void
Gesture::clearHands(){
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it) {
        it->second.positions.clear();
    }
}