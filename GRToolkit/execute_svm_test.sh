#!/bin/bash

dataset="grufba_svm"
# dataset="utkinect_svm"
# dataset="msr3d_svm"

# feature="centroid_derivative"
feature="centroid_length"
# feature="curvature"
# feature="lc"
# feature="lsc"
# feature="variance"

features="grufba_16/features/"$feature/grufba_descriptor
# features="msr_action_3d/features/"$feature/msr_action_3d_descriptor
# features="utkinect_action/features/"$feature/utkinect_descriptor

echo "***** Remove folder result_svm ****** "
rm -r $dataset

echo "***** Create folder result_svm ****** "
mkdir $dataset

echo "***** Testing SVM with LINEAR_KERNEL ****** "
./startSVM ../Datasets/$features\_0.grt 1 > $dataset/result_svm_1_0.txt
./startSVM ../Datasets/$features\_1.grt 1 > $dataset/result_svm_1_1.txt
./startSVM ../Datasets/$features\_2.grt 1 > $dataset/result_svm_1_2.txt
./startSVM ../Datasets/$features\_3.grt 1 > $dataset/result_svm_1_3.txt
./startSVM ../Datasets/$features\_4.grt 1 > $dataset/result_svm_1_4.txt
./startSVM ../Datasets/$features\_5.grt 1 > $dataset/result_svm_1_5.txt

echo "***** Testing SVM with POLY_KERNEL ****** "
./startSVM ../Datasets/$features\_0.grt 2 > $dataset/result_svm_2_0.txt
./startSVM ../Datasets/$features\_1.grt 2 > $dataset/result_svm_2_1.txt
./startSVM ../Datasets/$features\_2.grt 2 > $dataset/result_svm_2_2.txt
./startSVM ../Datasets/$features\_3.grt 2 > $dataset/result_svm_2_3.txt
./startSVM ../Datasets/$features\_4.grt 2 > $dataset/result_svm_2_4.txt
./startSVM ../Datasets/$features\_5.grt 2 > $dataset/result_svm_2_5.txt

echo "***** Testing SVM with RBF_KERNEL ****** "
./startSVM ../Datasets/$features\_0.grt 3 > $dataset/result_svm_3_0.txt
./startSVM ../Datasets/$features\_1.grt 3 > $dataset/result_svm_3_1.txt
./startSVM ../Datasets/$features\_2.grt 3 > $dataset/result_svm_3_2.txt
./startSVM ../Datasets/$features\_3.grt 3 > $dataset/result_svm_3_3.txt
./startSVM ../Datasets/$features\_4.grt 3 > $dataset/result_svm_3_4.txt
./startSVM ../Datasets/$features\_5.grt 3 > $dataset/result_svm_3_5.txt

echo "***** Testing SVM with SIGMOID_KERNEL ****** "
./startSVM ../Datasets/$features\_0.grt 4 > $dataset/result_svm_4_0.txt
./startSVM ../Datasets/$features\_1.grt 4 > $dataset/result_svm_4_1.txt
./startSVM ../Datasets/$features\_2.grt 4 > $dataset/result_svm_4_2.txt
./startSVM ../Datasets/$features\_3.grt 4 > $dataset/result_svm_4_3.txt
./startSVM ../Datasets/$features\_4.grt 4 > $dataset/result_svm_4_4.txt
./startSVM ../Datasets/$features\_5.grt 4 > $dataset/result_svm_4_5.txt
