#ifndef NON_LINEAR_SOLVER_H
#define NON_LINEAR_SOLVER_H

#include <math.h>
#include <stdio.h>
#include <nlopt.h>
#include <time.h>
#include <iostream>
#include <vector>

#define M_PI 3.141592653589793

typedef struct {
	double x, y, z;
}Point;

typedef struct{
	int n1, n2, n3;
}ThreePointsIndex;

typedef struct{
	int n1, n2; 
	double x, y, z;
}TwoPointsIndex;

typedef struct{
	int n; 
	double x1, y1, z1, x2, y2, z2;
}OnePointsIndex;


extern double minimizedfun(unsigned n, const double *x, double *grad, void *idealPoints);


extern void gradientIn1(double *grad, int n1, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double sign);
extern void gradientIn2(double *grad, int n2, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double sign);


extern double nonobtuseConThree_first(unsigned n, const double *x, double *grad, void *threeVertexIndex);
extern double smallAngConThree_first(unsigned n, const double *x, double *grad, void *threeVertexIndex);

extern double nonobtuseConThree_second(unsigned n, const double *x, double *grad, void *threeVertexIndex);
extern double smallAngConThree_second(unsigned n, const double *x, double *grad, void *threeVertexIndex);

extern double nonobtuseConThree_third(unsigned n, const double *x, double *grad, void *threeVertexIndex);
extern double smallAngConThree_third(unsigned n, const double *x, double *grad, void *threeVertexIndex);


extern double nonobtuseConTwo_first(unsigned n, const double *x, double *grad, void *twoVertexIndex);
extern double smallAngConTwo_first(unsigned n, const double *x, double *grad, void *twoVertexIndex);

extern double nonobtuseConTwo_second(unsigned n, const double *x, double *grad, void *twoVertexIndex);
extern double smallAngConTwo_second(unsigned n, const double *x, double *grad, void *twoVertexIndex);

extern double nonobtuseConTwo_third(unsigned n, const double *x, double *grad, void *twoVertexIndex);
extern double smallAngConTwo_third(unsigned n, const double *x, double *grad, void *twoVertexIndex);


extern double nonobtuseConOne_first(unsigned n, const double *x, double *grad, void *oneVertexIndex);
extern double smallAngConOne_first(unsigned n, const double *x, double *grad, void *oneVertexIndex);

extern double nonobtuseConOne_second(unsigned n, const double *x, double *grad, void *oneVertexIndex);
extern double smallAngConOne_second(unsigned n, const double *x, double *grad, void *oneVertexIndex);

extern double nonobtuseConOne_third(unsigned n, const double *x, double *grad, void *oneVertexIndex);
extern double smallAngConOne_third(unsigned n, const double *x, double *grad, void *oneVertexIndex);



extern bool moveAnyStencile(std::vector<Point> initialPoint, std::vector<Point> targetPoint, std::vector<Point> &resultPoint, std::vector<ThreePointsIndex> threePointIndex,
							std::vector<TwoPointsIndex> twoPointIndex, std::vector<OnePointsIndex> onePointIndex);

#endif