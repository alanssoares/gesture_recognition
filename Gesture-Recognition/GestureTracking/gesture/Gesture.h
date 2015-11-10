//
//  Gesture.h
//  GestureTracking
//
//  Created by Alan Santos on 14/02/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__Gesture__
#define __GestureTracking__Gesture__

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <XnCppWrapper.h>
#include "../logger/Logger.h"
#include "../geometry/DTW.h"
#include "../util/MathUtil.h"
#include "../util/ConstantsUtil.h"
#include "../util/FileUtil.h"

using namespace std;

class Gesture{
    
private:
    
    Gesture();
    ~Gesture();
    
public:
    
    void    computeCurve();
    void    clearHands();
    
    void    removeHand(const int idHand);
    void    addHand(const int idHand, XnPoint3D position);
    
    void    update(const int idHand, XnPoint3D position);
    void    updatePosition(const int idHand, XnPoint3D position);
    void    updateState();
    void    updateRecognition();
    
    bool    isGesturePerformed();
    
    void    recognizeDTW();
    
    void    setGesturesFromFile(std::vector<type_gesture> gestures);
    
    int     m_StateGesture;
    int     m_StateGesturePrev;
    float   m_Diff;
    
    map<int, type_hand> m_Hands;
    map<int,type_hand>::iterator it;
    
    DTW*     mDTW;
    FileUtil mFileUtil;
    
    std::vector<type_gesture> mGesturesFromFile;
    
public:
    static Gesture& getInstance();
    static Gesture*	m_Instance;
    
    int     m_TimeGestureStop;

};

#endif /* defined(__GestureTracking__GestureRecognize__) */
