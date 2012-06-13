/***************************************************************************
 *   Copyright (C) 2005 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/

#ifndef HELPER_CPP
#define HELPER_CPP

#include "helper.h"

// compare two double values with tolerence eps
bool equal(double a, double b, double eps)
{
	return (fabs(a - b) <= eps);
}

// Cross Product c = a x b
void crossProduct(double* c, double* a, double* b)
{
  c[0] = a[1]*b[2] - b[1]*a[2];
  c[1] = -a[0]*b[2] + b[0]*a[2];
  c[2] = a[0]*b[1] - b[0]*a[1];
}

// Cross Product c = a x b
// cx (out): x coord of vector c
// cy (out): y coord of vector c
// cz (out): z coord of vector c
// ax (in): x coord of vector a
// ay (in): y coord of vector a
// az (in): z coord of vector a
// bx (in): x coord of vector b
// by (in): y coord of vector b
// bz (in): z coord of vector b
void crossProduct(double &cx, double &cy, double &cz,
                  double ax, double ay, double az,
                  double bx, double by, double bz)
{
	cx = ay*bz - by*az;
	cy = -ax*bz + bx*az;
	cz = ax*by - bx*ay;
}

// Vector Subtraction c = a - b
void vectorSub(double* c, double* a, double* b)
{
  for (int i = 0; i < 3; i++)
    c[i] = a[i] - b[i];
}

// Vector Subtraction c = a - b
void vectorSub(double* c, double ax, double ay, double az,
                          double bx, double by, double bz)
{
  c[0] = ax - bx;
  c[1] = ay - by;
  c[2] = az - bz;
}

// Vector dot product
double dotProduct(double* a, double* b)
{
  return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

double dotProduct(double ax, double ay, double az, double bx, double by, double bz)
{
	return(ax*bx + ay*by + az*bz);
}

double dotProduct(double* a, double bx, double by, double bz)
{
	return(a[0]*bx + a[1]*by + a[2]*bz);
}

// vector length
double length(double* a)
{
  return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

double length(double ax, double ay, double az)
{
  return sqrt(ax*ax + ay*ay + az*az);
}

void computeFaceNormal(double pt1x, double pt1y, double pt1z, double pt2x, double pt2y, double pt2z, double pt3x, double pt3y, double pt3z, double* normal)
{
  double edge1[3];
  double edge2[3];

  vectorSub(edge1, pt2x, pt2y, pt2z, pt1x, pt1y, pt1z);
  vectorSub(edge2, pt3x, pt3y, pt3z, pt1x, pt1y, pt1z);

  crossProduct(normal, edge1, edge2);
  normalize(normal);
}

// angle between two vectors in radian
double getAngle(double* a, double* b)
{
  return acos(dotProduct(a, b) / (length(a) * length(b)));
}

// angle between two vectors in radian
double getAngle(double* anglePt, double* edgeEndPt1, double* edgeEndPt2)
{
  double* edge1 = new double[3];
  vectorSub(edge1, edgeEndPt1, anglePt);
  double* edge2 = new double[3];
  vectorSub(edge2, edgeEndPt2, anglePt);
  
  double angle = getAngle(edge1, edge2);
  
  delete[] edge1;
  delete[] edge2;
  
  return angle;
}

// angle between two vectors in radian
double getAngle(double anglePtx, double anglePty, double anglePtz,
		double edgeEndPt1x, double edgeEndPt1y, double edgeEndPt1z, 
		double edgeEndPt2x, double edgeEndPt2y, double edgeEndPt2z)
{
  double* edge1 = new double[3];
  vectorSub(edge1, edgeEndPt1x, edgeEndPt1y, edgeEndPt1z, anglePtx, anglePty, anglePtz);
  double* edge2 = new double[3];
  vectorSub(edge2, edgeEndPt2x, edgeEndPt2y, edgeEndPt2z, anglePtx, anglePty, anglePtz);
  
  double angle = getAngle(edge1, edge2);
  
  delete[] edge1;
  delete[] edge2;
  
  return angle;
}

// normalize a vector
void normalize(double* a)
{
  double len = length(a);
  if (len != 0.0)
  {
    a[0] /= len;
    a[1] /= len;
    a[2] /= len;
  }
}

void normalize(double &ax, double &ay, double &az)
{
	double len = length(ax, ay, az);
	if (len != 0.0)
	{
		ax /= len;
		ay /= len;
		az /= len;
	}
}

void midpoint(double* c, double ax, double ay, double az, double bx, double by, double bz)
{
	c[0] = (ax + bx)/2;
	c[1] = (ay + by)/2;
	c[2] = (az + bz)/2;
}

void midpoint(float* c, float ax, float ay, float az, float bx, float by, float bz)
{
	c[0] = (ax + bx)/2;
	c[1] = (ay + by)/2;
	c[2] = (az + bz)/2;
}

// linear interpolate between a and b
void linearInterp(float percentage, double* c, double ax, double ay, double az,
                                               double bx, double by, double bz)
{
  if (percentage > 1)
    return;
  
  c[0] = ax*(1-percentage) + bx*percentage;
  c[1] = ay*(1-percentage) + by*percentage;
  c[2] = az*(1-percentage) + bz*percentage;
}

// area of a triangle
double areaTriangle(double* a, double* b, double* c)
{
  double* edge1 = new double[3];
  vectorSub(edge1, a, b);
  double* edge2 = new double[3];
  vectorSub(edge2, a, c);
  double* xProd = new double[3];
  crossProduct(xProd, edge1, edge2);

  double area = 0.5 * length(xProd);

  delete[] edge1;
  delete[] edge2;
  delete[] xProd;

  return area;
}

// point-plane distance
double pointPlaneDistance(double* pt, double* ptOnPlane, double* normal)
{
  normalize(normal);

  // Use ax+by+cz+d = 0 to find d
  double d = -1.0 * dotProduct(normal, ptOnPlane);

  // Distance = |a*x0 + b*y0 + c*z0 + d|
  double distance = dotProduct(normal, pt) + d;

  if (distance < 0.0)   // not sure why fabs() doesn't work
    distance *= -1.0;

  return distance;
}

// point-line distance
double pointLineDistance(double* pt, double* endPt1, double* endPt2)
{
  // Distance = |(endPt2 - endPt1) x (endPt1 - pt)|  /  |endPt2 - endPt1|

  double* line = new double[3];
  double* endPt1MinusPt = new double[3];
  vectorSub(line, endPt2, endPt1);
  vectorSub(endPt1MinusPt, endPt1, pt);

  double* xprod = new double[3];
  crossProduct(xprod, line, endPt1MinusPt);

  delete[] line;
  delete[] endPt1MinusPt;
  delete[] xprod;

  if (length(line) == 0)  // something wrong with the end pts
    return -1;
  
  return length(xprod) / length(line);
}

// point-point distance
double pointPointDistance(double pt1x, double pt1y, double pt1z,
                          double pt2x, double pt2y, double pt2z)
{
  double diffx(pt2x - pt1x);
  double diffy(pt2y - pt1y);
  double diffz(pt2z - pt1z);
  return sqrt(diffx*diffx + diffy*diffy + diffz*diffz);
}

// set 'matrix' with dimention 'dim' as an identity matrix
void setIdentityMatrix(double** matrix, int dim)
{
  for (int i = 0; i < dim; i++)
  {
    for (int j = 0; j < dim; j++)
    {
      matrix[i][j] = 0.0;
      if (i == j)
        matrix[i][j] = 1.0;
    }
  }
}

// factorize matrix A with dimension n using Cholesky Factorization
void cholesky(double* A, int dim, double* L)
{
  // sanity checks
  if (A == 0 || dim <= 0 || L == 0)
    return;

  int currentIdx(0);
  // step 1: set L11 = sqrt(A11);
  L[currentIdx++] = sqrt(A[0]);
  
  // step 2: define 1st column of L
  for (int i = 2; i <= dim; i++)
    L[currentIdx++] = A[i-1] / L[0];

  // step 3 - 5 (See Numerical Analysis)
  double sumL(0.0);
  int index1, index2;
  for (int i = 2; i <= dim-1; i++)
  {
    // entry ii
    sumL = 0.0;
    for (int k = 1; k <= i-1; k++)
    {
      index1 = (k-1)*dim + i - (k*(k-1))/2 - 1;
      sumL += L[index1]*L[index1];
    }
    L[currentIdx++] = sqrt(A[(i-1)*dim+(i-1)] - sumL);

    // entry ji
    for (int j = i+1; j <= dim; j++)
    {
      sumL = 0.0;
      for (int k = 1; k <= i-1; k++)
      {
        index1 = (k-1)*dim + i - (k*(k-1))/2 - 1;
        index2 = (k-1)*dim + j - (k*(k-1))/2 - 1;
        sumL += L[index1]*L[index2];
      }
      index1 = (i-1)*dim - ((i-2)*(i-1))/2;
      L[currentIdx++] = (A[(i-1)*dim+(j-1)] - sumL) / L[index1];
    }
  }  

  // step 6: set Lnn
  sumL = 0.0;
  for (int k = 1; k <= dim-1; k++)
  {
    index1 = (k-1)*dim + dim - (k*(k-1))/2 - 1;
    sumL += L[index1]*L[index1];
  }
  L[currentIdx] = sqrt(A[dim*dim-1] - sumL);
}

// back-substitution on lower triangular matrix
// B will be replaced with X
void L_backSub(double* L, int dim, double* B, int numColInB)
{
  double* sum = new double[numColInB];
  int index;
  for (int i = 1; i <= dim ; i++)
  {
	 for (int j = 0; j < numColInB; ++j)
    	sum[j] = 0.0;

    for (int j = 1; j < i; j++)
    {         
      index = (j-1)*dim + i - (j*(j-1))/2 - 1;
		for (int k = 0; k < numColInB; ++k)
			sum[k] += B[(k*dim)+(j-1)] * L[index];
    }
    index = (i-1)*dim - ((i-2)*(i-1))/2;
	 for (int k = 0; k < numColInB; ++k)
	 	B[k*dim+(i-1)] = (B[k*dim+(i-1)] - sum[k]) / L[index];
  }
  delete[] sum;
}

// back-substitution on lower triangular matrix transpose
// B will be replaced with X
void LT_backSub(double* L, int dim, double* B, int numColInB)
{
  double* sum = new double[numColInB];
  int index;
  for (int i = dim; i >= 1; i--)
  {
    for (int j = 0; j < numColInB; ++j)
    	sum[j] = 0.0;

    for (int j = dim; j > i; j--)
    {
      index = (i-1)*dim + j - (i*(i-1))/2 - 1;
		for (int k = 0; k < numColInB; ++k)
			sum[k] += B[(k*dim)+(j-1)] * L[index];
    }
    index = (i-1)*dim - ((i-2)*(i-1))/2;
	 for (int k = 0; k < numColInB; ++k)
	 	B[k*dim+(i-1)] = (B[(k*dim)+(i-1)] - sum[k]) / L[index];
  }
  delete[] sum;
}

// computes the intersection between a line and a triangle
// return value (out):  0  - no intersection
//			1  - intersection is inside the triangle (ray hits frontside)
//			-1 - intersection is inside the triangle (ray hits backside)
//			2  - intersection is at a vertex (ray hits frontside)
//			-2 - intersection is at a vertex (ray hits backside)
//			3  - intersection is on an edge (ray hits frontside)
//			-3 - intersection is on an edge (ray hits backside)
//			4 - intersection contains an edge (coplanar)
//			5 - intersection through 2 edges (coplanar)
//			6 - intersection through one vertex and an edge (coplanar)
//			7 - intersection is at a vertex (coplanar)
int line_triangle_intersection(double *intPt, double *pt1, double *pt2, 
				double *triPt1, double *triPt2, double *triPt3,
				double *triNormal)
{
	// sanity checks
	if (pt1[0] == pt2[0] && pt1[1] == pt2[1] && pt1[2] == pt2[2])
		return 0;
	if (triPt1[0] == triPt2[0] && triPt1[1] == triPt2[1] && triPt1[2] == triPt2[2])
		return 0;
	if (triPt1[0] == triPt3[0] && triPt1[1] == triPt3[1] && triPt1[2] == triPt3[2])
		return 0;
	if (triPt3[0] == triPt2[0] && triPt3[1] == triPt2[1] && triPt3[2] == triPt2[2])
		return 0;

	// check if a normal is specified, if not, then calculate it
	double triNorm[3] = {0,0,0};
	if (triNormal == 0)
	{
		// edge 1 of triangle
		double edge1[3] = {0,0,0};
		vectorSub(edge1, triPt2, triPt1);
		
		// edge 2 of triangle
		double edge2[3] = {0,0,0};
		vectorSub(edge2, triPt3, triPt1);
				
		// vector normal of triangle
		crossProduct(triNorm, edge1, edge2);
	}
	else
	{
		triNorm[0] = triNormal[0];
		triNorm[1] = triNormal[1];
		triNorm[2] = triNormal[2];
	}
	
	// line pt1-to-pt2
	double line[3] = {0,0,0};
	vectorSub(line, pt2, pt1);

	// line and triangle are parallel to each other
	double dp = dotProduct(triNorm, line);
	if (dp == 0.0)	// coplanar
	{
		// check if it intersects triPt1, triPt2, or triPt3
		unsigned int numPtInt(0);
		bool intTriPt1(false);
		bool intTriPt2(false);
		bool intTriPt3(false);
		if (intTriPt1 = line_pt_intersection(triPt1, pt1, pt2))
			++numPtInt;
		if (intTriPt2 = line_pt_intersection(triPt2, pt1, pt2))
			++numPtInt;
		if (intTriPt3 = line_pt_intersection(triPt3, pt1, pt2))
			++numPtInt;

		if (numPtInt == 2)	// line is same as an edge in the triangle
			return 4;

		// check if it intersects edge1, edge2, or edge3
		double intersectPt[3] = {0,0,0};
		if (numPtInt == 1)	// line intersects one vertex already, check if it intersects a line
		{
			double edgePt1[3] = {triPt1[0], triPt1[1], triPt1[2]};
			double edgePt2[3] = {triPt2[0], triPt2[1], triPt2[2]};
			if (intTriPt1)
			{
				edgePt1[0] = triPt2[0];
				edgePt1[1] = triPt2[1];
				edgePt1[2] = triPt2[2];
				edgePt2[0] = triPt3[0];
				edgePt2[1] = triPt3[1];
				edgePt2[2] = triPt3[2];
			}
			else if (intTriPt2)
			{
				edgePt1[0] = triPt3[0];
				edgePt1[1] = triPt3[1];
				edgePt1[2] = triPt3[2];
				edgePt2[0] = triPt1[0];
				edgePt2[1] = triPt1[1];
				edgePt2[2] = triPt1[2];
			}
		
			if (segment_line_intersection(intersectPt, pt1, pt2, edgePt1, edgePt2) == 1)
				return 6;
			else
				return 7;
		}
		else	// line doesn't intersect any of the 3 vertices
		{
			unsigned int numEdgeInt(0);
			if (segment_line_intersection(intPt, pt1, pt2, triPt1, triPt2) == 1)
				++numEdgeInt;
			if (segment_line_intersection(intPt, pt1, pt2, triPt2, triPt3) == 1)
				++numEdgeInt;
			if (segment_line_intersection(intPt, pt1, pt2, triPt3, triPt1) == 1)
				++numEdgeInt;

			if (numEdgeInt == 2)
				return 5;
		}
	}		
	else
	{
		// calculate the intersection point
		double pt1totriPt1[3] = {0,0,0};
		vectorSub(pt1totriPt1, triPt1, pt1);
		double t = dotProduct(triNorm, pt1totriPt1) / dotProduct(triNorm, line);
		intPt[0] = pt1[0] + t*line[0];
		intPt[1] = pt1[1] + t*line[1];
		intPt[2] = pt1[2] + t*line[2];
		
		// now check if the intersecting point is one the 3 vertices
		if ((triPt1[0] == intPt[0] && triPt1[1] == intPt[1] && triPt1[2] == intPt[2]) ||
			(triPt2[0] == intPt[0] && triPt2[1] == intPt[1] && triPt2[2] == intPt[2]) ||
			(triPt3[0] == intPt[0] && triPt3[1] == intPt[1] && triPt3[2] == intPt[2]))
		{
			if (dp < 0.0)	// check facing
				return 2;	// hits front side
			else
				return -2;	// hits back side
		}
		
		// check if the intersecting point is on one of the 3 edges
		if (segment_pt_intersection(intPt, triPt1, triPt2)  ||
			segment_pt_intersection(intPt, triPt2, triPt3)  ||
			segment_pt_intersection(intPt, triPt3, triPt1))
		{
			// intPt is on an edge
			if (dp < 0.0)
				return 3;	// hits front side
			else
				return -3;	// hits back side
		}
		
		// check if intersecting pt is inside the triangle
		if (check_same_clock_dir(triPt1, triPt2, intPt, triNorm)
			&& check_same_clock_dir(triPt2, triPt3, intPt, triNorm)
			&& check_same_clock_dir(triPt3, triPt1, intPt, triNorm))
		{
			if (dp < 0.0)	// point is inside the triangle
				return 1;	// hits front side
			else
				return -1;	// hits back side
		}
		
		/*
		double w[3] = {intPt[0] - triPt1[0], intPt[1] - triPt1[1], intPt[2] - triPt1[2]};
		double u[3] = {triPt2[0] - triPt1[0], triPt2[1] - triPt1[1], triPt2[2] - triPt1[2]};
		double v[3] = {triPt3[0] - triPt1[0], triPt3[1] - triPt1[1], triPt3[2] - triPt1[2]};
		double uu = dotProduct(u,u);
    	double uv = dotProduct(u,v);
    	double vv = dotProduct(v,v);
    	double wu = dotProduct(w,u);
    	double wv = dotProduct(w,v);
		double D = uv * uv - uu * vv;
		double s_ = (uv * wv - vv * wu) / D;
		double t_ = (uv * wu - uu * wv) / D;

		if ((s_ == 0.0 && t_ == 0.0) || ((s_ == 0.0) && (t_ == 1.0)) || ((s_ == 1.0) && (t_ == 0.0)))
		{
			if (dp < 0.0)	// check facing
				return 2;	// hits front side
			else
				return -2;	// hits back side
		}
		else if (s_ == 0.0 || t_ == 0.0 || (s_+t_) == 1.0)		// on edge
		{
			// intPt is on an edge
			if (dp < 0.0)
				return 3;	// hits front side
			else
				return -3;	// hits back side
		}
		else if (s_ > 0.0 && t_ > 0.0 && (s_+t_) <= 1.0)	// inside the triangle
		{
			if (dp < 0.0)
				return 1;	// hits front side
			else
				return -1;	// hits back side
		}
		*/
	}

	return 0;
}

