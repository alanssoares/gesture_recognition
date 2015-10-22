//
//  MathUtil.h
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__MathUtil__
#define __GestureTracking__MathUtil__

#include <stdio.h>
#include <XnCppWrapper.h>
#include <vector>
#include <math.h>
#include "../util/ConstantsUtil.h"

using namespace std;

/*
 This class was created to support the development
 of Gesture Recognition Project based in methods that compare trajectories.
 */
class MathUtil
{
    public:
        static double length(XnPoint3D point);
        static XnPoint3D subtract(XnPoint3D a, XnPoint3D b);
        static XnPoint3D normalize(XnPoint3D point);
        static double getAngleBetween2Points(XnPoint3D a, XnPoint3D b);
        static double getMaxValue(std::vector<double> values);
        static double getSumDiff(vector<XnPoint3D> positions);
        static XnPoint3D calcCentroid(vector<XnPoint3D> positions);
        static vector<XnPoint3D> translateToOrigin(vector<XnPoint3D> positions);
};

#endif /* defined(__GestureTracking__MathUtil__) */
