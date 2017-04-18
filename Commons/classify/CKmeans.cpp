//
//  CKmeans.cpp
//  GestureTracking
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

#include "CKmeans.h"

type_gesture
CKmeans::classify(type_gesture gesture){
  int clustering = 1, searching, k, n, probMatched = 0, probUnmatched = 0, totalParticle = 5;

  while(clustering){
    k = findNearestCluster(gesture, totalParticle);
    searching = 1;
    while (searching) {
      n = mClusters[k].mCollection.size();
      for (int i = 0; i < n; i++) {
        if(isCloseCluster(mClusters[k].mCollection[i], gesture)){
          probMatched++;
        } else {
          probUnmatched++;
        }
        // std::cout << "PM = " << probMatched << " PUM = " << probUnmatched << " K = " << k << std::endl;
        if(probMatched > n/4){
          searching = clustering = 0;
          break;
        } else if(probUnmatched > n/4){
          searching = probMatched = probUnmatched = 0;
          mClusters.erase(mClusters.begin() + k);
          break;
        }
      }
    }
  }

  return mClusters[k].mCollection[0];
}

void
CKmeans::createClusters(std::vector<type_gesture> gestures){
  mClusters.clear();
  int n = gestures.size(), s = n - 1, i = 0;
  std::sort(gestures.begin(), gestures.end(), MathUtil::sortByName);
  while (i < n) {
    ClusterCKmeans cluster;
    while(i < s && gestures[i].name.compare(gestures[i + 1].name) == 0){
      cluster.addGesture(gestures[i++]);
    }
    if(i <= s) cluster.addGesture(gestures[i]);
    mClusters.push_back(cluster); i++;
  }
}

void
ClusterCKmeans::addGesture(type_gesture gesture) {
  gesture.handOne.centroid = MathUtil::calcCentroid(gesture.handOne.positions);
  gesture.handTwo.centroid = MathUtil::calcCentroid(gesture.handTwo.positions);
  mCollection.push_back(gesture);
}

size_t
CKmeans::findNearestCluster(type_gesture gesture, size_t totalParticle){
  XnPoint3D c1, c2;
  size_t k = -1, totalClusters = mClusters.size(), total = 0, minTotal = 0;

  for (size_t i = 0; i < totalClusters; i++, total = 0) {
    for (size_t j = 0; j < totalParticle; j++) {
      if(isCloseCluster(mClusters[i].mCollection[j], gesture)){
        total++;
      }
    }
    if(total > minTotal){
      minTotal = total;
      k = i;
    }
  }

  return k;
}

bool
CKmeans::isCloseCluster(type_gesture gA, type_gesture gB){
  double dA = MathUtil::computeDistanceBetweenTwoTrajectories(gA.handOne.positions, gB.handOne.positions),
         dB = MathUtil::computeDistanceBetweenTwoTrajectories(gA.handTwo.positions, gB.handTwo.positions);
  return dA < 0.5 && dB < 0.5 ? true : false;
}