// check if pt lies on the segment connecting linePt1 and linePt2
// return value (out): true if pt lies on the segment
bool segment_pt_intersection(double *pt, double *linePt1, double *linePt2)
{
	// sanity checks
	if (linePt1[0] == linePt2[0] && linePt1[1] == linePt2[1] && linePt1[2] == linePt2[2])
		return false;

	double line[3] = {0,0,0};
	vectorSub(line, linePt2, linePt1);

	double test[3] = {0,0,0};
	vectorSub(test, pt, linePt1);

	double unitLine[3] = {line[0], line[1], line[2]};
	normalize(unitLine);
	double unitTest[3] = {test[0], test[1], test[2]};
	normalize(unitTest);

	double dp = dotProduct(unitLine, unitTest);
	if (dp > 0 && equal(dp, 1.0, 0.000001))
	{
		if ((length(test) / length(line)) <= 1.0)
			return true;
	}
	
	return false;
}


// check if pt lies on the line connecting linePt1 and linePt2
// return value (out): true if pt lies on the line
bool line_pt_intersection(double *pt, double *linePt1, double *linePt2)
{
	// sanity checks
	if (linePt1[0] == linePt2[0] && linePt1[1] == linePt2[1] && linePt1[2] == linePt2[2])
		return false;

	// boundary cases
	if (linePt1[0] == pt[0] && linePt1[1] == pt[1] && linePt1[2] == pt[2])
		return true;
	if (linePt2[0] == pt[0] && linePt2[1] == pt[1] && linePt2[2] == pt[2])
		return true;
	
	double line[3] = {0,0,0};
	vectorSub(line, linePt2, linePt1);
	normalize(line);

	double test[3] = {0,0,0};
	vectorSub(test, pt, linePt1);
	normalize(test);

	double dp = dotProduct(line, test);
	return (equal(fabs(dp), 1.0, 0.000001));
}

