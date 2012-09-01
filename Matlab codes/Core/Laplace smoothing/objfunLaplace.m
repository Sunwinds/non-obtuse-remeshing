function [val, grad] = objfunLaplace(x, centroid)
%Hui Wang, July 14, 2012

%%The functional value
% centroid = x + 0.000000025 * (centroid - x);
val = sum((x - centroid) .^ 2);

grad = zeros(3, 1);
%%The gradients
if nargout > 1
    grad = 2.0 * (x - centroid);
end