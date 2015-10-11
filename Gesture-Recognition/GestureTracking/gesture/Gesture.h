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

/*
  --------- Defines -------------
 */
#define MAX_HANDS 2
#define MAX_HAND_CONTROL_POINTS 10
#define MIN_DIFF_LENGTH 50 //Best aproximated
#define MIN_CONTROL_POINTS 3 //Quadratic Bezier Curve
#define NOT_HAND -1

using namespace std;

/*
 --------- TYPES Gestures ----------
 */
enum{
    
    BEM_VINDO = 1,
    CANCELAR = 2,
    CATEGORIA = 3,
    PAISAGEM = 4,
    MESTRADO = 5,
    PERFEITO = 6
};

/*
 ---------- STATE Gesture -----------
 */
enum{

    GESTURE_DOING = 1,
    GESTURE_STOPED = 2
};

typedef struct type_hand{
    int id_hand;
    vector<XnPoint3D> positions;
}type_hand;


class Gesture{
    
private:
    
    Gesture();
    ~Gesture();
    
public:
    
    void    printPoints();
    void    computeCurve();
    
    void    removeHand(const int idHand);
    void    addHand(const int idHand, XnPoint3D position);
    
    void    update(const int idHand, XnPoint3D position);
    void    updatePosition(const int idHand, XnPoint3D position);
    void    updateState();
    void    updateRecognition();
    
    bool    isGesturePerformed();
    
    void    recognizeDTW();
    
    float     length(XnPoint3D point);
    XnPoint3D subtract(XnPoint3D a, XnPoint3D b);
    XnPoint3D normalize(XnPoint3D point);
    XnPoint3D calcCentroid(vector<XnPoint3D> positions);
    vector<XnPoint3D> translateToOrigin(vector<XnPoint3D> positions);
    
    float   getMaxValue(vector<float> values);
    float   getAngleBetween2Points(XnPoint3D a, XnPoint3D b);
    float   getSumAngles();
    float   getSumDiff(vector<XnPoint3D> positions);
    
    int     m_StateGesture;
    int     m_StateGesturePrev;
    float   m_Diff;
    
    map<int, type_hand> m_Hands;
    map<int,type_hand>::iterator it;
    
    DTW*     mDTW;
    
public:
    static Gesture& getInstance();
    static Gesture*	m_Instance;
    
    int     m_TimeGestureStop;

};

#endif /* defined(__GestureTracking__GestureRecognize__) */
