#include "nonlinearSolver.h"


//The minimized function |||x - idealPoints|^2
extern double minimizedfun(unsigned n, const double *x, double *grad, void *idealPoints)
{
	double val;
	std::vector<Point>  *points = (std::vector<Point> *)idealPoints; 
	int i, numberOfPoints = points->size(), index;

	val = 0.0;
	for(i = 0; i < numberOfPoints; i++)
	{
		index = 3 * i;

		val += (x[index] - points->at(i).x) * (x[index] - points->at(i).x);
		val += (x[index + 1] - points->at(i).y) * (x[index + 1] - points->at(i).y);
		val += (x[index + 2] - points->at(i).z) * (x[index + 2] - points->at(i).z);
	}
	if(grad)
	{
		for(i = 0; i < numberOfPoints; i++)
		{
			index = 3 * i;

			grad[index] = 2.0 * (x[index] - points->at(i).x);
			grad[index + 1] = 2.0 * (x[index + 1] - points->at(i).y);
			grad[index + 2] = 2.0 * (x[index + 2] - points->at(i).z);
		}
	} 

	//std::cout << x[0] << " " << x[1] << " " << x[2] << std::endl;
	//std::cout << x[3] << " " << x[4] << " " << x[5] << std::endl;
	//std::cout << x[6] << " " << x[7] << " " << x[8] << std::endl;

	//for(i = 0; i < numberOfPoints; i++)
	//std::cout << points->at(i).x << " " << points->at(i).y << " " << points->at(i).z << std::endl;
	//std::cout << val << std::endl;
	return val;
}

//Compute the gradient of the cos of angle at the vertex (x1, y1, z1) with variable (x1, y1, z1)
extern void gradientIn1(double *grad, int n1, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double sign)
{
	double dot, length1, length2;
	double term1,term2,term3;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));

	term1 = -1.0 * ((x2 - x1) + (x3 - x1)) / (length1 * length2);
	term2 = dot / length2 * (x2 - x1) * pow(length1, -3.0);
	term3 = dot / length1 * (x3 - x1) * pow(length2, -3.0);
	grad[3 * n1] = sign * (term1 + term2 + term3);

	term1 = -1.0 * ((y2 - y1) + (y3 - y1)) / (length1 * length2);
	term2 = dot / length2 * (y2 - y1) * pow(length1, -3.0);
	term3 = dot / length1 * (y3 - y2) * pow(length2, -3.0);
	grad[3 * n1 + 1] = sign * (term1 + term2 + term3);

	term1 = -1.0 * ((z2 - z1) + (z3 - z1)) / (length1 * length2);
	term2 = dot / length2 * (z2 - z1) * pow(length1, -3.0);
	term3 = dot / length1 * (z3 - z1) * pow(length2, -3.0);
	grad[3 * n1 + 2] = sign * (term1 + term2 + term3);

}


//Compute the gradient of the cos of angle at the vertex (x1, y1, z1) with variable (x2, y2, z2)
extern void gradientIn2(double *grad, int n2, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double sign)
{
	double dot, length1, length2;
	double term1,term2;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));

	term1 = (x3 - x1) / (length1 * length2);
	term2 = dot / length2 * (x1 - x2) * pow(length1, -3.0);
	grad[3 * n2] = sign * (term1 + term2);

	term1 = (y3 - y1) / (length1 * length2);
	term2 = dot / length2 * (y1 - y2) * pow(length1, -3.0);
	grad[3 * n2 + 1] = sign * (term1 + term2);

	term1 = (z3 - z1) / (length1 * length2);
	term2 = dot / length2 * (z1 - z2) * pow(length1, -3.0);
	grad[3 * n2 + 2] = sign * (term1 + term2);
}

//Non-obtuse constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2,n3)
extern double nonobtuseConThree_first(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;
	n3 = index->n3;

	//std:: cout << n1 << " " << n2 << " " << n3 << std::endl;
	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	//std::cout << val << std::endl;
	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2,n3)
extern double smallAngConThree_first(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;
	n3 = index->n3;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, 1.0);
	}
	return val;
}


//Non-obtuse constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2,n3)
extern double nonobtuseConThree_second(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n2;
	n2 = index->n3;
	n3 = index->n1;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2,n3)
extern double smallAngConThree_second(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n2;
	n2 = index->n3;
	n3 = index->n1;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, 1.0);
	}
	return val;
}


//Non-obtuse constraints on the angles corresponding the n3-th vertex of triangle of (n1, n2,n3)
extern double nonobtuseConThree_third(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n3;
	n2 = index->n1;
	n3 = index->n2;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n3-th vertex of triangle of (n1, n2,n3)
extern double smallAngConThree_third(unsigned n, const double *x, double *grad, void *threeVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	ThreePointsIndex *index = (ThreePointsIndex *) threeVertexIndex;
	//The three vertices
	n1 = index->n3;
	n2 = index->n1;
	n3 = index->n2;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = x[3 * n3];
	y3 = x[3 * n3 + 1];
	z3 = x[3 * n3 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n3, x1, y1, z1, x3, y3, z3, x2, y2, z2, 1.0);
	}
	return val;
}

