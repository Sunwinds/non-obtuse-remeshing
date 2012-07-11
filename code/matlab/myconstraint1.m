function [val, gradient] = myconstraint1(x, F, i, j)
%Hui Wang, July 5, 2012

index = setdiff([1, 2, 3], j);
v1 = F(i, j);
v2 = F(i, index(1));
v3 = F(i, index(2));

numOfVertex = length(x) / 3;
v1x = v1;
v1y = numOfVertex + v1;
v1z = 2 * numOfVertex + v1;

v2x = v2;
v2y = numOfVertex + v2;
v2z = 2 * numOfVertex + v2;

v3x = v3;
v3y = numOfVertex + v3;
v3z = 2 * numOfVertex + v3;

val = (x(v2x) - x(v1x)) * (x(v3x) - x(v1x)) + (x(v2y) - x(v1y)) * (x(v3y) - x(v1y)) + (x(v2z) - x(v1z)) * (x(v3z) - x(v1z));
val = -val;

if (nargout > 1)
   V = reshape(x, numOfVertex, 3);
   L = 2 * graphLaplacian(F);
   D = L * V;
   gradient = reshape(D, 1, 3 * numOfVertex);
end