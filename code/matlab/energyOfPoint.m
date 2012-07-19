function value = energyOfPoint(point, vIndex, V_nonobtuse, V_ideal, F_ideal, alpha)
%Compute the energy of the vIndex-th vertex in the nonobtuse mesh
%Hui Wang, July 15, 2012, wanghui19841109@gmail.com

global adjacentMatrix_obtuse;
global neighborFaceOfFace_ideal;
global planes_ideal;

%Compute the centroid
 neighborIndexOf1ring = find(adjacentMatrix_obtuse(vIndex, :));
 neighborVertexOf1ring = V_nonobtuse(neighborIndexOf1ring, :);
 centroid = mean(neighborVertexOf1ring);

 %Compute the quardic matrix
 distance = pointToTriangles(point, V_ideal(F_ideal(:, 1), :), V_ideal(F_ideal(:, 2), :), V_ideal(F_ideal(:, 3), :));
 closestTriangle = find(distance == min(distance));
 neighborFacesIndex = neighborFaceOfFace_ideal{closestTriangle, 1};
 planes = planes_ideal(neighborFacesIndex, :);
 Q = planesToQuadrics(planes);

 value = objfun(point, centroid, Q, alpha);