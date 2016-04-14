//
//  ConstantsUtil.h
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//  Link - http://www.nsl.tuis.ac.jp/doxygen/JunkBox_Lib++/OpenNiDevice_8cpp_source.html
//

#ifndef GestureTracking_ConstantsUtil_h
#define GestureTracking_ConstantsUtil_h

#include <iostream>
#include <vector>
#include <XnCppWrapper.h>

using namespace std;

/* Check the result and return 1 if XN_STATUS is not ok */
#define CHECK_RC(rc, message){ if(rc != XN_STATUS_OK){ printf(message, xnGetStatusString(rc)); return 1; }}

/* Check the result and return if XN_STATUS is not ok */
#define CHECK_RC_VOID(rc, message){ if(rc != XN_STATUS_OK){ printf(message, xnGetStatusString(rc)); return; }}

/* Receive a time to sleep */
#define Sleep(t) usleep((t)*1000)

/* Max number of hands to recognize */
#define MAX_HANDS 2

/* Max number of last points to verify the hand stopped ou moving */
#define MAX_HAND_CONTROL_POINTS 10
/*
 Define the min diff length between the MAX_HAND_CONTROL_POINTS
 to verify if the hand is stopped or moving
 */
#define MIN_DIFF_LENGTH 1 //Best aproximated

/* The gesture is considered when have more then 3 points */
#define MIN_CONTROL_POINTS 3 //Quadratic Bezier Curve

/* Is not an hand */
#define NOT_HAND -1

/* Define the minimum distance threshold to recognize gestures */
#define MIN_DISTANCE_TRESHOLD 0.5

/* Define the min distance between two hands */
#define MIN_DISTANCE_TRESHOLD_TWO_HANDS 50

/* Define a quantidade de steps para cada intervalo da curva bspline */
#define MIN_STEP_BSPLINE 7

/* Define o tipo de método que será usado para suavizar a trajetória */
#define TYPE_SMOOTH 1

/* Define a taxa de incremento usada na interpolação das curvas */
#define INCREMENT_RATE_INTERPOLATION 0.02

/* Number of times that the smooth will be applied */
#define NUMBER_SMOOTH_NB 1

/* Factor for simplify using the algorithm Douglas-Peucker Radial */
#define DPR_FACTOR_SIMPLIFY 0.01

/* Flag for high quality for simplify using the algorithm Douglas-Peucker Radial */
#define DPR_HIGH_QUALITY false

#define GESTURE "gesture"
#define NAME_FILE_DATA "../Samples.txt"

#define NAME_WINDOW_GR "Project GRS"
#define GAP_WINDOW      25
#define GL_WIN_SIZE_X   1024
#define GL_WIN_SIZE_Y   512
#define GL_WIN_SIZE_MAIN_X 512
#define GL_WIN_SIZE_MAIN_Y 512

 /*
 Define the name of the file that will be saved
 */
typedef struct params {
    int modeOnline;
    std::string fileImage;
    std::string fileDepth;
} params;

/*
 --------- TYPES Gestures ----------
 */
enum {
    BEM_VINDO = 1,
    CANCELAR = 2,
    CATEGORIA = 3,
    PAISAGEM = 4,
    MESTRADO = 5,
    PERFEITO = 6
};

/*
------ Types methods for smooth
*/
enum {
    MEAN_NEIGHBORING = 1,
    CUBIC_B_SPLINE = 2,
    CUBIC_BEZIER = 3
};
/*
 ---------- STATE Gesture -----------
 */
enum {
    GESTURE_DOING = 1,
    GESTURE_STOPED = 2
};

/* Define the left and right hands */
enum {  
    LEFT_HAND = 0,
    RIGHT_HAND = 1
};
/*
 ---- Object Types created to define an hand and an type of gesture
 */
typedef struct type_hand {
    int id_hand;
    int side_hand;
    vector<XnPoint3D> positions;
} type_hand;

typedef struct type_gesture {
    int numHands;
    std::string name;
    type_hand handOne;
    type_hand handTwo;
} type_gesture;

#endif
