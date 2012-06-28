/***************************************************************************
                                cnonobtusemc.h
                             -------------------
    begin                : Mon Nov 7 2005
    author               : John Li in 2005
    email                : ysl@cs.sfu.ca
 ***************************************************************************/
 
#ifndef CNONOBTUSEMC_H
#define CNONOBTUSEMC_H

#include "CIsoSurface.h"
#include "helper.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

#define MIDPOINT 0
#define INTERPOLATE 1
#define NONOBTUSE_INTERPOLATE 2

using namespace std;

/**
  *@author John Li
  */

template <class T> class CNonobtuseMC : public CIsoSurface<T>  {
public: 
	CNonobtuseMC();
	~CNonobtuseMC();

	// Generates the isosurface from the scalar field contained in the
	// buffer ptScalarField[].
	// Modified from the base class to give nonobtuse marching cube
	void GenerateSurface(const T* ptScalarField, const vector<lineMeshIntType>* xIntPtList, const vector<lineMeshIntType>* yIntPtList, const vector<lineMeshIntType>* zIntPtList, double xoffsets, double yoffsets, double zoffsets, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ, unsigned int surfaceType, const vector<int*> &ignoreCubes);
	
	// Generates the isosurface from the scalar field contained in the file scalarFieldFileName
	// Modified from the base class to give nonobtuse marching cube
	// return value (out): true if success
	bool GenerateSurface(char* scalarFieldFileName, T tIsoLevel, unsigned int surfaceType);

	// given a list of triangles, return a list of cubes that intersects these triangles
	void GetIgnoreCubes(unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ,
						float fCellLengthX, float fCellLengthY, float fCellLengthZ,
						double xoffsets, double yoffsets, double zoffsets,
						const double* const* exceptTriList, unsigned int numExceptTri, 
						vector<int*> &ignoreCubes);
	
	// Deletes the isosurface.
	void DeleteSurface();
	
	// returns xoffsets
	float getXOffset() const;
	// returns yoffsets
	float getYOffset() const;
	// returns zoffsets
	float getZOffset() const;

//protected:
	// Renames vertices and triangles so that they can be accessed more
	// efficiently.
	// unused vertices are moved to end of array;
	void RenameVerticesAndTriangles();

	// Returns the edge ID.
	unsigned int GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);

	// Returns the vertex ID.
	unsigned int GetVertexID(unsigned int nX, unsigned int nY, unsigned int nZ);
	
	// Calculates the intersection point of the isosurface with an
	// edge.
	POINT3DID CalculateIntersection(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo, unsigned int surfaceType);
	
	// returns true if all angles incident to testPoint is nonobtuse
	bool checkNonobtuse(double *testPoint, unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);
	
	// returns true if all angles in cube nX, nY, nZ at edge nEdgeNo incident to testPoint is nonobtuse
	bool checkNonobtuse_singleCube(double *testPoint, unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);
	
	// intersecting points from casting rays at x,y,z directions
	const vector<lineMeshIntType>* m_IntPtList[3];
	
	// offset from the origin of the mesh
	double m_offsets[3];
	
	// tableIndex for all cubes
	// ONLY VALID DURING GenerateSurface();
	unsigned int *allTableIndex;
};

#endif
