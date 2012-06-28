/***************************************************************************
                                cnonobtusemc.h
                             -------------------
    begin                : Mon Nov 7 2005
    author               : John Li in 2005
    email                : ysl@cs.sfu.ca
 ***************************************************************************/
 
#include "cnonobtusemc.h"

template <class T> CNonobtuseMC<T>::CNonobtuseMC()
:CIsoSurface<T>::CIsoSurface(), allTableIndex(NULL)
{
}

template <class T> CNonobtuseMC<T>::~CNonobtuseMC()
{
	this->DeleteSurface();
}

template <class T> void CNonobtuseMC<T>::GenerateSurface(const T* ptScalarField, const vector<lineMeshIntType>* xIntPtList, const vector<lineMeshIntType>* yIntPtList, const vector<lineMeshIntType>* zIntPtList, double xoffsets, double yoffsets, double zoffsets, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ, unsigned int surfaceType, const vector<int*> &ignoreCubes)
{
	if (this->m_bValidSurface)
		this->DeleteSurface();

	this->m_tIsoLevel = tIsoLevel;
	this->m_nCellsX = nCellsX;
	this->m_nCellsY = nCellsY;
	this->m_nCellsZ = nCellsZ;
	this->m_fCellLengthX = fCellLengthX;
	this->m_fCellLengthY = fCellLengthY;
	this->m_fCellLengthZ = fCellLengthZ;
	
	this->m_ptScalarField = ptScalarField;
	m_IntPtList[0] = xIntPtList;
	m_IntPtList[1] = yIntPtList;
	m_IntPtList[2] = zIntPtList;
	m_offsets[0] = xoffsets;
	m_offsets[1] = yoffsets;
	m_offsets[2] = zoffsets;

	unsigned int nPointsInXDirection = (this->m_nCellsX + 1);
	unsigned int nPointsInSlice = nPointsInXDirection*(this->m_nCellsY + 1);
	allTableIndex = new unsigned int[nCellsX * nCellsY * nCellsZ];
	
	unsigned int currentIgnoreCubeIdx = 0;

	// Generate isosurface.
	for (unsigned int z = 0; z < this->m_nCellsZ; z++)
		for (unsigned int y = 0; y < this->m_nCellsY; y++)
			for (unsigned int x = 0; x < this->m_nCellsX; x++) {
				// first check if the cube intersects any of the triangles in exceptTriList
				// if so, make sure we don't triangulate the cube
				bool intersects(false);
				if (ignoreCubes.size() > currentIgnoreCubeIdx && 
					ignoreCubes[currentIgnoreCubeIdx][0] == (int)x &&
					ignoreCubes[currentIgnoreCubeIdx][1] == (int)y &&
					ignoreCubes[currentIgnoreCubeIdx][2] == (int)z)
				{
					intersects = true;
					++currentIgnoreCubeIdx;
				}
				

				// Calculate table lookup index from those
				// vertices which are below the isolevel.
				unsigned int tableIndex = 0;
				//    1----2
				//   /|  / |
				// 5----6  |
				// | /0-|--3
				// |/   |/
				// 4----7
				
				if (this->m_ptScalarField[z*nPointsInSlice + y*nPointsInXDirection + x] < this->m_tIsoLevel)
					tableIndex |= 1;
				if (this->m_ptScalarField[z*nPointsInSlice + (y+1)*nPointsInXDirection + x] < this->m_tIsoLevel)
					tableIndex |= 2;
				if (this->m_ptScalarField[z*nPointsInSlice + (y+1)*nPointsInXDirection + (x+1)] < this->m_tIsoLevel)
					tableIndex |= 4;
				if (this->m_ptScalarField[z*nPointsInSlice + y*nPointsInXDirection + (x+1)] < this->m_tIsoLevel)
					tableIndex |= 8;
				if (this->m_ptScalarField[(z+1)*nPointsInSlice + y*nPointsInXDirection + x] < this->m_tIsoLevel)
					tableIndex |= 16;
				if (this->m_ptScalarField[(z+1)*nPointsInSlice + (y+1)*nPointsInXDirection + x] < this->m_tIsoLevel)
					tableIndex |= 32;
				if (this->m_ptScalarField[(z+1)*nPointsInSlice + (y+1)*nPointsInXDirection + (x+1)] < this->m_tIsoLevel)
					tableIndex |= 64;
				if (this->m_ptScalarField[(z+1)*nPointsInSlice + y*nPointsInXDirection + (x+1)] < this->m_tIsoLevel)
					tableIndex |= 128;
				
				allTableIndex[z*nCellsX*nCellsY + y*nCellsX + x] = tableIndex;
					
				// Now create a triangulation of the isosurface in this
				// cell.
				if (this->m_edgeTable[tableIndex] != 0)
				{
					if (this->m_edgeTable[tableIndex] & 8) {
						POINT3DID pt = this->CalculateIntersection(x, y, z, 3, surfaceType);
						pt.newID = 0;
						unsigned int id = GetEdgeID(x, y, z, 3);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
					}
					if (this->m_edgeTable[tableIndex] & 1) {
						POINT3DID pt = this->CalculateIntersection(x, y, z, 0, surfaceType);
						pt.newID = 0;
						unsigned int id = GetEdgeID(x, y, z, 0);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
					}
					if (this->m_edgeTable[tableIndex] & 256) {
						POINT3DID pt = this->CalculateIntersection(x, y, z, 8, surfaceType);
						pt.newID = 0;
						unsigned int id = GetEdgeID(x, y, z, 8);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
					}

					if (x == this->m_nCellsX - 1) {
						if (this->m_edgeTable[tableIndex] & 4) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 2, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 2);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
						if (this->m_edgeTable[tableIndex] & 2048) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 11, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 11);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
					}
					if (y == this->m_nCellsY - 1) {
						if (this->m_edgeTable[tableIndex] & 2) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 1, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 1);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
						if (this->m_edgeTable[tableIndex] & 512) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 9, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 9);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
					}
					if (z == this->m_nCellsZ - 1) {
						if (this->m_edgeTable[tableIndex] & 16) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 4, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 4);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
						if (this->m_edgeTable[tableIndex] & 128) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 7, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 7);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
					}
					if ((x == this->m_nCellsX - 1) && (y == this->m_nCellsY - 1))
						if (this->m_edgeTable[tableIndex] & 1024) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 10, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 10);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
					if ((x == this->m_nCellsX - 1) && (z == this->m_nCellsZ - 1))
						if (this->m_edgeTable[tableIndex] & 64) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 6, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 6);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
					if ((y == this->m_nCellsY - 1) && (z == this->m_nCellsZ - 1))
						if (this->m_edgeTable[tableIndex] & 32) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 5, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 5);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}

					if (intersects)
						continue;
					else if  (this->m_triTable[tableIndex][0] == -3)
					{
						// -3:	Special Case -3
						// {-3, a, b, c, d, e, f, -1, ... }
						// where a,b,c,d forms a quad, and d,e,f,a forms the other quad
						// The right hand ordering determines the normals
						// A new point m should be added to the midpoint of a,d
						// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
						
						// get point a, d
						POINT3DID pta;
						POINT3DID ptd;
						if (surfaceType == NONOBTUSE_INTERPOLATE)
						{
							pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], MIDPOINT);
							ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], MIDPOINT);
						}
						else
						{
							pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
							ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);
						}

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 18);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
						
						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
							pointID2 = GetEdgeID(x, y, z, 18);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					else if (this->m_triTable[tableIndex][0] == -5)
					{
						// -5:  Special Case 5b
						// {-5, a, b, c, d, e, -1, ... }
						// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
						// The right hand ordering determins the normals
						// A new point m should be added to the midpoint of b,e
						// Then, the 5 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, -1}

						// get point b, e
						POINT3DID ptb;
						POINT3DID pte;
						if (surfaceType == NONOBTUSE_INTERPOLATE)
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], MIDPOINT);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], MIDPOINT);
						}
						else
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
						}

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 12);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
            
						// create triangles
						for (unsigned int i = 1; i <= 5; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%5 + 1]);
							pointID2 = GetEdgeID(x, y, z, 12);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					else if (this->m_triTable[tableIndex][0] == -7)
					{
						// -7:  Special Case -7
						// {-7, a, b, c, d, e, f, i, j, k, -1, ... }
						// where a,b,c,d,e,f forms a hexagon and i,j,k forms a triangle
						// The right hand ordering determines the normals
						// A new point m should be added to the centroid of a,b,c,d,e,f
						// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, i, j, k, -1}
						
						
						// get point b, e
						POINT3DID ptb;
						POINT3DID pte;
						if (surfaceType == NONOBTUSE_INTERPOLATE)
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], MIDPOINT);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], MIDPOINT);
						}
						else
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
						}

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 17);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 7; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							if (i == 7)
							{
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+1]);
								pointID2 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+2]);
							}
							else
							{
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								pointID2 = GetEdgeID(x, y, z, 17);
							}
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					else if (this->m_triTable[tableIndex][0] == -9)
					{
						// -9:  Special Case 9b
						// {-9, a, b, c, d, e, f, -1, ... }
						// where a,b,c,d,e,f forms a hexagon.
						// The right hand ordering determines the normals
						// A new point m should be added to the centroid of a,b,c,d,e,f
						// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

						// get point b, e
						POINT3DID ptb;
						POINT3DID pte;
						if (surfaceType == NONOBTUSE_INTERPOLATE)
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], MIDPOINT);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], MIDPOINT);
						}
						else
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
						}

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 13);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
							pointID2 = GetEdgeID(x, y, z, 13);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					/*
					else if (this->m_triTable[tableIndex][0] == -11)
					{
						// -11: Special Case 11b	*** OBSOLETE ***
						// {-11, a, b, c, d, e, f, -1, ... ]
						// where a,b,f forms the triangle that has degree 120 at angle(fab)
						//       b,c,d and d,e,f forms the 2 triangles that have right angle
						//       b,d,f forms the equilateral triangle
						// The right hand ordering determines the normals
						// A new point m should be added to the midpoint of b,f
						// Then, the 6 new triangles are {a, b, m, b, d, m, c, d, b, d, f, m, e, f, d, f, a, m, -1}

						// get point b, f
						POINT3DID ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
						POINT3DID ptf = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][6], surfaceType);

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, ptf.x, ptf.y, ptf.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 14);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;

							unsigned int idx2 = 0, idx3 = 0;
							switch (i) {
								case 1:
									idx2 = this->m_triTable[tableIndex][2];
									idx3 = 14;
									break;
								case 2:
									idx2 = this->m_triTable[tableIndex][4];
									idx3 = 14;
									break;
								case 3:
									idx2 = this->m_triTable[tableIndex][4];
									idx3 = this->m_triTable[tableIndex][2];
									break;
								case 4:
									idx2 = this->m_triTable[tableIndex][6];
									idx3 = 14;
									break;
								case 5:
									idx2 = this->m_triTable[tableIndex][6];
									idx3 = this->m_triTable[tableIndex][4];
									break;
								case 6:
									idx2 = this->m_triTable[tableIndex][1];
									idx3 = 14;
									break;
							}
							
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, idx2);
							pointID2 = GetEdgeID(x, y, z, idx3);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}*/
					else if (this->m_triTable[tableIndex][0] == -11)	
					{
						// -11: Special Case 11c
						// {-11, a, b, c, d, e, f, -1, ... ]
						// where a,b,c,d,e,f forms a bent hexagon.
						//       new point m is added in the center of the cube
						//       b,c,m and e,f,m are right isoscele triangles
						//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
						// The right hand ordering determines the normals
						// A new point m should be added to the center of cube (ie. midpoint of a and d)
						// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

						// get point a, d
						POINT3DID pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
						POINT3DID ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 14);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
							pointID2 = GetEdgeID(x, y, z, 14);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					else if (this->m_triTable[tableIndex][0] == -12)
					{
						// -12: Special Case 12b
						// {-12, a, b, c, d, e, i, j, k, -1, ... }
						// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
						//       and i, j, k forms a seperate triangle
						// The right hand ordering determines the normals
						// A new point m should be added to the midpoint of b,e
						// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, a, m, i, j, k, -1}

						// get point b, e
						POINT3DID ptb;
						POINT3DID pte;
						if (surfaceType == NONOBTUSE_INTERPOLATE)
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], MIDPOINT);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], MIDPOINT);
						}
						else
						{
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
						}

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 15);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;

							unsigned int idx2 = this->m_triTable[tableIndex][i%5 + 1];
							unsigned int idx3 = 15;
							if (i == 6)
							{
								idx2 = this->m_triTable[tableIndex][i+1];
								idx3 = this->m_triTable[tableIndex][i+2];
							}
							
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, idx2);
							pointID2 = GetEdgeID(x, y, z, idx3);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}/*
					else if (this->m_triTable[tableIndex][0] == -14)
					{
						// -14: Special Case 14b	*** OBSOLETE ***
						// {-14, a, b, c, d, e, f, -1, ... ]
						// where a,b,f forms the triangle that has degree 120 at angle(fab)
						//       b,c,d and d,e,f forms the 2 triangles that have right angle
						//       b,d,f forms the equilateral triangle                      	
						// The right hand ordering determines the normals
						// A new point m should be added to the midpoint of b,f
						// Then, the 6 new triangles are {a, b, m, b, d, m, c, d, b, d, f, m, e, f, d, f, a, m, -1}

						// get point b, f
						POINT3DID ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
						POINT3DID ptf = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][6], surfaceType);

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, ptb.x, ptb.y, ptb.z, ptf.x, ptf.y, ptf.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 16);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;

							unsigned int idx2 = 0, idx3 = 0;
							switch (i) {
								case 1:
									idx2 = this->m_triTable[tableIndex][2];
									idx3 = 16;
									break;
								case 2:
									idx2 = this->m_triTable[tableIndex][4];
									idx3 = 16;
									break;
								case 3:
									idx2 = this->m_triTable[tableIndex][4];
									idx3 = this->m_triTable[tableIndex][2];
									break;
								case 4:
									idx2 = this->m_triTable[tableIndex][6];
									idx3 = 16;
									break;
								case 5:
									idx2 = this->m_triTable[tableIndex][6];
									idx3 = this->m_triTable[tableIndex][4];
									break;
								case 6:
									idx2 = this->m_triTable[tableIndex][1];
									idx3 = 16;
									break;
							}

							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, idx2);
							pointID2 = GetEdgeID(x, y, z, idx3);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}*/
					else if (this->m_triTable[tableIndex][0] == -14)	
					{
						// -14: Special Case 14c
						// {-14, a, b, c, d, e, f, -1, ... ]
						// where a,b,c,d,e,f forms a bent hexagon.
						//       new point m is added in the center of the cube
						//       b,c,m and e,f,m are right isoscele triangles
						//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
						// The right hand ordering determines the normals
						// A new point m should be added to the center of cube (ie. midpoint of a and d)
						// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

						// get point a, d
						POINT3DID pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
						POINT3DID ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);

						// calculate midpoint m
						double *m = new double[3];
						midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
						POINT3DID ptm;
						ptm.newID = 0;
						ptm.x = m[0];
						ptm.y = m[1];
						ptm.z = m[2];
						delete[] m;

						// add vertex m
						unsigned int id = GetEdgeID(x, y, z, 16);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));

						// create triangles
						for (unsigned int i = 1; i <= 6; ++i) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
							pointID2 = GetEdgeID(x, y, z, 16);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
					else
					{
						for (unsigned int i = 0; this->m_triTable[tableIndex][i] != -1; i += 3) {
							TRIANGLE triangle;
							unsigned int pointID0, pointID1, pointID2;
							pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
							pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+1]);
							pointID2 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+2]);
							triangle.pointID[0] = pointID0;
							triangle.pointID[1] = pointID1;
							triangle.pointID[2] = pointID2;
							this->m_trivecTriangles.push_back(triangle);
						}
					}
				}
			}

	this->RenameVerticesAndTriangles();
	this->CalculateNormals();
	this->m_bValidSurface = true;
	
	delete[] allTableIndex;
	allTableIndex = NULL;
}