//Non-obtuse constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double nonobtuseConTwo_first(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = index->x;
	y3 = index->y;
	z3 = index->z;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
	}
	return val;
}



//Non-obtuse constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double smallAngConTwo_first(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = index->x;
	y3 = index->y;
	z3 = index->z;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
	}
	return val;
}


//Non-obtuse constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double nonobtuseConTwo_second(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n2;
	n2 = index->n1;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = index->x;
	y3 = index->y;
	z3 = index->z;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
	}
	return val;
}



//Non-obtuse constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double smallAngConTwo_second(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n2;
	n2 = index->n1;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = x[3 * n2];
	y2 = x[3 * n2 + 1];
	z2 = x[3 * n2 + 2];
	x3 = index->x;
	y3 = index->y;
	z3 = index->z;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
	}
	return val;
}

//Non-obtuse constraints on the angles corresponding the n3-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double nonobtuseConTwo_third(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, n3, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;

	//The coordinates of the three vertices
	x1 = index->x;
	y1 = index->y;
	z1 = index->z;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = x[3 * n2];
	y3 = x[3 * n2 + 1];
	z3 = x[3 * n2 + 2];


	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
		gradientIn2(grad, n2, x1, y1, z1, x3, y3, z3, x2, y2, z2, -1.0);
	}
	return val;
}



//Non-obtuse constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2, n3), where n3 is known
extern double smallAngConTwo_third(unsigned n, const double *x, double *grad, void *twoVertexIndex)
{
	int n1, n2, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	TwoPointsIndex *index = (TwoPointsIndex *) twoVertexIndex;
	//The three vertices
	n1 = index->n1;
	n2 = index->n2;

	//The coordinates of the three vertices
	x1 = index->x;
	y1 = index->y;
	z1 = index->z;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = x[3 * n2];
	y3 = x[3 * n2 + 1];
	z3 = x[3 * n2 + 2];

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
		gradientIn2(grad, n2, x1, y1, z1, x3, y3, z3, x2, y2, z2, 1.0);
	}
	return val;
}

//Non-obtuse constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double nonobtuseConOne_first(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//std::cout << n1 << std::endl;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	//std::cout << x1 << " " << y1 << " " << z1 << std::endl;

	x2 = index->x1;
	y2 = index->y1;
	z2 = index->z1;
	//std::cout << x2 << " " << y2 << " " << z2 << std::endl;

	x3 = index->x2;
	y3 = index->y2;
	z3 = index->z2;
	//std::cout << x3 << " " << y3 << " " << z3 << std::endl;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	//std::cout << val << std::endl;
	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double smallAngConOne_first(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//The coordinates of the three vertices
	x1 = x[3 * n1];
	y1 = x[3 * n1 + 1];
	z1 = x[3 * n1 + 2];
	x2 = index->x1;
	y2 = index->y1;
	z2 = index->z1;
	x3 = index->x2;
	y3 = index->y2;
	z3 = index->z2;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn1(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
	}
	return val;
}


//Non-obtuse constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double nonobtuseConOne_second(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//The coordinates of the three vertices
	x1 = index->x1;
	y1 = index->y1;
	z1 = index->z1;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = index->x2;
	y3 = index->y2;
	z3 = index->z2;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	//std::cout << val << std::endl;
	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n2-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double smallAngConOne_second(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//The coordinates of the three vertices
	x1 = index->x1;
	y1 = index->y1;
	z1 = index->z1;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = index->x2;
	y3 = index->y2;
	z3 = index->z2;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
	}
	return val;
}

//Non-obtuse constraints on the angles corresponding the n3-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double nonobtuseConOne_third(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//The coordinates of the three vertices
	x1 = index->x2;
	y1 = index->y2;
	z1 = index->z2;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = index->x1;
	y3 = index->y1;
	z3 = index->z1;

	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = -1.0 * dot / (length1 * length2);

	//std::cout << val << std::endl;
	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, -1.0);
	}
	return val;
}