// compute the intersection pt of a line and a segment
// return value (out):  0 - no intersection
//			1 - intersect at a point
//			2 - same line (multiple intersecting points)
int segment_line_intersection(double *intPt, double *pt1, double *pt2, double *seg1, double *seg2)
{
	int returnVal = line_line_intersection(intPt, pt1, pt2, seg1, seg2);
	if (returnVal == 1)
	{
		if (!segment_pt_intersection(intPt, seg1, seg2))
			returnVal = 0;
	}

	return returnVal;
}

// compute the intersection pt of two lines
// return value (out):  0 - no intersection
//			1 - intersect at a point
//			2 - same line
int line_line_intersection(double *intPt, double *pt1, double *pt2, double *pt3, double *pt4)
{
	// sanity checks
	if (pt1[0] == pt2[0] && pt1[1] == pt2[1] && pt1[2] == pt2[2])
		return -1;
	if (pt3[0] == pt4[0] && pt3[1] == pt4[1] && pt3[2] == pt4[2])
		return -1;

	double line1[3] = {0,0,0};
	vectorSub(line1, pt2, pt1);
	
	double line2[3] = {0,0,0};
	vectorSub(line2, pt4, pt3);

	double unitLine1[3] = {line1[0], line1[1], line1[2]};
	normalize(unitLine1);
	double unitLine2[3] = {line2[0], line2[1], line2[2]};
	normalize(unitLine2);

	double dp = dotProduct(unitLine1, unitLine2);
	if (dp >= 1.0 || dp <= -1.0)	// 2 lines are parallel
	{
		if (line_pt_intersection(pt3, pt1, pt2))
			return 2;	// same line
	}
	else
	{
		// a - u*t = b - v*s
		// where a, b are points on line 1 and 2
		//       u, v are vectors for line 1 and 2
		//       a: pt1
		//       b: pt3
		//       u: line1
		//       v: line2
		double s(0.0);
		double t(0.0);
		
		unsigned int comp1 = 0;
		unsigned int comp2 = 1;
		unsigned int comp3 = 2;
		// check if x,y components can be used
		if (line1[comp1] == 0 && line1[comp2] == 0)
		{
			// if so, then use x and z
			comp2 = 2;
			comp3 = 1;
		}
		
		// solve for s,t using 2 components
		s = (pt1[comp2]*line1[comp1] + pt3[comp1]*line1[comp2] - pt1[comp1]*line1[comp2] - pt3[comp2]*line1[comp1]) / (line1[comp1]*line2[comp2] - line1[comp2]*line2[comp1]);

		if (line1[comp1] == 0.0)
			t = (pt3[comp2] + line2[comp2]*s - pt1[comp2]) / line1[comp2];
		else
			t = (pt3[comp1] + line2[comp1]*s - pt1[comp1]) / line1[comp1];
		
		// now check if s,t is consistent with the third component
		if (pt1[comp3] + line1[comp3]*t == pt3[comp3] + line2[comp3]*s)
		{
			intPt[0] = pt1[0] + line1[0]*t;
			intPt[1] = pt1[1] + line1[1]*t;
			intPt[2] = pt1[2] + line1[2]*t;
			return 1;
		}
	}

	return 0;
}

