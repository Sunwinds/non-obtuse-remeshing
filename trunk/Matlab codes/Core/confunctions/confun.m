function [c, ceq, gradc, gradceq] = confun(x, firstPointOfEdge, secondPointOfEdge) 
%Hui Wang, July 14


%%Inequality constraints 
%%Inequality constraints 
k = 1;
for j = 1:size(firstPointOfEdge, 1);
  point1 = firstPointOfEdge(j, :);
  point2 = secondPointOfEdge(j, :);
      
  c(k) = -1.0 * dot(point1 - x, point2 - x);
  k = k + 1;
      
  c(k) = -1.0 * dot(x - point1, point2 - point1);
  k = k + 1;
  
  c(k) = -1.0 * dot(x - point2, point1 - point2);
  k = k + 1;
end

%%Equality constraints
ceq = [];
