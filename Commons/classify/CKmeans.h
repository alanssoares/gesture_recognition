//
//  CKmeans.h
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

#ifndef __CKmeans__
#define __CKmeans__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <XnCppWrapper.h>

#include "../commons.hpp"

class ClusterCKmeans {

public:

  ClusterCKmeans() {};
  ~ClusterCKmeans() {};

  /**
  * Assign gesture to cluster collection
  * @param gesture
  */
  void addGesture(type_gesture gesture);

  std::vector<type_gesture> mCollection;
  type_gesture mBaseCluster;
};

/*
 - Busca nos n clusters qual possui a melhor possibilidade de classificação
 - Para o cluster encontrado, efetua a busca do gesto até que ele seja aceito ou rejeitado pelo cluster de acordo com a probabilidade
 - Caso seja rejeitado, o cluster é descartado e um novo é buscado
 - Continua o loop com n - 1 clusters até que o gesto seja classificado ou rejeitado pela classificação (não encontrou nenhum)
*/
class CKmeans {

public:

  CKmeans() { m_RecThreshold = 0.5; };
  ~CKmeans() {};

  /**
  * Classify gesture using an adaptative K-Means
  * @param gesture to be classified
  * @return type_gesture that best match
  */
  type_gesture classify(type_gesture gesture);

  /**
  * Init a set of K Clusters according with gestures class
  * @param gestures
  * @return std::vector<Cluster> with K clusters
  */
  void createClusters(std::vector<type_gesture> gestures);

  /**
  * Find nearest cluster according with the gesture
  * @param gesture
  * @param totalParticle
  * @return size_t
  */
  size_t findNearestCluster(type_gesture gesture, size_t totalParticle);

  /**
  * Check the distance matching between gA and gB using DTW
  * @param gA
  * @param gB
  * @return bool
  */
  bool isCloseCluster(type_gesture gA, type_gesture gB);

public:

  std::vector<ClusterCKmeans> mClusters;
  double m_RecThreshold;

};

#endif /* defined(__CKmeans__) */
