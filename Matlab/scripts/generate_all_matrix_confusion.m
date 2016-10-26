function generate_all_matrix_confusion(partfile)

Normal = gesture_matrix_confusion('0', '0.5.txt', partfile)
Laplacian = gesture_matrix_confusion('1', '0.5.txt', partfile)
Curvature = gesture_matrix_confusion('2', '0.5.txt', partfile)
DouglasPeucker = gesture_matrix_confusion('3', '0.5.txt', partfile)
Laplacian_Curvature = gesture_matrix_confusion('4', '0.5.txt', partfile)
Laplacian_DouglasPeucker = gesture_matrix_confusion('5', '0.5.txt', partfile)

end
