//
//  MathUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "MathUtil.h"

std::string
MathUtil::intToString(int n){
    std::ostringstream converter;
    converter << n;
    return converter.str();
}

std::string
MathUtil::floatToString(float n){
    std::ostringstream converter;
    converter << n;
    return converter.str();
}

double
MathUtil::length(XnPoint3D point){
    return sqrt(pow(point.X, 2) + pow(point.Y, 2) + pow(point.Z, 2));
}

XnPoint3D
MathUtil::subtract(XnPoint3D a, XnPoint3D b){
    a.X = b.X - a.X;
    a.Y = b.Y - a.Y;
    a.Z = b.Z - a.Z;
    return a;
}

XnPoint3D
MathUtil::normalize(XnPoint3D point) {
    float len = length(point);
    point.X = point.X / len;
    point.Y = point.Y / len;
    point.Z = point.Z / len;
    return point;
}

XnPoint3D
MathUtil::sum(XnPoint3D a, XnPoint3D b) {
    XnPoint3D c;
    c.X = a.X + b.X;
    c.Y = a.Y + b.Y;
    c.Z = a.Z + b.Z;
    return c;
}

XnPoint3D
MathUtil::interpolate(XnPoint3D p0, XnPoint3D p1, float t){
    XnPoint3D c;

    if(t > 1.f) t = 1.f;
    if(t < 0.f) t = 0.f;

    c.X = (1 - t) * p0.X + t * p1.X;
    c.Y = (1 - t) * p0.Y + t * p1.Y;
    c.Z = (1 - t) * p0.Z + t * p1.Z;

    return c;
}

double
MathUtil::getAngleBetween2Points(XnPoint3D a, XnPoint3D b){
    double mU, mV, mUV, uv, angle;

    //Produto vetorial
    uv = a.X * b.X + a.Y * b.Y + a.Z * b.Z;

    //Módulo dos vetores
    mU = sqrt(pow(a.X, 2) + pow(a.Y, 2) + pow(a.Z, 2));
    mV = sqrt(pow(b.X, 2) + pow(b.Y, 2)) + pow(b.Z, 2);

    //Produto vetorial dos módulos
    mUV = mU * mV;

    angle = acos(cos(uv/mUV));

    return angle;
}

double
MathUtil::getMaxValue(vector<double> values){
    size_t n = values.size();
    double max = 0.0;
    for (int i = 0; i < n; i++) {
        if (max < values[i]) {
            max = values[i];
        }
    }

    return max;
}

bool
MathUtil::isGestureDoing(vector<XnPoint3D> positions){
    size_t n = positions.size();
    int i = n - NUM_LAST_POINTS;
    if(i < 0) return false;
    double sum = 0.0;
    for (; i < n - 1; i++){
        sum += getDistancePointToPoint(positions[i + 1], positions[i]);
    }
    sum = sum / NUM_LAST_POINTS;
    if(sum < MIN_DIFF_LENGTH){
        return false;
    }
    return true;
}

XnPoint3D
MathUtil::calcCentroid(vector<XnPoint3D> positions){
    size_t n = positions.size();
    XnPoint3D centroid;
    centroid.X = centroid.Y = centroid.Z = 0;
    for(int i = 0; i < n; i++) {
        centroid.X += positions[i].X;
        centroid.Y += positions[i].Y;
        centroid.Z += positions[i].Z;
    }
    //Sum 1 to prevent division by zero
    centroid.X = (centroid.X + 1)/(n + 1) - 1;
    centroid.Y = (centroid.Y + 1)/(n + 1) - 1;
    centroid.Z = (centroid.Z + 1)/(n + 1) - 1;

    return centroid;
}

vector<XnPoint3D>
MathUtil::translateToOrigin(vector<XnPoint3D> positions){
    size_t n = positions.size();
    XnPoint3D centroid = calcCentroid(positions);

    for(int i = 0; i < n; i++) {
        positions[i].X -= centroid.X;
        positions[i].Y -= centroid.Y;
        positions[i].Z -= centroid.Z;
    }

    return positions;
}

vector<XnPoint3D>
MathUtil::normalizeTrajectory(vector<XnPoint3D> positions){
    XnPoint3D minPos = minValueXYZ(positions);
    XnPoint3D maxPos = maxValueXYZ(positions);
    XnPoint3D originalRange = subtract(minPos, maxPos);
    const double desiredMin = -1.0;
    const double desiredMax = 1.0;
    double desiredRange = desiredMax - desiredMin;
    size_t n = positions.size();
    for(int i = 0; i < n; i++) {
        positions[i].X = desiredRange * (positions[i].X - minPos.X)/ originalRange.X + desiredMin;
        positions[i].Y = desiredRange * (positions[i].Y - minPos.Y)/ originalRange.Y + desiredMin;
        positions[i].Z = desiredRange * (positions[i].Z - minPos.Z)/ originalRange.Z + desiredMin;
    }
    return positions;
}

