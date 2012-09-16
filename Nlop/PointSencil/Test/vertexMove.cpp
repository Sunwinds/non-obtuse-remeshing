#pragma once

//C version
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

typedef struct {
	double x1, y1, z1, x2, y2, z2;
}TwoPoints;


//The minimized function |||x - idealPoints|^2
double myfunc(unsigned n, const double *x, double *grad, void *idealPoints)
{
	double val;
    Point *d = (Point *)idealPoints; 

    val = (x[0] - d->x) * (x[0] - d->x) + (x[1] - d->y) * (x[1] - d->y) + (x[2] - d->z) * (x[2] - d->z);
	
	if (grad) {
		grad[0] = 2.0 * (x[0] - d->x);
		grad[1] = 2.0 * (x[1] - d->y);
		grad[2] = 2.0 * (x[2] - d->z);
	}

	return val;
}


//Non-obtuse angle constraint on the current vertex 
double nonobtuseConCur(unsigned n, const double *x, double *grad, void *points)
{	
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	TwoPoints *d = (TwoPoints *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;
	
	dot = (x1 - x[0]) * (x2 - x[0]) + (y1 - x[1]) * (y2 - x[1]) + (z1 - x[2]) * (z2 - x[2]);
	length1 = sqrt((x1 - x[0]) * (x1 - x[0]) + (y1 - x[1]) * (y1 - x[1]) + (z1 - x[2]) * (z1 - x[2]));
	length2 = sqrt((x2 - x[0]) * (x2 - x[0]) + (y2 - x[1]) * (y2 - x[1]) + (z2 - x[2]) * (z2 - x[2]));
    val = -1.0 * dot / (length1 * length2);

	if(grad)
	{   
		double term1,term2,term3;

		term1 = -1.0 * ((x1 - x[0]) + (x2 - x[0])) / (length1 * length2);
		term2 = dot / length2 * (x1 - x[0]) * pow(length1, -3.0);
		term3 = dot / length1 * (x2 - x[0]) * pow(length2, -3.0);
		grad[0] = -1.0 * (term1 + term2 + term3);

		term1 = -1.0 * ((y1 - x[1]) + (y2 - x[1])) / (length1 * length2);
		term2 = dot / length2 * (y1 - x[1]) * pow(length1, -3.0);
		term3 = dot / length1 * (y2 - x[1]) * pow(length2, -3.0);
		grad[1] = -1.0 * (term1 + term2 + term3);

		term1 = -1.0 * ((z1 - x[2]) + (z2 - x[2])) / (length1 * length2);
		term2 = dot / length2 * (z1 - x[2]) * pow(length1, -3.0);
		term3 = dot / length1 * (z2 - x[2]) * pow(length2, -3.0);
		grad[2] = -1.0 * (term1 + term2 + term3);
	}
	
	return val;
}


//30 degree angle constraint on the current vertex
double smallAngConCur(unsigned n, const double *x, double *grad, void *points)
{
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	TwoPoints *d = (TwoPoints *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x1 - x[0]) * (x2 - x[0]) + (y1 - x[1]) * (y2 - x[1]) + (z1 - x[2]) * (z2 - x[2]);
	length1 = sqrt((x1 - x[0]) * (x1 - x[0]) + (y1 - x[1]) * (y1 - x[1]) + (z1 - x[2]) * (z1 - x[2]));
	length2 = sqrt((x2 - x[0]) * (x2 - x[0]) + (y2 - x[1]) * (y2 - x[1]) + (z2 - x[2]) * (z2 - x[2]));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{   
		double term1,term2,term3;
        
		term1 = -1.0 * ((x1 - x[0]) + (x2 - x[0])) / (length1 * length2);
		term2 = dot / length2 * (x1 - x[0]) * pow(length1, -3.0);
		term3 = dot / length1 * (x2 - x[0]) * pow(length2, -3.0);
		grad[0] = term1 + term2 + term3;
        
		term1 = -1.0 * ((y1 - x[1]) + (y2 - x[1])) / (length1 * length2);
		term2 = dot / length2 * (y1 - x[1]) * pow(length1, -3.0);
		term3 = dot / length1 * (y2 - x[1]) * pow(length2, -3.0);
		grad[1] = term1 + term2 + term3;

		term1 = -1.0 * ((z1 - x[2]) + (z2 - x[2])) / (length1 * length2);
		term2 = dot / length2 * (z1 - x[2]) * pow(length1, -3.0);
		term3 = dot / length1 * (z2 - x[2]) * pow(length2, -3.0);
		grad[2] = term1 + term2 + term3;
	}

	//std::cout << val << std::endl;
	return val;
}

//Non-obtuse angle constraint on the other two vertexes of the triangle  
double nonobtuseConOther(unsigned n, const double *x, double *grad, void *points)
{	
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	TwoPoints *d = (TwoPoints *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x[0] - x1) * (x2 - x1) + (x[1] - y1) * (y2 - y1) + (x[2] - z1) * (z2 - z1);
	length1 = sqrt((x[0] - x1) * (x[0] - x1) + (x[1] - y1) * (x[1] - y1) + (x[2] - z1) * (x[2] - z1));
	length2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
    val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		double term1,term2;

		term1 = (x2 - x1) / (length1 * length2);
		term2 = dot / length2 * (x1 - x[0]) * pow(length1, -3.0);
		grad[0] = -1.0 * (term1 + term2);

		term1 = (y2 - y1) / (length1 * length2);
		term2 = dot / length2 * (y1 - x[1]) * pow(length1, -3.0);
		grad[1] = -1.0 * (term1 + term2);

		term1 = (z2 - z1) / (length1 * length2);
		term2 = dot / length2 * (z1 - x[2]) * pow(length1, -3.0);
		grad[2] = -1.0 * (term1 + term2);
	}
    
	//std::cout << val << std::endl;
	return val;
	
}


