function [val, gradient] = myconstraint22(x, point, normal)
%Hui Wang, July 10, 2012

val = dot(point - x, normal);

if (nargout > 1)
  gradient = -1.0 * normal;
end