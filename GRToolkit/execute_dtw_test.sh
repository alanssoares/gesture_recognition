#!/bin/bash

result_grufba="grufba_dtw"
result_utkinect="utkinect_dtw"
result_msr3d="msr3d_dtw"

dataset_grufba="grufba_16/grufba_grt_series.grt"
dataset_utkinect="utkinect_action/utkinect_series.grt"
dataset_msr3d="msr_action_3d/msr_action_3d_series.grt"

echo "***** Remove folders ****** "
rm -r $result_grufba
rm -r $result_utkinect
rm -r $result_msr3d

echo "***** Create folders ****** "
mkdir $result_grufba
mkdir $result_utkinect
mkdir $result_msr3d

echo "***** Testing DTW ****** "

./startDTW ../Datasets/$dataset_grufba > $result_grufba/result_dtw.txt
./startDTW ../Datasets/$dataset_utkinect > $result_utkinect/result_dtw.txt
./startDTW ../Datasets/$dataset_msr3d > $result_msr3d/result_dtw.txt
