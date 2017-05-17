#!/bin/bash

echo "***** Remove folder result_svm ****** "
rm -r result_svm

echo "***** Create folder result_svm ****** "
mkdir result_svm

echo "***** Testing SVM with LINEAR_KERNEL ****** "

./startSVM ../Datasets/ufbagre_dataset_0.grt 1 > result_svm/result_svm_1_0.txt
./startSVM ../Datasets/ufbagre_dataset_1.grt 1 > result_svm/result_svm_1_1.txt
./startSVM ../Datasets/ufbagre_dataset_2.grt 1 > result_svm/result_svm_1_2.txt
./startSVM ../Datasets/ufbagre_dataset_3.grt 1 > result_svm/result_svm_1_3.txt
./startSVM ../Datasets/ufbagre_dataset_4.grt 1 > result_svm/result_svm_1_4.txt
./startSVM ../Datasets/ufbagre_dataset_5.grt 1 > result_svm/result_svm_1_5.txt

echo "***** Testing SVM with POLY_KERNEL ****** "
./startSVM ../Datasets/ufbagre_dataset_0.grt 2 > result_svm/result_svm_2_0.txt
./startSVM ../Datasets/ufbagre_dataset_1.grt 2 > result_svm/result_svm_2_1.txt
./startSVM ../Datasets/ufbagre_dataset_2.grt 2 > result_svm/result_svm_2_2.txt
./startSVM ../Datasets/ufbagre_dataset_3.grt 2 > result_svm/result_svm_2_3.txt
./startSVM ../Datasets/ufbagre_dataset_4.grt 2 > result_svm/result_svm_2_4.txt
./startSVM ../Datasets/ufbagre_dataset_5.grt 2 > result_svm/result_svm_2_5.txt

echo "***** Testing SVM with RBF_KERNEL ****** "
./startSVM ../Datasets/ufbagre_dataset_0.grt 3 > result_svm/result_svm_3_0.txt
./startSVM ../Datasets/ufbagre_dataset_1.grt 3 > result_svm/result_svm_3_1.txt
./startSVM ../Datasets/ufbagre_dataset_2.grt 3 > result_svm/result_svm_3_2.txt
./startSVM ../Datasets/ufbagre_dataset_3.grt 3 > result_svm/result_svm_3_3.txt
./startSVM ../Datasets/ufbagre_dataset_4.grt 3 > result_svm/result_svm_3_4.txt
./startSVM ../Datasets/ufbagre_dataset_5.grt 3 > result_svm/result_svm_3_5.txt

echo "***** Testing SVM with SIGMOID_KERNEL ****** "
./startSVM ../Datasets/ufbagre_dataset_0.grt 4 > result_svm/result_svm_4_0.txt
./startSVM ../Datasets/ufbagre_dataset_1.grt 4 > result_svm/result_svm_4_1.txt
./startSVM ../Datasets/ufbagre_dataset_2.grt 4 > result_svm/result_svm_4_2.txt
./startSVM ../Datasets/ufbagre_dataset_3.grt 4 > result_svm/result_svm_4_3.txt
./startSVM ../Datasets/ufbagre_dataset_4.grt 4 > result_svm/result_svm_4_4.txt
./startSVM ../Datasets/ufbagre_dataset_5.grt 4 > result_svm/result_svm_4_5.txt

echo "***** Testing SVM with PRECOMPUTED_KERNEL ****** "
./startSVM ../Datasets/ufbagre_dataset_0.grt 5 > result_svm/result_svm_5_0.txt
./startSVM ../Datasets/ufbagre_dataset_1.grt 5 > result_svm/result_svm_5_1.txt
./startSVM ../Datasets/ufbagre_dataset_2.grt 5 > result_svm/result_svm_5_2.txt
./startSVM ../Datasets/ufbagre_dataset_3.grt 5 > result_svm/result_svm_5_3.txt
./startSVM ../Datasets/ufbagre_dataset_4.grt 5 > result_svm/result_svm_5_4.txt
./startSVM ../Datasets/ufbagre_dataset_5.grt 5 > result_svm/result_svm_5_5.txt