// Generates the isosurface from the scalar field contained in the file scalarFieldFileName
// Modified from the base class to give nonobtuse marching cube
// return value (out): true if success
template <class T> bool CNonobtuseMC<T>::GenerateSurface(char* scalarFieldFileName, T tIsoLevel, unsigned int surfaceType)
{
	if (this->m_bValidSurface)
		this->DeleteSurface();
	
	ifstream inFile;
	inFile.open(scalarFieldFileName, ifstream::in);
	
	if (!inFile.is_open())
		return false;
	
	// get scalar field properties
	unsigned int numScalars(0);
	unsigned int nCellsX;
	unsigned int nCellsY;
	unsigned int nCellsZ;
	float fCellLengthX;
	float fCellLengthY;
	float fCellLengthZ;
	double xoffsets, yoffsets, zoffsets;
	if (inFile.good())
	{
		char tmp[16];
		inFile >> tmp;
		inFile >> numScalars;
		
		inFile >> tmp;
		inFile >> nCellsX;
		inFile >> tmp;
		inFile >> nCellsY;
		inFile >> tmp;
		inFile >> nCellsZ;
		
		inFile >> tmp;
		inFile >> fCellLengthX;
		inFile >> tmp;
		inFile >> fCellLengthY;
		inFile >> tmp;
		inFile >> fCellLengthZ;
		
		inFile >> tmp;
		inFile >> xoffsets;
		inFile >> tmp;
		inFile >> yoffsets;
		inFile >> tmp;
		inFile >> zoffsets;
	}
	else
		return false;

	double** exceptTriList = NULL;
	unsigned int numExceptTri = 0;
	vector<int*> ignoreCubes;
	if (inFile.good())
	{
		char tmp[16];
		inFile >> tmp;
		inFile >> numExceptTri;
		if (numExceptTri > 0)
		{
			exceptTriList = new double*[3*numExceptTri];
			for (unsigned int i = 0; i < numExceptTri; ++i)
			{
				exceptTriList[3*i] = new double[3];
				inFile >> exceptTriList[3*i][0];
				inFile >> exceptTriList[3*i][1];
				inFile >> exceptTriList[3*i][2];
				exceptTriList[3*i+1] = new double[3];
				inFile >> exceptTriList[3*i+1][0];
				inFile >> exceptTriList[3*i+1][1];
				inFile >> exceptTriList[3*i+1][2];
				exceptTriList[3*i+2] = new double[3];
				inFile >> exceptTriList[3*i+2][0];
				inFile >> exceptTriList[3*i+2][1];
				inFile >> exceptTriList[3*i+2][2];
			}
			
			// --- get cubes to ignore
			this->GetIgnoreCubes(nCellsX, nCellsY, nCellsZ, fCellLengthX, fCellLengthY, fCellLengthZ,
								xoffsets, yoffsets, zoffsets, exceptTriList, numExceptTri, ignoreCubes);
			
		}
	}
	else
		return false;
	
	// get scalar field
	T *scalarField = new T[numScalars];
	for (unsigned int i = 0; i < numScalars && inFile.good(); ++i)
	{	
		inFile >> scalarField[i];
	}
	
	// get intersecting points from x rays
	unsigned int numXRays = (nCellsY+1)*(nCellsZ+1);
	vector<lineMeshIntType>* xIntPtList = new vector<lineMeshIntType>[numXRays];
	for (unsigned int i = 0; i < numXRays && inFile.good(); ++i)
	{
		unsigned int numIntPts;
		inFile >> numIntPts;
		for (unsigned int j = 0; j < numIntPts && inFile.good(); ++j)
		{
			lineMeshIntType intPt;
			inFile >> intPt.intType;
			inFile >> intPt.pt[0];
			inFile >> intPt.pt[1];
			inFile >> intPt.pt[2];
			xIntPtList[i].push_back(intPt);
		}
	}
	
	// get intersecting points from y rays
	unsigned int numYRays = (nCellsX+1)*(nCellsZ+1);
	vector<lineMeshIntType>* yIntPtList = new vector<lineMeshIntType>[numYRays];
	for (unsigned int i = 0; i < numYRays && inFile.good(); ++i)
	{
		unsigned int numIntPts;
		inFile >> numIntPts;
		for (unsigned int j = 0; j < numIntPts && inFile.good(); ++j)
		{
			lineMeshIntType intPt;
			inFile >> intPt.intType;
			inFile >> intPt.pt[0];
			inFile >> intPt.pt[1];
			inFile >> intPt.pt[2];
			yIntPtList[i].push_back(intPt);
		}
	}
	
	// get intersecting points from z rays
	unsigned int numZRays = (nCellsX+1)*(nCellsY+1);
	vector<lineMeshIntType>* zIntPtList = new vector<lineMeshIntType>[numZRays];
	for (unsigned int i = 0; i < numZRays && inFile.good(); ++i)
	{
		unsigned int numIntPts;
		inFile >> numIntPts;
		for (unsigned int j = 0; j < numIntPts && inFile.good(); ++j)
		{
			lineMeshIntType intPt;
			inFile >> intPt.intType;
			inFile >> intPt.pt[0];
			inFile >> intPt.pt[1];
			inFile >> intPt.pt[2];
			zIntPtList[i].push_back(intPt);
		}
	}
	
	GenerateSurface(scalarField, xIntPtList, yIntPtList, zIntPtList, xoffsets, yoffsets, zoffsets, tIsoLevel, nCellsX, nCellsY, nCellsZ, fCellLengthX, fCellLengthY, fCellLengthZ, surfaceType, ignoreCubes);
	
	// TODO: commenting this statement is for debug only, uncomment afterwards.
	//delete[] scalarField;
	delete[] xIntPtList;
	delete[] yIntPtList;
	delete[] zIntPtList;

	for (unsigned int i = 0; i < 3*numExceptTri; ++i)
		delete[] exceptTriList[i];
	delete[] exceptTriList;
	
	inFile.close();
	
	cout << "\nScalar field successfully loaded from: " << scalarFieldFileName << endl;
	
	return true;
}

