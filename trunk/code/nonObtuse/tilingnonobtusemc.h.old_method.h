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

struct EdgeOnFace {
	unsigned int v1ID;		// ID of vertex 1
	unsigned int v2ID;		// ID of vertex 2
	unsigned int triID;		// ID of triangle that contains this edge
	unsigned int v1Edge;	// indicates which edge v1 lies on
	unsigned int v2Edge;	// indicates which edge v1 lies on
};

struct Intersection {
	double intPt[3];		// intersected pt
	unsigned int triIdx;	// index of triangle that caused the intersection
	bool isFacingPos;		// true if intersected triangle is facing toward the positive direction of the axis
};

struct IntersectingTri {
	int intTriIdx;			// index of the intersecting triangle
	unsigned int groupIdx;	// index of connected component the triangle is part of
	bool insideCube;		// true if triangle is completely inside the cube
};

struct IntersectedCube {
	IntersectedCube()
	:isSorted(false), isTriangulated(false)
    {
    }

	vector<IntersectingTri> intersectedTriangle;		// list of triangle that intersects the cube
	vector<list<unsigned int> > triGroups;				// list of connected components formed by the intersected triangles; value are indices to intersectedTriangle
	vector<vector<int> > signs;							// signs used for triangulating the patch; determines inside-outside test
	bool isSorted;										// true if the intersected triangles are sorted into connected components
	vector<bool> isSignSet;								// true if the signs are set for the patch
	bool isTriangulated;								// true if the cube is triangulated
	vector<EdgeOnFace> edgesOnFaces[6];					// list of edges on the 6 faces introduced during triangulations of the cube
};

bool operator==(IntersectingTri a, IntersectingTri b);

struct VertexInfo {
	unsigned int x;				// x-coord of cube that introduced the vertex
	unsigned int y;				// y-coord of cube that introduced the vertex
	unsigned int z;				// z-coord of cube that introduced the vertex
	unsigned int patchNo;		// patch number that introduced the vertex
	vector<int> intTriIdx;			// index of intersecting triangle that caused the introduction of the vertex; empty means those special vertices that are inside the cube; merged vertex will have more than one intersecting triangle
	vector<unsigned int> triNbrs;	// list of tri IDs that uses this vertex
	unsigned int edgeID;		// edge ID the vertex lies on
};

typedef std::map<unsigned int, VertexInfo> ID2VERTEXINFO;

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
	time_t GenerateSurface(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ);

	// returns the list of associated triangle for each vertex; -1 if no associated triangle
	//
	// assoTriList (out): array of associated triangles to return
	// return value (out): number of slots in array
	const int* getAssociatedTri() const;

//protected:
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

	// determine if patch on cube(x,y,z) will introduce new triangles
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// patchNo (in): patch number we are triangulating
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// return value (out): true if patch will introduce new triangles
	bool willTriangulate(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList);

	// check the triangulated cube(x,y,z) if it needs any stitching, if so, stitch it
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	void checkStitching(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList);

	// check the triangulated cube(x,y,z) if it generates any non-manifold vertices; if so, fix it
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// openEdgesList (in/out): store the list of open edges after all stitching has been processed
	// faceOpenEdgesLieOnList (in/out): store the face the open edges are on in 'openEdgesList'
	void checkNonManifoldVertices(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, vector<EdgeOnFace> &openEdgesList, vector<unsigned int> &faceOpenEdgesLieOnList);

	// check if e1 can be stitched against neighbour cube 'nbrCubeIdx'
	//
	// e1 (in): edge 1
	// faceIdx (in): face that e1 and e2 lies on
	// nbrCubeIdx (in): index of neighbour cube
	// return value (in): true if edge can be stitched
	bool canStitch(EdgeOnFace e1, unsigned int faceIdx, unsigned int nbrCubeIdx);

	// stitch the two edges e1 and e2 together
	//
	// e1 (in): edge 1
	// e2 (in): edge 2
	// faceIdx (in): index of face where the stitching occurs
	// x (in): x-coord of cube where stitching occurs
	// y (in): y-coord of cube where stitching occurs
	// z (in): z-coord of cube where stitching occurs
	// e1nbrPatch1No (in): neighbour patch number that connects e1.v1
	// e1nbrPatch2No (in): neighbour patch number that connects e1.v2
	// e2nbrPatch1No (in): neighbour patch number that connects e2.v1
	// e2nbrPatch2No (in): neighbour patch number that connects e2.v2
	// eofs (out): list of edges on the face 'faceIdx' of the cube; will be modified to update the new vIDs
	// nbrPatch1NoList (out): stores the neighbour patch for v1 of edge stored in edgesOnFaces
	// nbrPatch2NoList (out): stores the neighbour patch for v2 of edge stored in edgesOnFaces
	void stitch(EdgeOnFace e1, EdgeOnFace e2, unsigned int faceIdx, unsigned int x, unsigned int y, unsigned int z, unsigned int e1nbrPatch1No, unsigned int e1nbrPatch2No, unsigned int e2nbrPatch1No, unsigned int e2nbrPatch2No, vector<EdgeOnFace> &eofs, vector<int> (&nbrPatch1NoList)[6], vector<int> (&nbrPatch2NoList)[6]);

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

	// find the associated triangle index to use for the new vertex at edgeNo for patchNo of cube(x,y,z)
	//
	// x (in): x-coord of cube
	// y (in): y-coord of cube
	// z (in): z-coord of cube
	// patchNo (in): patch number 
	// edgeNo (in): edge number the vertex is to be put on
	// edgeInt (in): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
	// verticesList (in): source mesh's vertices list
	// numVertices (in): number of vertices in source mesh
	// polygonsList (in): source mesh's polygons list
	// numPolygons (in): number of polygons in source mesh
	// polygonNeighbourList (in): polygon neighbour information for source mesh
	// return value (out): index of associated triangle
	int getAssociatedTriIdx(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, unsigned int edgeNo, const vector<Intersection> (&edgeInt)[12], const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList);

	// returns true if the nonmanifold vertices case is a special one that needs 2 triangles to triangulate
	//
	// commonEdge (in): the edge index where the non-manifold vertex lies on
	// edge1 (in): the (open) edge index where a neighbour of the non-manifold vertex lies on; index relative to commonEdge
	// edge2 (in): the (open) edge index where a neighbour of the non-manifold vertex lies on; index relative to diagonal cube
	bool isSpecialNonmanifoldCase(unsigned int commonEdge, unsigned int edge1, unsigned int edge2) const;

	// Returns the edge ID.
	unsigned int GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);

	// unused vertices are moved to end of array;
	void RenameVerticesAndTriangles();

	// list of cube properties
	IntersectedCube* cubesProperty;

	// stores the list of vertices existing on edge
	vector<unsigned int> *existingVerticesOnEdge;

	// a map data structure that stores all the existing vertex info; access by vertex ID
	ID2VERTEXINFO vertexInfo;

	// the next unused vertex ID; used for adding new vertices
	unsigned int nextUnusedVertexID;

	// stores the associated triangles for the renamed vertices
	int* assoTriList;

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
