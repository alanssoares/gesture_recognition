//
//  CKMeans.h
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

#ifndef __CKMeans__
#define __CKMeans__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <XnCppWrapper.h>

#include "../commons.hpp"

class Cluster {

public:

  Cluster() {};
  ~Cluster() {};

  /**
  * Assign gesture to cluster collection
  * @param gesture
  */
  void addGesture(type_gesture gesture);

  std::vector<type_gesture> mCollection;
  type_gesture mBaseCluster;
};

class CKMeans {

public:

  CKMeans() {};
  ~CKMeans() {};

  /**
  * Execute K-Means using a set of gestures
  * @param gestures to be clustered
  * @param medians with initial centroids
  */
  std::vector<Cluster> cluster(std::vector<type_gesture> gestures, std::vector<type_gesture> medians);

  /**
  * Init a set of K Clusters with median gestures
  * @param gestures
  * @return std::vector<Cluster> with K clusters
  */
  std::vector<Cluster> createClusters(std::vector<type_gesture> gestures);

  /**
  * Fin the nearest cluster according with the point
  * @param clusters
  * @param gesture
  * @return size_t
  */
  size_t findNearestCluster(std::vector<Cluster> clusters, type_gesture gesture);

};

#endif /* defined(__CKMeans__) */
