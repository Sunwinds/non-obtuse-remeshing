function [newPosition, fval] = optimalPositionOfPoint_Laplace(vIndex, V_nonobtuse, F_nonobtuse)
%Hui Wang, August 17, 2012

global adjacentMatrix_obtuse;
global neighborFaceOfVertex_obtuse;

%Compute the centroid
neighborIndexOf1ring = find(adjacentMatrix_obtuse(vIndex, :));
neighborVertexOf1ring = V_nonobtuse(neighborIndexOf1ring, :);
centroid = mean(neighborVertexOf1ring);
 
%Compute the constraints for the neighbors edge
neighborFaceIndex = neighborFaceOfVertex_obtuse{vIndex, 1};
firstPointOfEdge = zeros(length(neighborFaceIndex), 3);
secondPointOfEdge = zeros(length(neighborFaceIndex), 3);
for j = 1:length(neighborFaceIndex)
     edge = setdiff(F_nonobtuse(neighborFaceIndex(j), :), vIndex);
     firstPointOfEdge(j, :) = V_nonobtuse(edge(1), :);
     secondPointOfEdge(j, :) = V_nonobtuse(edge(2), :);
end
% [A, b] = constraintPlanes(firstPointOfEdge, secondPointOfEdge, V_nonobtuse(vIndex, :));

%Solve
x0 = V_nonobtuse(vIndex, :);     % Make a starting guess at the solution
% options = optimset('Algorithm', 'active-set', 'Display', 'notify-detailed', 'TolFun', 1e-16, 'TolX', 1e-16);
options = optimset('Algorithm', 'sqp', 'Display', 'notify-detailed');
[x, fval, exitflag] = fmincon(@(x)objfunLaplace(x, centroid), x0, [], [], [], [], [], [], @(x)confun(x, firstPointOfEdge, secondPointOfEdge), options);
% [x, fval, exitflag] = fmincon(@(x)objfunLaplace(x, centroid), x0, A, b, [], [], [], [], [], options);

newPosition = x;