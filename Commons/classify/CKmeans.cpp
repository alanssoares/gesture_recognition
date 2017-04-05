//
//  CKMeans.cpp
//  GestureTracking
//
//  Created by Alan Santos on 02/04/17.
//  Copyright (c) 2017 Alan Santos. All rights reserved.
//

/*
 O gesto entre e começa a comparação com os n clusters (quantidade de classes de gestos). Na primeira comparação,
 o cluster mais próximo será utilizado como base e a comparação com os gestos do cluster prossegue até chegar a uma consluão
 de que para x testes com o cluster o gesto foi y % reconhecido (alta confiabilidade). Caso a taxa diminua, o gesto passa a ser
 comparado com outro (o cluster anterior não é mais comparado) e o processo segue até encontrar o que mais se aproxima do real.

 - O K-means só é necessário se todos os gestos estiverem agrupados
*/
#include "CKMeans.h"

std::vector<Cluster>
CKMeans::cluster(std::vector<type_gesture> gestures, std::vector<type_gesture> medians){

  // Init clusters according with the number of K
  std::vector<Cluster> clusters = createClusters(medians);

  // Assigns all the points of the trajectories to one of the k clusters
  for (size_t i = 0; i < gestures.size(); i++) {
    clusters[0].addGesture(gestures[i]);
  }

  // Apply clustering
  int clustering = 1;
  while(clustering){
    clustering = 0;
    for (size_t i = 0; i < clusters.size(); i++) {
      size_t n = clusters[i].mCollection.size();
      for (size_t j = 0; j < n; j++) {
        type_gesture gesture = clusters[i].mCollection[j];
        size_t k = findNearestCluster(clusters, gesture);
        if(k != i && clusters[i].mCollection.size() > 1){
          clusters[i].mCollection.erase(clusters[i].mCollection.begin() + j);
          clusters[k].mCollection.push_back(gesture);
          clustering++; n--;
        }
      }
      std::cout << "C " << i << " N = " << clusters[i].mCollection.size() << '\n';
    }
  }

  return clusters;
}

std::vector<Cluster>
CKMeans::createClusters(std::vector<type_gesture> gestures){
  std::vector<Cluster> clusters;
  for (size_t i = 0; i < gestures.size(); i++) {
    Cluster newCluster;
    newCluster.mBaseCluster = gestures[i];
    newCluster.mBaseCluster.handOne.centroid = MathUtil::calcCentroid(newCluster.mBaseCluster.handOne.positions);
    newCluster.mBaseCluster.handTwo.centroid = MathUtil::calcCentroid(newCluster.mBaseCluster.handTwo.positions);
    clusters.push_back(newCluster);
  }
  return clusters;
}

size_t
CKMeans::findNearestCluster(std::vector<Cluster> clusters, type_gesture gesture){
  XnPoint3D c1, c2;
  double minC1 = 999999999, minC2 = 999999999, distC1 = 0.0, distC2 = 0.0;
  int k = -1;
  for (int i = 0; i < clusters.size(); i++) {
    c1 = MathUtil::calcCentroid(gesture.handOne.positions);
    distC1 = MathUtil::getArcLength(c1, clusters[i].mBaseCluster.handOne.centroid);
    c2 = MathUtil::calcCentroid(gesture.handTwo.positions);
    distC2 = MathUtil::getArcLength(c2, clusters[i].mBaseCluster.handTwo.centroid);
    if(distC1 < minC1 && distC2 < minC2){
      minC1 = distC1;
      minC2 = distC2;
      k = i;
    }
  }
  return k;
}

void
Cluster::addGesture(type_gesture gesture) {
  mCollection.push_back(gesture);
}
