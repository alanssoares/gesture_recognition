//
//  ConstantsUtil.h
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef GestureTracking_ConstantsUtil_h
#define GestureTracking_ConstantsUtil_h

#include <iostream>
#include <vector>
#include <XnCppWrapper.h>

using namespace std;

/* Max number of hands to recognize */
#define MAX_HANDS 2

/* Max number of last points to verify the hand stopped ou moving */
#define MAX_HAND_CONTROL_POINTS 10
/*
 Define the min diff length between the MAX_HAND_CONTROL_POINTS
 to verify if the hand is stopped or moving
 */
#define MIN_DIFF_LENGTH 50 //Best aproximated

/* The gesture is considered when have more then 3 points */
#define MIN_CONTROL_POINTS 3 //Quadratic Bezier Curve

/* Is not an hand */
#define NOT_HAND -1

/*
 Define the minimum distance threshold to recognize gestures
 */
#define MIN_DISTANCE_TRESHOLD 100

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

/*
 ---- Object Types created to define an hand and an type of gesture
 */
typedef struct type_hand{
    int id_hand;
    vector<XnPoint3D> positions;
} type_hand;

typedef struct type_gesture{
    std::string name;
    vector<XnPoint3D> positions;
} type_gesture;

#endif
