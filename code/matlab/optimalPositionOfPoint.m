function [newPosition, fval] = optimalPositionOfPoint(vIndex, V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha)
%Compute the optimal position of the vIndex-th vertex in the nonobtuse mesh
%Hui Wang, July 15, 2012, wanghui19841109@gmail.com

global adjacentMatrix_obtuse;
global neighborFaceOfVertex_obtuse;
global neighborFaceOfFace_ideal;
global planes_ideal;

%Compute the centroid
 neighborIndexOf1ring = find(adjacentMatrix_obtuse(vIndex, :));
 neighborVertexOf1ring = V_nonobtuse(neighborIndexOf1ring, :);
 centroid = mean(neighborVertexOf1ring);

 %Compute the quardic matrix
 distance = pointToTriangles(V_nonobtuse(vIndex, :), V_ideal(F_ideal(:, 1), :), V_ideal(F_ideal(:, 2), :), V_ideal(F_ideal(:, 3), :));
 closestTriangle = find(distance == min(distance));
 neighborFacesIndex = neighborFaceOfFace_ideal{closestTriangle, 1};
 planes = planes_ideal(neighborFacesIndex, :);
 Q = planesToQuadrics(planes);

%Compute the constraints for the neighbors edge
neighborFaceIndex = neighborFaceOfVertex_obtuse{vIndex, 1};
firstPointOfEdge = zeros(length(neighborFaceIndex), 3);
secondPointOfEdge = zeros(length(neighborFaceIndex), 3);
for j = 1:length(neighborFaceIndex)
     edge = setdiff(F_nonobtuse(neighborFaceIndex(j), :), vIndex);
     firstPointOfEdge(j, :) = V_nonobtuse(edge(1), :);
     secondPointOfEdge(j, :) = V_nonobtuse(edge(2), :);
end

x0 = V_nonobtuse(vIndex, :);     % Make a starting guess at the solution
options = optimset('Algorithm', 'sqp', 'Display', 'notify-detailed', 'GradObj','on');
[x, fval, exitflag] = fmincon(@(x)objfun(x, centroid, Q, alpha), x0,[],[],[],[],[],[], @(x)confun(x, firstPointOfEdge, secondPointOfEdge), options);

newPosition = x;