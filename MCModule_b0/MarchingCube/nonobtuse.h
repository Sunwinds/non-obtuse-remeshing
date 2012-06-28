/***************************************************************************
 *   Copyright (C) 2005 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/
#ifndef NONOBTUSE_H
#define NONOBTUSE_H

#include "helper.h"
#include "renderer.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

/**
@author John Li
*/

struct ObtuseAngle {
	int vertexIdx;		// index of vertex that the two edges meet
	int neighbour1Idx;	// index of neighbour vertex to vertexIdx that makes up one of the edges
	int neighbour2Idx;	// index of neighbour vertex to vertexIdx that makes up the other edge
	int polyIdx;		// index of polygon the obtuse angle resides in
};

class Nonobtuse{
public:
	Nonobtuse();
	
	~Nonobtuse();

	// returns angles histogram
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// stepSize (in): size of each bin in the histogram
	// histogram (out): angles histogram
	void getAngleHistogram(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, float stepSize, vector<int> &histogram);
	
	// find all obtuse angles in mesh and set it to obtuseList
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	void findObtuseAngles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons);

	// find all obtuse angles in a polygon
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// polyIndex (in): index of the polygon
	// obAngleList (out): vector of ObtuseAngles found in polygon polyIndex
	void findObtuseAngles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, int polyIndex, vector<ObtuseAngle> &obAngleList);
	
	// find all obtuse angles in a polygon
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// v1Idx (in): index of first vertex in triangle
	// v2Idx (in): index of second vertex in triangle
	// v3Idx (in): index of third vertex in triangle
	// obAngleList (out): vector of ObtuseAngles found in polygon polyIndex
	void findObtuseAngles(const double* verticesList, int numVertices, int v1Idx, int v2Idx, int v3Idx, vector<ObtuseAngle> &obAngleList);
	
	// find all obtuse angles within the one ring of vertexIdx
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// vertexIdx (in): index of the vertex we are checking
	// vertexNeighbourList (in): list of vertex neighbours for the mesh
	// obAngleList (out): vector of ObtuseAngles found in the one ring
	void findObtuseAngles(const double* verticesList, int numVertices, 
				const int* const* polygonsList, int numPolygons,
				int vertexIdx, const vector<int>* vertexNeighbourList, vector<ObtuseAngle> &obAngleList);
	
	// find all obtuse angles within the one ring of vertexIdx, but with vertexIdx's position as the given one
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// vertexPos (in): position for vertexIdx; must be an array of 3
	// vertexIdx (in): index of the vertex we are checking
	// vertexNeighbourList (in): list of vertex neighbours for the mesh
	// obAngleList (out): vector of ObtuseAngles found in the one ring
	void findObtuseAngles(const double* verticesList, int numVertices, 
				const int* const* polygonsList, int numPolygons,
				double *vertexPos, int vertexIdx, const vector<int>* vertexNeighbourList,
				vector<ObtuseAngle> &obAngleList);
	
	// return the list of obtuse angles in mesh
	//
	// return value: list of obtuse angles in mesh
	const vector<ObtuseAngle>& getObtuseAngles();
	
	// remove all old obtuse angles
	//
	void clearObtuseAngles();
	
	// remove obtuse angles by blindly removing one at a time
	//
	// rdr (in): renderer that stores the mesh information
	void blindEdgeFlips(Renderer &rdr);
	
	// remove obtuse angles by flipping an edge that will not cause new obtuse angles
	//
	// rdr (in): renderer that stores the mesh information
	void greedyEdgeFlips(Renderer &rdr);
	
	// remove obtuse angles by lifting a vertex upwards along the vertex normal direction
	//
	// rdr (out): renderer that stores the mesh information
	void liftVertices(Renderer &rdr);
	
	// remove obtuse angles by moving a vertex to the centroid of the one ring
	//
	// rdr (out): renderer that stores the mesh information
	void moveVerticesToCentroid(Renderer &rdr);
	
	// remove obtuse angles by moving a vertex to the median of the one ring
	// median is computed by taking the average of min(x) and max(x) of the one ring vertices
	// respectively for y and z coordinate
	//
	// rdr (out): renderer that stores the mesh information
	void moveVerticesToMedian(Renderer &rdr);

	// Given a mesh (vertices + faces), returns the scalar field of it
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// scalarField (out): array of scalar field returned
	// xIntPtList (out): array of intersecting points casted by rays parallel to x-axis
	// yIntPtList (out): array of intersecting points casted by rays parallel to y-axis
	// zIntPtList (out): array of intersecting points casted by rays parallel to z-axis
	// xoffsets (out): x-offset from the origin of the mesh
	// yoffsets (out): y-offset from the origin of the mesh
	// zoffsets (out): z-offset from the origin of the mesh
	// nCellsX (in): number of cells across the x direction
	// nCellsY (in): number of cells across the y direction
	// nCellsZ (in): number of cells across the z direction
	// fCellLengthX (in): length of each cell across x direction
	// fCellLengthY (in): length of each cell across y direction
	// fCellLengthZ (in): length of each cell across z direction
	// return value (out): number of scalar fields returned
	int getScalarField(const double* verticesList, int numVertices,
			const int* const* polygonsList, int numPolygons,
			float* &scalarField, 
			vector<lineMeshIntType>* &xIntPtList,
			vector<lineMeshIntType>* &yIntPtList,
			vector<lineMeshIntType>* &zIntPtList,
			double &xoffsets, double &yoffsets, double &zoffsets,
			unsigned int nCellsX, unsigned int nCellsY,  unsigned int nCellsZ,
			float fCellLengthX, float fCellLengthY,	float fCellLengthZ);
			
	// Given a mesh (vertices + faces), writes the scalar field to a file
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// filename (in): filename of the scalarfield to write to
	// nCellsX (in): number of cells across the x direction
	// nCellsY (in): number of cells across the y direction
	// nCellsZ (in): number of cells across the z direction
	// fCellLengthX (in): length of each cell across x direction
	// fCellLengthY (in): length of each cell across y direction
	// fCellLengthZ (in): length of each cell across z direction
	// ignorePolyIdx (in): start index for polygons to be ignored
	// return value (out): true if success
	bool writeScalarField(const double* verticesList, int numVertices,
			const int* const* polygonsList, int numPolygons,
			char* filename, unsigned int nCellsX,
			unsigned int nCellsY,  unsigned int nCellsZ,
			float fCellLengthX, float fCellLengthY, float fCellLengthZ,
			int ignorePolyIdx = -1);

	// Given a mesh (vertices + faces), return all one-ring vertices that are holes
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// edgeNeighbourList (in): edge neighbour information
	// holesOneRingList (out): list of one-ring vertices seperated by delimiter -1
	// return value (out): number of holes found
	unsigned int getHoles(const double* verticesList, int numVertices,
						const int* const* polygonsList, int numPolygons,
						const vector<EdgeNeighbours>* edgeNeighbourList,
						vector<int> &holesOneRingList);

	// Given a mesh (vertices + faces), one-ring vertices that are holes, return a new mesh with holes filled up
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// holesOneRingList (in): list of one-ring vertices seperated by delimiter -1
	// numHoles (in): number of holes to fill
	// newVList (in): list of vertices in new mesh
	// numNewVertices (in): number of vertices in newVList
	// newPList (in): list of polygons in new mesh
	// numNewPolygons (in): number of polygons in newPList
	// return value (out): true if success
	bool fillHoles(const double* verticesList, int numVertices,
					const int* const* polygonsList, int numPolygons,
					const vector<int> &holesOneRingList, unsigned int numHoles,
					double* &newVList, int &numNewVertices,
					int** &newPList, int &numNewPolygons);

