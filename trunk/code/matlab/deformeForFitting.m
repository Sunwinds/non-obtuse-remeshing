function V_nonobtuse = deformeForFitting(V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha)
%Deform the non-obtuse mesh to the ideal one for non-obtuse remeshing
%Hui Wang, July 15, 2012, wanghui19841109@gmail.com



%%Global connectivity informations and planes of the ideal mesh
%For finding 1-ring neighbor vertexes of each vertex of the non-obtuse mesh
global adjacentMatrix_obtuse;
adjacentMatrix_obtuse = graphAdjacencyMatrix(F_nonobtuse);
%For finding 1-ring neighbor faces of each vertex of the non-obtuse mesh
global neighborFaceOfVertex_obtuse;
neighborFaceOfVertex_obtuse = neighbourFacesOfVertex(F_nonobtuse, size(V_nonobtuse, 1)); 
%For finding 1-ring neighbor faces of each face of the ideal mesh
global neighborFaceOfFace_ideal;
neighborFaceOfFace_ideal = neighbourFacesOfFace(F_ideal, size(V_ideal, 1)); 
global planes_ideal;
planes_ideal = planeOfTriangle(V_ideal, F_ideal);


%%Initialization 
numberOfVertex_nonobtuse = size(V_nonobtuse);

currentEnergy = zeros(numberOfVertex_nonobtuse, 1);
newEnergy = zeros(numberOfVertex_nonobtuse, 1);
priority = zeros(numberOfVertex_nonobtuse, 1);

newPosition = zeros(numberOfVertex_nonobtuse, 3);
fval = zeros(numberOfVertex_nonobtuse, 1);

t = cputime;
for i = 1:numberOfVertex_nonobtuse
   currentEnergy(i, 1) = energyOfPoint(V_nonobtuse(i, :), i, V_nonobtuse, V_ideal, F_ideal, alpha);
   [newPosition(i, :), fval(i, 1)] = optimalPositionOfPoint(i, V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha);
   newEnergy(i, 1) = energyOfPoint(newPosition(i, :), i, V_nonobtuse, V_ideal, F_ideal, alpha);
   priority(i, 1) = currentEnergy(i, 1) - newEnergy(i, 1);
   
   if(mod(i, 1000) == 0)
      str = strcat('Time cost for', num2str(i), 'vertices:', num2str(cputime - t));
      disp(str);
   end
end


%%Deform to fit
t = cputime;
i = 0;
while(max(priority) > 0.0)
    %Update for the moved vertex
    maximumPriority = max(priority);
    movedIndex = find(priority == maximumPriority);
    V_nonobtuse(movedIndex, :) = newPosition(movedIndex, :);
    optimalEnergy = newEnergy(movedIndex, 1);
    currentEnergy(movedIndex, 1) = optimalEnergy;
    
    %Update for the 1-ring neighbors
    neighborVertexIndex = find(adjacentMatrix_obtuse(movedIndex, :));
    neighborVertexIndex(end + 1) = movedIndex;
    for j = 1:length(neighborVertexIndex)
        neighborIndex = neighborVertexIndex(j);
        newPosition(neighborIndex, :) = optimalPositionOfPoint(neighborIndex, V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha);
        newEnergy(neighborIndex, 1) = energyOfPoint(newPosition(neighborIndex, :), neighborIndex, V_nonobtuse, V_ideal, F_ideal, alpha);
        priority(neighborIndex, 1) = currentEnergy(neighborIndex, 1) - newEnergy(neighborIndex, 1);
    end
    
    i = i + 1;
    if(mod(i, 1000) == 0)
      str1 = strcat('Time cost for', num2str(i), 'vertices:', num2str(cputime - t));
      disp(str1);
      str2 = strcat('Maximum of priority', num2str(maximumPriority));
      disp(str2);
    end
   if ( i > 3 * numberOfVertex_nonobtuse)
     break;
   end
end