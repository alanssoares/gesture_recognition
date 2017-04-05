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

// Definitions of the classes
class Cluster;
class KMeans;

/**
* Implementation of the K-Means algorithm
* Author - Alan Soares
*/
class KMeans {

public:

  KMeans() {};
  ~KMeans() {};

  /**
  * Cluster the points
  * @param points
  * @param K number of clusters
  */
  std::vector<Cluster> cluster(std::vector<XnPoint3D> points, int K);

  /**
  * Init a set of K clusters
  * @param K
  */
  std::vector<Cluster> createClusters(int K);

  /**
  * Calculates the cluster centroids
  * @param clusters
  */
  std::vector<XnPoint3D> calculateCentroids(std::vector<Cluster> clusters);

  /**
  * Find the nearest cluster according with the centroid and the point
  * @param centroids
  * @param point
  */
  int findNearestCluster(std::vector<XnPoint3D> centroids, XnPoint3D point);

};

/**
* Implementation of the Cluster used in K-Means
* @author Alan Soares
*/
class Cluster {

public:

  Cluster() {};
  ~Cluster() {};

  /**
  * Assign a collection of points to collection
  * @param points
  */
  void addCollection(std::vector<XnPoint3D> points);

  std::vector<XnPoint3D> mCollection;
};

#endif /* defined(__KMeans__) */
