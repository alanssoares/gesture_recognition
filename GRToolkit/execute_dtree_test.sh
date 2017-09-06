#!/bin/bash

dataset="grufba_dtree"
# dataset="utkinect_dtree"
# dataset="msr3d_dtree"

# feature="centroid_derivative"
feature="centroid_length"
# feature="curvature"
# feature="lc"
# feature="lsc"
# feature="variance"

features="grufba_16/features/"$feature/grufba_descriptor
# features="msr_action_3d/features/"$feature/msr_action_3d_descriptor
# features="utkinect_action/features/"$feature/utkinect_descriptor

echo "***** Remove folder result_dtree ****** "
rm -r $dataset

echo "***** Create folder result_dtree ****** "
mkdir $dataset

echo "***** Testing DTree with DecisionTreeClusterNode - 1 ****** "

./startDecisionTree ../Datasets/$features\_0.grt 1 > $dataset/result_dtree_1_0.txt
./startDecisionTree ../Datasets/$features\_1.grt 1 > $dataset/result_dtree_1_1.txt
./startDecisionTree ../Datasets/$features\_2.grt 1 > $dataset/result_dtree_1_2.txt
./startDecisionTree ../Datasets/$features\_3.grt 1 > $dataset/result_dtree_1_3.txt
./startDecisionTree ../Datasets/$features\_4.grt 1 > $dataset/result_dtree_1_4.txt
./startDecisionTree ../Datasets/$features\_5.grt 1 > $dataset/result_dtree_1_5.txt

echo "***** Testing DTree with DecisionTreeThresholdNode - 2 ****** "
./startDecisionTree ../Datasets/$features\_0.grt 2 > $dataset/result_dtree_2_0.txt
./startDecisionTree ../Datasets/$features\_1.grt 2 > $dataset/result_dtree_2_1.txt
./startDecisionTree ../Datasets/$features\_2.grt 2 > $dataset/result_dtree_2_2.txt
./startDecisionTree ../Datasets/$features\_3.grt 2 > $dataset/result_dtree_2_3.txt
./startDecisionTree ../Datasets/$features\_4.grt 2 > $dataset/result_dtree_2_4.txt
./startDecisionTree ../Datasets/$features\_5.grt 2 > $dataset/result_dtree_2_5.txt
