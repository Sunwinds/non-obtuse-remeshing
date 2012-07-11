function angles = anglesOfFaces(V, F) 

%Compute the three angules for each faces
%Hui Wang, Apir 12, 2012

numOfFaces = size(F, 1);
angles = zeros(numOfFaces, 3);

for i = 1:numOfFaces
   vertex1 = V(F(i,1), :);
   vertex2 = V(F(i,2), :); 
   vertex3 = V(F(i,3), :); 
   
   angles(i,1) = angleComputation(vertex1, vertex2, vertex3);
   angles(i,2) = angleComputation(vertex2, vertex1, vertex3);
   angles(i,3) = angleComputation(vertex3, vertex1, vertex2);
end


%In a triangle(p1, p2, p3), compute the angle at vertex p1
function  a1 = angleComputation(p1, p2, p3)

v1 = p2 - p1;
v2 = p3 - p1;

a1 = acos(dot(v1,v2) / (norm(v1) * norm(v2)));