// given a list of triangles, return a list of cubes that intersects these triangles
template <class T> void CNonobtuseMC<T>::GetIgnoreCubes(unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ,
														float fCellLengthX, float fCellLengthY, float fCellLengthZ,
														double xoffsets, double yoffsets, double zoffsets,
														const double* const* exceptTriList, unsigned int numExceptTri, 
														vector<int*> &ignoreCubes)
{
	for (unsigned int i = 0; i < numExceptTri; ++i)
	{
		// find min x, y, z and max x, y, z
		double minx = exceptTriList[3*i][0];
		double maxx = exceptTriList[3*i][0];
		double miny = exceptTriList[3*i][1];
		double maxy = exceptTriList[3*i][1];
		double minz = exceptTriList[3*i][2];
		double maxz = exceptTriList[3*i][2];

		if (minx > exceptTriList[3*i+1][0])
			minx = exceptTriList[3*i+1][0];
		if (maxx < exceptTriList[3*i+1][0])
			maxx = exceptTriList[3*i+1][0];

		if (miny > exceptTriList[3*i+1][1])
			miny = exceptTriList[3*i+1][1];
		if (maxy < exceptTriList[3*i+1][1])
			maxy = exceptTriList[3*i+1][1];

		if (minz > exceptTriList[3*i+1][2])
			minz = exceptTriList[3*i+1][2];
		if (maxz < exceptTriList[3*i+1][2])
			maxz = exceptTriList[3*i+1][2];

		if (minx > exceptTriList[3*i+2][0])
			minx = exceptTriList[3*i+2][0];
		if (maxx < exceptTriList[3*i+2][0])
			maxx = exceptTriList[3*i+2][0];
		
		if (miny > exceptTriList[3*i+2][1])
			miny = exceptTriList[3*i+2][1];
		if (maxy < exceptTriList[3*i+2][1])
			maxy = exceptTriList[3*i+2][1];

		if (minz > exceptTriList[3*i+2][2])
			minz = exceptTriList[3*i+2][2];
		if (maxz < exceptTriList[3*i+2][2])
			maxz = exceptTriList[3*i+2][2];

		unsigned int minXCube = (unsigned int)floor((minx-xoffsets) / fCellLengthX);
		unsigned int maxXCube = (unsigned int)ceil((maxx-xoffsets) / fCellLengthX);
		unsigned int minYCube = (unsigned int)floor((miny-yoffsets) / fCellLengthY);
		unsigned int maxYCube = (unsigned int)ceil((maxy-yoffsets) / fCellLengthY);
		unsigned int minZCube = (unsigned int)floor((minz-zoffsets) / fCellLengthZ);
		unsigned int maxZCube = (unsigned int)ceil((maxz-zoffsets) / fCellLengthZ);

		float boxhalfsize[3] = {fCellLengthX/2.0, fCellLengthY/2.0, fCellLengthZ/2.0};
		for (unsigned int z = minZCube; z < maxZCube; ++z)
		{
			for (unsigned int y = minYCube; y < maxYCube; ++y)
			{
				for (unsigned int x = minXCube; x < maxXCube; ++x) 
				{
					bool intersects(false);
					float boxcenter[3] = {x*fCellLengthX + boxhalfsize[0], y*fCellLengthY + boxhalfsize[1], z*fCellLengthZ + boxhalfsize[2]};
					float triverts[3][3] = {{exceptTriList[3*i][0] - xoffsets, exceptTriList[3*i][1] - yoffsets, exceptTriList[3*i][2] - zoffsets},
											{exceptTriList[3*i+1][0] - xoffsets, exceptTriList[3*i+1][1] - yoffsets, exceptTriList[3*i+1][2] - zoffsets},
											{exceptTriList[3*i+2][0] - xoffsets, exceptTriList[3*i+2][1] - yoffsets, exceptTriList[3*i+2][2] - zoffsets}};
					intersects = (triBoxOverlap(boxcenter, boxhalfsize, triverts) == 1);
					if (intersects)
					{
						int* cubeIdx = new int[3];
						cubeIdx[0] = x;
						cubeIdx[1] = y;
						cubeIdx[2] = z;
						
						if (ignoreCubes.size() == 0)
							ignoreCubes.push_back(cubeIdx);
						else
						{
							vector<int*>::iterator iter;
							for (iter = ignoreCubes.begin(); iter != ignoreCubes.end(); iter++)
							{
								if (((*iter)[2] == cubeIdx[2]) &&
									((*iter)[1] == cubeIdx[1]) &&
									((*iter)[0] == cubeIdx[0]))
									break;
								else if ((*iter)[2] > cubeIdx[2])
								{
									ignoreCubes.insert(iter, cubeIdx);
									break;
								}
								else if (((*iter)[2] == cubeIdx[2]) && ((*iter)[1] > cubeIdx[1]))
								{
									ignoreCubes.insert(iter, cubeIdx);
									break;
								}
								else if (((*iter)[2] == cubeIdx[2]) && ((*iter)[1] == cubeIdx[1]) && (*iter)[0] > cubeIdx[0])
								{
									ignoreCubes.insert(iter, cubeIdx);
									break;
								}
							}
							
							if (iter == ignoreCubes.end())
								ignoreCubes.push_back(cubeIdx);
						}
					}
				}
			}
		}
	}
}

