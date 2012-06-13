/***************************************************************************
 *   Copyright (C) 2005 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <math.h>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "tribox.h"

using namespace std;

struct Edge {
    Edge()
        :vertex1Idx(-1), vertex2Idx(-1)
    {
    }

    int vertex1Idx;
    int vertex2Idx;
};

struct EdgeInfo {
	pair<int, int> edge;				// edge (v1, v2)
	vector<int> nbrPolys;		// neighbour polygons that uses this edge
};

struct lineMeshIntType {
	int intType;	// type of intersection with ray
	// 0  - no intersection
	// 1  - intersection is inside the triangle (ray hits frontside)
	// -1 - intersection is inside the triangle (ray hits backside)
	// 2  - intersection is at a vertex (ray hits frontside)
	// -2 - intersection is at a vertex (ray hits backside)
	// 3  - intersection is on an edge (ray hits frontside)
	// -3 - intersection is on an edge (ray hits backside)
	// 4  - intersection contains an edge (coplanar)
	// 5  - intersection through 2 edges (coplanar)
	// 6  - intersection through one vertex and an edge (coplanar)
	// 7  - intersection is at a vertex (coplanar)
	double pt[3];	// intersecting point
};

// compare two double values with tolerence eps
// a (in): vector a
// b (in): vector b
// eps (in): epsilon
bool equal(double a, double b, double eps);

// Cross Product c = a x b
// c (out): vector c
// a (in): vector a
// b (in): vector b
void crossProduct(double* c, double* a, double* b);

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
                  double bx, double by, double bz);

// Vector Subtraction c = a - b
// c (out): vector c
// a (in): vector a
// b (in): vector b
void vectorSub(double* c, double* a, double* b);

// Vector Subtraction c = a - b
// c (out): vector c
// ax (in): x coord of vector a
// ay (in): y coord of vector a
// az (in): z coord of vector a
// bx (in): x coord of vector b
// by (in): y coord of vector b
// bz (in): z coord of vector b
void vectorSub(double* c, double ax, double ay, double az,
                          double bx, double by, double bz);

// Vector dot product (a . b)
// a (in): vector a
// b (in): vector b
// return val (out): (a . b)
double dotProduct(double* a, double* b);
double dotProduct(double ax, double ay, double az, double bx, double by, double bz);
double dotProduct(double* a, double bx, double by, double bz);

// angle between two vectors in radian
// a (in): vector a
// b (in): vector b
// return val (out): angle between a,b in radian
double getAngle(double* a, double* b);

// angle between two vectors in radian
// anglePt (in): location of the point coincides with the angle
// edgeEndPt1 (in): location of the point that makes up an edge for the angle at anglePt
// edgeEndPt2 (in): location of the point that makes up an edge for the angle at anglePt
// return val (out): angle between two vectors in radian
double getAngle(double* anglePt, double* edgeEndPt1, double* edgeEndPt2);

// angle between two vectors in radian
// anglePt (in): location of the point coincides with the angle
// edgeEndPt1 (in): location of the point that makes up an edge for the angle at anglePt
// edgeEndPt2 (in): location of the point that makes up an edge for the angle at anglePt
// return val (out): angle between two vectors in radian
double getAngle(double anglePtx, double anglePty, double anglePtz,
		double edgeEndPt1x, double edgeEndPt1y, double edgeEndPt1z, 
		double edgeEndPt2x, double edgeEndPt2y, double edgeEndPt2z);

// normalize a vector
// a (in/out): vector to be normalize
void normalize(double* a);
void normalize(double &ax, double &ay, double &az);

// vector length
// a (in): vector a
// return val (out): length of a
double length(double* a);
double length(double ax, double ay, double az);

// returns the face normal of triangle (pt1, pt2, pt3)
void computeFaceNormal(double pt1x, double pt1y, double pt1z, double pt2x, double pt2y, double pt2z, double pt3x, double pt3y, double pt3z, double* normal);

// calculate midpoint between a and b
// c (out): midpoint of a,b
// ax (in): x coord of point a
// ay (in): y coord of point a
// az (in): z coord of point a
// bx (in): x coord of point b
// by (in): y coord of point b
// bz (in): z coord of point b
void midpoint(double* c, double ax, double ay, double az, double bx, double by, double bz);
void midpoint(float* c, float ax, float ay, float az, float bx, float by, float bz);

// linear interpolate between a and b
// percentage (in): % used for interpolation
// c (out): midpoint of a,b
// ax (in): x coord of point a
// ay (in): y coord of point a
// az (in): z coord of point a
// bx (in): x coord of point b
// by (in): y coord of point b
// bz (in): z coord of point b
void linearInterp(float percentage, double* c, double ax, double ay, double az,
                                               double bx, double by, double bz);

// area of a triangle
// a (in): point a
// b (in): point b
// c (in): point c
double areaTriangle(double* a, double* b, double* c);

// point-plane distance
// pt (in): point
// ptOnPlane (in): a point on the plane
// normal (in): normal of plane
// return val (out): distance from pt to plane
double pointPlaneDistance(double* pt, double* ptOnPlane, double* normal);

// point-line distance
// pt (in): point
// endPt1 (in): a point on the line
// endPt2 (in): a point on the line
// return val (out): distance from pt to line
double pointLineDistance(double* pt, double* endPt1, double* endPt2);

// point-point distance
// pt1 (in): point
// pt2 (in): point
// return val (out): distance from pt to line
double pointPointDistance(double pt1x, double pt1y, double pt1z,
                          double pt2x, double pt2y, double pt2z);

// set 'matrix' with dimention 'dim' as an identity matrix
// matrix (out): matrix
// dim (in): dimension of matrix
void setIdentityMatrix(double** matrix, int dim);

// factorize matrix A with dimension n using Cholesky Factorization
// A (in): matrix A
// dim (in): dimension of A
// L (out): factorized matrix of A
void cholesky(double* A, int dim, double* L);

// back-substitution on lower triangular matrix Lx = B
// B will be replaced with X
// numColInB(in): num column in B
// B (in/out): input B
// dim (in): dimension of A
// L (in): lower triangular matrix
void L_backSub(double* L, int dim, double* B, int numColInB = 3);

// back-substitution on lower triangular matrix transpose
// numColInB(in): num column in B
// B will be replaced with X
// B (in/out): input B
// dim (in): dimension of A
// L (in): lower triangular matrix
void LT_backSub(double* L, int dim, double* B, int numColInB = 3);

// computes the intersection between a line and a triangle
// intPt (out): intersected point
// pt1 (in): point on the line
// pt2 (in): point on the line
// triPt1 (in): point of triangle
// triPt2 (in): point of triangle
// triPt3 (in): point of triangle
// triNormal (in): normal of triangle
// return value (out):  0  - no intersection
//			1  - intersection is inside the triangle (ray hits frontside)
//			-1 - intersection is inside the trianglee (ray hits backside)
//			2  - intersection is at a vertex (ray hits frontside)
//			-2 - intersection is at a vertex (ray hits backside)
//			3  - intersection is on an edge (ray hits frontside)
//			-3 - intersection is on an edge (ray hits backside)
//			4  - intersection contains an edge (coplanar)
//			5  - intersection through 2 edges (coplanar)
//			6  - intersection through one vertex and an edge (coplanar)
//			7  - intersection is at a vertex (coplanar)
int line_triangle_intersection(double *intPt, double *pt1, double *pt2, 
				double *triPt1, double *triPt2, double *triPt3,
				double *triNormal = 0);

// check if pt lies on the segment connecting linePt1 and linePt2
// pt (in): point
// linePt1 (in): endpoint of segment
// linePt2 (in): endpoint of segment
// return value (out): true if pt lies on the segment
bool segment_pt_intersection(double *pt, double *linePt1, double *linePt2);

// check if pt lies on the line connecting linePt1 and linePt2
// pt (in): point
// linePt1 (in): point of line
// linePt2 (in): point of line
// return value (out): true if pt lies on the line
bool line_pt_intersection(double *pt, double *linePt1, double *linePt2);

// compute the intersection pt of a line and a segment
// intPt (out): intersecting point
// pt1 (in): point of line
// pt2 (in): point of line
// seg1 (in): endpoint of segment
// seg2 (in): endpoint of segment
// return value (out):  0 - no intersection
//			1 - intersect at a point
//			2 - same line (multiple intersecting points)
int segment_line_intersection(double *intPt, double *pt1, double *pt2, double *seg1, double *seg2);

// compute the intersection pt of a segment and a plane
// intPt (out): intersecting point
// normal (in): normal of plane
// pt (in): a point on plane
// seg1 (in): endpoint of segment
// seg2 (in): endpoint of segment
// return value (out):  0 - no intersection
//						1 - intersect at a point
//						-1 - colinear line (multiple intersecting points)
int segment_plane_intersection(double *intPt, double *normal, double *pt, double *seg1, double *seg2);

// compute the intersection pt of a point and a plane
// intPt (out): intersecting point
// normal (in): normal of plane
// pt (in): a point on plane
// seg1 (in): endpoint of segment
// seg2 (in): endpoint of segment
// return value (out):  0 - no intersection
//						1 - intersect at a point
//						-1 - colinear line (multiple intersecting points)
int line_plane_intersection(double *intPt, double *normal, double *pt, double *seg1, double *seg2);

// compute the intersection pt of two lines
// intPt (out): intersecting point
// pt1 (in): point of line 1
// pt2 (in): point of line 1
// pt3 (in): point of line 2
// pt4 (in): point of line 2
// return value (out):  0 - no intersection
//			1 - intersect at a point
//			2 - same line
int line_line_intersection(double *intPt, double *pt1, double *pt2, double *pt3, double *pt4);

// check if norm is pointing the same direction as the normal of the plane created by triPt1, triPt2, triPt3
// triPt1 (in): point of triangle
// triPt2 (in): point of triangle
// triPt3 (in): point of triangle
// norm (in): normal we are checking against
bool check_same_clock_dir(double *triPt1, double *triPt2, double *triPt3, double *norm);

// find all intersection point between the line pt1-pt2 to the mesh
// intPtList (out): vector of intersecting point and its intersecting type
// pt1 (in): point of line
// pt2 (in): point of line
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// sortCoord (in): the coordinate to sort the array of intersecting points by; 0=xcoord, 1=ycoord, 2=zcoord
// return val (out): true if at least 1 intersecting pt is found
bool line_mesh_intersection(vector<lineMeshIntType> &intPtList, double *pt1, double *pt2, 
				const double* verticesList, int numVertices,
				const int* const* polygonsList, int numPolygons,
				unsigned int sortCoord = 0);

// returns the shortest distance from pt to segment (segpt1, segpt2)
// pt (in): point
// segpt1 (in): point 1 of segment
// segpt2 (in): point 2 of segment
// closestPt (out): closest point from pt to segment
// return val (out): shortest distance from pt to segment (segpt1, segpt2)
double pt_segment_distance(double* pt, double* segpt1, double* segpt2, double* closestPt);

// returns the shortest distance from pt to triangle (tript1, tript2, tript3)
// pt (in): point
// tript1 (in): point 1 of triangle
// tript2 (in): point 2 of triangle
// tript3 (in): point 3 of triangle
// closestPt (out): closest point from pt to segment
// return val (out): shortest distance from pt to triangle (tript1, tript2, tript3)
double pt_triangle_distance(double* pt, double* tript1, double* tript2, double* tript3, double* closestPt);

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
int pt_triangle_distance(double* pt, double* tript1, double* tript2, double* tript3, double* closestPt, double &dist);

// check for degenerate cases
//
// vList (in): list of vertices
// numV (in): number of vertices
// fList (in): list of polygons
// numF (in): number of polygons
// checkGeoDegenerateTriangles (in): true - check geometrically degenerate triangles
// return value (out): true if no degenerate cases
bool checkDegenerateCases(const double* vList, int numV, const int* const* fList, int numF, bool checkGeoDegenerateTriangles);

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
bool checkManifold(const double* vList, int numV, const int* const* fList, int numF, bool checkClosedManifold, vector<int> &nonManifoldVertices, vector<Edge> &nonManifoldEdges, vector<Edge> &openManifoldEdges);

// returns the number of boundaries from the set of open edges
unsigned int numBoundaries(const vector<Edge> &openManifoldEdges);
// aux function for numBoundaries
void numBoundaries_aux(const vector<Edge> &openManifoldEdges, vector<unsigned int> &needSortedList, int v1Idx, int v2Idx);

// return the bounding box diagonal
double boundingBoxDiagonal(const double* vList, int numV);
/*
template <class T>
void swap(T &a, T &b);
*/
#endif