// compute the intersection pt of a segment and a plane
// intPt (out): intersecting point
// normal (in): normal of plane
// pt (in): a point on plane
// seg1 (in): endpoint of segment
// seg2 (in): endpoint of segment
// return value (out):  0 - no intersection
//						1 - intersect at a point
//						-1 - colinear line (multiple intersecting points)
int segment_plane_intersection(double *intPt, double *normal, double *pt, double *seg1, double *seg2)
{
	double segment[3] = {seg2[0]-seg1[0], seg2[1]-seg1[1], seg2[2]-seg1[2]};
	double seg1ToPt[3] = {pt[0]-seg1[0], pt[1]-seg1[1], pt[2]-seg1[2]};
	double nominator = dotProduct(normal, seg1ToPt);
	double denominator = dotProduct(normal, segment);

	int flag = 0;
	if (denominator == 0)	// parallel to plane
	{
		if (dotProduct(normal, seg1[0]-pt[0], seg1[1]-pt[1], seg1[2]-pt[2]) == 0 &&
			dotProduct(normal, seg2[0]-pt[0], seg2[1]-pt[1], seg2[2]-pt[2]) == 0)
			flag = -1;
		else
			flag = 0;
	}
	else
	{
		double s = nominator / denominator;
		if (s > 1.0 || s < 0)
			return 0;

		intPt[0] = seg1[0] + s*segment[0];
		intPt[1] = seg1[1] + s*segment[1];
		intPt[2] = seg1[2] + s*segment[2];
		flag = 1;
	}

	return flag;
}

