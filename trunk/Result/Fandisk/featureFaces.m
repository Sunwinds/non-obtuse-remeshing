function index = featureFaces(V1, F1, V2, F2)
%Hui Wang, August 31, 2012

[in, dis, reliableCorrespondence] = pointsToTriangles_fast(V1, V2(F2(:, 1), :), V2(F2(:, 2), :), V2(F2(:, 3), :));

% Vector = reliableCorrespondence - V1;
% d = sum(Vector .^2, 2);
% dd = 1.0 / (sqrt(d) + eps);
% 
% nVertex = size(Vector, 1);
% Normal = sparse(1:nVertex, 1:nVertex, dd) * Vector;

fNormal = facetNormals(V2, F2);
Normal = fNormal(in, :);

nFace = size(F1, 1);
index = zeros(nFace, 1);
thre = 0.6;
for i = 1:nFace
   n1 = Normal(F1(i, 1), :);
   n2 = Normal(F1(i, 2), :);
   n3 = Normal(F1(i, 3), :);
   
   if(dot(n1, n2) < thre)
      index(i) = 1;
   end
   if(dot(n1, n3) < thre)
      index(i) = 1;
   end
   if(dot(n2, n3) < thre)
      index(i) = 1;
   end
end