// unused vertices are moved to end of array;
template <class T> void CNonobtuseMC<T>::RenameVerticesAndTriangles()
{
	unsigned int nextID = 0;
	ID2POINT3DID::iterator mapIterator = this->m_i2pt3idVertices.begin();

	vector<unsigned int> pointIDList;

	// Rename vertices.
	this->m_nVertices = this->m_i2pt3idVertices.size();
	this->m_ppt3dVertices = new POINT3D[this->m_nVertices];
	while (mapIterator != this->m_i2pt3idVertices.end()) {
		this->m_ppt3dVertices[nextID][0] = (*mapIterator).second.x;
		this->m_ppt3dVertices[nextID][1] = (*mapIterator).second.y;
		this->m_ppt3dVertices[nextID][2] = (*mapIterator).second.z;

		// keep a list of pointID; used for removing unused vertices
		pointIDList.push_back(nextID);

		(*mapIterator).second.newID = nextID;
		nextID++;
		mapIterator++;
	}

	// Now rename triangles.
	vector<unsigned int>* point2Tri = new vector<unsigned int>[this->m_nVertices];

	vector<unsigned int>::iterator end = pointIDList.end();
	vector<unsigned int>::iterator last = end;

	this->m_nTriangles = this->m_trivecTriangles.size();
	this->m_piTriangleIndices = new unsigned int[this->m_nTriangles*3]; 
	for (unsigned int i = 0; i < this->m_nTriangles; ++i)
	{
		this->m_piTriangleIndices[3*i] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[0]].newID;
		this->m_piTriangleIndices[3*i+1] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[1]].newID;
		this->m_piTriangleIndices[3*i+2] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[2]].newID;

		// add reference for point to triangle
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int id = this->m_piTriangleIndices[3*i+j];
			vector<unsigned int> &nbrList = point2Tri[id];
			nbrList.push_back(i);
		}

		// remove referenced points from pointIDList
		last = remove(pointIDList.begin(), last, this->m_piTriangleIndices[3*i]);
		last = remove(pointIDList.begin(), last, this->m_piTriangleIndices[3*i+1]);
		last = remove(pointIDList.begin(), last, this->m_piTriangleIndices[3*i+2]);
	}

	// remove unreference vertices
	unsigned int numValidPts = end - last;
	unsigned int numUnusedPts = this->m_nVertices - numValidPts;
	for (unsigned int i = 0; i < numUnusedPts; ++i)
	{
		unsigned int vIdx = pointIDList[i];
		unsigned int lastVIdx = this->m_nVertices - 1;

		// make sure lastVIdx is a valid vertex
		for (unsigned int j = i; j < numUnusedPts; ++j)
		{
			if (pointIDList[j] == lastVIdx)
			{
				--lastVIdx;
				break;
			}
		}

		if (vIdx < lastVIdx)
		{
			// swap position with the last vertex
			swap(this->m_ppt3dVertices[vIdx][0], this->m_ppt3dVertices[lastVIdx][0]);
			swap(this->m_ppt3dVertices[vIdx][1], this->m_ppt3dVertices[lastVIdx][1]);
			swap(this->m_ppt3dVertices[vIdx][2], this->m_ppt3dVertices[lastVIdx][2]);
			// swap vertex-neighbour relationship
			swap(point2Tri[vIdx], point2Tri[lastVIdx]);

			// update polyList
			unsigned int numNeighbours = point2Tri[vIdx].size();
			for (unsigned int j = 0; j < numNeighbours; ++j)
			{
				unsigned int polyIdx = point2Tri[vIdx][j];

				for (unsigned int k = 0; k < 3; ++k)
				{
					if (this->m_piTriangleIndices[3*polyIdx+k] == lastVIdx)
					{
						this->m_piTriangleIndices[3*polyIdx+k] = vIdx;
						break;
					}
				}
			}
		}
		// update numNoVertices
		this->m_nVertices = this->m_nVertices - 1;
	}

	delete[] point2Tri;

	this->m_i2pt3idVertices.clear();
	this->m_trivecTriangles.clear();
}

