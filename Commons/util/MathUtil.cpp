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
        sum += getArcLength(positions[i + 1], positions[i]);
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
MathUtil::normalizeTrajectory(vector<XnPoint3D> positions, XnPoint3D min, XnPoint3D max){
    size_t n = positions.size();
    for(int i = 0; i < n; i++) {
        positions[i].X = ((positions[i].X - min.X) / (max.X - min.X));
        positions[i].Y = ((positions[i].Y - min.Y) / (max.Y - min.Y));
        positions[i].Z = ((positions[i].Z - min.Z) / (max.Z - min.Z));
    }
    return positions;
}

XnPoint3D
MathUtil::findMinFromTwo(vector<XnPoint3D> a, vector<XnPoint3D> b){
  XnPoint3D min1 = MathUtil::minValueXYZ(a);
  XnPoint3D min2 = MathUtil::minValueXYZ(b);
  if(min1.X > min2.X) min1.X = min2.X;
  if(min1.Y > min2.Y) min1.Y = min2.Y;
  if(min1.Z > min2.Z) min1.Z = min2.Z;
  return min1;
}

XnPoint3D
MathUtil::findMaxFromTwo(vector<XnPoint3D> a, vector<XnPoint3D> b){
  XnPoint3D max1 = MathUtil::maxValueXYZ(a);
  XnPoint3D max2 = MathUtil::maxValueXYZ(b);
  if(max1.X < max2.X) max1.X = max2.X;
  if(max1.Y < max2.Y) max1.Y = max2.Y;
  if(max1.Z < max2.Z) max1.Z = max2.Z;
  return max1;
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
        sqDistance = getArcLength(point, prevPoint);
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

Descriptor
MathUtil::generateDescritpor(std::vector<XnPoint3D> dataSet) {
  PointData data;
  Descriptor descriptor;
  descriptor.qtdPts = 0;
  descriptor.qtdPtsInflection = 0;
  descriptor.qtdMax = 0;
  descriptor.qtdMin = 0;
  descriptor.positions.push_back(dataSet.front());

  int curIndex = 0;
  while (curIndex < dataSet.size() - 1) {
    data = findNextInflectionOrLocalMinMax(dataSet, curIndex, SEARCH_RIGHT, ANY);
    descriptor.positions.push_back(data.point);
    // Verify the type of the point (min, max, inflection or none)
    if(data.type == LOCAL_MIN){
      descriptor.qtdMin++;
    } else if(data.type == LOCAL_MAX){
      descriptor.qtdMax++;
    } else if(data.type == INFLECTION_POINT){
      descriptor.qtdPtsInflection++;
    }
    // What index of the data set corresponds to the next control point?
    int oldIndex = curIndex;
    for (int i = curIndex; i < dataSet.size(); i++) {
      if (abs(dataSet[i].X - data.point.X) < 1E-4
          && abs(dataSet[i].Y - data.point.Y) < 1E-4
          && abs(dataSet[i].Z - data.point.Z) < 1E-4) {
        curIndex = i;
        break;
      }
    }
    if (curIndex == oldIndex) {
      std::cout << "Then Could not locate next min/max/inflection/edge after point" << std::endl;
      break;
    }
  }

  // Make sure that the last point is included in the data set.
  XnPoint3D lastControlPoint = descriptor.positions.back();
  XnPoint3D lastPoint = dataSet.back();
  if ((abs(lastPoint.X - lastControlPoint.X) > 1E-6) ||
      (abs(lastPoint.Y - lastControlPoint.Y) > 1E-6) ||
      (abs(lastPoint.Z - lastControlPoint.Z) > 1E-6)) {
    descriptor.positions.push_back(lastPoint);
  }

  descriptor.qtdPts = descriptor.positions.size();

  PRINT("QtdPts : " << descriptor.qtdPts);
  PRINT("QtdPtsInflection : " << descriptor.qtdPtsInflection);
  PRINT("QtdMax : " << descriptor.qtdMax);
  PRINT("QtdMin : " << descriptor.qtdMin);

  return descriptor;
}

PointData
MathUtil::findNextInflectionOrLocalMinMax(std::vector<XnPoint3D> dataSet, int startingIndex, int direction, int pointType){
  if (direction != SEARCH_LEFT && direction != SEARCH_RIGHT) {
    std::cout<<"Direction must be DataSetUtils.SEARCH_LEFT or DataSetUtils.SEARCH_RIGHT"<<std::endl;
    exit(1);
  }
  if (pointType != LOCAL_MIN && pointType != LOCAL_MAX
      && pointType != INFLECTION_POINT && pointType != ANY) {
    std::cout<<"DataSetUtils.findNextInflectionOrLocalMinMax() improper usage.  pointType must be DataSetUtils.LOCAL_MIN, DataSetUtils.LOCAL_MAX, DataSetUtils.INFLECTION_POINT, or DataSetUtils.ANY."<<std::endl;
    exit(1);
  }
  if (startingIndex < 0 || startingIndex >= dataSet.size()) {
    std::cout<<"DataSetUtils.findNextInflectionOrLocalMinMax() improper usage. "<<std::endl;
    std::cout<<"Your starting index must be an index of the data set.  startingIndex = "<<startingIndex<< ", dataSetsize() = "<< dataSet.size()<<std::endl;
    exit(1);
  }

  std::vector<XnPoint3D> subset;
  if (direction == SEARCH_LEFT) {
    if (startingIndex <= 1) {
      // They are searching left from the left most point or next to left most
      // point. Return the left end point.
      PointData data;
      data.point = dataSet.front();
      data.type = END_POINT;
      return data;
    }
    /*
     * If starting index is 2, then we want a 2-element array and fill
     * with points 0 and 1.
     */
    subset.push_back(dataSet[0]);
    subset.push_back(dataSet[1]);
  }
  if (direction == SEARCH_RIGHT) {
    if (startingIndex >= dataSet.size() - 2) {
      // They are searching right from the right most point or next to
      // right most point. Return the right end point.
      PointData data;
      data.point = dataSet.back();
      data.type = END_POINT;
      return data;
    }
    /*
     * If starting index is 2 and the size() of the global data set is
     * 5, then we want a 2-element array and fill with points 3 and 4.
     * So the size of the search array is 5 - 2 - 1 = 2.
     * Using above scenario, we want to fill subset with dataSet
     * elements 3, and 4. Subset has indicies 0 and 1 so we add an
     * offset of to map the data set to the subset.
     */
     subset.push_back(dataSet[0 + startingIndex + 1]);
     subset.push_back(dataSet[1 + startingIndex + 1]);
     subset.push_back(dataSet[2 + startingIndex + 1]);
  }
  return search(subset, direction, pointType);
}

PointData
MathUtil::search(std::vector<XnPoint3D> subset, int direction, int pointType) {
  PointData data;
  size_t nSubset = subset.size();
  // To check for local min, max, inflection, three points are needed.
  int curIndex = (direction == SEARCH_LEFT) ? nSubset - 1 : 0;
  int nextIndex = (direction == SEARCH_LEFT) ? nSubset - 2 : 1;
  int followingIndex = (direction == SEARCH_LEFT) ? nSubset - 3 : 2;

  if (direction == SEARCH_LEFT && followingIndex <= 0) {
    data.point = subset.front();
    data.type = END_POINT;
    return data;
  }
  if (direction == SEARCH_RIGHT && followingIndex >= subset.size() - 1) {
    data.point = subset.back();
    data.type = END_POINT;
    return data;
  }

  int indexStep = (direction == SEARCH_RIGHT) ? 1 : -1;
  while (followingIndex >= 0 && followingIndex <= subset.size() - 1) {
    // Search for the min, max, or inflection.
    int nextType = (direction == SEARCH_LEFT) ? checkMinMax(
        subset[followingIndex], subset[nextIndex], subset[curIndex])
        : checkMinMax(subset[curIndex], subset[nextIndex],
            subset[followingIndex]);
    if (nextType == LOCAL_MIN && (pointType == LOCAL_MIN || pointType == ANY)) {
      //Return the "middle" of the three points which should be on
      //average closest to the true min, max, or inflection point.
      data.point = subset[nextIndex];
      data.type = LOCAL_MIN;
      return data;
    } else if (nextType == LOCAL_MAX && (pointType == LOCAL_MAX || pointType == ANY)) {
      //Return the "middle" of the three points which should be on
      //average closest to the true min, max, or inflection point.
      data.point = subset[nextIndex];
      data.type = LOCAL_MAX;
      return data;
    }

    nextType = -1;

    if (pointType == INFLECTION_POINT || pointType == ANY) {
      // We need 5 data points to test for an inflection point.
      if (direction == SEARCH_RIGHT
          && ((followingIndex + 1) < subset.size())
          && ((curIndex - 1) >= 0)) {
        nextType = checkInflectionPoint(subset[curIndex - 1],
            subset[curIndex], subset[nextIndex],
            subset[followingIndex], subset[followingIndex + 1]);
      } else if (direction == SEARCH_LEFT && ((followingIndex - 1) >= 0)
          && ((curIndex + 1) < subset.size())) {
        nextType = checkInflectionPoint(subset[followingIndex - 1],
            subset[followingIndex], subset[nextIndex],
            subset[curIndex], subset[curIndex + 1]);
      }
    }

    if (nextType == INFLECTION_POINT && (pointType == INFLECTION_POINT || pointType == ANY)) {
      //Return the "middle" of the three points which should be on
      //average closest to the true min, max, or inflection point.
      PointData data;
      data.point = subset[nextIndex];
      data.type = INFLECTION_POINT;
      return data;
    }

    curIndex += indexStep;
    nextIndex += indexStep;
    followingIndex += indexStep;
  }

  // If we get here, we did not find a local min, max, or inflection point
  // before reaching the edge of the graph.
  if (direction == SEARCH_LEFT && followingIndex < 0) {
    data.point = subset.front();
    data.type = END_POINT;
    return data;
  }

  data.point = subset.back();
  data.type = END_POINT;
  return data;
}

int
MathUtil::checkMinMax(XnPoint3D p1, XnPoint3D p2, XnPoint3D p3) {
    double slope1 = calcSlope(p1, p2);
    double slope2 = calcSlope(p2, p3);
    if (slope1 * slope2 <= 0.0) {
        return (slope2 > slope1) ? LOCAL_MIN : LOCAL_MAX;
    }
    return -1;
}

double
MathUtil::calcSlope(XnPoint3D p1, XnPoint3D p2) {
    XnPoint3D p3 = subtract(p1, p2);
    double run = sqrt(pow(p3.X,2) + pow(p3.Y,2));
    return p3.Z / run;
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
        return -1;
    }

    // For the next test, the absolute value of the slope is decreasing
    // for slopes 1 and 2 while increasing for slopes 3 and 4, there is an
    // inflection point.
    if (abs(slope2) < abs(slope1) && abs(slope4) > abs(slope3)){
        return INFLECTION_POINT;
    }

    // For the next test, the absolute value of the slope is increasing
    // for slopes 1 and 2 while decreasing for slopes 3 and 4, there is an
    // inflection point.
    if (abs(slope2) > abs(slope1) && abs(slope4) < abs(slope3)){
        return INFLECTION_POINT;
    }

    // We have not determined that there is an inflection point.
    return -1;
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
MathUtil::getArcLength(XnPoint3D p1, XnPoint3D p2){
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
            trajectory = smoothMeanNeighboring(trajectory);
            break;
        case CUBIC_B_SPLINE:
            trajectory = BSpline::curvePoints(trajectory, NUM_STEP_BSPLINE);
            break;
        case CUBIC_BEZIER:
            trajectory = applyCubicBezier(trajectory);
            break;
        default:
            trajectory = smoothMeanNeighboring(trajectory);
            break;
    }
    return trajectory;
}

