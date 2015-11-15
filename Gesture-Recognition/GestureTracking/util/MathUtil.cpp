//
//  MathUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "MathUtil.h"

double
MathUtil::length(XnPoint3D point){
    return sqrt(pow(point.X, 2) + pow(point.Y, 2) + pow(point.Z, 2));
}

XnPoint3D
MathUtil::subtract(XnPoint3D a, XnPoint3D b){
    a.X = a.X - b.X;
    a.Y = a.Y - b.Y;
    a.Z = a.Z - b.Z;
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

double
MathUtil::getAngleBetween2Points(XnPoint3D a, XnPoint3D b){
    double mU, mV, mUV, uv, angle;
    
    //Produto vetorial
    uv = a.X * b.X + a.Y * b.Y;
    
    //Módulo dos vetores
    mU = sqrt(pow(a.X, 2) + pow(a.Y, 2));
    mV = sqrt(pow(b.X, 2) + pow(b.Y, 2));
    
    //Produto vetorial dos módulos
    mUV = mU * mV;
    
    angle = cos(uv/mUV);
    
    angle = acos(angle);
    
    return angle;
}

double
MathUtil::getMaxValue(vector<double> values)
{
    double max = 0.0;
    for (double value : values)
    {
        if (max < value) {
            max = value;
        }
    }
    
    return max;
}

/*
 Somatório das diferenças de distância euclidiana entre as n posições anteriores
 */
double
MathUtil::getSumDiff(vector<XnPoint3D> positions)
{
    float total = 0.0;
    size_t n = positions.size();
    size_t len = n - MAX_HAND_CONTROL_POINTS;
    
    XnPoint3D ant = positions[n-1];
    XnPoint3D result;
    for (size_t i = len; i < n; i++){
        result = MathUtil::subtract(ant, positions[i]);
        total += MathUtil::length(result);
        ant = positions[i];
    }
    
    return total;
}

XnPoint3D
MathUtil::calcCentroid(vector<XnPoint3D> positions)
{
    XnPoint3D centroid;
    centroid.X = centroid.Y = centroid.Z = 0;
    for(int i = 0; i < positions.size(); i++) {
        centroid.X += positions[i].X;
        centroid.Y += positions[i].Y;
        centroid.Z += positions[i].Z;
    }
    //Sum 1 to prevent division by zero
    centroid.X = (centroid.X + 1)/(positions.size() + 1) - 1;
    centroid.Y = (centroid.Y + 1)/(positions.size() + 1) - 1;
    centroid.Z = (centroid.Z + 1)/(positions.size() + 1) - 1;
    
    return centroid;
}

vector<XnPoint3D>
MathUtil::translateToOrigin(vector<XnPoint3D> positions){
    
    XnPoint3D centroid = calcCentroid(positions);
    
    for(int i = 0; i < positions.size(); i++) {
        positions[i].X -= centroid.X;
        positions[i].Y -= centroid.Y;
        positions[i].Z -= centroid.Z;
    }
    
    return positions;
}

vector<XnPoint3D>
MathUtil::normalizeTrajectory(vector<XnPoint3D> positions){
    double desiredMin = -1.0;
    double desiredMax = 1.0;
    double desiredRange = desiredMax - desiredMin;
    
    XnPoint3D minPos = minValueXYZ(positions);
    XnPoint3D maxPos = maxValueXYZ(positions);
    
    double originalRangeX = maxPos.X - minPos.X;
    double originalRangeY = maxPos.Y - minPos.Y;
    double originalRangeZ = maxPos.Z - minPos.Z;
    
    for(int i = 0; i < positions.size(); i++) {
        positions[i].X = desiredRange * (positions[i].X - minPos.X)/ originalRangeX + desiredMin;
        positions[i].Y = desiredRange * (positions[i].Y - minPos.Y)/ originalRangeY + desiredMin;
        positions[i].Z = desiredRange * (positions[i].Z - minPos.Z)/ originalRangeZ + desiredMin;
    }
    
    return positions;
}

XnPoint3D
MathUtil::minValueXYZ(vector<XnPoint3D> positions){
    XnPoint3D minPos;
    minPos.X = 99999999;
    minPos.Y = 99999999;
    minPos.Z = 99999999;
    for(XnPoint3D pos : positions) {
        if(pos.X < minPos.X) minPos.X = pos.X;
        if(pos.Y < minPos.Y) minPos.Y = pos.Y;
        if(pos.Z < minPos.Z) minPos.Z = pos.Z;
    }
    return minPos;
}

XnPoint3D
MathUtil::maxValueXYZ(vector<XnPoint3D> positions){
    XnPoint3D maxPos;
    maxPos.X = -99999999;
    maxPos.Y = -99999999;
    maxPos.Z = -99999999;
    for(XnPoint3D pos : positions) {
        if(pos.X > maxPos.X) maxPos.X = pos.X;
        if(pos.Y > maxPos.Y) maxPos.Y = pos.Y;
        if(pos.Z > maxPos.Z) maxPos.Z = pos.Z;
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