// modified from base class
// edge 12: (b,e) in special case 5b
// edge 13: (b,e) in special case 9b
// edge 14: (b,f) in special case 11b ***OBSOLETE***
// edge 14: (a,d) in special case 11c
// edge 15: (b,e) in special case 12b
// edge 16: (b,f) in special case 14b ***OBSOLETE***
// edge 16: (a,d) in special case 14c
// edge 17: (b,e) in special case -7
// edge 18: (a,d) in special case -3
template <class T> unsigned int CNonobtuseMC<T>::GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo)
{
	switch (nEdgeNo) {
	case 0:
		return GetVertexID(nX, nY, nZ) + 1;
	case 1:
		return GetVertexID(nX, nY + 1, nZ);
	case 2:
		return GetVertexID(nX + 1, nY, nZ) + 1;
	case 3:
		return GetVertexID(nX, nY, nZ);
	case 4:
		return GetVertexID(nX, nY, nZ + 1) + 1;
	case 5:
		return GetVertexID(nX, nY + 1, nZ + 1);
	case 6:
		return GetVertexID(nX + 1, nY, nZ + 1) + 1;
	case 7:
		return GetVertexID(nX, nY, nZ + 1);
	case 8:
		return GetVertexID(nX, nY, nZ) + 2;
	case 9:
		return GetVertexID(nX, nY + 1, nZ) + 2;
	case 10:
		return GetVertexID(nX + 1, nY + 1, nZ) + 2;
	case 11:
		return GetVertexID(nX + 1, nY, nZ) + 2;
	case 12:
		return GetVertexID(nX, nY, nZ) + 3;
	case 13:
		return GetVertexID(nX, nY, nZ) + 4;
	case 14:                           
		return GetVertexID(nX, nY, nZ) + 5;
	case 15:
		return GetVertexID(nX, nY, nZ) + 6;
	case 16:
		return GetVertexID(nX, nY, nZ) + 7;
	case 17:
		return GetVertexID(nX, nY, nZ) + 8;
	case 18:
		return GetVertexID(nX, nY, nZ) + 9;
	default:
		// Invalid edge no.
		return -1;
	}
}

// modified from base class
// base class: each cube has 3 vertices
//             1st - on edge 3
//             2nd - on edge 0
//             3rd - on edge 8
//
// this class: each cube has 8 vertices
//             1st - on edge 3
//             2nd - on edge 0
//             3rd - on edge 8
//             4th - on edge 12: (b,e) in special case 5b
//             5th - on edge 13: (b,e) in special case 9b
//             6th - on edge 14: (b,f) in special case 11b ***OBSOLETE***
//             6th - on edge 14: (a,d) in special case 11c ***OBSOLETE***
//             7th - on edge 15: (b,e) in special case 12b
//             8th - on edge 16: (b,f) in special case 14b ***OBSOLETE***
//             8th - on edge 16: (a,d) in special case 14c ***OBSOLETE***
//             9th - on edge 17: (b,e) in special case -7
//            10th - on edge 18: (a,d) in special case -3
template <class T> unsigned int CNonobtuseMC<T>::GetVertexID(unsigned int nX, unsigned int nY, unsigned int nZ)
{
	return 10*(nZ*(this->m_nCellsY + 1)*(this->m_nCellsX + 1) + nY*(this->m_nCellsX + 1) + nX);
}

template <class T> void CNonobtuseMC<T>::DeleteSurface()
{
	CIsoSurface<T>::DeleteSurface();
	
	m_IntPtList[0] = NULL;
	m_IntPtList[1] = NULL;
	m_IntPtList[2] = NULL;
	m_offsets[0] = 0.0;
	m_offsets[1] = 0.0;
	m_offsets[2] = 0.0;
}