std::vector<XnPoint3D>
MathUtil::normCenterOrigin(std::vector<XnPoint3D> trajectory) {
    //Translate the hand trajectory to origin
    trajectory = translateToOrigin(trajectory);
    //Normalize between the interval -1 to 1
    return normalizeTrajectory(trajectory, minValueXYZ(trajectory), maxValueXYZ(trajectory));
}

std::vector<XnPoint3D>
MathUtil::smoothAndReduce(std::vector<XnPoint3D> trajectory) {
    //Remove points according with curvature
    trajectory = reduceByCurvature(trajectory, THRESHOLD_CURVATURE);
    //Smooth the trajectory
    return smooth(trajectory);
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

void
MathUtil::uniformCurveByArcLength(std::vector<XnPoint3D> *points, double dL){
  size_t n = points->size();
  double aL, t = 1.0;
  int i = 0;
  XnPoint3D prev;
  while(i < n - 1) {
    aL = getArcLength(points->at(i), points->at(i + 1));
    if(aL < dL){
      points->erase(points->begin() + i + 1);
      n--;
      continue;
    }
    while(aL > dL){
      prev = interpolate(points->at(i), points->at(i + 1), t);
      aL = getArcLength(points->at(i), prev);
      t -= 0.01;
    }
    points->at(i + 1) = prev;
    i++;
    t = 1.0;
  }
}
