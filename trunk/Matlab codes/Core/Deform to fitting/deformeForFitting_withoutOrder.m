function V_nonobtuse = deformeForFitting_withoutOrder(V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha)
%Deform the non-obtuse mesh to the ideal one for non-obtuse remeshing
%Hui Wang, August 12, 2012, wanghui19841109@gmail.com



%%Global connectivity informations and planes of the ideal mesh
global neighborFaceOfVertex_obtuse;
neighborFaceOfVertex_obtuse = neighbourFacesOfVertex(F_nonobtuse, size(V_nonobtuse, 1));

%For finding 1-ring neighbor vertexes of each vertex of the non-obtuse mesh
adjacentMatrix_obtuse = graphAdjacencyMatrix(F_nonobtuse);

%The correspondence by normal direction 
% %Intersection points from normal direction
% vNormal_nonobtuse = vertexNormals(V_nonobtuse, F_nonobtuse);
% M = meanMatrix(F_nonobtuse, 1);
% vNormal_nonobtuse = M * vNormal_nonobtuse;
% fNormal_ideal = facetNormals(V_ideal, F_ideal);
% [intersectedTriangleIndex, intersectedPoints] = linesToTriangles_fast(V_nonobtuse, vNormal_nonobtuse, V_ideal(F_ideal(:, 1), :), V_ideal(F_ideal(:, 2), :), V_ideal(F_ideal(:, 3), :));
% %Get the reliable correspondences
% index1 = find(intersectedTriangleIndex);
% normalsDifference = sum(vNormal_nonobtuse(index1, :) .* fNormal_ideal(intersectedTriangleIndex(index1), :), 2);
% index2 = find(normalsDifference < 0);
% reliableIndex = index1(index2);
% 
% L = tutteGraphLaplacian(F_nonobtuse);
% reliableCorrespondence = ConLinSys_elimination(L, 0 * intersectedPoints, reliableIndex, intersectedPoints(reliableIndex, :));

[index, dis, reliableCorrespondence] = pointsToTriangles_fast(V_nonobtuse, V_ideal(F_ideal(:, 1), :), V_ideal(F_ideal(:, 2), :), V_ideal(F_ideal(:, 3), :));
 

%%Initialization 
numberOfVertex_nonobtuse = size(V_nonobtuse, 1);

t = cputime;
% d = sum((reliableCorrespondence - V_nonobtuse).^2, 2);
% [dd, index] = sort(d, 'descend');
% for j = 1:length(index)
%    i = index(j);
for i = 1:numberOfVertex_nonobtuse
   neighborIndexOf1ring = find(adjacentMatrix_obtuse(i, :));
   neighborVertexOf1ring = V_nonobtuse(neighborIndexOf1ring, :);
   centroid = mean(neighborVertexOf1ring);
   V_nonobtuse(i, :) = optimalPositionOfPoint_point(i, centroid, reliableCorrespondence(i, :), V_nonobtuse, F_nonobtuse, alpha);
   
   if(mod(i, 100) == 0)
      str = strcat('Time cost for', num2str(i), 'vertices:', num2str(cputime - t));
      disp(str);
   end
end