// compute the intersecting point on the specified edge of the cube
// modified from base class; uses m_IntPtList to compute
template <class T> POINT3DID CNonobtuseMC<T>::CalculateIntersection(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo, unsigned int surfaceType)
{
	POINT3DID midpoint = CIsoSurface<T>::CalculateIntersection(nX, nY, nZ, nEdgeNo);
	midpoint.x += m_offsets[0];
	midpoint.y += m_offsets[1];
	midpoint.z += m_offsets[2];
	if (surfaceType == MIDPOINT)
		return midpoint;
	
	float pt1[3];
	float pt2[3];
	unsigned int v1x = nX, v1y = nY, v1z = nZ;
	unsigned int v2x = nX, v2y = nY, v2z = nZ;
	
	unsigned int edgeOrientation(0);
	unsigned int rayIdx(0);
	
	//    1----2
	//   /|  / |
	// 5----6  |
	// | /0-|--3
	// |/   |/
	// 4----7
	
	switch (nEdgeNo)
	{
	case 0:
		v2y += 1;
		edgeOrientation = 1;
		rayIdx = nZ * (this->m_nCellsX + 1) + nX;
		break;
	case 1:
		v1y += 1;
		v2x += 1;
		v2y += 1;
		edgeOrientation = 0;
		rayIdx = nZ * (this->m_nCellsY + 1) + (nY+1);
		break;
	case 2:
		v1x += 1;
		v1y += 1;
		v2x += 1;
		edgeOrientation = 1;
		rayIdx = nZ * (this->m_nCellsX + 1) + (nX+1);
		break;
	case 3:
		v1x += 1;
		edgeOrientation = 0;
		rayIdx = nZ * (this->m_nCellsY + 1) + nY;
		break;
	case 4:
		v1z += 1;
		v2y += 1;
		v2z += 1;
		edgeOrientation = 1;
		rayIdx = (nZ+1) * (this->m_nCellsX + 1) + nX;
		break;
	case 5:
		v1y += 1;
		v1z += 1;
		v2x += 1;
		v2y += 1;
		v2z += 1;
		edgeOrientation = 0;
		rayIdx = (nZ+1) * (this->m_nCellsY + 1) + (nY+1);
		break;
	case 6:
		v1x += 1;
		v1y += 1;
		v1z += 1;
		v2x += 1;
		v2z += 1;
		edgeOrientation = 1;
		rayIdx = (nZ+1) * (this->m_nCellsX + 1) + (nX+1);
		break;
	case 7:
		v1x += 1;
		v1z += 1;
		v2z += 1;
		edgeOrientation = 0;
		rayIdx = (nZ+1) * (this->m_nCellsY + 1) + nY;
		break;
	case 8:
		v2z += 1;
		edgeOrientation = 2;
		rayIdx = nY * (this->m_nCellsX + 1) + nX;
		break;
	case 9:
		v1y += 1;
		v2y += 1;
		v2z += 1;
		edgeOrientation = 2;
		rayIdx = (nY+1) * (this->m_nCellsX + 1) + nX;
		break;
	case 10:
		v1x += 1;
		v1y += 1;
		v2x += 1;
		v2y += 1;
		v2z += 1;
		edgeOrientation = 2;
		rayIdx = (nY+1) * (this->m_nCellsX + 1) + (nX+1);
		break;
	case 11:
		v1x += 1;
		v2x += 1;
		v2z += 1;
		edgeOrientation = 2;
		rayIdx = nY * (this->m_nCellsX + 1) + (nX+1);
		break;
	}

	pt1[0] = v1x*(this->m_fCellLengthX) + m_offsets[0];
	pt1[1] = v1y*(this->m_fCellLengthY) + m_offsets[1];
	pt1[2] = v1z*(this->m_fCellLengthZ) + m_offsets[2];
	pt2[0] = v2x*(this->m_fCellLengthX) + m_offsets[0];
	pt2[1] = v2y*(this->m_fCellLengthY) + m_offsets[1];
	pt2[2] = v2z*(this->m_fCellLengthZ) + m_offsets[2];
	
	// find intersecting point
	POINT3DID intersection;
	intersection.newID = 0;
	unsigned int numIntPt = m_IntPtList[edgeOrientation][rayIdx].size();
	for (unsigned int i = 0; i < numIntPt; ++i)
	{
		double intPt[3] = {m_IntPtList[edgeOrientation][rayIdx][i].pt[0],
					m_IntPtList[edgeOrientation][rayIdx][i].pt[1],
					m_IntPtList[edgeOrientation][rayIdx][i].pt[2]};
		
		if (nEdgeNo != 2 && nEdgeNo != 3 && nEdgeNo != 6 && nEdgeNo != 7)	// pt1 is the lower point
		{
			if (pt1[edgeOrientation] <= intPt[edgeOrientation])
			{
				intersection.x = intPt[0];
				intersection.y = intPt[1];
				intersection.z = intPt[2];
				intersection.newID = 1;
				break;
			}
		}
		else
		{
			if (pt2[edgeOrientation] <= intPt[edgeOrientation])
			{
				intersection.x = intPt[0];
				intersection.y = intPt[1];
				intersection.z = intPt[2];
				intersection.newID = 1;
				break;
			}
		}
	}
	
	// TODO: FIX THIS!!! it should find an intersection no matter what!
	//       it seems like this doesn't occur for closed manifolds *phew*
	if (intersection.newID == 0)
	{
		cout << "intersection not found" 
		     << " at cube (" << nX << ", " << nY << ", " << nZ
		     << ") on edge " << nEdgeNo << endl;
		intersection = midpoint;
	}
	
	if (surfaceType == INTERPOLATE)
		return intersection;
		
	double *testPoint = new double[3];
	testPoint[0] = midpoint.x;
	testPoint[1] = midpoint.y;
	testPoint[2] = midpoint.z;
	double *lastTestPoint = new double[3];
	float eps = 0.01;
		
	if (checkNonobtuse(testPoint, nX, nY, nZ, nEdgeNo))
	{
		while (true)
		{	
			::midpoint(testPoint, midpoint.x, midpoint.y, midpoint.z, intersection.x, intersection.y, intersection.z);
			if (!checkNonobtuse(testPoint, nX, nY, nZ, nEdgeNo))
			{
				intersection.x = testPoint[0];
				intersection.y = testPoint[1];
				intersection.z = testPoint[2];
				
				if (pointPointDistance(testPoint[0], testPoint[1], testPoint[2], lastTestPoint[0], lastTestPoint[1], lastTestPoint[2]) <= eps)
					break;
				
			}
			else if (pointPointDistance(testPoint[0], testPoint[1], testPoint[2], lastTestPoint[0], lastTestPoint[1], lastTestPoint[2]) > eps)
			{
				midpoint.x = testPoint[0];
				midpoint.y = testPoint[1];
				midpoint.z = testPoint[2];
			}
			else
				break;
				
			lastTestPoint[0] = testPoint[0];
			lastTestPoint[1] = testPoint[1];
			lastTestPoint[2] = testPoint[2];
		} 
	}
	intersection.x = testPoint[0];
	intersection.y = testPoint[1];
	intersection.z = testPoint[2];
	delete[] testPoint;
	delete[] lastTestPoint;
	
	return intersection;
}

