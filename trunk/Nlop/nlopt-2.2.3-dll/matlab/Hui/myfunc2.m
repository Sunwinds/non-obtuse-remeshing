function [val, gradient] = myfunc2(x, ideal)
%Hui Wang, July 10, 2012


val = sum((x - ideal) .^ 2);
if (nargout > 1)
     gradient = 2 * (x - ideal);
end

