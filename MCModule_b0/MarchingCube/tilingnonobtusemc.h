/***************************************************************************
 *   Copyright (C) 2006 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/
#ifndef TILINGNONOBTUSEMC_H
#define TILINGNONOBTUSEMC_H

#include "cnonobtusemc.h"
#include "helper.h"
#include <math.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <time.h>

using namespace std;

/**
@author John Li
*/

typedef map<unsigned int, Edge> ID2EDGE;

typedef map<unsigned int, vector<unsigned int> > VTOE;
typedef map<unsigned int, vector<unsigned int> > VTOTRI;
typedef map<unsigned int, vector<unsigned int> > ETOTRI;

typedef pair<unsigned int, unsigned int> VPAIR;
typedef map<VPAIR, unsigned int> VPAIRTOE;

typedef map<unsigned int, unsigned int> VTOEOFCUBE;
typedef map<unsigned int, unsigned int> ETOFACE;


struct EdgePosInfo {
	unsigned int edgeIdx;	// edge index
	unsigned int v1Edge;	// indicates which edge v1 lies on
	unsigned int v2Edge;	// indicates which edge v1 lies on
	bool isV1First;			// determine the direction of the edge on the face
};

struct CubeID {
	CubeID()
	:x(0), y(0), z(0)
    {
	};

