#!/bin/bash

echo "***** Remove folder result_dtree ****** "
rm -r result_dtree

echo "***** Create folder result_dtree ****** "
mkdir result_dtree

echo "***** Testing DTree with DecisionTreeClusterNode - 1 ****** "

./startDecisionTree ../Datasets/ufbagre_dataset_0.grt 1 > result_dtree/result_dtree_1_0.txt
./startDecisionTree ../Datasets/ufbagre_dataset_1.grt 1 > result_dtree/result_dtree_1_1.txt
./startDecisionTree ../Datasets/ufbagre_dataset_2.grt 1 > result_dtree/result_dtree_1_2.txt
./startDecisionTree ../Datasets/ufbagre_dataset_3.grt 1 > result_dtree/result_dtree_1_3.txt
./startDecisionTree ../Datasets/ufbagre_dataset_4.grt 1 > result_dtree/result_dtree_1_4.txt
./startDecisionTree ../Datasets/ufbagre_dataset_5.grt 1 > result_dtree/result_dtree_1_5.txt

echo "***** Testing DTree with DecisionTreeThresholdNode - 2 ****** "
./startDecisionTree ../Datasets/ufbagre_dataset_0.grt 2 > result_dtree/result_dtree_2_0.txt
./startDecisionTree ../Datasets/ufbagre_dataset_1.grt 2 > result_dtree/result_dtree_2_1.txt
./startDecisionTree ../Datasets/ufbagre_dataset_2.grt 2 > result_dtree/result_dtree_2_2.txt
./startDecisionTree ../Datasets/ufbagre_dataset_3.grt 2 > result_dtree/result_dtree_2_3.txt
./startDecisionTree ../Datasets/ufbagre_dataset_4.grt 2 > result_dtree/result_dtree_2_4.txt
./startDecisionTree ../Datasets/ufbagre_dataset_5.grt 2 > result_dtree/result_dtree_2_5.txt