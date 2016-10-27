function generate_all_time_processing_by_method(partfile)

G1 = gesture_time_processing_by_method('g1', partfile);
G2 = gesture_time_processing_by_method('g2', partfile);
G3 = gesture_time_processing_by_method('g3', partfile);
G4 = gesture_time_processing_by_method('g4', partfile);
G5 = gesture_time_processing_by_method('g5', partfile);
G6 = gesture_time_processing_by_method('g6', partfile);
G7 = gesture_time_processing_by_method('g7', partfile);

figure;

  hold on
  x1 = [ G1(1,1), G2(1,1), G3(1,1), G4(1,1), G5(1,1), G6(1,1), G7(1,1) ];
  y1 = [ G1(1,2), G2(1,2), G3(1,2), G4(1,2), G5(1,2), G6(1,2), G7(1,2) ];
  stem(x1, y1);
  x2 = [ G1(2,1), G2(2,1), G3(2,1), G4(2,1), G5(2,1), G6(2,1), G7(2,1) ];
  y2 = [ G1(2,2), G2(2,2), G3(2,2), G4(2,2), G5(2,2), G6(2,2), G7(2,2) ];
  stem(x2, y2);
  x3 = [ G1(3,1), G2(3,1), G3(3,1), G4(3,1), G5(3,1), G6(3,1), G7(3,1) ];
  y3 = [ G1(3,2), G2(3,2), G3(3,2), G4(3,2), G5(3,2), G6(3,2), G7(3,2) ];
  stem(x3, y3);
  x4 = [ G1(4,1), G2(4,1), G3(4,1), G4(4,1), G5(4,1), G6(4,1), G7(4,1) ];
  y4 = [ G1(4,2), G2(4,2), G3(4,2), G4(4,2), G5(4,2), G6(4,2), G7(4,2) ];
  stem(x4, y4);
  x5 = [ G1(5,1), G2(5,1), G3(5,1), G4(5,1), G5(5,1), G6(5,1), G7(5,1) ];
  y5 = [ G1(5,2), G2(5,2), G3(5,2), G4(5,2), G5(5,2), G6(5,2), G7(5,2) ];
  stem(x5, y5);
  x6 = [ G1(6,1), G2(6,1), G3(6,1), G4(6,1), G5(6,1), G6(6,1), G7(6,1) ];
  y6 = [ G1(6,2), G2(6,2), G3(6,2), G4(6,2), G5(6,2), G6(6,2), G7(6,2) ];
  stem(x6, y6);
  hold off

  xlabel('Average Number of Points');
  ylabel('Average Time Processing - Milliseconds');
  legend('Normal', 'Laplacian', 'Curvature', 'DouglasPeucker', 'Laplacian + Curvature', 'Laplacian + DouglasPeucker');
end
