function generate_all_matrix_confusion()

Normal = gesture_matrix_confusion('0', '0.5.txt')
Laplacian = gesture_matrix_confusion('1', '0.5.txt')
Curvature = gesture_matrix_confusion('2', '0.5.txt')
DouglasPeucker = gesture_matrix_confusion('3', '0.5.txt')
Laplacian_Curvature = gesture_matrix_confusion('4', '0.5.txt')
Laplacian_DouglasPeucker = gesture_matrix_confusion('5', '0.5.txt')

end