////30 degree angle constraint on the other two vertexes of the triangle
double smallAngConOther(unsigned n, const double *x, double *grad, void *points)
{
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	TwoPoints *d = (TwoPoints *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x[0] - x1) * (x2 - x1) + (x[1] - y1) * (y2 - y1) + (x[2] - z1) * (z2 - z1);
	length1 = sqrt((x[0] - x1) * (x[0] - x1) + (x[1] - y1) * (x[1] - y1) + (x[2] - z1) * (x[2] - z1));
	length2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		double term1,term2;

		term1 = (x2 - x1) / (length1 * length2);
		term2 = dot / length2 * (x1 - x[0]) * pow(length1, -3.0);
		grad[0] = term1 + term2;

		term1 = (y2 - y1) / (length1 * length2);
		term2 = dot / length2 * (y1 - x[1]) * pow(length1, -3.0);
		grad[1] = term1 + term2;

		term1 = (z2 - z1) / (length1 * length2);
		term2 = dot / length2 * (z1 - x[2]) * pow(length1, -3.0);
		grad[2] = term1 + term2;
	}
   
	return val;
}

//Move vertex from "initialPoint" to "targetPoint" with non-obtuse constraints
bool movePerVertex(Point initialPoint, Point targetPoint, std::vector<Point> firstEdgePoint, std::vector<Point> secondEdgePoint, Point &resultPoint)
{
	double lb[3] = {-1e+19, -1e+19, -1e+19}; // lower bounds 
	nlopt_opt opt;

	//opt = nlopt_create(NLOPT_LD_MMA, 3); // algorithm and dimensionality 
	//opt = nlopt_create(NLOPT_LD_SLSQP, 3); //faster
	opt = nlopt_create( NLOPT_LN_COBYLA, 3);

	nlopt_set_lower_bounds(opt, lb); 

    
	//Minimized function
	Point data;
	data.x = targetPoint.x;
	data.y = targetPoint.y;
	data.z = targetPoint.z;
	nlopt_set_min_objective(opt, myfunc, &data);

	
	//Constraints
	int i, numberOfConstraints;
    numberOfConstraints = firstEdgePoint.size();
	
	/*
	TwoPoints *data11,*data12;
	data11 = new TwoPoints[numberOfConstraints];
    data12 = new TwoPoints[numberOfConstraints];
    */
	std::vector<TwoPoints> data11,data12;
    data11.resize(numberOfConstraints);
    data12.resize(numberOfConstraints);

	for(i = 0; i < numberOfConstraints; i++)
	{
		data11[i].x1 = data12[i].x2 = firstEdgePoint[i].x;
		data11[i].y1 = data12[i].y2 = firstEdgePoint[i].y;
		data11[i].z1 = data12[i].z2 = firstEdgePoint[i].z;
		data11[i].x2 = data12[i].x1 = secondEdgePoint[i].x;
		data11[i].y2 = data12[i].y1 = secondEdgePoint[i].y;
		data11[i].z2 = data12[i].z1 = secondEdgePoint[i].z;

		nlopt_add_inequality_constraint(opt, nonobtuseConCur, &data11[i], 1e-12);
		nlopt_add_inequality_constraint(opt, smallAngConCur, &data11[i], 1e-12);

		nlopt_add_inequality_constraint(opt, nonobtuseConOther, &data11[i], 1e-12);
		nlopt_add_inequality_constraint(opt, smallAngConOther, &data11[i], 1e-12);

		nlopt_add_inequality_constraint(opt, nonobtuseConOther, &data12[i], 1e-12);
		nlopt_add_inequality_constraint(opt, smallAngConOther, &data12[i], 1e-12);

	}
	nlopt_set_xtol_rel(opt, 1e-8);
	nlopt_set_ftol_rel(opt, 1e-8);

	//Initial guess
	double x[3] = {0.0, 0.0, 0.0}; 
    x[0] = initialPoint.x;
	x[1] = initialPoint.y;
	x[2] = initialPoint.z;

	double minf; // the minimum objective value, upon return
	bool isSolved;
	clock_t start, end;
	start = clock();
	if (nlopt_optimize(opt, x, &minf) < 0) {
		//printf("nlopt failed!\n");
		isSolved = false;
	}
	else 
	{
	   //printf("found minimum at f(%g,%g,%g) = %0.10g\n", x[0], x[1], x[2], minf);
		isSolved = true;
		resultPoint.x = x[0];
		resultPoint.y = x[1];
		resultPoint.z = x[2];
	}
	end = clock();
	printf("Times: %g", (double)(end - start) / CLOCKS_PER_SEC);
    
	//std::cout << nonobtuseConCur(3,x,NULL,&data11[3]) << std::endl;
	//std::cout << nonobtuseConOther(3,x,NULL,&data11[3]) << std::endl;
    //std::cout << nonobtuseConOther(3,x,NULL,&data12[3]) << std::endl;

	//Destroy
	nlopt_destroy(opt);
	//delete [] data11;
	//delete [] data12;
	data11.clear();
	data12.clear();

	return isSolved;
}

