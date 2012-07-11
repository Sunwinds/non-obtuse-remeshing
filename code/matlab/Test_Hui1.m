function VV = Test_Hui1(V, F)
%Hui

numOfVertex = size(V, 1);

opt.algorithm = NLOPT_LN_COBYLA;
opt.min_objective = @(x)myfunc1(x, F);
% opt.lower_bounds = -inf * ones(1, 3 * numOfVertex);
% opt.upper_bounds = inf * ones(1, 3 * numOfVertex);

% opt.fc = {(@(x)myconstraint1(x, F, 1, 1)), (@(x)myconstraint1(x, F, 1, 2)) };
k = 1;
for i = 1 : size(F, 1)
   for j = 1 : 3
     fc{1, k} = (@(x)myconstraint1(x, F, i, j));
     k = k + 1;
   end
end
opt.fc = fc;
opt.xtol_rel = 1e-4;
% opt.maxtime = 1000;

% 
% opt.fc_tol = 1e-8 * ones(1, 3 * numOfVertex);
x = reshape(V, 1, 3 * numOfVertex);

[xopt, fmin, retcode] = nlopt_optimize(opt, x);
VV = reshape(xopt, numOfVertex, 3);


