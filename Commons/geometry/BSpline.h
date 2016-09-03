#ifndef BSPLINE_H
#define BSPLINE_H

#include <string>
#include <cmath>
#include <XnCppWrapper.h>

#include "Vector3f.h"



class BSpline {
    
public:
	BSpline(){};
    ~BSpline(){};

    /*
     Calc the basis function for a cubic B spline
     @param i
     @param t
    */
    static double baseFunction(int i, double t);

    /*
    Create the cubic bspline using the points and step
    @param points of control
    @param steps
    */
    static std::vector<XnPoint3D> curvePoints(std::vector<XnPoint3D> points, int steps);

    /*
    Evaluate an ith point on the B spline
    @param i
    @param t
    @param points
    */
    static XnPoint3D predictPoint(int i, double t, std::vector<XnPoint3D> points);

};

#endif // BSPLINE_H
