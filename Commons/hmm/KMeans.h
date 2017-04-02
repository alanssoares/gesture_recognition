//
//  KMeans.h
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

#ifndef __KMeans__
#define __KMeans__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <XnCppWrapper.h>

#include "../commons.hpp"

class Cluster {

public:

  Cluster() {};
  ~Cluster() {};

  void assignPoints(std::vector<XnPoint3D> points);

  std::vector<XnPoint3D> mCollection;
};

class KMeans {

public:

  KMeans() {};
  ~KMeans() {};

  std::vector<Cluster> cluster(std::vector<type_gesture> gestures, int K);

  std::vector<Cluster> createClusters(int K);

  std::vector<XnPoint3D> calculateCentroids(std::vector<Cluster> clusters);

  size_t findNearestCluster(std::vector<XnPoint3D> centroids, XnPoint3D point);

};

#endif /* defined(__KMeans__) */
