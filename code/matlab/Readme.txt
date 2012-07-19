The main function is deformeForFitting.m
V_nonobtuse = deformeForFitting(V_nonobtuse, F_nonobtuse, V_ideal, F_ideal, alpha)
alpha: the weight of the "deform to fit" energy term and smoothness term

Some key functions
energyOfPoint.m: compute the energy of the "vIndex-th" vertex with the position of "point"
optimalPositionOfPoint.m: compute the optimal position of "cIndex" vertex with the non-linear constraints
optimalPositionOfPoint_linear.m: compute the optimal position of "cIndex" vertex with the linear constraints

Other functions
objfun.m: the object function of each vertex
confun.m: the non-linear constranit function 

This program is very slow...