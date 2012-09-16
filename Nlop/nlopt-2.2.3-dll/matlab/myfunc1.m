function [val, gradient] = myfunc1(x, F)
%Hui Wang, July 5, 2012

n = length(x);
V = reshape(x, n / 3, 3);

L = tutteGraphLaplacian(F);
% L = LaplaceBeltrami(V,F) + tutteGraphLaplacian(F);
D = L * V;
val = norm(D);
val = val^2;


if (nargout > 1)
     gradient = 2 * L^2 * V;
     gradient = reshape(gradient, 1, n);
end