// returns xoffsets
template <class T> float CNonobtuseMC<T>::getXOffset() const
{
	return m_offsets[0];
}

// returns yoffsets
template <class T> float CNonobtuseMC<T>::getYOffset() const
{
	return m_offsets[1];
}

// returns zoffsets
template <class T> float CNonobtuseMC<T>::getZOffset() const
{
	return m_offsets[2];
}

// returns true if all angles incident to testPoint is nonobtuse
template <class T> bool CNonobtuseMC<T>::checkNonobtuse(double *testPoint, unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo)
{
	// check current cube
	bool result = this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ, nEdgeNo);
	
	if (result)
	{
		switch (nEdgeNo)
		{
		case 0:
			if (nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ, 2));
			
			if (!result)
				break;
			else if (nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ-1, 4));
				
			if (!result)
				break;
			else if (nX-1 >= 0 && nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ-1, 6));
			
			break;
		case 1:
			if (nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ-1, 5));
			
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ, 3));
				
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY && nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ-1, 7));
		
			break;
		case 2:
			if (nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ-1, 6));
			
			if (!result)
				break;
			else if (nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ, 0));
				
			if (!result)
				break;
			else if (nX+1 < this->m_nCellsX && nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ-1, 4));
				
			break;
		case 3:
			if (nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ-1, 7));
			
			if (!result)
				break;
			else if (nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ, 1));
			
			if (!result)
				break;
			else if (nY-1 >= 0 && nZ-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ-1, 5));
			
			break;
		case 4:
			if (nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ, 6));
			
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ+1, 0));
				
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ && nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ+1, 2));
				
			break;
		case 5:
			if (nZ+1 < this->m_nCellsZ)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ+1, 1));
			
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ, 7));
				
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY && nZ+1 < this->m_nCellsZ)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ+1, 3));
			
			break;
		case 6:
			if (nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ, 4));
			
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ+1, 2));
				
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ && nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ+1, 0));
				
			break;
		case 7:
			if (nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ, 5));
			
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY, nZ+1, 3));
				
			if (!result)
				break;
			else if (nZ+1 < this->m_nCellsZ && nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ+1, 1));
		
			break;
		case 8:
			if (nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ, 11));

			if (!result)
				break;
			else if (nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ, 9));

			if (!result)
				break;
			else if (nX-1 >= 0 && nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY-1, nZ, 10));

			break;
		case 9:
			if (nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY, nZ, 10));
			
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ, 8));
				
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY && nX-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX-1, nY+1, nZ, 11));
		
			break;
		case 10:
			if (nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ, 9));
			
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY+1, nZ, 11));
				
			if (!result)
				break;
			else if (nY+1 < this->m_nCellsY && nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY+1, nZ, 8));
		
			break;
		case 11:
			if (nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX, nY-1, nZ, 10));
			
			if (!result)
				break;
			else if (nX+1 < this->m_nCellsX)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY, nZ, 8));
				
			if (!result)
				break;
			else if (nX+1 < this->m_nCellsX && nY-1 >= 0)
				result = (result && this->checkNonobtuse_singleCube(testPoint, nX+1, nY-1, nZ, 9));
		
			break;
		}
	}

	return result;
}