XnPoint3D
MathUtil::minValueXYZ(vector<XnPoint3D> positions){
    size_t n = positions.size();
    XnPoint3D minPos;
    minPos.X = 99999999;
    minPos.Y = 99999999;
    minPos.Z = 99999999;
    for(int i = 0; i < n; i++) {
        if(positions[i].X < minPos.X) minPos.X = positions[i].X;
        if(positions[i].Y < minPos.Y) minPos.Y = positions[i].Y;
        if(positions[i].Z < minPos.Z) minPos.Z = positions[i].Z;
    }
    return minPos;
}

XnPoint3D
MathUtil::maxValueXYZ(vector<XnPoint3D> positions){
    size_t n = positions.size();
    XnPoint3D maxPos;
    maxPos.X = -99999999;
    maxPos.Y = -99999999;
    maxPos.Z = -99999999;
    for(int i = 0; i < n; i++) {
        if(positions[i].X > maxPos.X) maxPos.X = positions[i].X;
        if(positions[i].Y > maxPos.Y) maxPos.Y = positions[i].Y;
        if(positions[i].Z > maxPos.Z) maxPos.Z = positions[i].Z;
    }
    return maxPos;
}

std::vector<XnPoint3D>
MathUtil::applyCubicBezier(std::vector<XnPoint3D> positions){
    Bezier bezier;
    size_t n  = positions.size();
    size_t mean = ceil(n/2);
    XnPoint3D p0, p1, p2, p3, newPos;
    std::vector<XnPoint3D> positionsInterpolated;

    p0 = positions[0];
    p1 = positions[ceil(mean/2)];
    p2 = positions[ceil(n - (mean/2))];
    p3 = positions[n - 1];

    bezier.setCubic(p0, p1, p2, p3, 1.0f);

    for (float i = 0.0; i < 1.0; i+=INCREMENT_RATE_INTERPOLATION) {
        Vector3f pos = bezier.getCubicPosition(i);
        newPos.X = pos.getX();
        newPos.Y = pos.getY();
        newPos.Z = pos.getZ();
        positionsInterpolated.push_back(newPos);
    }

    return positionsInterpolated;
}

std::vector<XnPoint3D>
MathUtil::smoothMeanNeighboring(std::vector<XnPoint3D> positions){
    std::vector<XnPoint3D> smoothed;
    XnPoint3D meanPoint;
    const int numPoints = 3;
    size_t n  = positions.size();
    if(n >= MIN_CONTROL_POINTS){
        smoothed.push_back(positions.front());
        for (int i = 0; i < n - 2; i++) {
            meanPoint.X = (positions[i].X + positions[i + 1].X + positions[i + 2].X)/numPoints;
            meanPoint.Y = (positions[i].Y + positions[i + 1].Y + positions[i + 2].Y)/numPoints;
            meanPoint.Z = (positions[i].Z + positions[i + 1].Z + positions[i + 2].Z)/numPoints;
            smoothed.push_back(meanPoint);
        }
        smoothed.push_back(positions.back());
    } else {
        //Is not possible smooth, return the original positions
        smoothed = positions;
    }
    return smoothed;
}

std::vector<XnPoint3D>
MathUtil::simplify(std::vector<XnPoint3D> points, double tolerance, bool highestQuality){
    double sqTolerance = pow(tolerance, 2);

    if(points.size() <= 2) {
        return points;
    }

    if(highestQuality){
        points = simplifyRadialDist(points, sqTolerance);
    }

    return simplifyDouglasPeucker(points, sqTolerance);
}

std::vector<XnPoint3D>
MathUtil::simplifyRadialDist(std::vector<XnPoint3D> points, double sqTolerance){
    XnPoint3D prevPoint, point;
    std::vector<XnPoint3D> newPoints;
    double sqDistance = 0.0;
    size_t n = points.size();
    prevPoint = points[0];
    newPoints.push_back(prevPoint);

    for (int i = 1; i < n; i++) {
        point = points[i];
        sqDistance = getDistancePointToPoint(point, prevPoint);
        if (sqDistance > sqTolerance) {
            newPoints.push_back(point);
            prevPoint = point;
        }
    }

    if (!pointsEqual(prevPoint, point)) {
        newPoints.push_back(point);
    }

    return newPoints;
}

std::vector<XnPoint3D>
MathUtil::simplifyDouglasPeucker(std::vector<XnPoint3D> points, double sqTolerance){
    std::vector<XnPoint3D> simplified;
    simplified.push_back(points.front());
    simplifyDPStep(points, 0, points.size() - 1, sqTolerance, &simplified);
    simplified.push_back(points.back());
    return simplified;
}

