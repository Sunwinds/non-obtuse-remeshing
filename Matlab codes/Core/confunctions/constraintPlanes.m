function [A, b] = constraintPlanes(firstPointOfEdge, secondPointOfEdge, point) 
%Hui Wang, July 17, 2012


numberOfEges = size(firstPointOfEdge, 1);
A = zeros(3 * numberOfEges, 3);
b = zeros(3 * numberOfEges, 1);

for j = 1:numberOfEges;
  p1 = firstPointOfEdge(j, :);
  p2 = secondPointOfEdge(j, :);

  index = 3 * (j - 1) + 1;
  %The first plane constraints
  point1 = p1;
  normal1 = p1 - p2;
  A(index, :) = normal1;
  b(index, 1) = dot(point1, normal1);
 
  
  %The second plane constraints
  point2 = p2;
  normal2 = p2 - p1;
  A(index + 1, :) = normal2;
  b(index + 1, 1) = dot(point2, normal2);
  
  %The third plane constraints
  midPoint = (point1 + point2) / 2;
  direction = point - midPoint;
  uniformDirection = direction / norm(direction);
  radius = 0.5 * norm(p2 - p1);
  
  point3 = midPoint + radius * uniformDirection;
  normal3 = -1.0 * direction;
  A(index + 2, :) = normal3;
  b(index + 2, 1) = dot(point3, normal3);
end