#!/bin/bash

# dataset="grufba_dtree"
# dataset="utkinect_dtree"
# dataset="msr3d_dtree"

# feature="centroid_derivative"
# feature="centroid_length"
# feature="curvature"
# feature="lc"
# feature="lsc"
# feature="variance"

# features="grufba_16/features/"$feature/grufba_descriptor
# features="msr_action_3d/features/"$feature/msr_action_3d_descriptor
# features="utkinect_action/features/"$feature/utkinect_descriptor

# echo "***** Remove folder result_dtree ****** "
# rm -r $dataset\_$feature
#
# echo "***** Create folder result_dtree ****** "
# mkdir $dataset\_$feature
#
# echo "***** Testing DTree with DecisionTreeClusterNode - 1 ****** "
#
# ./startDecisionTree ../Datasets/$features\_0.grt > $dataset\_$feature/result_dtree_1_0.txt
# ./startDecisionTree ../Datasets/$features\_1.grt > $dataset\_$feature/result_dtree_1_1.txt
# ./startDecisionTree ../Datasets/$features\_2.grt > $dataset\_$feature/result_dtree_1_2.txt
# ./startDecisionTree ../Datasets/$features\_3.grt > $dataset\_$feature/result_dtree_1_3.txt
# ./startDecisionTree ../Datasets/$features\_4.grt > $dataset\_$feature/result_dtree_1_4.txt
# ./startDecisionTree ../Datasets/$features\_5.grt > $dataset\_$feature/result_dtree_1_5.txt

# echo "***** Testing DTree with DecisionTreeThresholdNode - 2 ****** "
# ./startDecisionTree ../Datasets/$features\_0.grt > $dataset/result_dtree_2_0.txt
# ./startDecisionTree ../Datasets/$features\_1.grt > $dataset/result_dtree_2_1.txt
# ./startDecisionTree ../Datasets/$features\_2.grt > $dataset/result_dtree_2_2.txt
# ./startDecisionTree ../Datasets/$features\_3.grt > $dataset/result_dtree_2_3.txt
# ./startDecisionTree ../Datasets/$features\_4.grt > $dataset/result_dtree_2_4.txt
# ./startDecisionTree ../Datasets/$features\_5.grt > $dataset/result_dtree_2_5.txt


# listFeatures="centroid_derivative
# centroid_length
# curvature
# lc
# lsc
# variance"

# listFeatures="lc
# lsc
# vel
# lc_lsc_vel
# lc_vel
# lsc_vel
# lc_lsc"

# listFeatures="
# lc_lsc"

# rm -r $dataset
# mkdir $dataset

# for feature in $listFeatures
# do
  # features="grufba_16/features_lp_dp/grufba_min_lp_dp_"$feature
  # features="msr_action_3d/features_lp_dp/msr_3d_action_min_lp_dp_"$feature
  # features="utkinect_action/features/"$feature/utkinect_descriptor
  # ./startDecisionTree ../Datasets/$features.grt > $dataset/$feature\_result_dtree.txt
  # ./startDTreePCAPipeline ../Datasets/$features.grt > $dataset/$feature\_result_dtree.txt
  # ./startPCAExample ../Datasets/$features.grt > $dataset/$feature\_result_dtree.txt
  # ./startRandomForests ../Datasets/$features.grt > $dataset/$feature\_result_dtree.txt
  # ./startHMM ../Datasets/$features.grt > $dataset/$feature\_result_dtree.txt
# done

feature="msr3d_action_origin_as1_arc_crv_max.grt"

./startDecisionTree ../Datasets/msr_action_3d/group/features/$feature > result_msr3d/as1_dtree.txt
./startRandomForests ../Datasets/msr_action_3d/group/features/$feature > result_msr3d/as1_random_forests.txt
./startDTreePCAPipeline ../Datasets/msr_action_3d/group/features/$feature > result_msr3d/as1_dtree_pca.txt

# ./startDTreePCAPipeline ../Datasets/asl/features/asl_raw_min_lc_lsc.grt > result_asl_min/min_lc_lsc.txt
# ./startDTreePCAPipeline ../GRToolkit/result_min/cv/datasets_msr3d_t7/min_lp_cv_lc_lsc.grt > result_min/cv/datasets_msr3d_t7/result/min_lp_cv_lc_lsc.txt
# ./startDTreePCAPipeline ../GRToolkit/result_min/lp/datasets_msr3d_t8/min_lp_lc_lsc.grt > result_min/lp/datasets_msr3d_t8/result/min_lp_lc_lsc.txt
# ./startDTreePCAPipeline ../GRToolkit/datasets_msr3d_t1/min_lp_cv_lc_lsc.grt > datasets_msr3d_t1/result/min_lp_cv_lc_lsc.txt
# ./startDTreePCAPipeline ../GRToolkit/datasets_msr3d_t1/min_lp_dp_lc_lsc.grt > datasets_msr3d_t1/result/min_lp_dp_lc_lsc.txt
