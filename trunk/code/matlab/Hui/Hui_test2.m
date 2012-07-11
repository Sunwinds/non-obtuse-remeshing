%Hui Wang, July 11, 2012
clear;
clc;

[V, F] = mRead;
V0 = V;

nv = size(V, 1);
W = graphAdjacencyMatrix(F);
for i = 1:10
   %Add minimization energy
   neighVertex = V(find(W(i, :)), :);
   ideal = mean(neighVertex); 
   opt.algorithm = NLOPT_LN_COBYLA;
   opt.min_objective = @(x)myfunc2(x, ideal);
   
   %Add constraints
   [row, col] = find(F == i);
   neighFace = row;
   k = 1;
   for j = 1:length(neighFace)
      edge = setdiff(F(neighFace(j), :), i);
      p1 = V(edge(1), :);
      p2 = V(edge(2), :);
      
      %Sphere constraints
      fc{1, k} = (@(x)myconstraint21(x, (p1 + p2) / 2, 0.5 * norm(p1 - p2)));
      k = k + 1;
      
      %Plane constraints
      fc{1, k} = (@(x)myconstraint22(x, p1, p2 - p1));
      k = k + 1;
      fc{1, k} = (@(x)myconstraint22(x, p2, p1 - p2));
      k = k + 1;
   end
   opt.fc = fc;
%    opt.verbose = 1;
   opt.maxtime = 50;
   
   %Solve
   opt.xtol_rel = 1e-4;
   [xopt, fmin, retcode] = nlopt_optimize(opt, V(i, :));
   retcode
   
   %Update
   V(i, :) = xopt;
end