void
MathUtil::simplifyDPStep(std::vector<XnPoint3D> points, int first, int last, double sqTolerance, std::vector<XnPoint3D> *simplified){
    double maxSqDist = sqTolerance;
    int index = -1;

    for (int i = first + 1; i < last; i++) {
        double sqDist = getDistancePointToSegment(points[i], points[first], points[last]);
        if (sqDist > maxSqDist) {
            index = i;
            maxSqDist = sqDist;
        }
    }

    if (maxSqDist > sqTolerance) {
        if (index - first > 1) {
            simplifyDPStep(points, first, index, sqTolerance, simplified);
        }

        simplified->push_back(points[index]);

        if (last - index > 1) {
            simplifyDPStep(points, index, last, sqTolerance, simplified);
        }
    }
}

std::vector<XnPoint3D>
MathUtil::reduceByCurvature(std::vector<XnPoint3D> points, double threshold){
    std::vector<XnPoint3D> newPoints;
    size_t n = points.size();
    if(n == 0) return newPoints;
    double curvature = 0.0;

    newPoints.push_back(points.front());

    for (int i = 0; i < n - 2; i+=2){
        curvature = calcCurvature(points[i], points[i + 1], points[i + 2]);
        if(curvature > threshold){
            newPoints.push_back(points[i + 1]);
        }
    }

    newPoints.push_back(points.back());

    return newPoints;
}

float
MathUtil::calcCurvature(XnPoint3D a, XnPoint3D b, XnPoint3D c){
    XnPoint3D l1, l2;
    l1 = subtract(a, b);
    l2 = subtract(b, c);
    return length(subtract(l1, l2));
}

int
MathUtil::checkMinMax(XnPoint3D p1, XnPoint3D p2, XnPoint3D p3) {
    double slope1 = calcSlope(p1, p2);
    double slope2 = calcSlope(p2, p3);
    if (slope1 * slope2 <= 0.0) { // Local min / max
        return (slope2 > slope1) ? -1 : 1;
    }
    return 0;
}

double
MathUtil::calcSlope(XnPoint3D p1, XnPoint3D p2) {
    return length(subtract(p1, p2));
}

int
MathUtil::checkInflectionPoint(XnPoint3D p1, XnPoint3D p2,
            XnPoint3D p3, XnPoint3D p4, XnPoint3D p5) {
    double slope1 = calcSlope(p1, p2);
    double slope2 = calcSlope(p2, p3);
    double slope3 = calcSlope(p3, p4);
    double slope4 = calcSlope(p4, p5);
    bool functionDecreasing = (slope1 < 0.0 && slope2 < 0.0
            && slope3 < 0.0 && slope4 < 0.0);
    bool functionIncreasing = (slope1 > 0.0 && slope2 > 0.0
            && slope3 > 0.0 && slope4 > 0.0);

    // It cannot be an inflection point. The function must
    // always be increasing or decreasing through all five
    // points.
    if (!functionDecreasing && !functionIncreasing){
        return 0;
    }

    // For the next test, the absolute value of the slope is decreasing
    // for slopes 1 and 2 while increasing for slopes 3 and 4, there is an
    // inflection point.
    if (abs(slope2) < abs(slope1) && abs(slope4) > abs(slope3)){
        return 1;
    }

    // For the next test, the absolute value of the slope is increasing
    // for slopes 1 and 2 while decreasing for slopes 3 and 4, there is an
    // inflection point.
    if (abs(slope2) > abs(slope1) && abs(slope4) < abs(slope3)){
        return 1;
    }

    // We have not determined that there is an inflection point.
    return 0;
}

Descriptor
MathUtil::extractDescriptor(std::vector<XnPoint3D> points){
    Descriptor descriptor;
    descriptor.qtdPts = points.size();
    descriptor.qtdPtsInflection = 0;
    descriptor.qtdMax = 0;
    descriptor.qtdMin = 0;
    descriptor.length = 0;
    int c = 0;

    for(int i = 1; i < descriptor.qtdPts - 1; i++){
        c = checkMinMax(points[i - 1], points[i], points[i + 1]);
        if(c == -1) {
            descriptor.qtdMin++;
        } else if(c == 1) {
            descriptor.qtdMax++;
        }
    }

    for(int i = 1; i < descriptor.qtdPts - 3; i++){
        c = checkInflectionPoint(points[i - 1], points[i], points[i + 1], points[i + 2], points[i + 3]);
        if(c == 1){
            descriptor.qtdPtsInflection++;
        }
    }

    // PRINT("QtdPts : " << descriptor.qtdPts);
    // PRINT("QtdPtsInflection : " << descriptor.qtdPtsInflection);
    // PRINT("QtdMax : " << descriptor.qtdMax);
    // PRINT("QtdMin : " << descriptor.qtdMin);

    return descriptor;
}