// compute the intersection pt of a point and a plane
// intPt (out): intersecting point
// normal (in): normal of plane
// pt (in): a point on plane
// seg1 (in): endpoint of segment
// seg2 (in): endpoint of segment
// return value (out):  0 - no intersection
//						1 - intersect at a point
//						-1 - colinear line (multiple intersecting points)
int line_plane_intersection(double *intPt, double *normal, double *pt, double *seg1, double *seg2)
{
	double segment[3] = {seg2[0]-seg1[0], seg2[1]-seg1[1], seg2[2]-seg1[2]};
	double seg1ToPt[3] = {pt[0]-seg1[0], pt[1]-seg1[1], pt[2]-seg1[2]};
	double nominator = dotProduct(normal, seg1ToPt);
	double denominator = dotProduct(normal, segment);

	int flag = 0;
	if (denominator == 0)	// parallel to plane
	{
		if (dotProduct(normal, seg1[0]-pt[0], seg1[1]-pt[1], seg1[2]-pt[2]) == 0 &&
			dotProduct(normal, seg2[0]-pt[0], seg2[1]-pt[1], seg2[2]-pt[2]) == 0)
			flag = -1;
		else
			flag = 0;
	}
	else
	{
		double s = nominator / denominator;
		intPt[0] = seg1[0] + s*segment[0];
		intPt[1] = seg1[1] + s*segment[1];
		intPt[2] = seg1[2] + s*segment[2];
		flag = 1;
	}

	return flag;
}

// check if norm is pointing the same direction as the normal of the plane created by triPt1, triPt2, triPt3
bool check_same_clock_dir(double *triPt1, double *triPt2, double *triPt3, double *norm)
{
	// edge 1 of triangle
	double edge1[3] = {0,0,0};
	vectorSub(edge1, triPt2, triPt1);
	
	// edge 2 of triangle
	double edge2[3] = {0,0,0};
	vectorSub(edge2, triPt3, triPt1);
			
	// vector normal of triangle
	double triNormal[3] = {0,0,0};
	crossProduct(triNormal, edge1, edge2);

	// dot product with norm
	return (dotProduct(norm, triNormal) > 0.0);
}

// find all intersection point between the line pt1-pt2 to the mesh
// return true if at least 1 intersecting pt is found
bool line_mesh_intersection(vector<lineMeshIntType> &intPtList, double *pt1, double *pt2, 
				const double* verticesList, int numVertices,
				const int* const* polygonsList, int numPolygons,
				unsigned int sortCoord)
{
	// sanity checks
	if (!intPtList.empty())
		intPtList.clear();
	if (numVertices == 0 || numPolygons == 0)
		return false;
	if (sortCoord != 0 && sortCoord != 1 && sortCoord != 2)
		sortCoord = 0;

	// brute-force-ly intersect all triangles in mesh
	for (int i = 0; i < numPolygons; ++i)
	{
		// intersecting point
		double intPt[3] = {0,0,0};

		// get the 3 pts of current triangle
		double triPt1[3] = {verticesList[3*polygonsList[i][0]], verticesList[3*polygonsList[i][0]+1], verticesList[3*polygonsList[i][0]+2]};
		double triPt2[3] = {verticesList[3*polygonsList[i][1]], verticesList[3*polygonsList[i][1]+1], verticesList[3*polygonsList[i][1]+2]};
		double triPt3[3] = {verticesList[3*polygonsList[i][2]], verticesList[3*polygonsList[i][2]+1], verticesList[3*polygonsList[i][2]+2]};

		// line-triangle intersection
		int intType = line_triangle_intersection(intPt, pt1, pt2, triPt1, triPt2, triPt3);
		if (intType != 0)
		{
			lineMeshIntType intPtInstance;
			intPtInstance.intType = intType;
			intPtInstance.pt[0] = intPt[0];
			intPtInstance.pt[1] = intPt[1];
			intPtInstance.pt[2] = intPt[2];
			
			// add to list in ascending order according to sortCoord
			bool hasInserted(false);
			for (vector<lineMeshIntType>::iterator iter = intPtList.begin(); iter != intPtList.end(); iter++)
			{
				if (iter->pt[sortCoord] > intPtInstance.pt[sortCoord])
				{
					intPtList.insert(iter, intPtInstance);
					hasInserted = true;
					break;
				}
			}

			// new point still hasn't been added, so add to the end of list
			if (!hasInserted)
				intPtList.push_back(intPtInstance);
		}
	}

	if (intPtList.empty())
		return false;
	
	return true;
}

// returns the shortest distance from pt to segment (segpt1, segpt2)
// pt (in): point
// segpt1 (in): point 1 of segment
// segpt2 (in): point 2 of segment
// closestPt (out): closest point from pt to segment
// return val (out): shortest distance from pt to segment (segpt1, segpt2)
double pt_segment_distance(double* pt, double* segpt1, double* segpt2, double* closestPt)
{
	double segpt1_to_pt[3];
	double segpt1_to_segpt2[3];
	vectorSub(segpt1_to_pt, pt, segpt1);
	vectorSub(segpt1_to_segpt2, segpt2, segpt1);
	double vecLength = length(segpt1_to_segpt2);
	normalize(segpt1_to_segpt2);
	
	// u = (p3-pt).(p2-p1) / (p2-p1).(p2-p1)
	double u = dotProduct(segpt1_to_pt, segpt1_to_segpt2);
	
	if (u < 0.0)
		u = 0.0;
	else if (u > vecLength)
		u = vecLength;
	
	// p = pt1 + u*(p2-p1)	
	closestPt[0] = segpt1[0] + u*segpt1_to_segpt2[0];
	closestPt[1] = segpt1[1] + u*segpt1_to_segpt2[1];
	closestPt[2] = segpt1[2] + u*segpt1_to_segpt2[2];
	
	double pt_to_closestPt[3];
	vectorSub(pt_to_closestPt, closestPt, pt);
		
	return length(pt_to_closestPt);
}