	CubeID(unsigned int a, unsigned int b, unsigned int c)
	:x(a), y(b), z(c)
	{
	};

	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct IntersectTri {
	IntersectTri()
	:triIdx(-1), patchNo(-1)
    {
    }

	int triIdx;			// index of the intersecting triangle
	int patchNo;		// patch no. that it belongs to
	bool insideCube;	// true if triangle is completely inside the cube
};

struct Intersection {
	double intPt[3];		// intersected pt
	unsigned int triIdx;	// index of triangle that caused the intersection
	bool isFacingPos;		// true if intersected triangle is facing toward the positive direction of the axis
};

struct IntersectedCube {
	vector<IntersectTri> intersectedTriangle;	// list of intersecting triangles
	vector<vector<int> > triGroups;				// list of connected components formed by the intersected triangles; values are indices to intersectedTriangle
	vector<vector<int> > signs;					// signs used for triangulating the patch; determines inside-outside test
	vector<bool> isConsistent;					// true if patch has a consistent sign assignment
	vector<bool> isEmptyTriangulation;			// true if patch introduces no triangles
};

bool operator==(IntersectTri a, IntersectTri b);
bool operator==(EdgePosInfo a, EdgePosInfo b);

template <class T> class TilingNonobtuseMC : public CNonobtuseMC<T>
{
public:
    TilingNonobtuseMC();

    ~TilingNonobtuseMC();

	// clean up all members
	void DeleteSurface();

	// Generates the isosurface from the source mesh
	// Modified from the base class to give nonobtuse marching cube without using a scalarfield
	// surfaceType = MIDPOINT
	//
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// tIsoLevel (in): isolevel
	// nCellsX (in): number of cells in x-dim
	// nCellsY (in): number of cells in y-dim
	// nCellsZ (in): number of cells in z-dim
	// fCellLengthX (in): cell length in x-dim
	// fCellLengthY (in): cell length in y-dim
	// fCellLengthZ (in): cell length in z-dim
	// return value (out): time taken
	clock_t GenerateSurface(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ);

	// Generates the isosurface from the source mesh (using simple MC)
	// surfaceType = MIDPOINT
	//
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
	// tIsoLevel (in): isolevel
	// nCellsX (in): number of cells in x-dim
	// nCellsY (in): number of cells in y-dim
	// nCellsZ (in): number of cells in z-dim
	// fCellLengthX (in): cell length in x-dim
	// fCellLengthY (in): cell length in y-dim
	// fCellLengthZ (in): cell length in z-dim
	// return value (out): time taken
	clock_t GenerateSurface_simpleMC(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ);

	// returns the list of closest triangle for each vertex; -1 if no closest triangle
	//
	// closestTriList (out): array of associated triangles to return
	// return value (out): number of slots in array
	const int* getClosestTri() const;

//protected:

	// triangulate the intersected cubes using simple MC
	//
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
	void triangulate_simpleMC(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList);

	// returns the edge index
	//
	// x (in): cube x-coord
	// y (in): cube y-coord
	// z (in): cube z-coord
	// edgeIdx (in): edge index of cube (x,y,z)
	// return value (out): edge index being query
	unsigned int edgeIndexing(unsigned int x, unsigned int y, unsigned int z, unsigned int edgeIdx) const;

	// post-processing for eliminating non-manifold vertices introduced during construction using simpleMC
	//
	void postProcess_simpleMC();

	// returns the scalar of grid point at (x,y,z); also return the closest triangle in the original mesh
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// closestTriIdx (out): index of closest triangle in original mesh 
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
	// return val (out): scalar value of the grid point
	double getScalar(unsigned int x, unsigned int y, unsigned int z, unsigned int &closestTriIdx, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList) const;

	

	// triangulate the cube(x,y,z) based on the intersecting triangles and the sorted groups
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
 	void triangulate(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList);

	// triangulate patch on cube(x,y,z) based on the signs given in 'signs'
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// patchNo (in): patch number we are triangulating
	// edgeInt (in): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	void triangulatePatch(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, const vector<Intersection> (&edgeInt)[12], const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList);

	// based on edge intersections, determine all signs for the cube
	// array of signs for all 8 corners of the cube; 1=outside, -1=inside
	//
	// edgeInt (in): array of 12 slots; stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// patchNo (in): patch number we are triangulating
	// 
	// return value (out): true if all signs can be assigned without conflict
	bool assignSigns(const vector<Intersection> (&edgeInt)[12], unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo);

	// find edge intersection of groupIdx on cube(x,y,z)
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// groupIdx (in): group index to check
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// edgeInt (out): array of 12 slots; stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
	void findEdgeIntersection(unsigned int x, unsigned int y, unsigned int z, unsigned int groupIdx, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, vector<Intersection> (&edgeInt)[12]);

	// determine which cubes are intersected by which triangle in source mesh
	//
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	void markIntersectedCubes(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons);

	// determine which cubes are intersected by the given triangle
	//
	// triangleIdx (in): index of triangle
	// pt1x (in): x-coord of vertex 1 of triangle
	// pt1y (in): y-coord of vertex 1 of triangle
	// pt1z (in): z-coord of vertex 1 of triangle
	// pt2x (in): x-coord of vertex 2 of triangle
	// pt2y (in): y-coord of vertex 2 of triangle
	// pt2z (in): z-coord of vertex 2 of triangle
	// pt3x (in): x-coord of vertex 3 of triangle
	// pt3y (in): y-coord of vertex 3 of triangle
	// pt3z (in): z-coord of vertex 3 of triangle
	void markIntersectedCubes(int triangleIdx, double pt1x, double pt1y, double pt1z, double pt2x, double pt2y, double pt2z, double pt3x, double pt3y, double pt3z);

	// determine which cubes between the minSliceCube and maxSliceCube that are intersected by the given triangle
	//
	// triangleIdx (in): index of triangle
	// triPts (in): vertex positions of the triangle
	// minSliceCube (in:) starting slice index
	// maxSliceCube (in:) ending slice index
	// largestProjCoord (in): determines which coordinate has the largest projection area
	// scanSliceCoord (in): determines which coordinate scan slice takes place along
	// scanLineCoord (in): determines which coordinate scan line takes place along
	// cellLength_PSLCoord (in): CellLenth in terms of Proj,Slice,Line coordinate
	// nCells_PSLCoord (in): number of Cells in terms of Proj,Slice,Line coordinate
	void markIntersectedCubes(int triangleIdx, double triPts[3][3], int minSliceCube, int maxSliceCube, unsigned int largestProjCoord, unsigned int scanSliceCoord, unsigned int scanLineCoord, double cellLength_PSLCoord[3], unsigned int nCells_PSLCoord[3]);
	
	// determine which cubes are intersected by the given line segment
	//
	// triangleIdx (in): index of triangle
	// pt1ScanLineCoord (in): scan-line coord. of vertex 1 of line segment
	// pt1ProjCoord (in): projection coord. of vertex 1 of line segment
	// pt2ScanLineCoord (in): scan-line coord. of vertex 2 of line segment
	// pt2ProjCoord (in): projection coord. of vertex 2 of line segment
	// scanSliceCubeIdx (in): slice index; determines which slice the line is at
	// projCoord (in): determines which coordinate has the largest projection area
	// scanLineCoord (in): determines which coordinate scan line takes place along
	void markIntersectedCubes(int triangleIdx, double pt1ScanLineCoord, double pt1ProjCoord, double pt2ScanLineCoord, double pt2ProjCoord, unsigned int scanSliceCubeIdx, unsigned int projCoord, unsigned int scanLineCoord);

	// add triangle 'triIdx' to be intersecting cube 'cubeIdx'
	//
	// triangleIdx (in): index of intersecting triangle
	// cubeIdx (in): cube index
	// insideCube (in): true if triangle is completely inside the cube
	void addIntersectingTriangle(unsigned int triangleIdx, unsigned int cubeIdx, bool insideCube);

	// sort the intersected triangles of cubeIdx into connected components
	//
	// cubeIdx (in): index of cube to be sorted
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	void sortIntersectingTriangle(unsigned int cubeIdx, const vector<int>* polygonNeighbourList);

	// expand a connected component group by adding triIdx into groupIdx and expand it
	//
	// cubeIdx (in): index of cube to be sorted
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// groupIdx (in): index of group to expand
	// triIdx (in): index of intersecting triangle in the needSortedList array to add to the group and expand
	// needSortedList (in/out): index of intersecting triangles in intersectedTriangle need to be sorted
	void expandGroup(unsigned int cubeIdx, const vector<int>* polygonNeighbourList, unsigned int groupIdx, unsigned int triIdx, vector<unsigned int> &needSortedList);

	// compute the intersecting point on the specified edge of the cube
	//
	// nX (in): x-coord of cube
	// nY (in): y-coord of cube
	// nZ (in): z-coord of cube
	// nEdgeNo (in): edge number the vertex is to be put on
	POINT3DID computeVertexLocation(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);

	// Returns the edge ID.
	unsigned int GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo) const;

