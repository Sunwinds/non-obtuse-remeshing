/***************************************************************************
                                nonobtoptn.h
                             -------------------
    begin                : Tue Feb 21 2006
    author               : John Li in 2006
    email                : ysl@cs.sfu.ca
 ***************************************************************************/

#ifndef NONOBTOPTN_H
#define NONOBTOPTN_H

#include "../MarchingCube/helper.h"
#include "../MarchingCube/nonobtuse.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <GondzioSolver.h>
#include <QpGenData.h>
#include <QpGenResiduals.h>
#include <QpGenSparseMa27.h>
#include <QpGenVars.h>

using namespace std;

/**
@author John Li
*/

struct ObjectiveFn {
	// Q(v) = vAv + 2bv + c
	double A[9];	// row major (symmetric)
	double b[3];
	double c;
};

// quadric includes objective function
struct Quadric {
	// vertex index this quadric belongs to
	int vIdx;
	
	// quadric value (without smoothing)
	double quadricValue;
	
	// quadric formula
	// Q(v) = vAv + 2bv + c
	double A[9];	// row major (symmetric)
	double b[3];
	double c;

	//objective function = quadric + smooth
	ObjectiveFn fn;

	// true - quadricValue is dirty (optimization)
	// true - quadric formula is dirty (decimation); quadric value is not used in decimation
	bool dirtyQVal;
};

bool my_quad_greater(Quadric q1, Quadric q2);
bool my_quad_less(Quadric q1, Quadric q2);

struct QuadricMove {
	// quadric
	Quadric q;

	// new position after optimization
	double newPos[3];

	// quadric value after moving
	double qValue_afterMove;

	// quadric improvement from last move
	// valid only if bCanMove is true
	double qValueImprovement;	// q.quadricValue - qValue_afterMove

	// determines if move was successful
	bool bCanMove;

	// true - qValueImprovement is dirty
	bool dirtyImprovement;
	// true - qValue_afterMove is dirty
	bool dirtyQVal_AfterMove;
};

bool my_quadMove_largestImprovementFirst(QuadricMove q1, QuadricMove q2);
bool my_quadMove_smallestImprovementFirst(QuadricMove q1, QuadricMove q2);
bool my_quadMove_largestQuadFirst(QuadricMove q1, QuadricMove q2);
bool my_quadMove_smallestQuadAfterMoveFirst(QuadricMove q1, QuadricMove q2);
bool my_quadMove_largestQuadBeforeMoveFirst(QuadricMove q1, QuadricMove q2);

bool my_quadMovePtr_largestImprovementFirst(QuadricMove* p_q1, QuadricMove* p_q2);
bool my_quadMovePtr_smallestQuadAfterMoveFirst(QuadricMove* p_q1, QuadricMove* p_q2);

struct PlaneEqn {
	// ax + by + cz + d = 0
	double a;
	double b;
	double c;
	double d;
};

enum NORegionType	// determines how the non-obtuse region is to be approximated 
{
	SIMPLE3PLANES,	// each nonobtuse region of an edge in the one-ring is approximated with 3 planes
					// where 2 planes are for the side, and 1 plane facing towards the interior of
					// the one-ring and perpendicular to avg. plane
	SIMPLE3PLANES_TOWARD_CENTER,
					// each nonobtuse region of an edge in the one-ring is approximated with 3 planes
					// where 2 planes are for the side, and 1 plane facing towards the vertex inside the one-ring
	ANGLEBOUND_7PLANES
					// each nonobtuse region of an edge in the one-ring is approximated with 7 planes
					// where 3 planes are the same as in the SIMPLE3PLANES_TOWARD_CENTER case,
					// 2 planes are for enforcing lower angle-bound at endpoint of the edge
					// 2 planes are for enforcing lower angle-bound at the 3rd point that will create a triangle with the edge
};

enum SmoothType		// determines how the smoothness is computed
{
	LAPLACIAN,		// use one-ring centroid as target location
	TAUBIN			// use TAUBIN-smoothed pt as target location
};

class NonobtOptn{
public:
    NonobtOptn();
    ~NonobtOptn();
	
	// copy mesh info as a pointer (shallow copy for nonobtuse mesh, deep copy for desired mesh)
	// dvList (in): pointer to vertices array of desired mesh
	// ndVertices (in): number of vertices dvList is pointing to
	// dpList (in): pointer to polygons array of desired mesh
	// ndPolygons (in): number of polygons dpList is pointing to
	// dvNbrList (in): list of vertex-to-polygon neighbours for desired mesh
	// deNbrList (in): list of vertex-to-edge neighbours for desired mesh
	// dpNbrList (in): list of polygon-to-polygon neighbours for desired mesh
	// novList (in): pointer to vertices array of nonobtuse mesh
	// nnoVertices (in): number of vertices novList is pointing to
	// nopList (in): pointer to polygons array of nonobtuse mesh
	// nnoPolygons (in): number of polygons nopList is pointing to
	// assoTriList (in): list of associated triangle in desired mesh for each vertices in non-obtuse mesh; used for initiate the closest triangle search
	void loadMesh(const double* dvList, int ndVertices, const int* const* dpList, int ndPolygons, const vector<int>* dvNbrList, const vector<EdgeNeighbours>* deNbrList, const vector<int>* dpNbrList, double* novList, int nnoVertices, int** nopList, int nnoPolygons, const int* assoTriList = NULL);
	