// returns the shortest distance from pt to triangle (tript1, tript2, tript3)
// pt (in): point
// tript1 (in): point 1 of triangle
// tript2 (in): point 2 of triangle
// tript3 (in): point 3 of triangle
// closestPt (out): closest point from pt to segment
// return val (out): shortest distance from pt to triangle (tript1, tript2, tript3)
double pt_triangle_distance(double* pt, double* tript1, double* tript2, double* tript3, double* closestPt)
{
	// ---- project pt to plane of triangle ----
	double tript1_to_tript2[3];
	double tript1_to_tript3[3];
	vectorSub(tript1_to_tript2, tript2, tript1);
	vectorSub(tript1_to_tript3, tript3, tript1);
	
	double triangleNorm[3];
	crossProduct(triangleNorm, tript1_to_tript2, tript1_to_tript3);
	normalize(triangleNorm);
	
	double tript1_to_pt[3];
	vectorSub(tript1_to_pt, pt, tript1);
	
	double alpha = dotProduct(tript1_to_pt, triangleNorm);
	
	double projectedPt[3] = {pt[0] - alpha*triangleNorm[0],
							pt[1] - alpha*triangleNorm[1],
							pt[2] - alpha*triangleNorm[2]};
	
	// ---- check if projected pt is inside triangle ----
	if (check_same_clock_dir(tript1, tript2, projectedPt, triangleNorm)
		&& check_same_clock_dir(tript2, tript3, projectedPt, triangleNorm)
		&& check_same_clock_dir(tript3, tript1, projectedPt, triangleNorm))
	{
		closestPt[0] = projectedPt[0];
		closestPt[1] = projectedPt[1];
		closestPt[2] = projectedPt[2];
	}
	else
	{
		// ---- pt is not inside triangle       ----
		// ---- check closest pt to the 3 edges ----
		double closestPt_seg[3];
		
		double minDist = pt_segment_distance(projectedPt, tript1, tript2, closestPt);
		double dist = pt_segment_distance(projectedPt, tript2, tript3, closestPt_seg);
		if (dist < minDist)
		{	
			minDist = dist;
			closestPt[0] = closestPt_seg[0];
			closestPt[1] = closestPt_seg[1];
			closestPt[2] = closestPt_seg[2];
		}
		dist = pt_segment_distance(projectedPt, tript3, tript1, closestPt_seg);
		if (dist < minDist)
		{	
			minDist = dist;
			closestPt[0] = closestPt_seg[0];
			closestPt[1] = closestPt_seg[1];
			closestPt[2] = closestPt_seg[2];
		}
	}
	
	double pt_to_closestPt[3];
	vectorSub(pt_to_closestPt, closestPt, pt);
	
	return length(pt_to_closestPt);
}

// returns the shortest distance from pt to triangle (tript1, tript2, tript3)
// pt (in): point
// tript1 (in): point 1 of triangle
// tript2 (in): point 2 of triangle
// tript3 (in): point 3 of triangle
// closestPt (out): closest point from pt to segment
// closestDist (out): shortest distance from pt to triangle (tript1, tript2, tript3)
// return val (out): 0 - closest pt is inside triangle, 
//                   1 - closest pt is tript1,
//                   2 - closest pt is tript2,
//                   3 - closest pt is tript3,
//                   -1 - closest pt is on edge (tript1, tript2),
//                   -2 - closest pt is on edge (tript2, tript3),
//                   -3 - closest pt is on edge (tript3, tript1),
int pt_triangle_distance(double* pt, double* tript1, double* tript2, double* tript3, double* closestPt, double &closestDist)
{
	// ---- project pt to plane of triangle ----
	double tript1_to_tript2[3];
	double tript1_to_tript3[3];
	vectorSub(tript1_to_tript2, tript2, tript1);
	vectorSub(tript1_to_tript3, tript3, tript1);
	
	double triangleNorm[3];
	crossProduct(triangleNorm, tript1_to_tript2, tript1_to_tript3);
	normalize(triangleNorm);
	
	double tript1_to_pt[3];
	vectorSub(tript1_to_pt, pt, tript1);
	
	double alpha = dotProduct(tript1_to_pt, triangleNorm);
	
	double projectedPt[3] = {pt[0] - alpha*triangleNorm[0],
							pt[1] - alpha*triangleNorm[1],
							pt[2] - alpha*triangleNorm[2]};
	
	int returnCode(0);

	// ---- check if projected pt is inside triangle ----
	if (check_same_clock_dir(tript1, tript2, projectedPt, triangleNorm)
		&& check_same_clock_dir(tript2, tript3, projectedPt, triangleNorm)
		&& check_same_clock_dir(tript3, tript1, projectedPt, triangleNorm))
	{
		closestPt[0] = projectedPt[0];
		closestPt[1] = projectedPt[1];
		closestPt[2] = projectedPt[2];
	}
	else
	{
		// ---- pt is not inside triangle       ----
		// ---- check closest pt to the 3 edges ----
		double closestPt_seg[3];
		
		returnCode = -1;
		double minDist = pt_segment_distance(projectedPt, tript1, tript2, closestPt);
		double dist = pt_segment_distance(projectedPt, tript2, tript3, closestPt_seg);
		if (equal(dist, minDist, 0.0000001))
		{
			returnCode = 2;
		}
		else if (dist < minDist)
		{	
			minDist = dist;
			closestPt[0] = closestPt_seg[0];
			closestPt[1] = closestPt_seg[1];
			closestPt[2] = closestPt_seg[2];
			returnCode = -2;
		}
		
		dist = pt_segment_distance(projectedPt, tript3, tript1, closestPt_seg);
		if (equal(dist, minDist, 0.0000001))
		{
			if (returnCode == -1)
				returnCode = 1;
			else if (returnCode == -2)
				returnCode = 3;
		}
		else if (dist < minDist)
		{	
			minDist = dist;
			closestPt[0] = closestPt_seg[0];
			closestPt[1] = closestPt_seg[1];
			closestPt[2] = closestPt_seg[2];
			returnCode = -3;
		}

		
	}
	
	double pt_to_closestPt[3];
	vectorSub(pt_to_closestPt, closestPt, pt);
	closestDist = length(pt_to_closestPt);
	return returnCode;
}