	// modified from base class
	// this class: each cube has 11 vertices
	// edge 12: (b,e) in special case 5b
	// edge 13: (b,e) in special case 9b
	// edge 14: (a,d) in special case 11c
	// edge 15: (b,e) in special case 12b
	// edge 16: (a,d) in special case 14c
	// edge 17: (b,e) in special case -7
	// edge 18: (a,d) in special case -3
	// edge 19: center of cube in special case -6
	unsigned int GetVertexID(unsigned int nX, unsigned int nY, unsigned int nZ) const;

	// Returns the face ID of cube.
	unsigned int GetFaceID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nFaceNo) const;

	// unused vertices are moved to end of array;
	//
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	void RenameVerticesAndTriangles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons);

	// check the triangulated cube(x,y,z) if it needs any stitching, if so, stitch it
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	void checkStitching(unsigned int x, unsigned int y, unsigned int z, const vector<int>* polygonNeighbourList);

	// returns true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
	//
	// edge1Idx (in): edge index 1
	// edge2Idx (in): edge index 2
	// return value (out): true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// x,y,z (in):	index of cube where stitching was invoked
	// faceIdx (in): face index of where stitching occurs on cube
	bool shouldStitch(unsigned int edge1Idx, unsigned int edge2Idx, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList);

	// returns true if patch is connected with some nontriangulable patch in adjcent cube
	// 
	// x, y, z (in): index of cube where patch is in
	// patchNo (in): patch number
	// helperCube (in): index of helper cube
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	bool isConnected_samePatch(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, CubeID helperCube, const vector<int>* polygonNeighbourList);

	// returns true if patch 1 and patch 2 are connected by a sequence of untriangulable patches
	// 
	// x1, y1, z1 (in): index of cube where patch 1 is in
	// patch1No (in): patch 1
	// x2, y2, z2 (in): index of cube where patch 2 is in
	// patch2No (in): patch 2
	// helperCube1 (in): index of helper cube 1
	// helperCube2 (in): index of helper cube 2
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	bool isConnected(unsigned int x1, unsigned int y1, unsigned int z1, unsigned int patch1No, unsigned int x2, unsigned int y2, unsigned int z2, unsigned int patch2No, CubeID helperCube1, CubeID helperCube2, const vector<int>* polygonNeighbourList);

	// check if the triangles in checkTriList are all connected by the helper triangles in helperList
	//
	// checkTriList (in/out): index of triangles to see if one patch can be formed
	// helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// triIdx (in): index of needSortedList array to add to the patch and expand
	// return value (out): true if the triangles in needSortedList are all connected into one big patch
	bool isOnePatch(vector<unsigned int> &checkTriList, vector<unsigned int> &helperList, const vector<int>* polygonNeighbourList);

	// check if the triangles in needSortedList are all connected into one big patch
	//
	// checkTriList (in/out): index of triangles to see if one patch can be formed
	// helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// index (in): index of needSortedList array to add to the patch and expand
	// indexFromCheckTriList (in): true if index is for checkTriList
	// return value (out): true if the triangles in needSortedList are all connected into one big patch
	void isOnePatch_aux(vector<unsigned int> &checkTriList, vector<unsigned int> &helperList, const vector<int>* polygonNeighbourList, unsigned int index, bool indexFromCheckTriList);

	// stitch the two edges e1 and e2 together
	//
	// e1Info (in): edge info 1
	// e2Info (in): edge info 2
	// x,y,z (in): index of cube we are stitching at
	// faceIdx (in): face index of where stitching occurs
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// return value (out): true if stitching successful
	bool stitch(EdgePosInfo e1Info, EdgePosInfo e2Info, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList);

	// Stitch overlapping edges; only call from stitch() to ensure we dont' introduce many overlapping vertices and run into infinite loops
	//
	// eInfo (in): info of edge that we want to stitch up with
	// x,y,z (in):	index of cube where stitching was invoked
	// faceIdx (in): face index of where stitching occurs on cube
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// return value (out): true if edge is stitched
	bool stitchOverlappingEdges(EdgePosInfo eInfo, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList);

	// join vertex v1Idx and v2Idx together
	//
	// v1Idx (in): index of vertex 1
	// v2Idx (in): index of vertex 2
	// return value (out): vertex ID of the joined vertex
	unsigned int stitchVertices(unsigned int v1Idx, unsigned int v2Idx);

	// check if vertex v1Idx and v2Idx can be joined together
	//
	// v1Idx (in): index of vertex 1
	// v2Idx (in): index of vertex 2
	// return value (out): true if can join
	unsigned int canVerticesJoin(unsigned int v1Idx, unsigned int v2Idx);

	// list of cube properties
	IntersectedCube* cubesProperty;
 
	// list of vertices created on edges of cube
	vector<vector<int> > v_on_e;

	// stores the relationship between v and which edge e it is on
	VTOEOFCUBE v_to_eOfCube;
 
	// list of edges created on faces of cube
	vector<vector<EdgePosInfo> > e_on_f;

	// stores the relationship between e and which face f it is on
	ETOFACE e_to_face;
 
	// list of edges in the constructed mesh
	ID2EDGE edgeList;

	// stores relationship of (v1,v2) to edge index
	VPAIRTOE vpair_to_e;

	// stores relationship of what edges are connected to v
	VTOE v_to_e;

	// stores relationship of what triangles are connected to v
	VTOTRI v_to_tri;

	// stores relationship of what triangles are connected to e
	ETOTRI e_to_tri;
	
	// stores relationship of what patch is responsible for creating the triangle
	vector<vector<unsigned int> > tri_to_patch;

	// stores relationship of which cube the triangle is in
	vector<vector<CubeID> > tri_to_CubeID;

	// the next unused vertex ID; used for adding new vertices
	unsigned int nextUnusedVertexID;

	// the next unused Edge ID; usd for adding new edges
	unsigned int nextUnusedEdgeID;

	// stores the associated triangles for the renamed vertices
	int* closestTriList;

	// number of cubes with multiple patches
	int numCubeMultiPatch;

	// number of patches ignored
	int numPatchIgnored;

	// number of faces needed for stitching
	int numFaceStitching;	
	
	// number of times non-manifold vertices were fixed
	int numNonManifoldFix;
};

#endif
