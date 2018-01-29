//
//  FeatureExtractor.h
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __FeatureExtractor__
#define __FeatureExtractor__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <XnCppWrapper.h>
#include "ConstantsUtil.h"
#include "../logger/Logger.h"
#include "MathUtil.h"

// http://www.academia.edu/2603409/Hand_Gesture_Recognition_Based_on_Combined_Features_Extraction

class FeatureExtractor {

public:

  FeatureExtractor();
  ~FeatureExtractor();

  double centroidLength(type_gesture gesture);
  double centroidDerivative(type_gesture gA, type_gesture gB);
  double mean(type_gesture gesture);
  double sumVariance(type_gesture gesture);
  double sumCurvature(type_gesture gesture);
  double sumOrientation(type_gesture gesture);
  double standardDeviation(type_gesture gesture);
  std::vector<double> angles(type_gesture sample);
  std::vector<double> curvature(type_gesture sample);
  std::vector<double> locationLC(type_gesture gesture);
  std::vector<double> locationLSC(type_gesture gesture);
  std::vector<double> velocity(type_gesture sample);

  std::vector<FeatureDescriptor> extractDescriptors(std::vector<type_gesture> dataset);
  FeatureDescriptor extractDescriptor(type_gesture sample);
  std::vector<XnPoint3D> merge(type_gesture sample);
};
#endif /* defined(__FeatureExtractor__) */
