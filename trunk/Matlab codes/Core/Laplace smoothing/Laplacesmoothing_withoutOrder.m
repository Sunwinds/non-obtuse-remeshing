function V_nonobtuse = Laplacesmoothing_withoutOrder(V_nonobtuse, F_nonobtuse)
%Constraint Laplace smoothing
%Hui Wang, August 12, 2012, wanghui19841109@gmail.com

%%Global connectivity informations and planes of the ideal mesh
%For finding 1-ring neighbor vertexes of each vertex of the non-obtuse mesh
global adjacentMatrix_obtuse;
adjacentMatrix_obtuse = graphAdjacencyMatrix(F_nonobtuse);
%For finding 1-ring neighbor faces of each vertex of the non-obtuse mesh
global neighborFaceOfVertex_obtuse;
neighborFaceOfVertex_obtuse = neighbourFacesOfVertex(F_nonobtuse, size(V_nonobtuse, 1)); 


%%Initialization 
numberOfVertex_nonobtuse = size(V_nonobtuse, 1);

t = cputime;
for i = 1:numberOfVertex_nonobtuse
   V_nonobtuse(i, :) = optimalPositionOfPoint_Laplace(i, V_nonobtuse, F_nonobtuse);
   
   if(mod(i, 100) == 0)
      str = strcat('Time cost for', num2str(i), 'vertices:', num2str(cputime - t));
      disp(str);
   end
end