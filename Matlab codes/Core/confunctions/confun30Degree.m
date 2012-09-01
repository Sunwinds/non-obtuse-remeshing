function [c, ceq, gradc, gradceq] = confun30Degree(x, firstPointOfEdge, secondPointOfEdge) 
%Hui Wang, July 14

 
%%Inequality constraints 
k = 1;
for j = 1:size(firstPointOfEdge, 1);
  p1 = firstPointOfEdge(j, :);
  p2 = secondPointOfEdge(j, :);
  
  %Smaller than 90 degree
  c(k) = -1.0 * dot(p1 - x, p2 - x);
  k = k + 1;
      
  c(k) = -1.0 * dot(x - p1, p2 - p1);
  k = k + 1;
  
  c(k) = -1.0 * dot(x - p2, p1 - p2);
  k = k + 1;
  
  %Larger than 30 degree  
  c(k) = dot(p1 - x, p2 - x) / (norm(p1 - x) * norm(p2 - x)) - cos(30 / 180 * pi);
  k = k + 1;
  
  c(k) = dot(p1 - p2, x - p2) / (norm(p1 - p2) * norm(x - p2)) - cos(30 / 180 * pi);
  k = k + 1;
  
  c(k) = dot(p2 - p1, x - p1) / (norm(p2 - p1) * norm(x - p1)) - cos(30 / 180 * pi);
  k = k + 1;
end

%%Equality constraints
ceq = [];