// returns true if all angles in cube nX, nY, nZ at edge nEdgeNo incident to testPoint is nonobtuse
template <class T> bool CNonobtuseMC<T>::checkNonobtuse_singleCube(double *testPoint, unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo)
{
	unsigned int nCubesInXDirection = (this->m_nCellsX);
	unsigned int nCubesInSlice = nCubesInXDirection*(this->m_nCellsY);
	unsigned int tableIndex = allTableIndex[nZ*nCubesInSlice + nY*nCubesInXDirection + nX];

	bool isNonobtuse(true);
	
	if (this->m_triTable[tableIndex][0] == -3)
	{
		// -3:	Special Case -3
		// {-3, a, b, c, d, e, f, -1, ... }
		// where a,b,c,d forms a quad, and d,e,f,a forms the other quad
		// The right hand ordering determines the normals
		// A new point m should be added to the midpoint of a,d
		// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
	
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 1;
		for (edgeIdx = 1; edgeIdx <= 6; ++edgeIdx)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
				break;
		}
		
		if (edgeIdx >= 1 && edgeIdx <= 6)	
		{
			// get location for m
			POINT3DID ptb = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][1], MIDPOINT);
			POINT3DID pte = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][4], MIDPOINT);
			double *m = new double[3];
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
		
			// find neighbour vertex
			// ie. if checking triangle abm and edgeIdx == 1, then we want to find b
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][(edgeIdx%6)+1]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], m[0], m[1], m[2], citer->second.x, citer->second.y, citer->second.z);
				
				if (angle >= M_PI_2)
				//if (angle - M_PI_2 >= 0.0001)
					isNonobtuse = false;
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
			
			delete[] m;
		}
		else	// this should not be happening
		{
			cout << "ERROR in checkNonobtuse_singleCube(): cannot find edge in m_triTable: Case -3\n";
		}
	}
	else if (this->m_triTable[tableIndex][0] == -5)
	{
		// -5:  Special Case 5b
		// {-5, a, b, c, d, e, -1, ... }
		// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
		// The right hand ordering determins the normals
		// A new point m should be added to the midpoint of b,e
		// Then, the 5 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, a, m, -1}
	
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 1;
		for (edgeIdx = 1; edgeIdx <= 5; ++edgeIdx)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
				break;
		}
		
		if (edgeIdx >= 1 && edgeIdx <= 5)	
		{
			// get location for m
			POINT3DID ptb = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][2], MIDPOINT);
			POINT3DID pte = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][5], MIDPOINT);
			double *m = new double[3];
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
		
			// find neighbour vertex
			// ie. if checking triangle abm and edgeIdx == 1, then we want to find b
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][(edgeIdx%5)+1]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], m[0], m[1], m[2], citer->second.x, citer->second.y, citer->second.z);
				
				if (angle >= M_PI_2)
				//if (angle - M_PI_2 >= 0.0001)
					isNonobtuse = false;
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
			
			delete[] m;
		}
		else	// this should not be happening
		{
			cout << "ERROR in checkNonobtuse_singleCube(): cannot find edge in m_triTable: Case -5" << endl;
		}
	}
	else if (this->m_triTable[tableIndex][0] == -7)
	{
		// -7:  Special Case -7
		// {-7, a, b, c, d, e, f, i, j, k, -1, ... }
		// where a,b,c,d,e,f forms a hexagon and i,j,k forms a triangle
		// The right hand ordering determines the normals
		// A new point m should be added to the centroid of a,b,c,d,e,f
		// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, i, j, k, -1}
		
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 1;
		for (edgeIdx = 1; edgeIdx <= 9; ++edgeIdx)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
				break;
		}
		
		if (edgeIdx >= 1 && edgeIdx <= 6)
		{
			// get location for m
			POINT3DID ptb = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][2], MIDPOINT);
			POINT3DID pte = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][5], MIDPOINT);
			double *m = new double[3];
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
		
			// find neighbour vertex
			// ie. if checking triangle abm and edgeIdx == 1, then we want to find b
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][(edgeIdx%6)+1]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], m[0], m[1], m[2], citer->second.x, citer->second.y, citer->second.z);
				
				if (angle >= M_PI_2)
				//if (angle - M_PI_2 >= 0.0001)
					isNonobtuse = false;
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
			
			delete[] m;
		}
		else if (edgeIdx >= 7 && edgeIdx <= 9)
		{
			// find neighbour vertex 1
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][((edgeIdx-6)%3)+7]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				// find neighbour vertex 2
				ID2POINT3DID::const_iterator citer2 = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][((edgeIdx-5)%3)+7]));
				if (citer2 != this->m_i2pt3idVertices.end())
				{
					double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], citer->second.x, citer->second.y, citer->second.z, citer2->second.x, citer2->second.y, citer2->second.z);
				
					if (angle >= M_PI_2)
					//if (angle - M_PI_2 >= 0.0001)
						isNonobtuse = false;
				}
				else	// can't find vertex; not computed yet
				{
					// ignore the triangle, and assume it is non-obtuse
				}
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
		}
		else	// this should not be happening
		{
			cout << "ERROR in checkNonobtuse_singleCube(): cannot find edge in m_triTable: Case -7\n";
		}
	}
	else if (this->m_triTable[tableIndex][0] == -9)
	{
		// -9:  Special Case 9b
		// {-9, a, b, c, d, e, f, -1, ... }
		// where a,b,c,d,e,f forms a hexagon.
		// The right hand ordering determines the normals
		// A new point m should be added to the centroid of a,b,c,d,e,f
		// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
		
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 1;
		for (edgeIdx = 1; edgeIdx <= 6; ++edgeIdx)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
				break;
		}
		
		if (edgeIdx >= 1 && edgeIdx <= 6)	
		{
			// get location for m
			POINT3DID ptb = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][2], MIDPOINT);
			POINT3DID pte = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][5], MIDPOINT);
			double *m = new double[3];
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
		
			// find neighbour vertex
			// ie. if checking triangle abm and edgeIdx == 1, then we want to find b
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][(edgeIdx%6)+1]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], m[0], m[1], m[2], citer->second.x, citer->second.y, citer->second.z);
				
				if (angle >= M_PI_2)
				//if (angle - M_PI_2 >= 0.0001)
					isNonobtuse = false;
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
			
			delete[] m;
		}
		else	// this should not be happening
		{
			cout << "ERROR in checkNonobtuse_singleCube(): cannot find edge in m_triTable: Case -9\n";
		}
	}
	else if (this->m_triTable[tableIndex][0] == -12)
	{
		// -12: Special Case 12b
		// {-12, a, b, c, d, e, i, j, k, -1, ... }
		// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
		//       and i, j, k forms a seperate triangle
		// The right hand ordering determines the normals
		// A new point m should be added to the midpoint of b,e
		// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, a, m, i, j, k, -1}
	
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 1;
		for (edgeIdx = 1; edgeIdx <= 8; ++edgeIdx)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
				break;
		}
		
		if (edgeIdx >= 1 && edgeIdx <= 5)
		{
			// get location for m
			POINT3DID ptb = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][2], MIDPOINT);
			POINT3DID pte = CalculateIntersection(nX, nY, nZ, this->m_triTable[tableIndex][5], MIDPOINT);
			double *m = new double[3];
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
		
			// find neighbour vertex
			// ie. if checking triangle abm and edgeIdx == 1, then we want to find b
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][(edgeIdx%5)+1]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], m[0], m[1], m[2], citer->second.x, citer->second.y, citer->second.z);
				
				if (angle >= M_PI_2)
				//if (angle - M_PI_2 >= 0.0001)
					isNonobtuse = false;
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
			
			delete[] m;
		}
		else if (edgeIdx >= 6 && edgeIdx <= 8)
		{
			// find neighbour vertex 1
			ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][((edgeIdx-5)%3)+6]));
			if (citer != this->m_i2pt3idVertices.end())
			{
				// find neighbour vertex 2
				ID2POINT3DID::const_iterator citer2 = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][((edgeIdx-4)%3)+6]));
				if (citer2 != this->m_i2pt3idVertices.end())
				{
					double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], citer->second.x, citer->second.y, citer->second.z, citer2->second.x, citer2->second.y, citer2->second.z);
				
					if (angle >= M_PI_2)
					//if (angle - M_PI_2 >= 0.0001)
						isNonobtuse = false;
				}
				else	// can't find vertex; not computed yet
				{
					// ignore the triangle, and assume it is non-obtuse
				}
			}
			else	// can't find vertex; not computed yet
			{
				// ignore the triangle, and assume it is non-obtuse
			}
		}
		else	// this should not be happening
		{
			cout << "ERROR in checkNonobtuse_singleCube(): cannot find edge in m_triTable: Case -12\n";
		}
	}
	else	// general marching cube cases
	{
		// find nEdgeNo in m_triTable entry
		unsigned int edgeIdx = 0;
		while (edgeIdx < 16)
		{
			if (this->m_triTable[tableIndex][edgeIdx] == (int)nEdgeNo)
			{
				unsigned int triInList = edgeIdx / 3;	// start counting from 0
				unsigned int nbIdx1 = 3*triInList + (((edgeIdx % 3) + 1) % 3);
				unsigned int nbIdx2 = 3*triInList + (((edgeIdx % 3) + 2) % 3);
				
				// find neighbour vertex 1
				ID2POINT3DID::const_iterator citer = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][nbIdx1]));
				if (citer != this->m_i2pt3idVertices.end())
				{
					// find neighbour vertex 2
					ID2POINT3DID::const_iterator citer2 = this->m_i2pt3idVertices.find(this->GetEdgeID(nX, nY, nZ, this->m_triTable[tableIndex][nbIdx2]));
					if (citer2 != this->m_i2pt3idVertices.end())
					{
						double angle = getAngle(testPoint[0], testPoint[1], testPoint[2], citer->second.x, citer->second.y, citer->second.z, citer2->second.x, citer2->second.y, citer2->second.z);
					
						if (angle >= M_PI_2)
						//if (angle - M_PI_2 >= 0.0001)
							isNonobtuse = false;
					}
					else	// can't find vertex; not computed yet
					{
						// ignore the triangle, and assume it is non-obtuse
					}
				}
				else	// can't find vertex; not computed yet
				{
					// ignore the triangle, and assume it is non-obtuse
				}
			}
			else if (this->m_triTable[tableIndex][edgeIdx] == -1)
				break;
			
			++edgeIdx;
		}
	}
	
	return isNonobtuse;
}

template class CNonobtuseMC<short>;
template class CNonobtuseMC<unsigned short>;
template class CNonobtuseMC<int>;
template class CNonobtuseMC<unsigned int>;
template class CNonobtuseMC<float>;
template class CNonobtuseMC<double>;