double
MathUtil::getDistancePointToPoint(XnPoint3D p1, XnPoint3D p2){
    return length(subtract(p1, p2));
}

double
MathUtil::getDistancePointToSegment(XnPoint3D p, XnPoint3D p1, XnPoint3D p2){
    double x = p1.X,
        y = p1.Y,
        z = p1.Z,
        dx = p2.X - x,
        dy = p2.Y - y,
        dz = p2.Z - z;

    if (dx != 0 || dy != 0 || dz != 0) {
        double t = ((p.X - x) * dx + (p.Y - y) * dy + (p.Z - z) * dz) / (pow(dx,2) + pow(dy,2) + pow(dz,2));
        if (t > 1.0) {
            x = p2.X;
            y = p2.Y;
            z = p2.Z;
        } else if (t > 0.0) {
            x += dx * t;
            y += dy * t;
            z += dz * t;
        }
    }

    dx = p.X - x;
    dy = p.Y - y;
    dz = p.Z - z;

    return pow(dx,2) + pow(dy,2) + pow(dz,2);
}

std::vector<XnPoint3D>
MathUtil::smooth(std::vector<XnPoint3D> trajectory){
    switch(TYPE_SMOOTH){
        case MEAN_NEIGHBORING:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory);
            break;
        case CUBIC_B_SPLINE:
            trajectory = BSpline::curvePoints(trajectory, NUM_STEP_BSPLINE);
            break;
        case CUBIC_BEZIER:
            trajectory = MathUtil::applyCubicBezier(trajectory);
            break;
        default:
            trajectory = MathUtil::smoothMeanNeighboring(trajectory);
            break;
    }
    return trajectory;
}

std::vector<XnPoint3D>
MathUtil::normCenterOrigin(std::vector<XnPoint3D> trajectory) {
    //Translate the hand trajectory to origin
    trajectory = MathUtil::translateToOrigin(trajectory);
    //Normalize between the interval -1 to 1
    return MathUtil::normalizeTrajectory(trajectory);
}

std::vector<XnPoint3D>
MathUtil::smoothAndReduce(std::vector<XnPoint3D> trajectory) {
    //Remove points according with curvature
    trajectory = MathUtil::reduceByCurvature(trajectory, THRESHOLD_CURVATURE);
    //Smooth the trajectory
    return MathUtil::smooth(trajectory);
}

double
MathUtil::computeDistanceBetweenTwoTrajectories(std::vector<XnPoint3D> trajectoryA, std::vector<XnPoint3D> trajectoryB){
    DTW2 dtw;
    //Initialize the dynamic time warping
    dtw.init();
    //Set sequences that will be computed
    dtw.setSequences(trajectoryA, trajectoryB);
    //Calc dtw distance between two trajectories
    dtw.compute();
    //Get the best cost distance computed by dtw
    return dtw.getDistance();
}

bool
MathUtil::pointsEqual(XnPoint3D p1, XnPoint3D p2){
    return p1.X == p2.X && p1.Y == p2.Y && p1.Z == p2.Z;
}

void
MathUtil::insertPoints(std::vector<XnPoint3D> *points, int diff){
    int i, n, index = -1;
    double curv = 0.0, minCurv = 9999999;
    XnPoint3D newPoint;
    while(diff > 0){
        n = points->size();
        for (i = 0; i < n - 2; i+=2){
            curv = calcCurvature(points->at(i), points->at(i + 1), points->at(i + 2));
            if(curv < minCurv){
                minCurv = curv;
                index = i + 1;
            }
        }
        if(index >= 0 && index + 1 < n){
            newPoint = interpolate(points->at(index), points->at(index + 1), 0.5);
            points->insert(points->begin() + index, newPoint);
        }
        minCurv = 99999999;
        index = -1;
        diff--;
    }
}

void
MathUtil::removePoints(std::vector<XnPoint3D> *points, int diff){
    int i, n, index = -1;
    double curv = 0.0, minCurv = 9999999;
    while(diff < 0){
        n = points->size();
        for (i = 0; i < n - 2; i+=2){
            curv = calcCurvature(points->at(i), points->at(i + 1), points->at(i + 2));
            if(curv < minCurv){
                minCurv = curv;
                index = i + 1;
            }
        }
        if(index >= 0 && index + 1 < n){
            points->erase(points->begin() + index);
        }
        minCurv = 99999999;
        index = -1;
        diff++;
    }
}
