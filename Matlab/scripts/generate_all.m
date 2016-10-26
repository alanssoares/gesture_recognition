function generate_all()

disp('-------- Experimental Results --------');
disp('Collection 1: VP + VN + FP + FN');
disp('----------- INIT STEP 1 ------------ ');
Normal = generate_all_vp_vn_fp_fn('result_experiment_normal_*')
Median = generate_all_vp_vn_fp_fn('result_experiment_median_*')
Equal_Size_of_Points = generate_all_vp_vn_fp_fn('result_experiment_equal_*')
disp('----------- END STEP 1 ------------ ');

disp('Collection 2: Confusion Matrix');
disp('----------- INIT STEP 2 ------------ ');
disp('Data - Normal');
generate_all_matrix_confusion('result_experiment_normal_*')
disp('Data - Median');
generate_all_matrix_confusion('result_experiment_median_*')
disp('Data - Equal Size of Points');
generate_all_matrix_confusion('result_experiment_equal_*')
disp('----------- END STEP 2 ------------ ');

disp('Collection 3: Time Processing by Method');
disp('----------- INIT STEP 3 ------------ ');
disp('Data - Normal');
generate_all_time_processing_by_method('result_experiment_normal_*')
disp('Data - Median');
generate_all_time_processing_by_method('result_experiment_median_*')
disp('Data - Equal Size of Points');
generate_all_time_processing_by_method('result_experiment_equal_*')
disp('----------- END STEP 3 ------------ ');

end