//Small angle constraints on the angles corresponding the n1-th vertex of triangle of (n1, n2,n3), where n2 and n3 are known
extern double smallAngConOne_third(unsigned n, const double *x, double *grad, void *oneVertexIndex)
{
	int n1, i;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3;
	double dot,length1,length2;
	double val;

	OnePointsIndex *index = (OnePointsIndex *) oneVertexIndex;
	//The three vertices
	n1 = index->n;

	//The coordinates of the three vertices
	x1 = index->x2;
	y1 = index->y2;
	z1 = index->z2;
	x2 = x[3 * n1];
	y2 = x[3 * n1 + 1];
	z2 = x[3 * n1 + 2];
	x3 = index->x1;
	y3 = index->y1;
	z3 = index->z1;


	dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	length1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	length2 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1));
	val = dot / (length1 * length2) - cos(M_PI / 6.0);

	if(grad)
	{
		for(i = 0; i < n; i++)
			grad[i] = 0;

		gradientIn2(grad, n1, x1, y1, z1, x2, y2, z2, x3, y3, z3, 1.0);
	}
	return val;
}

extern bool moveAnyStencile(std::vector<Point> initialPoint, std::vector<Point> targetPoint, std::vector<Point> &resultPoint, std::vector<ThreePointsIndex> threePointIndex,
					 std::vector<TwoPointsIndex> twoPointIndex, std::vector<OnePointsIndex> onePointIndex)
{
	//Dimensionality
	int i,n;
	n = 3 * initialPoint.size();

	//lower bounds
	double *lb;
	lb = new double[n];
	for(i = 0; i < n; i++)
		lb[i] = -1e+19;

	//Create the optimization and the lower bound
	nlopt_opt opt;
	//opt = nlopt_create(NLOPT_LD_MMA, n); (Slow)
	//opt = nlopt_create(NLOPT_LD_LBFGS, n); (Cannot work)
	opt = nlopt_create(NLOPT_LD_SLSQP, n);
	//opt = nlopt_create(NLOPT_LN_COBYLA,n); (Cannot work)
	nlopt_set_lower_bounds(opt, lb); 

	//Set the minimized function
	nlopt_set_min_objective(opt, minimizedfun, &targetPoint);

	//Constraints
	for(i = 0; i < threePointIndex.size(); i++)
	{
		nlopt_add_inequality_constraint(opt, nonobtuseConThree_first, &threePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConThree_first, &threePointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConThree_second, &threePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConThree_second, &threePointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConThree_third, &threePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConThree_third, &threePointIndex[i], 1e-8);
	}
	for(i = 0; i < twoPointIndex.size(); i++)
	{
		nlopt_add_inequality_constraint(opt, nonobtuseConTwo_first, &twoPointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConTwo_first, &twoPointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConTwo_second, &twoPointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConTwo_second, &twoPointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConTwo_third, &twoPointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConTwo_third, &twoPointIndex[i], 1e-8);
	}
	for(i = 0; i < onePointIndex.size(); i++)
	{
		nlopt_add_inequality_constraint(opt, nonobtuseConOne_first, &onePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConOne_first, &onePointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConOne_second, &onePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConOne_second, &onePointIndex[i], 1e-8);

		nlopt_add_inequality_constraint(opt, nonobtuseConOne_third, &onePointIndex[i], 1e-8);
		nlopt_add_inequality_constraint(opt, smallAngConOne_third, &onePointIndex[i], 1e-8);
	}

	nlopt_set_xtol_rel(opt, 1e-6);
	nlopt_set_ftol_rel(opt, 1e-6);

	//Initial guess
	double *x;
	x = new double[n]; 
	for(i = 0; i < initialPoint.size(); i++)
	{
		x[3 * i] = initialPoint[i].x;
		x[3 * i + 1] = initialPoint[i].y;
		x[3 * i + 2] = initialPoint[i].z;
	}

	//std::cout << x[0] << " " << x[1] << " " << x[2] << std::endl;
	//std::cout << x[3] << " " << x[4] << " " << x[5] << std::endl;
	//std::cout << x[6] << " " << x[7] << " " << x[8] << std::endl;

	nlopt_set_maxtime(opt, 10);
	double minf; // the minimum objective value, upon return
	bool isSolved;
	//clock_t start, end;
	//start = clock();
	if (nlopt_optimize(opt, x, &minf) < 0) {
		//printf("nlopt failed!\n");
		isSolved = false;
	}
	else 
	{
		//printf("found minimum at f(%g,%g,%g) = %0.10g\n", x[0], x[1], x[2], minf);
		isSolved = true;
		resultPoint.resize(initialPoint.size());
		for(i = 0; i < resultPoint.size(); i++)
		{
			resultPoint[i].x = x[3 * i];
			resultPoint[i].y = x[3 * i + 1];
			resultPoint[i].z = x[3 * i + 2];
		}
	}
	//end = clock();
	//printf("Times: %g\n", (double)(end - start) / CLOCKS_PER_SEC);
	//printf("Minimized function %g\n", minf);

	//Destroy
	nlopt_destroy(opt);
	delete [] lb;
	delete [] x;

	return isSolved;
}