int main()
{
	Point startPoint, targetPoint, resultPoint;

	std::vector<Point> firstEdgePoint(5), secondEdgePoint(5);

	std::cout << "Input the start point:" << std::endl;
	std::cin >> startPoint.x >> startPoint.y >> startPoint.z;

	std::cout << "Input the target point:" << std::endl;
	std::cin >> targetPoint.x >> targetPoint.y >> targetPoint.z;

	std::cout << "Input the constrained two vertices of 1-ring triangles:" << std::endl;
	for(int i = 0; i < 5; i++)
	{
		std::cin >> firstEdgePoint[i].x >> firstEdgePoint[i].y >> firstEdgePoint[i].z;
		std::cin >> secondEdgePoint[i].x >> secondEdgePoint[i].y >> secondEdgePoint[i].z;
	}

	if(movePerVertex(startPoint,targetPoint, firstEdgePoint, secondEdgePoint, resultPoint))
		std::cout << "Result:" << resultPoint.x << "  " << resultPoint.y << "  " << resultPoint.z << std::endl; 

	return 0;
}


/*
Test data 1:(The result is same as the function funmincon of MATLAB)
Input the start point:
0.2086    0.0420   -0.1632
Input the target point:
0.2086    0.0273   -0.1606
Input the constrained two vertices of 1-ring triangles:
0.1951    0.0285   -0.1632    0.2221    0.0285   -0.1632
0.2221    0.0285   -0.1632    0.2221    0.0420   -0.1497
0.2221    0.0420   -0.1497    0.2086    0.0555   -0.1497
0.1951    0.0555   -0.1632    0.2086    0.0555   -0.1497
0.1951    0.0285   -0.1632    0.1951    0.0555   -0.1632
Result:0.2086  0.0285  -0.1497

Test data 2: (The result of MATLAB is (0.0010   -0.3485   -0.3781))
Input the start point:
-0.0074   -0.3360   -0.3792
Input the target point:
0.0027   -0.3452   -0.3776
Input the constrained two vertices of 1-ring triangles:
-0.0209   -0.3360   -0.3927    0.0061   -0.3360   -0.3927
-0.0209   -0.3360   -0.3927   -0.0209   -0.3495   -0.3792
-0.0209   -0.3495   -0.3792   -0.0074   -0.3495   -0.3657
-0.0074   -0.3495   -0.3657    0.0061   -0.3360   -0.3657
0.0061   -0.3360   -0.3927    0.0061   -0.3360   -0.3657
Result:0.000996218  -0.348454  -0.378154
*/