	// reset all members
	void reset();
	
	// returns true if mesh info are loaded
	bool isLoaded() const;
	
	// optimize mesh based on quadric error after vertex move
	// numMaxIteration (in): max number of iteration to run
	// alpha (in): weighting alpha value for objective function
	// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the optimization
	// improvementThreshold (in): minimum improvement value before stopping the optimization
	// numOneRingSearch (in): number of one-ring to search for closest face
	// numOneRingQuadric (in): number of one-ring to use for computing quadric
	// angleBound (in): angle bound
	// bRecomputeQuadrics (in): true - recompute all quadrics
	// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated when bRecomputeQuadrics is true
	// bCleanDirtyWhenStuck (in): true - clean dirty vertices when optimization stops, then try to continue optimization
	// region (in): determines how the non-obtuse region is to be approximated
	// return value (out): total time taken
	clock_t optimize_afterMove(unsigned int numMaxIteration = 0, 
							double quadricThreshold = 0.0, 
							double improvementThreshold = 0.0,
							//double r = 0.10, // debug----------------------------
							double alpha = 0.5,
							unsigned int numOneRingSearch = 3, 
							unsigned int numOneRingQuadric = 1,
							double angleBound = 30.0,
							bool bRecomputeQuadrics = false,
							bool bUpdateClosestPolygon = true,
							bool bCleanDirtyWhenStuck = true,
							NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	clock_t optimize_afterMove_noPriority(unsigned int numMaxIteration = 0, 
								double quadricThreshold = 0.0, 
								double improvementThreshold = 0.0,
								//double r = 0.10, // debug----------------------------
								double alpha = 0.5,
								unsigned int numOneRingSearch = 3, 
								unsigned int numOneRingQuadric = 1,
								double angleBound = 30.0,
								bool bRecomputeQuadrics = false,
								bool bUpdateClosestPolygon = true,
								bool bCleanDirtyWhenStuck = true,
								NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	clock_t optimize_afterMove_MCA(unsigned int numMaxIteration = 0, 
								double quadricThreshold = 0.0, 
								double improvementThreshold = 0.0,
								//double r = 0.10, // debug----------------------------
								double alpha = 0.5,
								unsigned int numOneRingSearch = 3, 
								unsigned int numOneRingQuadric = 1,
								double angleBound = 30.0,
								bool bRecomputeQuadrics = false,
								bool bUpdateClosestPolygon = true,
								bool bCleanDirtyWhenStuck = true,
								NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	// non-obtuse smooth mesh
	// numMaxIteration (in): max number of iteration to run
	// smoothnessThreshold (in): minimum smoothness value the non-obtuse mesh can have before stopping the optimization
	// angleBound (in): angle bound
	// bRecomputeSmoothness (in): true - recompute all smoothness value
	// region (in): determines how the non-obtuse region is to be approximated
	// return value (out): total time taken
	clock_t optimize_smoothing(unsigned int numMaxIteration = 0, 
							double smoothnessThreshold = 0.0,
							double angleBound = 30.0,
							bool bRecomputeSmoothness = false, 
							NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	// optimize mesh via alternating optimize_afterMove() and optimize_smoothing()
	// numMaxIteration (in): max number of iteration to run
	// numMoveIteration (in): max number of iteration to move before switching to smooth
	// alpha (in): weighting alpha value for objective function
	// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the optimization
	// improvementThreshold (in): minimum improvement value before stopping the optimization
	// numOneRingSearch (in): number of one-ring to search for closest face
	// numOneRingQuadric (in): number of one-ring to use for computing quadric
	// numSmoothIteration (in): max number of iteration to smooth before switching to move
	// angleBound (in): angle bound
	// bDoOptnFirst (in): true - do optimization before smoothing
	// bRecompute (in): true - recompute quadrics and smoothness
	// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated when bRecomputeQuadrics is true
	// region (in): determines how the non-obtuse region is to be approximated
	// *********** OBSOLETE ***********
// 	void optimize_alternate(unsigned int numMaxIteration = 0, 
// 							unsigned int numMoveIteration = 10,
// 							double quadricThreshold = 0.0, 
// 							double improvementThreshold = 0.0,
// 							double alpha = 0.5,
// 							unsigned int numOneRingSearch = 3, 
// 							unsigned int numOneRingQuadric = 1,
// 							unsigned int numSmoothIteration = 10,
// 							double smoothnessThreshold = 0.0,
// 							double angleBound = 30.0,
// 							bool bDoOptnFirst = true,
// 							bool bRecompute = false, 
// 							bool bUpdateClosestPolygon = true,
// 							NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	// non-obtuse decimate the non-obtuse mesh
	// numRemaining (in): decimate until the mesh has numRemaining vertices
	// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the decimation
	// angleBound (in): angle bound
	// degreeN (in): max. of min. degrees that we want to avoid
	// alpha (in): weighting alpha value for objective function
	// region (in): determines how the non-obtuse region is to be approximated
	// return value (out): total time taken
	clock_t decimate(unsigned int numRemaining, double quadricThreshold = 0.0, double angleBound = 30.0, unsigned int degreeN = 2, double alpha = 0.5, NORegionType region = SIMPLE3PLANES_TOWARD_CENTER);

	// non-obtusely decimate the non-obtuse mesh using multiple choice + linear search scheme
	// numRemaining (in): decimate until the mesh has numRemaining vertices
	// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the decimation
	// angleBound (in): angle bound (in degrees)
	// degreeN (in): max. of min. degrees that we want to avoid
	// alpha (in): weighting alpha value for objective function
	// return value (out): total time taken
	clock_t decimate_MC_LinearSearch(unsigned int numRemaining, double quadricThreshold = 0.0, double angleBound = 30.0, unsigned int degreeN = 2, double alpha = 0.5);
	
	// return pointer to non-obtuse vertices list
	// return val (out): pointer to non-obtuse vertices list
	const double* getNoVertices() const;
	// return the number of vertices in the non-obtuse mesh
	// return val (out): number of vertices in the non-obtuse mesh
	int getNumNoVertices() const;
	// returns pointer to non-obtuse polygon list
	// return val (out): pointer to non-obtuse polygon list
	const int* const* getNoPolygons() const;
	// return the number of polygons in the non-obtuse mesh
	// return val (out): number of polygons in the non-obtuse mesh
	int getNumNoPolygons() const;

	// load debug info
	// filename (in): name of the debug file to load
	// return true if success
	bool loadDebug(char* filename);
	// saves debug info
	// filename (in): name of the debug file to save as
	// return true if success
	bool saveDebug(char* filename) const;

	// remove all vertices that have bad valence on the nonobtuse mesh
	//
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound
	void removeBadValence(NORegionType region, double angleBound);
	
//protected:

	// computes the face normal for the desired mesh and the nonobtuse mesh
	void computeFaceNormal();
	
	// returns the face normal of polygon with index 'polygonIndex'
	// polygonIndex (in): index of polygon we are computing
	// polygonsList (in): polygon array
	// verticesList (in): vertex array
	// normal (out): computed face normal
	void getFaceNormal(int polygonIndex, int** polygonsList, double* verticesList, double* normal) const;
	
	// computes the vertex normal for the mesh
	void computeVertexNormal();
	
	// returns the weighted average ratio for all neighbour polygons of 'vertexindex'
	// vertexIndex (in): index of vertex that is in the center of one-ring
	// polygonsList (in): polygon array
	// verticesList (in): vertex array
	// vertexNeighbourList (in): vertex neighbour array
	// weightedAvgRation (out): array of weighted avg. for each polygon in one-ring
	void getWeightedAvgRatio(int vertexIndex, 
							int** polygonsList, 
							double* verticesList, 
							vector<int>* vertexNeighbourList, 
							double* &weightedAvgRatio) const;
							
	// returns the vertex normal of the vertex with index 'vertexIndex'
	// vertexIndex (in): index of vertex that is in the center of one-ring
	// polygonsList (in): polygon array
	// verticesList (in): vertex array
	// vertexNeighbourList (in): vertex neighbour array
	// normal (out): vertex normal
	void getVertexNormal(int vertexIndex, 
						int** polygonsList, 
						double* verticesList, 
						vector<int>* vertexNeighbourList,
						double* faceNormalsList,
						double* normal) const;
	
	// compute quadric for vIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
	// alpha (in): weighting alpha value for objective function
	// numOneRingSearch (in): number of one-ring to search for closest face
	// numOneRingQuadric (in): number of one-ring to use for computing quadric
	// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated
	Quadric computeQuadric(int novIdx, double alpha = 0.5, unsigned int numOneRingSearch = 3, unsigned int numOneRingQuadric = 1, bool bUpdateClosestPolygon = true);// const;

	// compute boundary quadric for novIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
	// alpha (in): weighting alpha value for objective function
	// numOneRingSearch (in): number of one-ring to search for closest face/edge
	// numOneRingQuadric (in): number of one-ring/neighbour-edge to use for computing quadric
	// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated
	Quadric computeBoundaryQuadric(int novIdx, double alpha = 0.5, unsigned int numOneRingSearch = 3, unsigned int numOneRingQuadric = 1, bool bUpdateClosestPolygon = true) const;	

	// compute edge quadric for edgeIdx and returns it
	// novIdx (in): edge index (of the nonobtuse mesh) of the quadric to be computed
	// alpha (in): weighting alpha value for objective function
	Quadric computeEdgeQuadric(int edgeIdx, double alpha = 0.5) const;

	// get the two immediate boundary vertex neighbour of edgeIdx and store in 'edgeNbr'
	//
	// edgeIdx (in): boundary edge index (of the nonobtuse mesh) of the quadric to be computed
	// edgeNbr (out): stores the 2 immediate boundary vertex neighbour of edgeIdx
	void getEdgeBoundaryNeighbours(int edgeIdx, vector<unsigned int> &edgeNbr) const;

	// get the two immediate boundary vertex neighbour of boundary vertex novIdx and store in 'edgeNbr'
	//
	// novIdx (in): boundary vertex index (of the nonobtuse mesh) of the quadric to be computed
	// edgeNbr (out): stores the 2 immediate boundary vertex neighbour of edgeIdx
	void getVertexBoundaryNeighbours(int novIdx, vector<unsigned int> &edgeNbr) const;

	// move vertex (novIdx) to a better position that minimizes quadric q
	// ******* OBSOLETE *******
	// returns true if successful
	// novIdx (in): index of the vertex that is being moved
	// q (in): quadric value of novIdx
	// region (in): determines how the non-obtuse region is to be approximated
	//bool moveToOptimalLocation(int novIdx, Quadric q, NORegionType region = SIMPLE3PLANES);
	
	// updates vertex with index 'novIdx' on the nonobtuse mesh to position 'newloc'
	// novIdx (in): index of the vertex that is being moved
	// newloc (in): new location
	void updateMesh(int novIdx, double *newloc);

	// edge collapse the non-obtuse mesh
	// edgeIdx (in): index of the edge that is being collapsed
	// newloc (in): new location for the collapsed vertex
	// newUnusedEdges (out): array of new unused edges
	// return val (out): index of the new vertex where the edge collapsed into
	unsigned int edgeCollapse(int edgeIdx, double *newloc, vector<unsigned int> &newUnusedEdges);

	// remove unused vertices and polygons in noVerticesList and noPolygonsList
	// it actually moves the unused to the end of the arrays and update numNoVertices and numNoPolygons, array size stays the same
	// unusedPolyIndices (in): vector of unused polygons indices
	// unusedVertexIndices (in): vector of unused vertices indices
	// vertexIdx (in/out): index of a vertex to keep track of; returns the new index for the vertex if any idx swapping has been done on it
	void removeUnused(const	vector<int> &unusedPolyIndices, const vector<int> &unusedVertexIndices, unsigned int &vertexIdx);

	// remove unused edges in noEdgeList
	// unusedEdgeIdxList (in): vector of unused edge indices
	void removeUnused(vector<unsigned int> &unusedEdgeIdxList);
	
	// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
	// return val (out): number of planes in constraints
	// novIdx (in): vertex index (or edge vertex if bEdgeOneRing is set to true)
	// constratins (out): array of plane equations
	// region (in): determines how the non-obtuse region is to be approximated
	// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
	//                           on the one-ring vertices for the edge
	// angleBound (in): angle-bound
	unsigned int getNonobtuseRegion(int novIdx, PlaneEqn* &constraints, NORegionType region, bool bEdgeOneRing, double angleBound) const;
	
	// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
	// by way of SIMPLE3PLANES method
	// return val (out): number of planes in constraints
	// novIdx (in): vertex index
	// constratins (out): array of plane equations
	unsigned int getNonobtuseRegion_Simple3Planes(int novIdx, PlaneEqn* &constraints) const;

	// computes the set of planes that approximates the nonobtuse region of the one-ring of edgeIdx
	// by way of SIMPLE3PLANES method
	// return val (out): number of planes in constraints
	// edgeIdx (in): edge index
	// constratins (out): array of plane equations
	unsigned int getNonobtuseRegionForEdge_Simple3Planes(int edgeIdx, PlaneEqn* &constraints) const;

	// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
	// by way of SIMPLE3PLANES_TOWARD_CENTER method
	// return val (out): number of planes in constraints
	// novIdx (in): vertex index
	// constratins (out): array of plane equations
	unsigned int getNonobtuseRegion_Simple3Planes_TowardCenter(int novIdx, PlaneEqn* &constraints) const;

	// computes the set of planes that approximates the nonobtuse region of the one-ring of edgeIdx
	// by way of SIMPLE3PLANES_TOWARD_CENTER method
	// return val (out): number of planes in constraints
	// edgeIdx (in): edge index
	// constratins (out): array of plane equations
	unsigned int getNonobtuseRegionForEdge_Simple3Planes_TowardCenter(int edgeIdx, PlaneEqn* &constraints) const;

	// computes the set of planes that approximates the angle-bounded nonobtuse region of the one-ring of novIdx
	// by way of ANGLEBOUND_7PLANES method
	// return val (out): number of planes in constraints
	// novIdx (in): vertex index
	// constratins (out): array of plane equations
	// angleBound (in): angle-bound
	unsigned int getNonobtuseRegion_AngleBound_7Planes(int novIdx, PlaneEqn* &constraints, double angleBound) const;

	// computes the set of planes that approximates the angle-bounded nonobtuse region of the one-ring of edgeIdx
	// by way of ANGLEBOUND_7PLANES method
	// return val (out): number of planes in constraints
	// edgeIdx (in): edge index
	// constratins (out): array of plane equations
	// angleBound (in): angle-bound
	unsigned int getNonobtuseRegionForEdge_AngleBound_7Planes(int edgeIdx, PlaneEqn* &constraints, double angleBound) const;

	// construct all constraints for setting up the planes M_i and N_i; for angle bound region
	//
	// inRingPos (in): position of the in-ring vertex
	// e (in): the edge where planes M_i and N_i are constructing for
	// angleBound (in): angle bound requirement for the orientation of planes M_i and N_i
	// normalY (in): normal for plane Y
	// normalR (in): normal for plane R
	// ptr (in): a pt in plane R
	// numDivision (in): number of division used to construct the planes M_i and N_i
	// offset (in): offsets for the constructed planes along the normal
	// constraints (in/out): array of plane equations
	// currentConstraint (in/out): indices of the first available slot to store the constraints
	// return value (out): true - if all planes constructed successfully
	bool getPlanesMN(double inRingPos[3], Edge e, double angleBound, double normalY[3], double normalR[3], double ptr[3], unsigned int numDivision, double offset, PlaneEqn* &constraints, unsigned int &currentConstraint) const;

	// computes the avg. normal of one-ring of an edge
	// edgeIdx (in): index of edge
	// normal (out): avg. normal
	void computeOneRingAvgNormalForEdge(int edgeIdx, double* normal) const;
	
	// find the polygon in search_polyList that is closest to vIdx in vList
	// if numSearch is 0, all the polygons in search_polyList are searched
	// otherwise, only the polygon indices in searchIdx are searched
	//
	// return val (out): the polygon index in search_polyList that is closest to vIdx
	// vIdx (in): index of vertex we are searching for
	// vList (in): the vertex array vIdx is in
	// search_vList (in): vertex array of the search space
	// search_polyList (in): polygon array of the search space
	// search_vertexNeighbourList (in): vertex neighbour array of the search space
	// numPolygons (in): number of polygons in search_polyList
	// minDist (out): distance from vIdx to closest polygon
	// closestPt (out): location on desired mesh closest to pt
	// doFullSearch (in): determines if a full search on the search_polyList is performed
	//						otherwise, a one-ring search is done
	// closestPolyIdx (in): index of polygon to start performing local search
	// numOneRingSearch (in): number of one-ring to search if doFullSearch is false
	unsigned int findClosestPolygon(int vIdx, double* vList, 
									double* search_vList, int** search_polyList, vector<int>* search_vertexNeighbourList,
									unsigned int numPolygons, double &minDist, double* closestPt,
									bool doFullSearch, int closestPolyIdx, unsigned int numOneRingSearch = 3) const;

	// find the polygon in search_polyList that is closest to vIdx in vList
	// if numSearch is 0, all the polygons in search_polyList are searched
	// otherwise, only the polygon indices in searchIdx are searched
	//
	// return val (out): the polygon index in search_polyList that is closest to vIdx
	// pt (in): location of the vertex we are searching for
	// search_vList (in): vertex array of the search space
	// search_polyList (in): polygon array of the search space
	// search_vertexNeighbourList (in): vertex neighbour array of the search space
	// numPolygons (in): number of polygons in search_polyList
	// minDist (out): distance from vIdx to closest polygon
	// closestPt (out): location on desired mesh closest to pt
	// doFullSearch (in): determines if a full search on the search_polyList is performed
	//						otherwise, a one-ring search is done
	// closestPolyIdx (in): index of polygon to start performing local search
	// numOneRingSearch (in): number of one-ring to search if doFullSearch is false
	unsigned int findClosestPolygon(double *pt, 
									double* search_vList, int** search_polyList, vector<int>* search_vertexNeighbourList,
									unsigned int numPolygons, double &minDist, double* closestPt,
									bool doFullSearch, int closestPolyIdx, unsigned int numOneRingSearch = 3) const;

	// returns the quadric value of a point (x, y, z) based on some quadric q
	// q (in): Quadric
	// pt (in): coord of the point
	double computeQuadricValue(Quadric q, double *pt) const;

	// returns the quadric value of a point pt based on some quadric q computed by the polygon dPolyIdx
	// pt (in): vertex location (of the nonobtuse mesh) of the quadric to be computed
	// dPolyIdx (in): index of polygon used for computing the quadric
	// alpha (in): weighting alpha value for objective function
	// numOneRingQuadric (in): number of one-ring to use for computing quadric
	//double computeQuadricValue(double *no_pt, int dPolyIdx, double alpha = 0.5, unsigned int numOneRingQuadic = 1) const;
	double computeQuadricValue(double *no_pt, int dPolyIdx, double alpha = 0.5, unsigned int numOneRingQuadic = 1) const;

	// compute optimal location for vertex (novIdx) that minimizes quadric q
	// returns true if successful
	// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
	// q (in): quadric value of novIdx
	// pt (out): moved location
	// region (in): determines how the non-obtuse region is to be approximated
	// angleBound (in): angle bound
	// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
	//                           on the one-ring vertices for the edge
	bool computeOptimalLocation(int novIdx, Quadric q, double *pt, NORegionType region, double angleBound, bool bEdgeOneRing = false);

	// compute smoothness for vIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
	// centroid (out): centroid position for 'novIdx' one-ring
	// useCentroid (in): true - use centroid to compute smoothness
	// smtype (in): determines how smoothness is computed
	//Quadric computeSmoothness(int novIdx, double *centroid, SmoothType smtype = TAUBIN) const;
	Quadric computeSmoothness(int novIdx, double *centroid, bool useCentroid, SmoothType smtype = LAPLACIAN) const;

	// compute smoothness using LAPLACIAN for vIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
	// centroid (out): centroid position for 'novIdx' one-ring
	// useCentroid (in): true - use centroid to compute smoothness
	Quadric computeLaplacianSmoothness(int novIdx, double *centroid) const;
	
	// compute smoothness using TAUBIN for vIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
	// centroid (out): centroid position for 'novIdx' one-ring
	Quadric computeTaubinSmoothness(int novIdx, double *centroid) const;

	// update centroidList after novIdx has been moved
	// novIdx (in): index of vertex that moved
	void updateCentroidList(int novIdx);

	// remove arbitrary elements from heap
	// heap (in): heap
	// idx (in): index in "heap"
	// func (in): comparison function
	// TODO: test this
	void remove_from_heap(vector<QuadricMove> &heap, unsigned int idx, bool (*func)(QuadricMove, QuadricMove));

	// update heap by moving element 'idx' to the correct location
	// heap (in): heap
	// idx (in): index in "heap"
	// func (in): comparison function
	// TODO: test this
	void update_heap(vector<QuadricMove> &heap, unsigned int idx, bool (*func)(QuadricMove, QuadricMove));

	// initialize vertex decimate quadric
	void initializeVertexDecimateQuadric();

	// compute initial decimate quadric for vIdx and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
	Quadric computeInitialDecimateQuadric(int novIdx) const;

	// compute initial decimate quadric for boundary vertex 'vIdx' and returns it
	// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
	Quadric computeInitialBoundaryDecimateQuadric(int novIdx) const;

	// returns the list of one-ring vertices for an edge into oneRingVertices (ordered by right hand rule)
	// edgeIdx (in): index of edge
	// oneRingVertices (out): list of onering vertices
	// return value (out): true if no duplicates in one ring
	bool getEdgeOneRingVertices(int edgeIdx, vector<unsigned int> &oneRingVertices) const;	

	// returns true if an edge can be decimated
	// an edge can be decimated as long as the neighbour vertices don't have degree-3
	// edgeIdx (in): index of edge
	// degreeN (in): max. of min. degrees that we want to avoid
	// return val (out): true if edge can be decimated
	bool canDecimate(unsigned int edgeIdx, unsigned int degreeN = 2) const;

	// returns true if novIdx is at boundary
	// novIdx (in): index of vertex in nonobtuse mesh to check
	// return value (out): true if novIdx is at boundary
	bool isBoundaryVertex(int novIdx) const;

	// locating the closest boundary edge to vertex 'novIdx'
	//
	// novIdx (in): index of the vertex
	// pt (in): location of novIdx
	// closestPt (out): location on desired mesh closest to pt
	// numOneRingSearch (in): depth of search
	// doFullSearch (in): true - if search exhaustively in a breadth-first manner starting with the closest triangle of 'novIdx'
	// return value (out): the boundary edge closest to 'novIdx'; (-1,-1) if none found
	Edge findClosestBoundaryEdge(int novIdx, double *pt, double* closestPt, unsigned int numOneRingSearch, bool doFullSearch) const;

	// returns the quadric value of a point pt based on some quadric q computed by the polygon dPolyIdx
	// no_pt (in): vertex location (of the nonobtuse mesh) of the quadric to be computed
	// closestEdge (in): closest boundary edge used for computing the quadric
	// alpha (in): weighting alpha value for objective function
	// numOneRingQuadric (in): number of one-ring to use for computing quadric
	//double computeBoundaryQuadricValue(double *no_pt, Edge closestEdge, double alpha, unsigned int numOneRingQuadric) const;
	double computeBoundaryQuadricValue(double *no_pt, double* closestPt, double alpha, unsigned int numOneRingQuadric) const;

	// compute smoothness using LAPLACIAN for boundary vertex vIdx and returns it
	// novIdx (in): boundary vertex index (of the nonobtuse mesh) of the smoothness to be computed
	// centroid (out): centroid position for 'novIdx' one-ring
	Quadric computeBoundaryLaplacianSmoothness(int novIdx, double *centroid) const;

	// remove the interior vertex 'novIdx' and retriangulate region
	//
	// novIdx (in): index of vertex to be removed (interior vertex)
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound
	// return value (out): true if successful
	bool removeInterior4AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound);

	// remove the boundary valence-2 vertex 'novIdx'
	//
	// novIdx (in): index of vertex to be removed (boundary vertex)
	// return value (out): true if successful
	bool removeBoundary2(unsigned int novIdx);

	// remove the boundary valence-3 vertex 'novIdx' and triangulate
	//
	// novIdx (in): index of vertex to be removed (boundary vertex)
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound
	// return value (out): true if successful
	bool removeBoundary3AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound);
	
	// remove the interior valence-3 vertex 'novIdx'
	//
	// novIdx (in): index of vertex to be removed (interior vertex)
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound
	// return value (out): true if successful
	bool removeInterior3AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound);

	// remove the interior valence-4 vertex 'novIdx' by way of edge collapse
	//
	// novIdx (in): index of vertex to be removed (interior vertex)
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound (in radians)
	// return value (out): true if successful
	bool removeInterior4ByEdgeCollapse(unsigned int novIdx, NORegionType region, double angleBound);

	// remove the boundary valence-3 vertex 'novIdx' by way of edge collapse
	//
	// novIdx (in): index of vertex to be removed (boundary vertex)
	// region (in): determines how the non-obtuse region is to be approximated 
	// angleBound (in): angle bound (in radians)
	// return value (out): true if successful
	bool removeBoundary3ByEdgeCollapse(unsigned int novIdx, NORegionType region, double angleBound);

	// returns the list of one-ring vertices for a vertex on the nonobtuse mesh into oneRingVertices (ordered by right hand rule)
	// novIdx (in): index of Vertex
	// oneRingVertices (out): list of onering vertices
	// return value (out): true - if no duplicates in one-ring
	bool getOneRingVertices(int novIdx, vector<unsigned int> &oneRingVertices) const;

	// compute optimal location for vertex (novIdx) that minimizes quadric q using the linear search method
	// returns true if successful
	// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
	// q (in): quadric value of novIdx
	// pt (out): moved location
	// angleBound (in): angle bound
	// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
	//                           on the one-ring vertices for the edge
	bool computeOptimalLocation_linearSearch(int novIdx, Quadric q, double *pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing = false);

	// aux. function for computeOptimalLocation_linearSearch; does linear search between old_pt and optimal_pt
	// returns true if successful
	// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
	// old_pt (in/out): one end of linear search / best search point
	// optimal_pt (in): the other end of linear search
	// angleBound (in): angle bound
	// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
	//                           on the one-ring vertices for the edge
	bool computeOptimalLocation_linearSearch_aux(int novIdx, double *old_pt, double *optimal_pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing);

	// compute optimal location for vertex (novIdx) that minimizes quadric q using the radii random probing method
	// returns true if successful
	// novIdx (in): index of the vertex that is being moved (or edge index) if bEdgeOneRing is set to true
	// q (in): quadric value of novIdx
	// pt (in/out): location before/location after
	// angleBound (in): angle bound
	// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
	//                           on the one-ring vertices for the edge
	// TODO: add code for edges
	bool computeOptimalLocation_radiiRandomProbing(int novIdx, Quadric q, double *pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing = false);

	// number of vertices in dVerticesList
	int numDVertices;
	// pointer to desired mesh's vertices list (shallow copy); will clean when destroyed
	double* dVerticesList;
	// number of polygons in dPolygonsList
	int numDPolygons;
	// pointer to desired mesh's polygons list (shallow copy); will clean when destroyed
	int** dPolygonsList;
	// stores the polygon index that a vertex is part of in desired mesh
	vector<int>* dVertexNeighbourList;
	// stores the edge neighbour information
	vector<EdgeNeighbours>* dEdgeNeighbourList;
	// stores the polygon neighbour information
	vector<int>* dPolygonNeighbourList;
	// list of face normals of desired mesh
	double* dFaceNormalsList;
	// list of vertex normals of desired mesh
	double* dVertexNormalsList;
	
	// number of vertices in noVerticesList
	int numNoVertices;
	// pointer to nonobtuse mesh's vertices list (shallow copy); will clean when destroyed
	double* noVerticesList;
	// number of polygons in noPolygonsList
	int numNoPolygons;
	// pointer to nonobtuse mesh's polygons list (shallow copy); will clean when destroyed
	int** noPolygonsList;
	// stores the polygon index that a vertex is part of in nonobtuse mesh
	vector<int>* noVertexNeighbourList;
	// list of face normals of nonobtuse mesh
	double* noFaceNormalsList;
	// list of vertex normals of nonobtuse mesh
	double* noVertexNormalsList;
	// list of edges in nonobtuse mesh
	vector<Edge> noEdgeList;
	// list of indices to edges stored in noEdgeList
	vector<int>* noVertexEdgeList;
	// list of edge indices that are unused
	vector<int> unusedEdgeIdxList;
	
	// stores the index of the polygon in the desired mesh that is closest to a vertex in the nonobtuse mesh
	// an index of -1 means it's unassigned
	int* closestFaceList;

	// stores the boundary edge in the desired mesh that is closest to a vertex in the nonobtuse mesh
	// an index of -1 in Edge means it's unassigned
	Edge* closestBoundaryEdgeList;

	// stores the closest point in the desired mesh that is closest to a vertex in the nonobtuse mesh
	double* closestPtList;

	// list of quadric moves for each vertex in non-obtuse mesh
	// when sort_heap(), the list is sorted in order based on my_quadMove_largestImprovementFirst()
	// used by optimize_afterMove()
// 	vector<QuadricMove> qMoveList;

	// list of pointers to quadric moves of each vertex in non-obtuse mesh
	// when sort_heap(), the list is sorted in order based on my_quadMove_largestImprovementFirst()
	// used by optimize_afterMove()
	vector<QuadricMove*> qMovePtrList;

	// list of indices to elements in qMoveList
// 	vector<unsigned int> qMoveIndicesList;

	// list of sorted pointers to quadric moves of each vertex
	vector<QuadricMove*> qMoveSortPtrList;

	list<QuadricMove*> dirtyOptimizeQMs;			// stores the list of dirty QM
	vector<QuadricMove**> dirtyOptimizeBackLinks;	// sores the back links from vIdx to indices of dirtyQMs

	// list of square distance from one-ring centroid for each vertex in non-obtuse mesh
	// sort_heap() orders it from smallest to largest quadricImprovement
	// used by optimize_smoothing()
	vector<QuadricMove> qSmoothList;

	// list of indices to elements in qSmoothList
	vector<unsigned int> qSmoothIndicesList;

	// list of quadric moves for each edge in non-obtuse mesh
	// make_heap() orders it
	vector<QuadricMove> qDecimateList;

	// list of indices to elements in qDecimateList
	vector<unsigned int> qDecimateIndicesList;

	// list of pointers to quadric moves of each edge in non-obtuse mesh
	vector<QuadricMove*> qDecimatePtrList;

	// list of sorted pointers to quadric moves of each edge
	vector<QuadricMove*> qDecimateSortPtrList;

	list<QuadricMove*> dirtyDecimateQMs;			// list of decimate dirtyQMs
	vector<QuadricMove**> dirtyDecimateBackLinks;	// ptrs to ptrs to decimate dirtyQMs

	// list of (decimate) quadric for each vertex in non-obtuse mesh
	vector<Quadric> qVertexDecimateList;

	// number of planes used in quadric for each vertex in qVertexDecimateList
	// if quadric is boundary quadric, then it stores number of boundary pt it uses
	vector<unsigned int> numPlanesInQuadricsList;

	// list of centroid used to compute Taubin Smoothing
	double* centroidList;
	
	// position of the last vertex that was moved (prior to the move)
	double lastMovedVertexPos[3];
	
	// index of the last vertex that was moved
	int lastMovedVertexIdx;
	
	// flag to indicate if lastMovedVertex is set
	bool isLastMovedVertexSet;
	
	// flag to determine if mesh info are loaded
	bool bIsLoaded;

	// weights for taubin smoothing
	float TAUBIN_LAMBDA;
	float TAUBIN_MU;

	// diagonal distance of desired mesh's bounding box
	double bboxDiagonal;
	
	// ---- debug only ----
	double lastMovedVertexOldPos[3];
	
	double initAvgQuadValue;

	// ---- end debug -----

	double avgSolvingTime;
	double maxSolvingTime;
	double avgSetupTime;
	double maxSetupTime;
	int numSolve;
	int numSetup;

	int numClosestFaceChanged;

	int numLinSearchFound;
	int numLinSearchInFeasibleRegion;
};

#endif
