function [val, gradient] = myconstraint21(x, center, radius)
%Hui Wang, July 10, 2012


val = radius ^ 2 - sum((x - center) .^ 2);
if (nargout > 1)
  gradient = -2.0 * (x - center);
end