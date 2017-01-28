function generate_all_matrix_confusion(sufix, partfile)

Normal = gesture_matrix_confusion('0', sufix, partfile)
Laplacian = gesture_matrix_confusion('1', sufix, partfile)
Curvature = gesture_matrix_confusion('2', sufix, partfile)
DouglasPeucker = gesture_matrix_confusion('3', sufix, partfile)
Laplacian_Curvature = gesture_matrix_confusion('4', sufix, partfile)
Laplacian_DouglasPeucker = gesture_matrix_confusion('5', sufix, partfile)

end