protected:
	
	// check if obAngle is a valid obtuse angle
	//
	// obAngle (in): obtuse angle to be checked
	// polygonsList (in): list of polygons in mesh
	// numPolygons (in): number of polygons in polygonsList
	// return value: result of the check
	bool isValidObtuseAngle(const ObtuseAngle &obAngle, const int* const* polygonsList, int numPolygons);
	
	// check if any obtuse angles exist within the quad formed by vertices with indices: endPt1Idx, endPt2Idx, v1Idx, v2Idx
	// where an edge is present inside the quad with end point: endPt1Idx, endPt2Idx
	//
	// verticesList (in): list of vertices in mesh
	// numVertices (in): number of vertices in verticesList
	// endPt1Idx (in): end point of the inscribed edge
	// endPt2Idx (in): other end point of the inscribed edge
	// v1Idx (in): 3rd other point of the quad
	// v2Idx (in): 4th point of the quad
	// return value: true if there exist an obtuse angle within the two triangles: v1Idx,endPt1Idx,endPt2Idx and v2Idx,endPt1Idx,endPt2Idx
	bool obtuseAnglesExist(const double* verticesList, int numVertices, int endPt1Idx, int endPt2Idx, int v1Idx, int v2Idx);
	
	// try to find a new vertex position for vertex with index "vertexIdx" such that all neighbour angles of vertexIdx is nonobtuse
	//
	// newPos (out): returns the new position for the vertex; must be an array of 3 elements
	// vertexIdx (in): index of the vertex we are trying to move
	// rdr (in): renderer that stores the mesh information
	// return value: true if we found a new position, false otherwise
	//bool findLiftPosition(double *newPos, int vertexIdx, Renderer rdr);
	bool liftVertex(int vertexIdx, Renderer &rdr);
	
	// find a direction for the vertex "vertexIdx" to be lifted
	// determined by the position of the centroid of the 1-ring and the position of the vertex
	//
	// vertexIdx (in): index of the vertex we are trying to move
	// liftDir (out): vector pointing to the direction vertex should be lifted; must be an array of 3
	// rdr (in): renderer that stores the mesh information
	void findLiftDirection(int vertexIdx, double *liftDir, const Renderer &rdr);

	// list of obtuse angles
	// 	obtuse angles are stored as 3 vertex indices:
	//	first: vertex the two edges meet
	//	second: vertex of one edge that connects to the first vertex
	//	third: vertex of the other edge that connects to the first vertex
	vector<ObtuseAngle> obtuseList;	
};

#endif
