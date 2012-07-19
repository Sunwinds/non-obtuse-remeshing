function Q = planesToQuadrics(plane)
%Hui Wang, July 14, 2012

Q = zeros(4, 4);
for i = 1:size(plane, 1)
   a = plane(i, 1);
   b = plane(i, 2);
   c = plane(i, 3);
   d = plane(i, 4);
   
   Q = Q + [a*a a*b a*c a*d; a*b b*b b*c b*d; a*c b*c c*c c*d; a*d b*d c*d d*d];
end

Q = Q / size(plane, 1);