// check for degenerate cases
//
// vList (in): list of vertices
// numV (in): number of vertices
// fList (in): list of polygons
// numF (in): number of polygons
// checkGeoDegenerateTriangles (in): true - check geometrically degenerate triangles
// return value (out): true if no degenerate cases
bool checkDegenerateCases(const double* vList, int numV, const int* const* fList, int numF, bool checkGeoDegenerateTriangles)
{
	cout << "Checking degenerate cases... " << endl;

	// use to check for dangling vertex
	vector<unsigned int> vIndices(numV, 0);

	// stores degenerate polygons
	vector<unsigned int> degenPolys;
	vector<unsigned int> geoDegenPolys;
	
	// go through all the polygons to mark referenced vertices
	for (int i = 0; i < numF; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = fList[i][j];
			vIndices[vIdx] = 1;
		}

		// check for degenerate triangles
		if (fList[i][0] == fList[i][1] || fList[i][0] == fList[i][2] ||	fList[i][1] == fList[i][2])
			degenPolys.push_back(i);
		// geometric degeneracy
		if (checkGeoDegenerateTriangles) {
			if ((vList[3*(fList[i][0])] == vList[3*(fList[i][1])] &&
				vList[3*(fList[i][0])+1] == vList[3*(fList[i][1])+1] &&
				vList[3*(fList[i][0])+2] == vList[3*(fList[i][1])+2]) ||
				(vList[3*(fList[i][0])] == vList[3*(fList[i][2])] &&
				vList[3*(fList[i][0])+1] == vList[3*(fList[i][2])+1] &&
				vList[3*(fList[i][0])+2] == vList[3*(fList[i][2])+2]) ||
				(vList[3*(fList[i][1])] == vList[3*(fList[i][2])] &&
				vList[3*(fList[i][1])+1] == vList[3*(fList[i][2])+1] &&
				vList[3*(fList[i][1])+2] == vList[3*(fList[i][2])+2]))
				geoDegenPolys.push_back(i);
		}
	}
	
	bool result(true);
	unsigned int nDegenVer(0);
	vector<unsigned int>::iterator iter = vIndices.begin();
	do {
		iter = find(iter, vIndices.end(), (unsigned int)0);
		if (iter != vIndices.end()) {
			if (nDegenVer == 0)
				cout << "Degenerate Vertices: ";
			cout << iter - vIndices.begin() << " ";
			++nDegenVer;
			iter++;
		}
		else
			break;
	} while (iter != vIndices.end());
	if (nDegenVer > 0)
		cout << endl << "There are " << nDegenVer << " degenerate vertices" << endl;
	/*
	unsigned int nDegenVer = vIndices.size();
	if (nDegenVer > 0)
	{
		cout << "Warning: There are " << nDegenVer << " degenerate vertices: ";
		for (unsigned int i = 0; i < nDegenVer; ++i)
			cout << vIndices[i] << " ";
		cout << endl;

		result = false;
	}
	*/

	unsigned int nDegenPolys = degenPolys.size();
	if (nDegenPolys > 0)
	{
		cout << "Warning: There are " << nDegenPolys << " degenerate triangles: ";
		for (unsigned int i = 0; i < nDegenPolys; ++i)
			cout << degenPolys[i] << " ";
		cout << endl;

		result = false;
	}

	if (checkGeoDegenerateTriangles)
	{	
		unsigned int nGeoDegenPolys = geoDegenPolys.size();
		if (nGeoDegenPolys > 0)
		{
			cout << "Warning: There are " << nGeoDegenPolys << " geometrically degenerate triangles: ";
			for (unsigned int i = 0; i < nGeoDegenPolys; ++i)
				cout << geoDegenPolys[i] << " ";
			cout << endl;
	
			result = false;
		}
	}

	if (result)
	{
		cout << "Mesh has no degenerate cases." << endl;
	}

	return result;
}

