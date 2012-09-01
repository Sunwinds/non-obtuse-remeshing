function newPosition = optimalPositionOfPoint_point(vIndex, centroid, idealPoint, V_nonobtuse, F_nonobtuse, alpha)
%Compute the optimal position of the vIndex-th vertex in the nonobtuse mesh
%Hui Wang, August 12, 2012, wanghui19841109@gmail.com

global neighborFaceOfVertex_obtuse;
 
%Compute the constraints for the neighbors edge
neighborFaceIndex = neighborFaceOfVertex_obtuse{vIndex, 1};
firstPointOfEdge = zeros(length(neighborFaceIndex), 3);
secondPointOfEdge = zeros(length(neighborFaceIndex), 3);
for j = 1:length(neighborFaceIndex)
     edge = setdiff(F_nonobtuse(neighborFaceIndex(j), :), vIndex);
     firstPointOfEdge(j, :) = V_nonobtuse(edge(1), :);
     secondPointOfEdge(j, :) = V_nonobtuse(edge(2), :);
end
%[A, b] = constraintPlanes(firstPointOfEdge, secondPointOfEdge, V_nonobtuse(vIndex, :));

x0 = V_nonobtuse(vIndex, :);     % Make a starting guess at the solution
%options = optimset('Algorithm', 'sqp', 'Display', 'notify-detailed', 'TolFun', 1e-16, 'TolX', 1e-16);
options = optimset('Algorithm', 'sqp', 'Display', 'notify-detailed');
[x, fval, exitflag] = fmincon(@(x)objfunPoint(x, centroid, idealPoint, alpha), x0, [], [], [], [], [], [], @(x)confun(x, firstPointOfEdge, secondPointOfEdge), options);
%[x, fval, exitflag] = fmincon(@(x)objfunPoint(x, centroid, idealPoint, alpha), x0, A, b, [], [], [], [], [], options);
newPosition = x;
if fval >= objfunPoint(x0, centroid, idealPoint, alpha)
  newPosition = x0;
end