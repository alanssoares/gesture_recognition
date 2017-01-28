%http://www.dataschool.io/simple-guide-to-confusion-matrix-terminology/
function G = generate_all_vp_vn_fp_fn(sufix, partfile)

G = zeros(7,6);

Normal = custom_confusion('0', sufix, partfile);
Laplacian = custom_confusion('1', sufix, partfile);
Curvature = custom_confusion('2', sufix, partfile);
DouglasPeucker = custom_confusion('3', sufix, partfile);
Laplacian_Curvature = custom_confusion('4', sufix, partfile);
Laplacian_DouglasPeucker = custom_confusion('5', sufix, partfile);

G(1,1) = accuracy(Normal);
G(1,2) = accuracy(Laplacian);
G(1,3) = accuracy(Curvature);
G(1,4) = accuracy(DouglasPeucker);
G(1,5) = accuracy(Laplacian_Curvature);
G(1,6) = accuracy(Laplacian_DouglasPeucker);

G(2,1) = missclassification_rate(Normal);
G(2,2) = missclassification_rate(Laplacian);
G(2,3) = missclassification_rate(Curvature);
G(2,4) = missclassification_rate(DouglasPeucker);
G(2,5) = missclassification_rate(Laplacian_Curvature);
G(2,6) = missclassification_rate(Laplacian_DouglasPeucker);

G(3,1) = true_positive_rate(Normal);
G(3,2) = true_positive_rate(Laplacian);
G(3,3) = true_positive_rate(Curvature);
G(3,4) = true_positive_rate(DouglasPeucker);
G(3,5) = true_positive_rate(Laplacian_Curvature);
G(3,6) = true_positive_rate(Laplacian_DouglasPeucker);

G(4,1) = false_positive_rate(Normal);
G(4,2) = false_positive_rate(Laplacian);
G(4,3) = false_positive_rate(Curvature);
G(4,4) = false_positive_rate(DouglasPeucker);
G(4,5) = false_positive_rate(Laplacian_Curvature);
G(4,6) = false_positive_rate(Laplacian_DouglasPeucker);

G(5,1) = specificity(Normal);
G(5,2) = specificity(Laplacian);
G(5,3) = specificity(Curvature);
G(5,4) = specificity(DouglasPeucker);
G(5,5) = specificity(Laplacian_Curvature);
G(5,6) = specificity(Laplacian_DouglasPeucker);

G(6,1) = precision(Normal);
G(6,2) = precision(Laplacian);
G(6,3) = precision(Curvature);
G(6,4) = precision(DouglasPeucker);
G(6,5) = precision(Laplacian_Curvature);
G(6,6) = precision(Laplacian_DouglasPeucker);

G(7,1) = prevalence(Normal);
G(7,2) = prevalence(Laplacian);
G(7,3) = prevalence(Curvature);
G(7,4) = prevalence(DouglasPeucker);
G(7,5) = prevalence(Laplacian_Curvature);
G(7,6) = prevalence(Laplacian_DouglasPeucker);

end

function v = accuracy(m)
  v = (sum_vp(m) + sum_vn(m) + 1) / (sum_all(m) + 1) - 1;
end

function v = missclassification_rate(m)
  v = (sum_fp(m) + sum_fn(m) + 1) / (sum_all(m) + 1) - 1;
end

function v = true_positive_rate(m)
  v = (sum_vp(m) + 1) / (sum_vp(m) + sum_fn(m) + 1) - 1;
end

function v = false_positive_rate(m)
  v = (sum_fp(m) + 1) / (sum_vn(m) + sum_fp(m) + 1) - 1;
end

function v = specificity(m)
  v = (sum_vn(m) + 1) / (sum_vn(m) + sum_fp(m) + 1) - 1;
end

function v = precision(m)
  v = (sum_vp(m) + 1) / (sum_vp(m) + sum_fp(m) + 1) - 1;
end

function v = prevalence(m)
  v = (sum_vp(m) + sum_fn(m) + 1) / (sum_all(m) + 1) - 1;
end

function tot = sum_vp(m)
  tot = m(1,1) + m(1,2) + m(1,3) + m(1,4) + m(1,5) + m(1,6) + m(1,7);
end

function tot = sum_vn(m)
  tot = m(2,1) + m(2,2) + m(2,3) + m(2,4) + m(2,5) + m(2,6) + m(2,7);
end

function tot = sum_fp(m)
  tot = m(3,1) + m(3,2) + m(3,3) + m(3,4) + m(3,5) + m(3,6) + m(3,7);
end

function tot = sum_fn(m)
  tot = m(4,1) + m(4,2) + m(4,3) + m(4,4) + m(4,5) + m(4,6) + m(4,7);
end

function tot = sum_all(m)
  tot = 0;
  for r = 1:4
    for c = 1:7
      tot = tot + m(r,c);
    end
  end
end
