function [c, ceq, gradc, gradceq] = confun(x, firstPointOfEdge, secondPointOfEdge) 
%Hui Wang, July 14


%%Inequality constraints 
k = 1;
for j = 1:size(firstPointOfEdge, 1);
  p1 = firstPointOfEdge(j, :);
  p2 = secondPointOfEdge(j, :);
      
  %Two plane constraints
  point1 = p1;
  normal1 = p2 - p1;
  c(k) = dot(point1 - x, normal1);
  k = k + 1;
      
  point2 = p2;
  normal2 = p1 - p2;
  c(k) = dot(point2 - x, normal2);
  k = k + 1;
  
  %Sphere constraints
  center = (p1 + p2) / 2;
  radius = 0.5 * norm(p1 - p2);
  c(k) = radius ^ 2 - sum((x - center) .^ 2);
  k = k + 1;
end

%%Equality constraints
ceq = [];


%%Gradients
if nargout > 2
  %The gradient of the inquality constraints
  k = 1;
  for j = 1:size(firstPointOfEdge, 1);
    p1 = firstPointOfEdge(j, :);
    p2 = secondPointOfEdge(j, :);
      
    %Constraints of two plane constraints
    gradc(k, :) = p1 - p2;
    k = k + 1;
     
    gradc(k, :) = p2 - p1;
    k = k + 1;
  
    %Sphere constraints
    center = (p1 + p2) / 2;
    gradc(k, :) = -2.0 * (x - center);
    k = k + 1;
 end
  
    
  %The gradient of equality constraints
  gradceq = [];
end
