//
//  KMeans.cpp
//  GestureTracking
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

// 1 - First the algorithm randomly assigns all the points of the trajectories to one of the k clusters.
// 2 - For each cluster, k-means calculates the centroid of all the points in the cluster by using Eq. (1).
// 3 - Evaluates all the points and reassigns each point to the cluster whose centroid is nearest it, according to the proximity criteria (Eq. (3)). This criteria is the Euclidean distance between the point (Pi in the equation) and the cluster centroid (Cj in the equation).
// 4 - Having moved all the points to the corresponding cluster, the algorithm recalculates the centroid of each cluster.
// 5 - This process is repeated until no point movement is required, i.e., all the points are in the correct group.

#include "KMeans.h"

std::vector<Cluster>
KMeans::cluster(std::vector<type_gesture> gestures, int K){

  // Init clusters according with the number of K
  std::vector<Cluster> clusters = createClusters(K);

  // Assigns all the points of the trajectories to one of the k clusters
  for (size_t i = 0; i < gestures.size(); i++) {
    clusters[0].assignPoints(gestures[i].handOne.positions);
    clusters[0].assignPoints(gestures[i].handTwo.positions);
  }

  // Calculates the centroid of all the points in the cluster
  std::vector<XnPoint3D> centroids = calculateCentroids(clusters);

  int clustering = 1;
  while(clustering){
    clustering = 0;
    for (size_t i = 0; i < clusters.size(); i++) {
      size_t n = clusters[i].mCollection.size();
      for (size_t j = 0; j < n; j++) {
        XnPoint3D point = clusters[i].mCollection[j];
        size_t k = findNearestCluster(centroids, point);
        if(clusters[i].mCollection.size() > 1){
          clusters[i].mCollection.erase(clusters[i].mCollection.begin() + j);
          clusters[k].mCollection.push_back(point);
          clustering++; n--;
        }
      }
      // Recalculate all centroids
      centroids = calculateCentroids(clusters);
      std::cout << "C " << i << " N = " << clusters[i].mCollection.size() << '\n';
    }
  }

  return clusters;
}

std::vector<Cluster>
KMeans::createClusters(int K){
  std::vector<Cluster> clusters;
  for (size_t i = 0; i < K; i++) {
    Cluster randCluster;
    clusters.push_back(randCluster);
  }
  return clusters;
}

std::vector<XnPoint3D>
KMeans::calculateCentroids(std::vector<Cluster> clusters){
  std::vector<XnPoint3D> centroids;
  for (size_t i = 0; i < clusters.size(); i++) {
      centroids.push_back(MathUtil::calcCentroid(clusters[i].mCollection));
  }
  return centroids;
}

size_t
KMeans::findNearestCluster(std::vector<XnPoint3D> centroids, XnPoint3D point){
  double min = 999999999, dist = 0.0;
  size_t k = -1;
  for (size_t i = 0; i < centroids.size(); i++) {
    dist = MathUtil::getArcLength(point, centroids[i]);
    if(dist < min){
      dist = min;
      k = i;
    }
  }
  return k;
}

void
Cluster::assignPoints(std::vector<XnPoint3D> points) {
  for (size_t i = 0; i < points.size(); i++) {
    mCollection.push_back(points[i]);
  }
}
