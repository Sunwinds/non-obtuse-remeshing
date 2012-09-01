function [val, grad] = objfunPoint(x, centroid, idealPoints, s)
%Hui Wang, July 14, 2012

%%The functional value
val = s * sum((x - centroid) .^ 2) + (1 - s) * sum((x - idealPoints) .^ 2);

grad = zeros(3, 1);
%%The gradients
if nargout > 1
    grad1 = 2.0 * (x - centroid);
    grad2 = 2.0 * (x - idealPoints);
    grad = s * grad1  + (1 - s) * grad2;
end