function [val, grad] = objfun(x, centroid, quadricMatrix, s)
%Hui Wang, July 14, 2012

%%The functional value
v = x;
v(4) = 1.0;
val = s * sum((x - centroid) .^ 2) + (1 - s) * v * quadricMatrix * v';

grad = zeros(3, 1);
%%The gradients
if nargout > 1
    grad1 = 2.0 * (x - centroid);
    gradAll = 2.0 * quadricMatrix * v';
    grad2 = gradAll(1:3)';
    grad = s * grad1  + (1 - s) * grad2;
end