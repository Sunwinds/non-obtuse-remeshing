function P = planeOfTriangle(V, F)
%Compute the plane equation of each triangle of the triangular mesh
%Each plane can be represented as a*x + b*y + c*z + d = 0, then 
%P(i, 1) = [a b c d]

%Hui Wang, July 14, 2012, wanghui19841109@gmail.com
fNormal = facetNormals(V,F);
P(:, 1:3) = fNormal;

point = V(F(:, 1), :);
P(:, 4) = -1.0 * sum(fNormal .* point, 2);