function [newPosition, fval] = optimalPositionOfPoint_linear(vIndex, V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha)
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
 Q1 = centroidToQuadric(centroid);

 %Compute the quardic matrix
 distance = pointToTriangles(V_nonobtuse(vIndex, :), V_ideal(F_ideal(:, 1), :), V_ideal(F_ideal(:, 2), :), V_ideal(F_ideal(:, 3), :));
 closestTriangle = find(distance == min(distance));
 neighborFacesIndex = neighborFaceOfFace_ideal{closestTriangle, 1};
 planes = planes_ideal(neighborFacesIndex, :);
 Q2 = planesToQuadrics(planes);
 
%The minimized energy
Q = alpha * Q1 + (1.0 - alpha) * Q2;
H = 2 * Q(1:3, 1:3);
f = 2.0 * Q(4, 1:3); 
 
%Compute the constraints for the neighbors edge
neighborFaceIndex = neighborFaceOfVertex_obtuse{vIndex, 1};
firstPointOfEdge = zeros(length(neighborFaceIndex), 3);
secondPointOfEdge = zeros(length(neighborFaceIndex), 3);
for j = 1:length(neighborFaceIndex)
     edge = setdiff(F_nonobtuse(neighborFaceIndex(j), :), vIndex);
     firstPointOfEdge(j, :) = V_nonobtuse(edge(1), :);
     secondPointOfEdge(j, :) = V_nonobtuse(edge(2), :);
end
[A, b] = constraintPlanes(firstPointOfEdge, secondPointOfEdge, V_nonobtuse(vIndex, :));

%Solve
x0 = V_nonobtuse(vIndex, :);     % Make a starting guess at the solution
options = optimset('Algorithm', 'active-set', 'Display', 'notify-detailed');
[x, fval, exitflag] = quadprog(H, f, A, b, [], [], [], [], x0, options);
fval = 0.5 * x' * H * x + dot(f, x) + Q(4, 4);

% x0 = V_nonobtuse(vIndex, :);     % Make a starting guess at the solution
% options = optimset('Algorithm', 'active-set', 'Display', 'notify-detailed');
% [x, fval, exitflag] = fmincon(@(x)objfun(x, centroid, Q, alpha), x0, A, b, [], [], [], [], [], options);
% con = A * x' - b;
% if(max(con) > 0)
%    max(con)
% end

% if(exitflag < 0)
%    A * x0' - b
%    x = x0;
% end

newPosition = x;