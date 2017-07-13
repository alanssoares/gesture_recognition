//
//  FeatureExtractor.cpp
//  FeatureExtractor
//
//  Created by Alan Santos on 28/06/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor() {
}

FeatureExtractor::~FeatureExtractor(){}

double
FeatureExtractor::mean(type_gesture sample) {
  XnPoint3D mean;
  std::vector<XnPoint3D> positions = merge(sample);
  mean.X = 0.0;
  mean.Y = 0.0;
  mean.Z = 0.0;
  size_t n = positions.size();
  for (int i = 0; i < n; i++) {
    mean.X += positions[i].X;
    mean.Y += positions[i].Y;
    mean.Z += positions[i].Z;
  }
  mean.X = (mean.X + 1) / ( n + 1);
  mean.Y = (mean.Y + 1) / ( n + 1);
  mean.Z = (mean.Z + 1) / ( n + 1);
  return MathUtil::length(mean);
}

double
FeatureExtractor::sumVariance(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  double meanValue = mean(sample), len = 0.0, variance = 0.0;
  size_t n = positions.size();
  for (int i = 0; i < n; i++) {
    len = MathUtil::length(positions[i]);
    len -= meanValue;
    variance += pow(len, 2);
  }
  return variance;
}

double
FeatureExtractor::sumCurvature(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  size_t n = positions.size();
  if (n < 3) return 0.0;
  double curv = 0.0;
  for (int i = 0; i < n - 2; i += 2) {
    curv += MathUtil::calcCurvature(positions[i], positions[i + 1], positions[i + 2]);
  }
  return curv;
}

double
FeatureExtractor::sumOrientation(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  size_t n = positions.size();
  double angles = 0.0;
  for (int i = 0; i < n - 1; i++) {
    angles += MathUtil::getAngleBetween2Points(positions[i], positions[i + 1]);
  }
  return angles;
}

double
FeatureExtractor::standardDeviation(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  double variance = sumVariance(sample);
  return sqrt(variance / positions.size());
}

double
FeatureExtractor::centroidLength(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  XnPoint3D centroid = MathUtil::calcCentroid(positions);
  return MathUtil::length(centroid);
}

double
FeatureExtractor::locationLC(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  size_t n = positions.size();
  XnPoint3D centroid = MathUtil::calcCentroid(positions), diff;
  double lc = 0.0;
  for (int i = 0; i < n; i++) {
    diff = MathUtil::subtract(centroid, positions[i]);
    lc += MathUtil::length(diff);
  }
  return lc;
}

double
FeatureExtractor::locationLSC(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  size_t n = positions.size();
  XnPoint3D start = positions[0], diff;
  double lsc = 0.0;
  for (int i = 1; i < n; i++) {
    diff = MathUtil::subtract(start, positions[i]);
    lsc += MathUtil::length(diff);
  }
  return lsc;
}

std::vector<XnPoint3D>
FeatureExtractor::merge(type_gesture sample) {
  std::vector<XnPoint3D> positions;
  positions.reserve(sample.handOne.positions.size() + sample.handTwo.positions.size());
  positions.insert( positions.end(), sample.handOne.positions.begin(), sample.handOne.positions.end() );
  positions.insert( positions.end(), sample.handTwo.positions.begin(), sample.handTwo.positions.end() );
  return positions;
}

FeatureDescriptor
FeatureExtractor::extractDescriptor(type_gesture sample) {
  FeatureDescriptor descriptor;
  descriptor.mean = mean(sample);
  descriptor.centroidLength = centroidLength(sample);
  descriptor.sumVariance = sumVariance(sample);
  descriptor.sumCurvature = sumCurvature(sample);
  descriptor.sumOrientation = sumOrientation(sample);
  descriptor.standardDeviation = standardDeviation(sample);
  descriptor.lc = locationLC(sample);
  descriptor.lsc = locationLSC(sample);
  return descriptor;
}

XnPoint3D
FeatureExtractor::centroid(type_gesture sample) {
  std::vector<XnPoint3D> positions = merge(sample);
  return MathUtil::calcCentroid(positions);
}

XnPoint3D
FeatureExtractor::centroidDerivative(type_gesture gA, type_gesture gB) {
  std::vector<XnPoint3D> posA = merge(gA);
  std::vector<XnPoint3D> posB = merge(gB);
  XnPoint3D cA = MathUtil::calcCentroid(posA);
  XnPoint3D cB = MathUtil::calcCentroid(posB);
  return MathUtil::subtract(cA, cB);
}

double
FeatureExtractor::centroidAngle2D(type_gesture gA, type_gesture gB) {

}

double
FeatureExtractor::centroidAngle3D(type_gesture gA, type_gesture gB) {

}