// check if mesh is manifold
//
// vList (in): list of vertices
// numV (in): number of vertices
// fList (in): list of polygons
// numF (in): number of polygons
// checkClosedManifold (in): true - check closed manifold
// nonManifoldVertices (out): list of non-manifold vertices
// nonManifoldEdges (out): list of non-manifold edges
// openManifoldEdges (out): list open-manifold edges
// return value (out): true if no degenerate cases and is manifold
bool checkManifold(const double* vList, int numV, const int* const* fList, int numF, bool checkClosedManifold, vector<int> &nonManifoldVertices, vector<Edge> &nonManifoldEdges, vector<Edge> &openManifoldEdges)
{
	if (!checkDegenerateCases(vList, numV, fList, numF, false))
		return false;

	cout << "Checking manifold... " << endl;
	
	vector<int> *vNbrList = new vector<int>[numV];
	map<pair<int, int> , EdgeInfo> edgeList;
	for (int i = 0; i < numF; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			int vIdx = fList[i][j];
			vNbrList[vIdx].push_back(i);

			int v1 = vIdx;
			int v2 = fList[i][(j+1)%3];
			if (v2 < v1)
				swap(v2, v1);
			pair<int, int> edge(v1,v2);

			edgeList[edge].edge = edge;
			if (find(edgeList[edge].nbrPolys.begin(), edgeList[edge].nbrPolys.end(), i) == edgeList[edge].nbrPolys.end())
				edgeList[edge].nbrPolys.push_back(i);
		}
	}

	// check for nonmanifold edges
	nonManifoldEdges.clear();
	openManifoldEdges.clear();
	for (map<pair<int, int> , EdgeInfo>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		EdgeInfo eInfo = iter->second;
		if (iter->second.nbrPolys.size() > 2)
		{
			Edge e;
			e.vertex1Idx = eInfo.edge.first;
			e.vertex2Idx = eInfo.edge.second;
			nonManifoldEdges.push_back(e);
		}
		else if (checkClosedManifold && iter->second.nbrPolys.size() < 2)
		{
			Edge e;
			e.vertex1Idx = eInfo.edge.first;
			e.vertex2Idx = eInfo.edge.second;
			openManifoldEdges.push_back(e);
		}
	}

	// check for nonmanifold vertices
	nonManifoldVertices.clear();
	for (unsigned int i = 0; (int)i < numV; ++i)
	{
		vector<int> oneRingTri;
		if (vNbrList[i].empty())
			continue;

		vector<int> vNbrs = vNbrList[i];
		oneRingTri.push_back(vNbrs.front());
		vNbrs.erase(vNbrs.begin());

		// try to attach triangles to front of oneRingTri
		for (int j = 0; j < (int)vNbrs.size(); ++j)
		{
			int prevIdx = 0;
			int nextIdx = 0;
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (fList[vNbrs[j]][k] == (int)i)
				{
					prevIdx = fList[vNbrs[j]][(k+2)%3];
					nextIdx = fList[vNbrs[j]][(k+1)%3];
					break;
				}
			}

			
			int frontPrevIdx = 0;
			int frontNextIdx = 0;
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (fList[oneRingTri.front()][k] == (int)i)
				{
					frontPrevIdx = fList[oneRingTri.front()][(k+2)%3];
					frontNextIdx = fList[oneRingTri.front()][(k+1)%3];
					break;
				}
			}

			// check if current triangle is a neighbour of the front of oneRingTri
			if (prevIdx == frontNextIdx)
			{
				oneRingTri.insert(oneRingTri.begin(), vNbrs[j]);
				vNbrs.erase(vNbrs.begin() + j);
				j = -1;
				continue;
			}
		}
		
		// try to attach triangles to back of oneRingTri
		for (int j = 0; j < (int)vNbrs.size(); ++j)
		{
			int prevIdx = 0;
			int nextIdx = 0;
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (fList[vNbrs[j]][k] == (int)i)
				{
					prevIdx = fList[vNbrs[j]][(k+2)%3];
					nextIdx = fList[vNbrs[j]][(k+1)%3];
					break;
				}
			}

			
			int endPrevIdx = 0;
			int endNextIdx = 0;
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (fList[oneRingTri.back()][k] == (int)i)
				{
					endPrevIdx = fList[oneRingTri.back()][(k+2)%3];
					endNextIdx = fList[oneRingTri.back()][(k+1)%3];
					break;
				}
			}
			if (nextIdx == endPrevIdx)
			{
				oneRingTri.push_back(vNbrs[j]);
				vNbrs.erase(vNbrs.begin() + j);
				j = -1;
				continue;
			}
		}

		if (!vNbrs.empty())
			nonManifoldVertices.push_back(i);
	}
	
	delete[] vNbrList;

	bool result(true);
	unsigned int numNonManifoldEdges = nonManifoldEdges.size();
	if (numNonManifoldEdges > 0)
	{
		cout << "There are " << numNonManifoldEdges << " non-manifold edges: ";
// 		for (unsigned int i = 0; i < numNonManifoldEdges; ++i)
// 		{
// 			cout << "(" << nonManifoldEdges[i].vertex1Idx << ", " << nonManifoldEdges[i].vertex2Idx << ") ";
// 		}
		cout << endl;
		result = false;
	}

	if (checkClosedManifold)
	{
		unsigned int numOpenManifoldEdges = openManifoldEdges.size();
		if (numOpenManifoldEdges > 0)
		{
			cout << "There are " << numOpenManifoldEdges << " open manifold edges: ";
// 			for (unsigned int i = 0; i < numOpenManifoldEdges; ++i)
// 			{
// 				cout << "(" << openManifoldEdges[i].vertex1Idx << ", " << openManifoldEdges[i].vertex2Idx << ") ";
// 			}
			cout << endl;
			result = false;
		}

		// compute number of boundaries
		cout << "There are " << numBoundaries(openManifoldEdges) << " boundaries." << endl;
	}

	unsigned int numNonManifoldVertices = nonManifoldVertices.size();
	if (numNonManifoldVertices > 0)
	{
		cout << "There are " << numNonManifoldVertices << " non-manifold vertices: ";
// 		for (unsigned int i = 0; i < numNonManifoldVertices; ++i)
// 		{
// 			cout << nonManifoldVertices[i] << " ";
// 		}
		cout << endl;
		result = false;
	}

	if (result)
	{
		cout << "Mesh is a ";
		if (checkClosedManifold)
			cout << "closed-";
		cout << "manifold." << endl;
	}

	return result;
}

// returns the number of boundaries from the set of open edges
unsigned int numBoundaries(const vector<Edge> &openManifoldEdges)
{
	unsigned int numBoundaries(0);

	vector<unsigned int> needSortedList;
	for (unsigned int i = 0; i < openManifoldEdges.size(); ++i)
		needSortedList.push_back(i);

	while (!needSortedList.empty())
	{
		numBoundaries_aux(openManifoldEdges, needSortedList, -1, -1);
		++numBoundaries;
	}
	return numBoundaries;
}

// aux function for numBoundaries
void numBoundaries_aux(const vector<Edge> &openManifoldEdges, vector<unsigned int> &needSortedList, int v1Idx, int v2Idx)
{
	if (v1Idx < 0 || v2Idx < 0)
	{
		Edge e = openManifoldEdges[needSortedList.back()];
		needSortedList.pop_back();
		numBoundaries_aux(openManifoldEdges, needSortedList, e.vertex1Idx, e.vertex2Idx);
	}
	else
	{
		for (unsigned int i = 0; i < needSortedList.size(); ++i)
		{
			Edge e2 = openManifoldEdges[needSortedList[i]];
			if (e2.vertex1Idx == v1Idx)
			{
				needSortedList.erase(needSortedList.begin() + i);
				numBoundaries_aux(openManifoldEdges, needSortedList, e2.vertex2Idx, v2Idx);
				break;
			}
			else if (e2.vertex1Idx == v2Idx)
			{
				needSortedList.erase(needSortedList.begin() + i);
				numBoundaries_aux(openManifoldEdges, needSortedList, v1Idx, e2.vertex2Idx);
				break;
			}
			else if (e2.vertex2Idx == v1Idx)
			{
				needSortedList.erase(needSortedList.begin() + i);
				numBoundaries_aux(openManifoldEdges, needSortedList, e2.vertex1Idx, v2Idx);
				break;
			}
			else if (e2.vertex2Idx == v2Idx)
			{
				needSortedList.erase(needSortedList.begin() + i);
				numBoundaries_aux(openManifoldEdges, needSortedList, v1Idx, e2.vertex1Idx);
				break;
			}
		}
	}
}

// return the bounding box diagonal
double boundingBoxDiagonal(const double* vList, int numV)
{
	double minx(0);
	double miny(0);
	double minz(0);
	double maxx(0);
	double maxy(0);
	double maxz(0);

	for (int i = 0; i < numV; ++i)
	{
		if (i == 0 || vList[3*i] <= minx)
			minx = vList[3*i];
		if (i == 0 || vList[3*i+1] <= miny)
			miny = vList[3*i+1];
		if (i == 0 || vList[3*i+2] <= minz)
			minz = vList[3*i+2];
		if (i == 0 || vList[3*i] >= maxx)
			maxx = vList[3*i];
		if (i == 0 || vList[3*i+1] >= maxy)
			maxy = vList[3*i+1];
		if (i == 0 || vList[3*i+2] >= maxz)
			maxz = vList[3*i+2];
	}

	return length(maxx-minx, maxy-miny, maxz-minz);
}

/*
template <class T>
void swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
*/
#endif
