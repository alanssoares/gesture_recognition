#include "BSpline.h"


double
BSpline::baseFunction(int i, double t){
    switch (i) {
        case -2:
            return (((-t + 3) * t - 3) * t + 1) / 6;
        case -1:
            return (((3 * t - 6) * t) * t + 4) / 6;
        case 0:
            return (((-3 * t + 3) * t + 3) * t + 1) / 6;
        case 1:
            return (t * t * t) / 6;
    }
    return 0; //we only get here if an invalid i is specified
}

std::vector<XnPoint3D> 
BSpline::curvePoints(std::vector<XnPoint3D> points, int steps){
	std::vector<XnPoint3D> curve;
	int numPts = points.size() - 1;
  	int pts = numPts * steps + 1;
    XnPoint3D p = predictPoint(2, 0, points);
    curve.push_back(p);
    for (int i = 2; i < numPts; i++) {
        for (int j = 1; j <= steps; j++) {
        	p = predictPoint(i, j / (double) steps, points);
            curve.push_back(p);
        }
    }
    return curve;
}

XnPoint3D 
BSpline::predictPoint(int i, double t, std::vector<XnPoint3D> points){
    XnPoint3D point;
    point.X = 0; point.Y = 0; point.Z = 0;
    for (int j = -2; j <= 1; j++) {
        point.X += points[i + j].X * baseFunction(j, t);
        point.Y += points[i + j].Y * baseFunction(j, t);
        point.Z += points[i + j].Z * baseFunction(j, t);
    }
    return point;
}