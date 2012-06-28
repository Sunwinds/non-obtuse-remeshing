/***************************************************************************
                                nonobtoptn.cpp
                             -------------------
    begin                : Tue Feb 21 2006
    author               : John Li in 2006
    email                : ysl@cs.sfu.ca
 ***************************************************************************/

#include "nonobtoptn.h"

bool my_quad_greater(Quadric q1, Quadric q2)
{
	return (q1.quadricValue > q2.quadricValue);
}

bool my_quad_less(Quadric q1, Quadric q2)
{
	return (q1.quadricValue < q2.quadricValue);
}

bool my_quadMove_largestImprovementFirst(QuadricMove q1, QuadricMove q2)
{
	bool result(false);
	if (q1.bCanMove && q2.bCanMove)
		result = (q1.qValueImprovement < q2.qValueImprovement);
	else 
		result = (!q1.bCanMove && q2.bCanMove);

	return result;
}

bool my_quadMove_smallestImprovementFirst(QuadricMove q1, QuadricMove q2)
{
	bool result(false);
	if (q1.bCanMove && q2.bCanMove)
		result = (q1.qValueImprovement > q2.qValueImprovement);
	else 
		result = (!q1.bCanMove && q2.bCanMove);

	return result;
}


bool my_quadMove_largestQuadFirst(QuadricMove q1, QuadricMove q2)
{
	bool result(false);
	if (q1.bCanMove && q2.bCanMove)
	{
		if (q1.qValueImprovement > 0 && q2.qValueImprovement > 0)
			result = (q1.q.quadricValue < q2.q.quadricValue);
		else 
			result = (q1.qValueImprovement <= 0 && q2.qValueImprovement > 0);
	}
	else
		result = (!q1.bCanMove && q2.bCanMove);

	return result;
}

bool my_quadMove_smallestQuadAfterMoveFirst(QuadricMove q1, QuadricMove q2)
{
	bool result(false);
	if (q1.bCanMove && q2.bCanMove)
	{
		result = (q1.qValue_afterMove > q2.qValue_afterMove);
	}
	else
		result = (!q1.bCanMove && q2.bCanMove);

	return result;
}

bool my_quadMove_largestQuadBeforeMoveFirst(QuadricMove q1, QuadricMove q2)
{
	bool result(false);
	if (q1.bCanMove && q2.bCanMove)
	{
		result = (q1.q.quadricValue < q2.q.quadricValue);
	}
	else
		result = (!q1.bCanMove && q2.bCanMove);

	return result;
}

bool my_quadMovePtr_largestImprovementFirst(QuadricMove* p_q1, QuadricMove* p_q2)
{
	return my_quadMove_largestImprovementFirst(*p_q1, *p_q2);
}

bool my_quadMovePtr_smallestQuadAfterMoveFirst(QuadricMove* p_q1, QuadricMove* p_q2)
{
	return my_quadMove_smallestQuadAfterMoveFirst(*p_q1, *p_q2);
}

NonobtOptn::NonobtOptn()
:numDVertices(0), dVerticesList(NULL), numDPolygons(0), 
dPolygonsList(NULL), dVertexNeighbourList(NULL), dEdgeNeighbourList(NULL), dPolygonNeighbourList(NULL),
dFaceNormalsList(NULL), dVertexNormalsList(NULL),
numNoVertices(0), noVerticesList(NULL), numNoPolygons(0), 
noPolygonsList(NULL), noVertexNeighbourList(NULL),
noFaceNormalsList(NULL), noVertexNormalsList(NULL),
noVertexEdgeList(NULL),
closestFaceList(NULL), closestBoundaryEdgeList(NULL), closestPtList(NULL),
centroidList(NULL),
lastMovedVertexIdx(-1), isLastMovedVertexSet(false),
bIsLoaded(false), TAUBIN_LAMBDA(0.85), TAUBIN_MU(-0.9),
bboxDiagonal(0)
{
	srand(time(NULL));
}


NonobtOptn::~NonobtOptn()
{
	reset();
}

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
void NonobtOptn::loadMesh(const double* dvList, int ndVertices, const int* const* dpList, int ndPolygons, const vector<int>* dvNbrList, const vector<EdgeNeighbours>* deNbrList, const vector<int>* dpNbrList, double* novList, int nnoVertices, int** nopList, int nnoPolygons, const int* assoTriList)
{
	reset();
	
	numDVertices = ndVertices;
	dVerticesList = new double[3*numDVertices];
	dVertexNeighbourList = new vector<int>[numDVertices];
	dEdgeNeighbourList = new vector<EdgeNeighbours>[numDVertices];
	for (int i = 0; i < numDVertices; ++i)
	{
		dVerticesList[3*i] = dvList[3*i];
		dVerticesList[3*i+1] = dvList[3*i+1];
		dVerticesList[3*i+2] = dvList[3*i+2];
		
		dVertexNeighbourList[i] = dvNbrList[i];
		dEdgeNeighbourList[i] = deNbrList[i];
	}

	bboxDiagonal = boundingBoxDiagonal(dvList, ndVertices);

	numDPolygons = ndPolygons;
	dPolygonsList = new int*[numDPolygons];
	dPolygonNeighbourList = new vector<int>[numDPolygons];	
	for (int i = 0; i < numDPolygons; ++i)
	{
		dPolygonsList[i] = new int[3];
		for (int j = 0; j < 3; ++j)
			dPolygonsList[i][j] = dpList[i][j];
		
		dPolygonNeighbourList[i] = dpNbrList[i];
	}
	
	numNoVertices = nnoVertices;
	noVerticesList = novList;
	numNoPolygons = nnoPolygons;
	noPolygonsList = nopList;
	
	// assign vertex to neighbour polygon correspondence
	noVertexNeighbourList = new vector<int>[numNoVertices];
	for (int i = 0; i < numNoPolygons; ++i)
	{
		// stores vertex neighbour information
		noVertexNeighbourList[noPolygonsList[i][0]].push_back(i);
		noVertexNeighbourList[noPolygonsList[i][1]].push_back(i);
		noVertexNeighbourList[noPolygonsList[i][2]].push_back(i);
	}
	
	// initialize edge list for non-obtuse mesh
	noVertexEdgeList = new vector<int>[numNoVertices];
	for (int i = 0; i < numNoVertices; ++i)
	{
		vector<unsigned int> processedIndices;
		Edge e;
		e.vertex1Idx = i;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < noVertexNeighbourList[i].size(); ++j)
		{
			unsigned int polyIdx = noVertexNeighbourList[i][j];
			// find the 2 neighbour vertices
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (noPolygonsList[polyIdx][k] != i)
				{
					unsigned int vIdx = noPolygonsList[polyIdx][k];
					
					if (i >= (int)vIdx)	// we only store edge(v1,v2) for v1 < v2
						continue;

					// check if we added the edge already
					if (find(processedIndices.begin(), processedIndices.end(), vIdx) != processedIndices.end())
						continue;

					e.vertex2Idx = vIdx;
					noEdgeList.push_back(e);
					
					noVertexEdgeList[i].push_back(noEdgeList.size()-1);
					noVertexEdgeList[vIdx].push_back(noEdgeList.size()-1);

					processedIndices.push_back(vIdx);
				}
			}
			
		}
		processedIndices.clear();
	}

	// compute face and vertex normals
	computeFaceNormal();
	computeVertexNormal();
	
	// initialize closest face list
	closestFaceList = new int[numNoVertices];
	closestBoundaryEdgeList = new Edge[numNoVertices];
	closestPtList = new double[3*numNoVertices];
	for (int i = 0; i < numNoVertices; ++i)
	{
		closestFaceList[i] = -1;	// -1 meaning unassigned
		if (assoTriList != NULL)
		{
			closestFaceList[i] = assoTriList[i];
		}
	}

	// initialize centroid list for TAUBIN smoothing
	centroidList = new double[3*numNoVertices];
	for (int v = 0; v < numNoVertices; ++v)
	{
		// compute centroid based on TAUBIN_LAMBDA
		double centroid[3] = {0,0,0};
		unsigned int numNeighbours = noVertexNeighbourList[v].size();
		for (unsigned int i = 0; i < numNeighbours; ++i)
		{
			unsigned int polyIdx = noVertexNeighbourList[v][i];
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (noPolygonsList[polyIdx][j] == v)
				{
					unsigned int vIdx1 = noPolygonsList[polyIdx][(j+1)%3];
					unsigned int vIdx2 = noPolygonsList[polyIdx][(j+2)%3];
		
					centroid[0] += noVerticesList[3*vIdx1];
					centroid[1] += noVerticesList[3*vIdx1+1];
					centroid[2] += noVerticesList[3*vIdx1+2];
	
					centroid[0] += noVerticesList[3*vIdx2];
					centroid[1] += noVerticesList[3*vIdx2+1];
					centroid[2] += noVerticesList[3*vIdx2+2];
					break;
				}
			}
		}
	
		centroid[0] /= (2*numNeighbours);	// added each vertex twice, so divide 2*numNeighbours
		centroid[1] /= (2*numNeighbours);
		centroid[2] /= (2*numNeighbours);

		centroidList[3*v] = (1-TAUBIN_LAMBDA)*noVerticesList[3*v] + TAUBIN_LAMBDA*centroid[0];
		centroidList[3*v+1] = (1-TAUBIN_LAMBDA)*noVerticesList[3*v+1] + TAUBIN_LAMBDA*centroid[1];
		centroidList[3*v+2] = (1-TAUBIN_LAMBDA)*noVerticesList[3*v+2] + TAUBIN_LAMBDA*centroid[2];
	}

	bIsLoaded = true;
}

// reset all members
void NonobtOptn::reset()
{
	numDVertices = 0;
	if (dVerticesList != NULL)
		delete[] dVerticesList;
	dVerticesList = NULL;
	numDPolygons = 0;
	if (dPolygonsList != NULL)
		delete[] dPolygonsList;
	dPolygonsList = NULL;
	if (dVertexNeighbourList != NULL)
		delete[] dVertexNeighbourList;
	dVertexNeighbourList = NULL;
	if (dEdgeNeighbourList != NULL)
		delete[] dEdgeNeighbourList;
	dEdgeNeighbourList = NULL;
	if (dPolygonNeighbourList != NULL)
		delete[] dPolygonNeighbourList;
	dPolygonNeighbourList = NULL;
	if (dFaceNormalsList != NULL)
		delete[] dFaceNormalsList;
	dFaceNormalsList = NULL;
	if (dVertexNormalsList != NULL)
		delete[] dVertexNormalsList;
	dVertexNormalsList = NULL;
	
	numNoVertices = 0;
	if (noVerticesList != NULL)
		delete[] noVerticesList;
	noVerticesList = NULL;
	numNoPolygons = 0;
	if (noPolygonsList != NULL)
		delete[] noPolygonsList;
	noPolygonsList = NULL;
	if (noVertexNeighbourList != NULL)
		delete[] noVertexNeighbourList;
	noVertexNeighbourList = NULL;
	if (noFaceNormalsList != NULL)
		delete[] noFaceNormalsList;
	noFaceNormalsList = NULL;
	if (noVertexNormalsList != NULL)
		delete[] noVertexNormalsList;
	noVertexNormalsList = NULL;
	noEdgeList.clear();
	if (noVertexEdgeList != NULL)
		delete[] noVertexEdgeList;
	noVertexEdgeList = NULL;
	unusedEdgeIdxList.clear();

	if (closestFaceList != NULL)
		delete[] closestFaceList;
	closestFaceList = NULL;
	if (closestBoundaryEdgeList != NULL)
		delete[] closestBoundaryEdgeList;
	closestBoundaryEdgeList = NULL;
	if (closestPtList != NULL)
		delete[] closestPtList;	
	closestPtList = NULL;
		
// 	qMoveList.clear();
// 	qMoveIndicesList.clear();
	for (unsigned int i = 0; i < qMovePtrList.size(); ++i)
		delete qMovePtrList[i];
	qMovePtrList.clear();
	qMoveSortPtrList.clear();
	dirtyOptimizeQMs.clear();
	dirtyOptimizeBackLinks.clear();

	qSmoothList.clear();
	qSmoothIndicesList.clear();

	qDecimateList.clear();
	qDecimateIndicesList.clear();

	for (unsigned int i = 0; i < qDecimatePtrList.size(); ++i)
		delete qDecimatePtrList[i];
	qDecimatePtrList.clear();
	qDecimateSortPtrList.clear();
	dirtyDecimateQMs.clear();
	dirtyDecimateBackLinks.clear();

	qVertexDecimateList.clear();
	numPlanesInQuadricsList.clear();

	if (centroidList != NULL)
		delete[] centroidList;
	centroidList = NULL;

	lastMovedVertexPos[0] = 0;
	lastMovedVertexPos[1] = 0;
	lastMovedVertexPos[2] = 0;
	
	lastMovedVertexIdx = -1;
	
	isLastMovedVertexSet = false;
	
	bIsLoaded = false;
}

// load debug info
// filename (in): name of the debug file to load
// return true if success
// TODO: add dirtyOptimizeQMs, dirtyOptimizeBackLinks
bool NonobtOptn::loadDebug(char* filename)
{
	reset();
	
	ifstream inFile;
	inFile.open(filename, ifstream::in);
	inFile.precision(10);
	
	if (!inFile.is_open())
		return false;
	
	char temp[50];

	// -------- DESIRED MESH -------------

	// first line records the number of vertices and number of polygons for desired mesh
	if (inFile.good())
	{
		inFile >> temp;
		inFile >> numDVertices;
		inFile >> numDPolygons;
	}
	else
		return false;
	
	// record list of vertices
	if (inFile.good())
	{
		dVerticesList = new double[3*numDVertices];
		for (int i = 0; i < numDVertices; i++)
		{
			inFile >> temp;
			inFile >> dVerticesList[3*i];
			inFile >> dVerticesList[(3*i)+1];
			inFile >> dVerticesList[(3*i)+2];
		}
	}
	else
		return false;
	
	// record list of polygons
	if (inFile.good())
	{
		dPolygonsList = new int*[numDPolygons];
		for (int i = 0; i < numDPolygons; i++)
		{
			dPolygonsList[i] = new int[3];
			inFile >> temp;
			inFile >> dPolygonsList[i][0];
			inFile >> dPolygonsList[i][1];
			inFile >> dPolygonsList[i][2];
		}
	}
	else
		return false;

	// record vertex neighbour list
	if (inFile.good())
	{
		dVertexNeighbourList = new vector<int>[numDVertices];
		for (int i = 0; i < numDVertices; i++)
		{
			inFile >> temp;
			int numNeighbours;
			inFile >> numNeighbours;
			dVertexNeighbourList[i].resize(numNeighbours);
			for (int j = 0; j < numNeighbours; j++)
			{
				inFile >> dVertexNeighbourList[i][j];
			}
		}
	}
	else
		return false;

	// edge neighbour list
	if (inFile.good())
	{
		dEdgeNeighbourList = new vector<EdgeNeighbours>[numDVertices];
		for (int i = 0; i < numDVertices; i++)
		{
			inFile >> temp;
			int numNeighbours;
			inFile >> numNeighbours;
			dEdgeNeighbourList[i].resize(numNeighbours);
			for (int j = 0; j < numNeighbours; j++)
			{
				inFile >> dEdgeNeighbourList[i][j].e.vertex1Idx;
				inFile >> dEdgeNeighbourList[i][j].e.vertex2Idx;
				inFile >> dEdgeNeighbourList[i][j].polyNeighbour1Idx;
				inFile >> dEdgeNeighbourList[i][j].polyNeighbour2Idx;
			}
		}
	}
	else
		return false;

	if ( (inFile.rdstate() & ifstream::badbit ) != 0 )
    	cout << "critical error in stream buffer'\n";
	if ( (inFile.rdstate() & ifstream::eofbit ) != 0 )
    	cout << "End-Of-File reached while extracting'\n";
	if ( (inFile.rdstate() & ifstream::failbit ) != 0 )
    	cout << "failure extracting from stream'\n";
	if ( (inFile.rdstate() & ifstream::goodbit ) != 0 )
    	cout << "no error condition, represents the absence of the above bits'\n";

	// polygon neighbour list
	if (inFile.good())
	{
		dPolygonNeighbourList = new vector<int>[numDPolygons];
		for (int i = 0; i < numDPolygons; i++)
		{
			inFile >> temp;
			int numNeighbours;
			inFile >> numNeighbours;
			dPolygonNeighbourList[i].resize(numNeighbours);
			for (int j = 0; j < numNeighbours; j++)
			{
				inFile >> dPolygonNeighbourList[i][j];
			}
		}
	}
	else
		return false;

	// record face normals
	if (inFile.good())
	{
		dFaceNormalsList = new double[3*numDPolygons];
		for (int i = 0; i < numDPolygons; i++)
		{
			inFile >> dFaceNormalsList[3*i];
			inFile >> dFaceNormalsList[3*i+1];
			inFile >> dFaceNormalsList[3*i+2];
		}
	}
	else
		return false;

	// record vertex normals
	if (inFile.good())
	{
		dVertexNormalsList = new double[3*numDVertices];
		for (int i = 0; i < numDVertices; i++)
		{
			inFile >> dVertexNormalsList[3*i];
			inFile >> dVertexNormalsList[3*i+1];
			inFile >> dVertexNormalsList[3*i+2];
		}
	}
	else
		return false;

	// -------- NONOBTUSE MESH -------------

	// first line records the number of vertices and number of polygons for nonobtuse mesh
	if (inFile.good())
	{
		inFile >> temp;
		inFile >> numNoVertices;
		inFile >> numNoPolygons;
	}
	else
		return false;
	
	// record list of vertices
	if (inFile.good())
	{
		noVerticesList = new double[3*numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> temp;
			inFile >> noVerticesList[3*i];
			inFile >> noVerticesList[(3*i)+1];
			inFile >> noVerticesList[(3*i)+2];
		}
	}
	else
		return false;
	
	// record list of polygons
	if (inFile.good())
	{
		noPolygonsList = new int*[numNoPolygons];
		for (int i = 0; i < numNoPolygons; i++)
		{
			noPolygonsList[i] = new int[3];
			inFile >> temp;
			inFile >> noPolygonsList[i][0];
			inFile >> noPolygonsList[i][1];
			inFile >> noPolygonsList[i][2];
		}
	}
	else
		return false;

	// record vertex neighbour list
	if (inFile.good())
	{
		noVertexNeighbourList = new vector<int>[numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> temp;
			int numNeighbours;
			inFile >> numNeighbours;
			noVertexNeighbourList[i].resize(numNeighbours);
			for (int j = 0; j < numNeighbours; j++)
			{
				inFile >> noVertexNeighbourList[i][j];
			}
		}
	}
	else
		return false;
	

	// record face normals
	if (inFile.good())
	{
		noFaceNormalsList = new double[3*numNoPolygons];
		for (int i = 0; i < numNoPolygons; i++)
		{
			inFile >> noFaceNormalsList[3*i];
			inFile >> noFaceNormalsList[3*i+1];
			inFile >> noFaceNormalsList[3*i+2];
		}
	}
	else
		return false;
	

	// record vertex normals
	if (inFile.good())
	{
		noVertexNormalsList = new double[3*numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> noVertexNormalsList[3*i];
			inFile >> noVertexNormalsList[3*i+1];
			inFile >> noVertexNormalsList[3*i+2];
		}
	}
	else
		return false;

	// record noEdgeList
	if (inFile.good())
	{
		unsigned int numEdges;
		inFile >> numEdges;
		noEdgeList.resize(numEdges);
		for (unsigned int i = 0; i < numEdges; i++)
		{
			inFile >> noEdgeList[i].vertex1Idx;
			inFile >> noEdgeList[i].vertex2Idx;
		}	
	}
	else
		return false;

	// record noVertexEdgeList
	if (inFile.good())
	{
		noVertexEdgeList = new vector<int>[numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			int numEdges;
			inFile >> numEdges;
			noVertexEdgeList[i].resize(numEdges);
			for (int j = 0; j < numEdges; j++)
			{
				inFile >> noVertexEdgeList[i][j];
			}
		}	
	}
	else
		return false;

	// record unusedEdgeIdxList
	if (inFile.good())
	{
		unsigned int numUnusedEdge;
		inFile >> numUnusedEdge;
		unusedEdgeIdxList.resize(numUnusedEdge);
		for (unsigned int i = 0; i < numUnusedEdge; i++)
		{
			inFile >> unusedEdgeIdxList[i];
		}
	}
	else
		return false;

	// ------ other debug info ----------

	// record closest face list
	if (inFile.good())
	{
		closestFaceList = new int[numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> closestFaceList[i];
		}
	}
	else
		return false;

	// record closest boudnary edge list
	if (inFile.good())
	{
		closestBoundaryEdgeList = new Edge[numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> closestBoundaryEdgeList[i].vertex1Idx;
			inFile >> closestBoundaryEdgeList[i].vertex2Idx;
		}
	}
	else
		return false;

	// closest pt list
	if (inFile.good())
	{
		closestPtList = new double[3*numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> closestPtList[3*i];
			inFile >> closestPtList[3*i+1];
			inFile >> closestPtList[3*i+2];
		}
	}
	else
		return false;

	// record qMoveList
// 	if (inFile.good())
// 	{
// 		int numSize;
// 		inFile >> numSize;
// 		qMoveList.resize(numSize);
// 		for (int i = 0; i < numSize; i++)
// 		{
// 			inFile >> qMoveList[i].q.vIdx;
// 			for (int j = 0; j < 9; j++)
// 				inFile >> qMoveList[i].q.A[j];
// 			for (int j = 0; j < 3; j++)
// 				inFile >> qMoveList[i].q.b[j];
// 			inFile >> qMoveList[i].q.c;
// 			inFile >> qMoveList[i].q.quadricValue;
// 			for (int j = 0; j < 9; j++)
// 				inFile >> qMoveList[i].q.fn.A[j];
// 			for (int j = 0; j < 3; j++)
// 				inFile >> qMoveList[i].q.fn.b[j];
// 			inFile >> qMoveList[i].q.fn.c;
// 			inFile >> qMoveList[i].bCanMove;
// 			inFile >> qMoveList[i].newPos[0];
// 			inFile >> qMoveList[i].newPos[1];
// 			inFile >> qMoveList[i].newPos[2];
// 			inFile >> qMoveList[i].qValue_afterMove;
// 			inFile >> qMoveList[i].qValueImprovement;
// 		}
// 	}
// 	else
// 		return false;
// 
// 	// record qMoveIndicesList
// 	if (inFile.good())
// 	{
// 		unsigned int numMoveIndices;
// 		inFile >> numMoveIndices;
// 		qMoveIndicesList.resize(numMoveIndices);
// 		for (unsigned int i = 0; i < numMoveIndices; i++)
// 		{
// 			inFile >> qMoveIndicesList[i];
// 		}
// 	}
// 	else
// 		return false;

	// load qMovePtrList
	if (inFile.good())
	{
		int numSize;
		inFile >> numSize;
		qMovePtrList.resize(numSize);
		for (int i = 0; i < numSize; i++)
		{
			qMovePtrList[i] = new QuadricMove;
			inFile >> qMovePtrList[i]->q.vIdx;
			for (int j = 0; j < 9; j++)
				inFile >> qMovePtrList[i]->q.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qMovePtrList[i]->q.b[j];
			inFile >> qMovePtrList[i]->q.c;
			inFile >> qMovePtrList[i]->q.quadricValue;
			for (int j = 0; j < 9; j++)
				inFile >> qMovePtrList[i]->q.fn.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qMovePtrList[i]->q.fn.b[j];
			inFile >> qMovePtrList[i]->q.fn.c;
			inFile >> qMovePtrList[i]->bCanMove;
			inFile >> qMovePtrList[i]->newPos[0];
			inFile >> qMovePtrList[i]->newPos[1];
			inFile >> qMovePtrList[i]->newPos[2];
			inFile >> qMovePtrList[i]->qValue_afterMove;
			inFile >> qMovePtrList[i]->qValueImprovement;
		}
	}
	else
		return false;
	
	// load qMoveSortPtrList
	if (inFile.good())
	{
		unsigned int numMoveSortPtrs;
		inFile >> numMoveSortPtrs;
		qMoveSortPtrList.resize(numMoveSortPtrs);
		for (unsigned int i = 0; i < numMoveSortPtrs; i++)
		{
			unsigned int vIdx;
			inFile >> vIdx;
			qMoveSortPtrList[i] = qMovePtrList[vIdx];
		}
	}
	else
		return false;

	// record qSmoothList
	if (inFile.good())
	{
		int numSize;
		inFile >> numSize;
		qSmoothList.resize(numSize);
		for (int i = 0; i < numSize; i++)
		{
			inFile >> qSmoothList[i].q.vIdx;
			for (int j = 0; j < 9; j++)
				inFile >> qSmoothList[i].q.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qSmoothList[i].q.b[j];
			inFile >> qSmoothList[i].q.c;
			inFile >> qSmoothList[i].q.quadricValue;
			for (int j = 0; j < 9; j++)
				inFile >> qSmoothList[i].q.fn.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qSmoothList[i].q.fn.b[j];
			inFile >> qSmoothList[i].q.fn.c;
			inFile >> qSmoothList[i].bCanMove;
			inFile >> qSmoothList[i].newPos[0];
			inFile >> qSmoothList[i].newPos[1];
			inFile >> qSmoothList[i].newPos[2];
			inFile >> qSmoothList[i].qValue_afterMove;
			inFile >> qSmoothList[i].qValueImprovement;
		}
	}
	else
		return false;

	// record qSmoothIndicesList
	if (inFile.good())
	{
		unsigned int numSmoothIndices;
		inFile >> numSmoothIndices;
		qSmoothIndicesList.resize(numSmoothIndices);
		for (unsigned int i = 0; i < numSmoothIndices; i++)
		{
			inFile >> qSmoothIndicesList[i];
		}
	}
	else
		return false;

	// record qDecimateList	
	if (inFile.good())
	{
		int numSize;
		inFile >> numSize;
		qDecimateList.resize(numSize);
		for (int i = 0; i < numSize; i++)
		{
			inFile >> qDecimateList[i].q.vIdx;
			for (int j = 0; j < 9; j++)
				inFile >> qDecimateList[i].q.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qDecimateList[i].q.b[j];
			inFile >> qDecimateList[i].q.c;
			inFile >> qDecimateList[i].q.quadricValue;
			for (int j = 0; j < 9; j++)
				inFile >> qDecimateList[i].q.fn.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qDecimateList[i].q.fn.b[j];
			inFile >> qDecimateList[i].q.fn.c;
			inFile >> qDecimateList[i].bCanMove;
			inFile >> qDecimateList[i].newPos[0];
			inFile >> qDecimateList[i].newPos[1];
			inFile >> qDecimateList[i].newPos[2];
			inFile >> qDecimateList[i].qValue_afterMove;
			inFile >> qDecimateList[i].qValueImprovement;
		}
	}
	else
		return false;

	// record qDecimateIndicesList
	if (inFile.good())
	{
		unsigned int numDecimateIndices;
		inFile >> numDecimateIndices;
		qDecimateIndicesList.resize(numDecimateIndices);
		for (unsigned int i = 0; i < numDecimateIndices; i++)
		{
			inFile >> qDecimateIndicesList[i];
		}
	}
	else
		return false;

	// record qVertexDecimateList
	if (inFile.good())
	{
		int numSize;
		inFile >> numSize;
		qVertexDecimateList.resize(numSize);
		for (int i = 0; i < numSize; i++)
		{
			inFile >> qVertexDecimateList[i].vIdx;
			for (int j = 0; j < 9; j++)
				inFile >> qVertexDecimateList[i].A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qVertexDecimateList[i].b[j];
			inFile >> qVertexDecimateList[i].c;
			inFile >> qVertexDecimateList[i].quadricValue;
			for (int j = 0; j < 9; j++)
				inFile >> qVertexDecimateList[i].fn.A[j];
			for (int j = 0; j < 3; j++)
				inFile >> qVertexDecimateList[i].fn.b[j];
			inFile >> qVertexDecimateList[i].fn.c;
		}
	}
	else
		return false;

	// load numPlanesInQuadricsList
	if (inFile.good())
	{
		int numSize;
		inFile >> numSize;
		numPlanesInQuadricsList.resize(numSize);
		for (int i = 0; i < numSize; i++)
			inFile >> numPlanesInQuadricsList[i];
	}
	else
		return false;


	// record centroid for taubin smoothing
	if (inFile.good())
	{
		centroidList = new double[3*numNoVertices];
		for (int i = 0; i < numNoVertices; i++)
		{
			inFile >> centroidList[3*i];
			inFile >> centroidList[3*i+1];
			inFile >> centroidList[3*i+2];
		}
	}
	else
		return false;

	// record lastMovedVertexPos
	if (inFile.good())
	{
		inFile >> lastMovedVertexPos[0];
		inFile >> lastMovedVertexPos[1];
		inFile >> lastMovedVertexPos[2];
	}
	else
		return false;

	// record lastMovedVertexIdx
	if (inFile.good())
		inFile >> lastMovedVertexIdx;
	else
		return false;

	// record isLastMovedVertexSet
	if (inFile.good())
		inFile >> isLastMovedVertexSet;
	else
		return false;

	// record bIsLoaded
	if (inFile.good())
		inFile >> bIsLoaded;
	else
		return false;
	
	inFile.close();
	
	cout << "\nSuccessfully loaded " << filename << endl;
	
	return true;
}

// saves debug info
// filename (in): name of the debug file to save as
// return true if success
// TODO: add dirtyOptimizeQMs, dirtyOptimizeBackLinks
bool NonobtOptn::saveDebug(char* filename) const
{
	if (!isLoaded())
		return false;
	
	ofstream outFile;
	outFile.open(filename, ofstream::out);
	outFile.precision(10);
	
	if (!outFile.is_open())
		return false;
	
	// -------- DESIRED MESH -------------

	// first line records the number of vertices and number of polygons for desired mesh
	outFile << "#desiredMesh " << numDVertices << " " << numDPolygons << endl;
	
	// record list of vertices
	for (int i = 0; i < numDVertices; i++)
	{
		outFile << "v " << dVerticesList[3*i];
		outFile << " " << dVerticesList[(3*i)+1];
		outFile << " " << dVerticesList[(3*i)+2] << endl;
	}
	
	// record list of polygons
	for (int i = 0; i < numDPolygons; i++)
	{
		outFile << "f " << dPolygonsList[i][0];
		outFile << " " << dPolygonsList[i][1];
		outFile << " " << dPolygonsList[i][2] << endl;
	}

	// record vertex neighbour list
	for (int i = 0; i < numDVertices; i++)
	{
		outFile << "d_vNbr_numNeighbours " << dVertexNeighbourList[i].size() << endl;
		for (unsigned int j = 0; j < dVertexNeighbourList[i].size(); j++)
		{
			outFile << dVertexNeighbourList[i][j] << endl;
		}
	}

	// record edge neighbour list
	for (int i = 0; i < numDVertices; i++)
	{
		outFile << "d_eNbr_numNeighbours " << dEdgeNeighbourList[i].size() << endl;
		for (unsigned int j = 0; j < dEdgeNeighbourList[i].size(); j++)
		{
			outFile << dEdgeNeighbourList[i][j].e.vertex1Idx;
			outFile << " " << dEdgeNeighbourList[i][j].e.vertex2Idx;
			outFile << " " << dEdgeNeighbourList[i][j].polyNeighbour1Idx;
			outFile << " " << dEdgeNeighbourList[i][j].polyNeighbour2Idx << endl;
		}
	}

	// record polygon neighbour list
	for (int i = 0; i < numDPolygons; i++)
	{
		outFile << "d_pNbr_numNeighbours " << dPolygonNeighbourList[i].size() << endl;
		for (unsigned int j = 0; j < dPolygonNeighbourList[i].size(); j++)
		{
			outFile << dPolygonNeighbourList[i][j] << endl;
		}
	}

	// record face normals
	for (int i = 0; i < numDPolygons; i++)
	{
		outFile << dFaceNormalsList[3*i] << " " << dFaceNormalsList[3*i+1] << " " << dFaceNormalsList[3*i+2] << endl;
	}

	// record vertex normals
	for (int i = 0; i < numDVertices; i++)
	{
		outFile << dVertexNormalsList[3*i] << " " << dVertexNormalsList[3*i+1] << " " << dVertexNormalsList[3*i+2] << endl;
	}

	// -------- NONOBTUSE MESH -------------

	// first line records the number of vertices and number of polygons for nonobtuse mesh
	outFile << "#NonobtuseMesh " << numNoVertices << " " << numNoPolygons << endl;
	
	// record list of vertices
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << "v " << noVerticesList[3*i];
		outFile << " " << noVerticesList[(3*i)+1];
		outFile << " " << noVerticesList[(3*i)+2] << endl;
	}
	
	// record list of polygons
	for (int i = 0; i < numNoPolygons; i++)
	{
		outFile << "f " << noPolygonsList[i][0];
		outFile << " " << noPolygonsList[i][1];
		outFile << " " << noPolygonsList[i][2] << endl;
	}

	// record vertex neighbour list
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << "no_vNbr_numNeighbours " << noVertexNeighbourList[i].size() << endl;
		for (unsigned int j = 0; j < noVertexNeighbourList[i].size(); j++)
		{
			outFile << noVertexNeighbourList[i][j] << endl;
		}
	}

	// record face normals
	for (int i = 0; i < numNoPolygons; i++)
	{
		outFile << noFaceNormalsList[3*i] << " " << noFaceNormalsList[3*i+1] << " " << noFaceNormalsList[3*i+2] << endl;
	}

	// record vertex normals
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << noVertexNormalsList[3*i] << " " << noVertexNormalsList[3*i+1] << " " << noVertexNormalsList[3*i+2] << endl;
	}

	// record noEdgeList
	unsigned int numEdges = noEdgeList.size();
	outFile << numEdges << endl;
	for (unsigned int i = 0; i < numEdges; i++)
	{
		outFile << noEdgeList[i].vertex1Idx << " " << noEdgeList[i].vertex2Idx << endl;
	}

	// record noVertexEdgeList
	for (int i = 0; i < numNoVertices; i++)
	{
		int numEdges = noVertexEdgeList[i].size();
		outFile << numEdges << endl;
		for (int j = 0; j < numEdges; j++)
		{
			outFile << noVertexEdgeList[i][j] << endl;
		}
	}

	// record unusedEdgeIdxList
	unsigned int numUnusedEdge = unusedEdgeIdxList.size();
	outFile << numUnusedEdge << endl;
	for (unsigned int i = 0; i < numUnusedEdge; i++)
	{
		outFile << unusedEdgeIdxList[i] << endl;
	}
	
	// ------ other debug info ----------

	// record closest face list
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << closestFaceList[i] << endl;
	}

	// record closest boundary edge list
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << closestBoundaryEdgeList[i].vertex1Idx << endl;
		outFile << closestBoundaryEdgeList[i].vertex2Idx << endl;
	}

	// record closest pt list
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << closestPtList[3*i] << endl;
		outFile << closestPtList[3*i+1] << endl;
		outFile << closestPtList[3*i+2] << endl;
	}

	// record qMoveList
// 	outFile << qMoveList.size() << endl;
// 	for (unsigned int i = 0; i < qMoveList.size(); i++)
// 	{
// 		outFile << qMoveList[i].q.vIdx << endl;
// 		for (int j = 0; j < 9; j++)
// 			outFile << qMoveList[i].q.A[j] << endl;
// 		for (int j = 0; j < 3; j++)
// 			outFile << qMoveList[i].q.b[j] << endl;
// 		outFile << qMoveList[i].q.c << endl;
// 		outFile << qMoveList[i].q.quadricValue << endl;
// 		for (int j = 0; j < 9; j++)
// 			outFile << qMoveList[i].q.fn.A[j] << endl; 
// 		for (int j = 0; j < 3; j++)
// 			outFile << qMoveList[i].q.fn.b[j] << endl;
// 		outFile << qMoveList[i].q.fn.c << endl;
// 		if (qMoveList[i].bCanMove)
// 			outFile << 1 << endl;
// 		else
// 			outFile << 0 << endl;
// 		outFile << qMoveList[i].newPos[0] << " " << qMoveList[i].newPos[1] << " " << qMoveList[i].newPos[2] << endl;
// 		outFile << qMoveList[i].qValue_afterMove << " " << qMoveList[i].qValueImprovement << endl;
// 	}
// 
// 	// record qMoveIndicesList
// 	unsigned int numMoveIndices = qMoveIndicesList.size();
// 	outFile << numMoveIndices << endl;
// 	for (unsigned int i = 0; i < numMoveIndices; i++)
// 	{
// 		outFile << qMoveIndicesList[i] << endl;
// 	}

	// record qMovePtrList
	outFile << qMovePtrList.size() << endl;
	for (unsigned int i = 0; i < qMovePtrList.size(); ++i)
	{
		outFile << qMovePtrList[i]->q.vIdx << endl;
		for (int j = 0; j < 9; j++)
			outFile << qMovePtrList[i]->q.A[j] << endl;
		for (int j = 0; j < 3; j++)
			outFile << qMovePtrList[i]->q.b[j] << endl;
		outFile << qMovePtrList[i]->q.c << endl;
		outFile << qMovePtrList[i]->q.quadricValue << endl;
		for (int j = 0; j < 9; j++)
			outFile << qMovePtrList[i]->q.fn.A[j] << endl; 
		for (int j = 0; j < 3; j++)
			outFile << qMovePtrList[i]->q.fn.b[j] << endl;
		outFile << qMovePtrList[i]->q.fn.c << endl;
		if (qMovePtrList[i]->bCanMove)
			outFile << 1 << endl;
		else
			outFile << 0 << endl;
		outFile << qMovePtrList[i]->newPos[0] << " " << qMovePtrList[i]->newPos[1] << " " << qMovePtrList[i]->newPos[2] << endl;
		outFile << qMovePtrList[i]->qValue_afterMove << " " << qMovePtrList[i]->qValueImprovement << endl;
	}

	// record qMoveSortPtrList
	outFile << qMoveSortPtrList.size() << endl;
	for (unsigned int i = 0; i < qMoveSortPtrList.size(); ++i)
	{
		outFile << qMoveSortPtrList[i]->q.vIdx << endl;
	}

	// record qSmoothList
	outFile << qSmoothList.size() << endl;
	for (unsigned int i = 0; i < qSmoothList.size(); i++)
	{
		outFile << qSmoothList[i].q.vIdx << endl;
		for (int j = 0; j < 9; j++)
			outFile << qSmoothList[i].q.A[j] << endl;
		for (int j = 0; j < 3; j++)
			outFile << qSmoothList[i].q.b[j] << endl;
		outFile << qSmoothList[i].q.c << endl;
		outFile << qSmoothList[i].q.quadricValue << endl;
		for (int j = 0; j < 9; j++)
			outFile << qSmoothList[i].q.fn.A[j] << endl; 
		for (int j = 0; j < 3; j++)
			outFile << qSmoothList[i].q.fn.b[j] << endl;
		outFile << qSmoothList[i].q.fn.c << endl;
		if (qSmoothList[i].bCanMove)
			outFile << 1 << endl;
		else
			outFile << 0 << endl;
		outFile << qSmoothList[i].newPos[0] << " " << qSmoothList[i].newPos[1] << " " << qSmoothList[i].newPos[2] << endl;
		outFile << qSmoothList[i].qValue_afterMove << " " << qSmoothList[i].qValueImprovement << endl;
	}

	// record qSmoothIndicesList
	unsigned int numSmoothIndices = qSmoothIndicesList.size();
	outFile << numSmoothIndices << endl;
	for (unsigned int i = 0; i < numSmoothIndices; i++)
	{
		outFile << qSmoothIndicesList[i] << endl;
	}

	// record qDecimateList
	outFile << qDecimateList.size() << endl;
	for (unsigned int i = 0; i < qDecimateList.size(); i++)
	{
		outFile << qDecimateList[i].q.vIdx << endl;
		for (int j = 0; j < 9; j++)
			outFile << qDecimateList[i].q.A[j] << endl;
		for (int j = 0; j < 3; j++)
			outFile << qDecimateList[i].q.b[j] << endl;
		outFile << qDecimateList[i].q.c << endl;
		outFile << qDecimateList[i].q.quadricValue << endl;
		for (int j = 0; j < 9; j++)
			outFile << qDecimateList[i].q.fn.A[j] << endl; 
		for (int j = 0; j < 3; j++)
			outFile << qDecimateList[i].q.fn.b[j] << endl;
		outFile << qDecimateList[i].q.fn.c << endl;
		if (qDecimateList[i].bCanMove)
			outFile << 1 << endl;
		else
			outFile << 0 << endl;
		outFile << qDecimateList[i].newPos[0] << " " << qDecimateList[i].newPos[1] << " " << qDecimateList[i].newPos[2] << endl;
		outFile << qDecimateList[i].qValue_afterMove << " " << qDecimateList[i].qValueImprovement << endl;
	}

	// record qDecimateIndicesList
	unsigned int numDecimateIndices = qDecimateIndicesList.size();
	outFile << numDecimateIndices << endl;
	for (unsigned int i = 0; i < numDecimateIndices; i++)
	{
		outFile << qDecimateIndicesList[i] << endl;
	}

	// record qVertexDecimateList
	outFile << qVertexDecimateList.size() << endl;
	for (unsigned int i = 0; i < qVertexDecimateList.size(); i++)
	{
		outFile << qVertexDecimateList[i].vIdx << endl;
		for (int j = 0; j < 9; j++)
			outFile << qVertexDecimateList[i].A[j] << endl;
		for (int j = 0; j < 3; j++)
			outFile << qVertexDecimateList[i].b[j] << endl;
		outFile << qVertexDecimateList[i].c << endl;
		outFile << qVertexDecimateList[i].quadricValue << endl;
		for (int j = 0; j < 9; j++)
			outFile << qVertexDecimateList[i].fn.A[j] << endl;
		for (int j = 0; j < 3; j++)
			outFile << qVertexDecimateList[i].fn.b[j] << endl;
		outFile << qVertexDecimateList[i].fn.c << endl;
	}

	// record numPlanesInQuadricsList
	outFile << numPlanesInQuadricsList.size() << endl;
	for (unsigned int i = 0; i < numPlanesInQuadricsList.size(); i++)
		outFile << numPlanesInQuadricsList[i] << endl;

	// record centroid for taubin smoothing
	for (int i = 0; i < numNoVertices; i++)
	{
		outFile << centroidList[3*i] << " " << centroidList[3*i+1] << " " << centroidList[3*i+2] << endl;
	}

	// record lastMovedVertexPos
	outFile << lastMovedVertexPos[0] << " " << lastMovedVertexPos[1] << " " << lastMovedVertexPos[2] << endl;

	// record lastMovedVertexIdx
	outFile << lastMovedVertexIdx << endl;

	// record isLastMovedVertexSet
	if (isLastMovedVertexSet)
		outFile << 1 << endl;
	else
		outFile << 0 << endl;

	// record bIsLoaded
	if (bIsLoaded)
		outFile << 1 << endl;
	else
		outFile << 0 << endl;
	
	outFile.close();
	
	cout << "\nSuccessfully saved " << filename << endl;
	
	return true;
}

// returns true if mesh info are loaded
bool NonobtOptn::isLoaded() const
{
	return bIsLoaded;
}

// return pointer to non-obtuse vertices list
// return val (out): pointer to non-obtuse vertices list
const double* NonobtOptn::getNoVertices() const
{
	return noVerticesList;
}

// return the number of vertices in the non-obtuse mesh
// return val (out): number of vertices in the non-obtuse mesh
int NonobtOptn::getNumNoVertices() const
{
	return numNoVertices;
}

// returns pointer to non-obtuse polygon list
// return val (out): pointer to non-obtuse polygon list
const int* const* NonobtOptn::getNoPolygons() const
{
	return noPolygonsList;
}

// return the number of polygons in the non-obtuse mesh
// return val (out): number of polygons in the non-obtuse mesh
int NonobtOptn::getNumNoPolygons() const
{
	return numNoPolygons;
}

// computes the face normal for the desired mesh and the nonobtuse mesh
void NonobtOptn::computeFaceNormal()
{
	// --------- desired mesh ------------
	// sanity checks
	if (dFaceNormalsList != NULL)
		delete[] dFaceNormalsList;
	
	dFaceNormalsList = new double[numDPolygons*3];
	for (int i = 0; i < numDPolygons; i++)
	{
		double normal[3];
		getFaceNormal(i, dPolygonsList, dVerticesList, normal);
		for (int j = 0; j < 3; j++)
			dFaceNormalsList[3*i+j] = normal[j];
	}
	
	// --------- nonobtuse mesh ------------
	// sanity checks
	if (noFaceNormalsList != NULL)
		delete[] noFaceNormalsList;
	
	noFaceNormalsList = new double[numNoPolygons*3];
	for (int i = 0; i < numNoPolygons; i++)
	{
		double normal[3];
		getFaceNormal(i, noPolygonsList, noVerticesList, normal);
		for (int j = 0; j < 3; j++)
			noFaceNormalsList[3*i+j] = normal[j];
	}
	
}

// returns the face normal of polygon with index 'polygonIndex'
// polygonIndex (in): index of polygon we are computing
// polygonsList (in): polygon array
// verticesList (in): vertex array
// normal (out): computed face normal
void NonobtOptn::getFaceNormal(int polygonIndex, int** polygonsList, double* verticesList, double* normal) const
{
	double edge1[3];
	double edge2[3];
	
	vectorSub(edge1,
				verticesList[3*polygonsList[polygonIndex][1]],
				verticesList[3*polygonsList[polygonIndex][1]+1],
				verticesList[3*polygonsList[polygonIndex][1]+2],
				verticesList[3*polygonsList[polygonIndex][0]],
				verticesList[3*polygonsList[polygonIndex][0]+1],
				verticesList[3*polygonsList[polygonIndex][0]+2]);
	vectorSub(edge2,
				verticesList[3*polygonsList[polygonIndex][2]],
				verticesList[3*polygonsList[polygonIndex][2]+1],
				verticesList[3*polygonsList[polygonIndex][2]+2],
				verticesList[3*polygonsList[polygonIndex][0]],
				verticesList[3*polygonsList[polygonIndex][0]+1],
				verticesList[3*polygonsList[polygonIndex][0]+2]);
	crossProduct(normal, edge1, edge2);
	normalize(normal);
}

// computes the vertex normal for the mesh
void NonobtOptn::computeVertexNormal()
{
	// --------- desired mesh ------------
	// sanity checks
	if (dFaceNormalsList == NULL)    // must compute face normals first
		return;
	if (dVertexNormalsList != NULL)
		delete[] dVertexNormalsList;
	
	// allocate space
	dVertexNormalsList = new double[numDVertices*3];
	
	for (int i = 0; i < numDVertices; i++)
	{
		double vertexNormal[3];
		getVertexNormal(i, dPolygonsList, dVerticesList, dVertexNeighbourList, dFaceNormalsList, vertexNormal);
		for (int j = 0; j < 3; j++)
			dVertexNormalsList[3*i+j] = vertexNormal[j];    
	}
	
	// --------- nonobtuse mesh ------------
	// sanity checks
	if (noFaceNormalsList == NULL)    // must compute face normals first
		return;
	if (noVertexNormalsList != NULL)
		delete[] noVertexNormalsList;
	
	// allocate space
	noVertexNormalsList = new double[numNoVertices*3];
	
	for (int i = 0; i < numNoVertices; i++)
	{
		double vertexNormal[3];
		getVertexNormal(i, noPolygonsList, noVerticesList, noVertexNeighbourList, noFaceNormalsList, vertexNormal);
		for (int j = 0; j < 3; j++)
			noVertexNormalsList[3*i+j] = vertexNormal[j];    
	}
	
}

// returns the weighted average ratio for all neighbour polygons of 'vertexindex'
// vertexIndex (in): index of vertex that is in the center of one-ring
// polygonsList (in): polygon array
// verticesList (in): vertex array
// vertexNeighbourList (in): vertex neighbour array
// weightedAvgRation (out): array of weighted avg. for each polygon in one-ring
void NonobtOptn::getWeightedAvgRatio(int vertexIndex, 
									 int** polygonsList, 
									 double* verticesList, 
									 vector<int>* vertexNeighbourList, 
									 double* &weightedAvgRatio) const
{
	if (weightedAvgRatio != NULL) // weightedAvgRatio needs to be NULL
		return;
	
	// setup output allocation
	int numNeighbours = vertexNeighbourList[vertexIndex].size();
	weightedAvgRatio = new double[numNeighbours];
	
	// setup allocation to store the polygon's area
	double* neighbourArea = new double[numNeighbours];
	double sumNeighbourArea(0.0);
	
	// compute each polygon's area
	int i(0);
	double *pt1 = new double[3];
	double *pt2 = new double[3];
	double *pt3 = new double[3];
	for (vector<int>::iterator iter = vertexNeighbourList[vertexIndex].begin();
		iter != vertexNeighbourList[vertexIndex].end();
		iter++)
	{
		pt1[0] = verticesList[3*polygonsList[*iter][0]];
		pt1[1] = verticesList[(3*polygonsList[*iter][0])+1];
		pt1[2] = verticesList[(3*polygonsList[*iter][0])+2];
		pt2[0] = verticesList[3*polygonsList[*iter][1]];
		pt2[1] = verticesList[(3*polygonsList[*iter][1])+1];
		pt2[2] = verticesList[(3*polygonsList[*iter][1])+2];
		pt3[0] = verticesList[3*polygonsList[*iter][2]];
		pt3[1] = verticesList[(3*polygonsList[*iter][2])+1];
		pt3[2] = verticesList[(3*polygonsList[*iter][2])+2];
	
		neighbourArea[i] = areaTriangle(pt1, pt2, pt3);
		sumNeighbourArea += neighbourArea[i];
		i++;
	}
	
	if (sumNeighbourArea == 0.0)   // something wrong
		return;
	
	// calculate the weighted average
	for (int i = 0; i < numNeighbours; i++)
		weightedAvgRatio[i] = neighbourArea[i] / sumNeighbourArea;
	
	delete[] pt1;
	delete[] pt2;
	delete[] pt3;
	delete[] neighbourArea;
}

// returns the vertex normal of the vertex with index 'vertexIndex'
// vertexIndex (in): index of vertex that is in the center of one-ring
// polygonsList (in): polygon array
// verticesList (in): vertex array
// vertexNeighbourList (in): vertex neighbour array
// normal (out): vertex normal
void NonobtOptn::getVertexNormal(int vertexIndex, 
								int** polygonsList, 
								double* verticesList, 
								vector<int>* vertexNeighbourList,
								double* faceNormalsList,
								double* normal) const
{
	// sanity checks
	if (faceNormalsList == NULL)
		return;
	
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 0.0;
	
	double* weightedAvgRatio = NULL;
	getWeightedAvgRatio(vertexIndex, polygonsList, verticesList, vertexNeighbourList, weightedAvgRatio);
	
	// weighted avg of the nearby face normals
	int numNeighbours = vertexNeighbourList[vertexIndex].size();
	for (int i = 0; i < numNeighbours; i++)
	{
		normal[0] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]];
		normal[1] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]+1];
		normal[2] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]+2];
	}
	
	normalize(normal);
	
	// clean up
	delete[] weightedAvgRatio;
}

// optimize mesh based on quadric error after vertex move
// numMaxIteration (in): max number of iteration to run
// alpha (in): weighting alpha value for objective function
// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the optimization
// improvementThreshold (in): minimum improvement value before stopping the optimization
// numOneRingSearch (in): number of one-ring to search for closest face
// numOneRingQuadric (in): number of one-ring to use for computing quadric
// angleBound (in): angle bound (in degrees)
// bRecomputeQuadrics (in): true - recompute all quadrics
// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated when bRecomputeQuadrics is true
// bCleanDirtyWhenStuck (in): true - clean dirty vertices when optimization stops, then try to continue optimization
// region (in): determines how the non-obtuse region is to be approximated
// return value (out): total time taken
clock_t NonobtOptn::optimize_afterMove(unsigned int numMaxIteration, 
									double quadricThreshold, 
									double improvementThreshold,
									double alpha, 
									unsigned int numOneRingSearch, 
									unsigned int numOneRingQuadric,
									double angleBound,
									bool bRecomputeQuadrics,
									bool bUpdateClosestPolygon,
									bool bCleanDirtyWhenStuck,
									NORegionType region)
{
	avgSolvingTime = 0;
	maxSolvingTime = 0;
	avgSetupTime = 0;
	maxSetupTime = 0;
	numSolve = 0;
	numSetup = 0;

	numLinSearchFound = 0;
	numLinSearchInFeasibleRegion = 0;

	unsigned int numFullUpdate = 0;
	unsigned int iterationCount = 0;
	int lastVertexMovedIdx = -1;
	bool continueOptimize(true);

	double lastImprovementValue = 0.0;

	double lastavgQVal = 0.0;
	double avgQVal = 0.0;

	// ----- for error plot -----
// 	ofstream outMaxQMFile;
// 	outMaxQMFile.open("/cs/grad1/ysl/meshes/Nonobtuse/errorplot/maxQuadricErrorPlot.dat", ofstream::out);
// 	outMaxQMFile.precision(10);
// 
// 	ofstream outAvgQMFile;
// 	outAvgQMFile.open("/cs/grad1/ysl/gruvi_homeDir/sgp06/errorplot/avgQuadricErrorPlot.dat", ofstream::out);
// 	outAvgQMFile.precision(10);
// 	ofstream outAvgQMChangeFile;
// 	outAvgQMChangeFile.open("/cs/grad1/ysl/gruvi_homeDir/sgp06/errorplot/avgQuadricChangeErrorPlot.dat", ofstream::out);
// 	outAvgQMChangeFile.precision(10);
// 
// 	ofstream outCanMoveFile;
// 	outCanMoveFile.open("/cs/grad1/ysl/meshes/Nonobtuse/errorplot/numCanMovePlot.dat", ofstream::out);
// 	outCanMoveFile.precision(10);
// 
// 	ofstream outQEMFile;
// 	outQEMFile.open("/cs/grad1/ysl/meshes/Nonobtuse/errorplot/QEMPlot.dat", ofstream::out);
// 	outQEMFile.precision(10);
// 
// 	QuadricMove* maxQM = NULL;
// 	double avgQuadValue(0.0);
// 	unsigned int numCanMove = 0;
	// ----- end error plot -----

	clock_t b4Time = clock();
	clock_t afterTime = clock();
	
	cout << "Optimizing... " << endl;
	while (continueOptimize)
	{
// 		bool hasPassThreshold(false);	// true if at least one vertex has quadric value > quadricThreshold

		if (bRecomputeQuadrics || qMovePtrList.empty())
		{
			bRecomputeQuadrics = false;
			qMovePtrList.resize(numNoVertices);
			qMoveSortPtrList.clear();
			cout << "Computing Quadrics for all vertices (" << numNoVertices << ") for the first time" << flush;
			make_heap(qMoveSortPtrList.begin(), qMoveSortPtrList.end(), my_quadMovePtr_largestImprovementFirst);
			
			unsigned int lastPct = 0;
			// compute quadrics for each vertex
			for (int i = 0; i < numNoVertices; ++i)
			{
				//cout << "processing vertex " << i << "/" << numNoVertices << "..." << endl;
				unsigned int currentPct = (unsigned int)(((float)i/numNoVertices) * 100);
				if (currentPct > lastPct && currentPct % 5 == 0)
				{
					cout << "." << flush;
				}
				lastPct = currentPct;

				QuadricMove* qm = new QuadricMove;
				qm->q = computeQuadric(i, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
// 				qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);
				unsigned int numTries(0);
				qm->bCanMove = computeOptimalLocation_linearSearch(qm->q.vIdx, qm->q, qm->newPos, angleBound, numTries);
// 				if (!qm->bCanMove)
// 					qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);

				avgQVal += qm->q.quadricValue;

				double minDist(0.0);
				double closestPt[3] = {0,0,0};
				if (isBoundaryVertex(qm->q.vIdx))
				{
					Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm->q.vIdx, qm->newPos, closestPt, numOneRingSearch, false);
					qm->qValue_afterMove = computeBoundaryQuadricValue(qm->newPos, closestPt, alpha, numOneRingQuadric);
					qm->dirtyQVal_AfterMove = false;
				}
				else
				{
					int closestPolyIdx_afterMove = findClosestPolygon(qm->newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], numOneRingSearch);
					qm->qValue_afterMove = computeQuadricValue(qm->newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
					qm->dirtyQVal_AfterMove = false;
				}
				qm->qValueImprovement = qm->q.quadricValue - qm->qValue_afterMove;	
				qm->dirtyImprovement = false;

				dirtyOptimizeBackLinks.push_back(NULL);

				qMovePtrList[i] = qm;
				qMoveSortPtrList.push_back(qm);
				push_heap(qMoveSortPtrList.begin(), qMoveSortPtrList.end(), my_quadMovePtr_largestImprovementFirst);
			}
			avgQVal /= numNoVertices;
			cout << "done!" << endl;
		}

// 		for (unsigned int i = 0; i < qMovePtrList.size(); ++i)
// 		{
// 			// check if current vertex has quadrics exceeded the threshold
// 			if (!hasPassThreshold && qMovePtrList[i]->q.quadricValue > quadricThreshold) {
// 				hasPassThreshold = true;
// 				break;
// 			}
// 		}

		// ------ for error plot ------
// 		numCanMove = 0;
// 		for (unsigned int i = 0; i < qMovePtrList.size(); ++i)
// 		{
// 			QuadricMove* qm = qMovePtrList[i];
// 			if (maxQM == NULL || maxQM->q.quadricValue < qm->q.quadricValue)
// 				maxQM = qm;
// 			if (qm->bCanMove)
// 				++numCanMove;
// 
// 			avgQuadValue += qm->q.quadricValue;
// 		}
// 		avgQuadValue /= qMovePtrList.size();
// 		
// 		outMaxQMFile << iterationCount << " " << maxQM->q.quadricValue << endl;
// 		outAvgQMFile << iterationCount << " " << avgQuadValue << endl;
// 		outCanMoveFile << iterationCount << " " << numCanMove << endl;
		// ------ end error plot ------

		if (iterationCount != 0 && iterationCount == numMaxIteration) {	// max iterations reached
			break;
		}
// 		if (!hasPassThreshold) {	// all vertices have quadrics within the threshold
// 			break;
// 		}

//   		cout << iterationCount << " - Avg. Quadric value: " << avgQVal << endl;
//   		cout << iterationCount << " - Avg. Quadric value (normalize): " << avgQVal/bboxDiagonal << endl;
//  		cout << iterationCount << " - Change in Avg. Quadric value: " << lastavgQVal - avgQVal << endl;
// 		cout << iterationCount << " - Change in Avg. Quadric value (normalize): " << lastavgQVal/bboxDiagonal - avgQVal/bboxDiagonal << endl;
//  	outAvgQMFile << iterationCount << " " << avgQVal << endl;
// 		outAvgQMChangeFile << iterationCount << " " << lastavgQVal - avgQVal << endl;
// 		if (avgQVal/bboxDiagonal < quadricThreshold)
// 			break;

		// --- debug
//		vector<QuadricMove*> qMoveList_debug = qMovePtrList;
//		make_heap(qMoveList_debug.begin(), qMoveList_debug.end(), my_quadMovePtr_largestImprovementFirst);
//		sort_heap(qMoveList_debug.begin(), qMoveList_debug.end(), my_quadMovePtr_largestImprovementFirst);
		// --- end debug

		QuadricMove* qm_front = qMoveSortPtrList.front();
		while (qm_front->dirtyImprovement)
		{
			pop_heap(qMoveSortPtrList.begin(), qMoveSortPtrList.end(), my_quadMovePtr_largestImprovementFirst);

			// update quadrics
			if (qm_front->q.dirtyQVal)
				qm_front->q = computeQuadric(qm_front->q.vIdx, alpha, numOneRingSearch, numOneRingQuadric);

			// update moved quadric
// 			qm_front->bCanMove = computeOptimalLocation(qm_front->q.vIdx, qm_front->q, qm_front->newPos, region, angleBound);
			unsigned int numTries(0);
			qm_front->bCanMove = computeOptimalLocation_linearSearch(qm_front->q.vIdx, qm_front->q, qm_front->newPos, angleBound, numTries);
// 			if (!qm_front->bCanMove)
// 				qm_front->bCanMove = computeOptimalLocation(qm_front->q.vIdx, qm_front->q, qm_front->newPos, region, angleBound);
				
			double minDist(0.0);
			double closestPt[3] = {0,0,0};
			if (isBoundaryVertex(qm_front->q.vIdx))
			{
				Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm_front->q.vIdx, qm_front->newPos, closestPt, numOneRingSearch, false);
				qm_front->qValue_afterMove = computeBoundaryQuadricValue(qm_front->newPos, closestPt, alpha, numOneRingQuadric);
				qm_front->dirtyQVal_AfterMove = false;
			}
			else
			{
				int closestPolyIdx_afterMove = findClosestPolygon(qm_front->newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[qm_front->q.vIdx], numOneRingSearch);
				qm_front->qValue_afterMove = computeQuadricValue(qm_front->newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
				qm_front->dirtyQVal_AfterMove = false;
			}
			
			qm_front->qValueImprovement = qm_front->q.quadricValue - qm_front->qValue_afterMove;
			qm_front->dirtyImprovement = false;

			unsigned int swapIdx = dirtyOptimizeQMs.back()->q.vIdx;
			swap(*(dirtyOptimizeBackLinks[qm_front->q.vIdx]), dirtyOptimizeQMs.back());
			swap(dirtyOptimizeBackLinks[qm_front->q.vIdx], dirtyOptimizeBackLinks[swapIdx]);

			dirtyOptimizeQMs.pop_back();
			dirtyOptimizeBackLinks[qm_front->q.vIdx] = NULL;

			push_heap(qMoveSortPtrList.begin(), qMoveSortPtrList.end(), my_quadMovePtr_largestImprovementFirst);
			qm_front = qMoveSortPtrList.front();
		}

		if (qm_front->bCanMove && qm_front->qValueImprovement/bboxDiagonal > improvementThreshold)
		{
// 			cout << iterationCount << " - Improvement value: " << qm_front->qValueImprovement << endl;
//   			cout << iterationCount << " - Improvement value (normalize): " << qm_front->qValueImprovement/bboxDiagonal << endl;

			lastImprovementValue = qm_front->qValueImprovement;

			lastavgQVal = avgQVal;
			avgQVal = (avgQVal * numNoVertices - qm_front->q.quadricValue + qm_front->qValue_afterMove) / numNoVertices;

			// ---- debug only ----
			lastMovedVertexOldPos[0] = noVerticesList[3*(qm_front->q.vIdx)];
			lastMovedVertexOldPos[1] = noVerticesList[3*(qm_front->q.vIdx)+1];
			lastMovedVertexOldPos[2] = noVerticesList[3*(qm_front->q.vIdx)+2];
			lastMovedVertexIdx = qm_front->q.vIdx;
			isLastMovedVertexSet = true;
			// ---- end debug -----

			updateMesh(qm_front->q.vIdx, qm_front->newPos);
			lastVertexMovedIdx = qm_front->q.vIdx;
			qm_front->dirtyImprovement = true;
			qm_front->dirtyQVal_AfterMove = true;
			qm_front->q.dirtyQVal = true;

			dirtyOptimizeQMs.push_back(qm_front);
			dirtyOptimizeBackLinks[qm_front->q.vIdx] = &(dirtyOptimizeQMs.back());

			//cout << "Iteration " << iterationCount << ": moved vertex " << lastVertexMovedIdx << endl;

			// make dirty quadrics for the moved vertex and its one-ring
			unsigned int numOneRingTriangles = noVertexNeighbourList[lastVertexMovedIdx].size();
			for (unsigned int i = 0; i < numOneRingTriangles; ++i)
			{
				unsigned int currentPolyIdx = noVertexNeighbourList[lastVertexMovedIdx][i];
				for (unsigned int j = 0; j < 3; ++j)
				{
					unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
					if (!qMovePtrList[vIdx]->dirtyImprovement)
					{
						qMovePtrList[vIdx]->dirtyQVal_AfterMove = true;
						qMovePtrList[vIdx]->dirtyImprovement = true;
						
						qMovePtrList[vIdx]->q.dirtyQVal = true;		// update the smoothness term since centroid has moved

						dirtyOptimizeQMs.push_back(qMovePtrList[vIdx]);
						dirtyOptimizeBackLinks[vIdx] = &(dirtyOptimizeQMs.back());
					}
				}
			}
		}
// 		else if (!dirtyOptimizeQMs.empty())	// time to clean up all dirty vertices
		if (!dirtyOptimizeQMs.empty())	// time to clean up all dirty vertices
		{
			if (!bCleanDirtyWhenStuck)
 				break;

			++numFullUpdate;
// 			cout << "Cleaning up dirty vertices..." << flush;
			while (!dirtyOptimizeQMs.empty())
			{
				QuadricMove* qm = dirtyOptimizeQMs.back();

				//cout << "Cleaning up dirty vertex " << qm->q.vIdx << "..." << endl;

				// update quadrics
				if (qm->q.dirtyQVal)
					qm->q = computeQuadric(qm->q.vIdx, alpha, numOneRingSearch, numOneRingQuadric);
	
				// update moved quadric
// 				qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);
				unsigned int numTries(0);
				qm->bCanMove = computeOptimalLocation_linearSearch(qm->q.vIdx, qm->q, qm->newPos, angleBound, numTries);
// 				if (!qm->bCanMove)
// 					qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);
				
				double minDist(0.0);
				double closestPt[3] = {0,0,0};
				if (isBoundaryVertex(qm->q.vIdx))
				{
					Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm->q.vIdx, qm->newPos, closestPt, numOneRingSearch, false);
					qm->qValue_afterMove = computeBoundaryQuadricValue(qm->newPos, closestPt, alpha, numOneRingQuadric);
					qm->dirtyQVal_AfterMove = false;
				}
				else
				{
					int closestPolyIdx_afterMove = findClosestPolygon(qm->newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[qm->q.vIdx], numOneRingSearch);
					qm->qValue_afterMove = computeQuadricValue(qm->newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
					qm->dirtyQVal_AfterMove = false;
				}
				
				qm->qValueImprovement = qm->q.quadricValue - qm->qValue_afterMove;
				qm->dirtyImprovement = false;

				dirtyOptimizeQMs.pop_back();
				dirtyOptimizeBackLinks[qm_front->q.vIdx] = NULL;

				// check if the cleaned vertex is better than qm_front
// 				if (my_quadMovePtr_largestImprovementFirst(qm_front, qm))
// 					break;
			}
			make_heap(qMoveSortPtrList.begin(), qMoveSortPtrList.end(), my_quadMovePtr_largestImprovementFirst);
			afterTime = clock();
// 			cout << "done! (" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
		}
		else		// no vertices can move
			break;

		iterationCount++;
		if (iterationCount % 1000 == 0) {
			afterTime = clock();
			cout << iterationCount << " iterations processed...(" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
		}
	}

	// ----- for error plot -----
// 	outMaxQMFile.close();
// 	outAvgQMFile.close();
// 	outAvgQMChangeFile.close();
// 	outCanMoveFile.close();
// 
// 	for (unsigned int i = 0; i < qMovePtrList.size(); ++i)
// 	{
// 		QuadricMove* qm = qMovePtrList[i];
// 		outQEMFile << qm->q.vIdx << " " << qm->q.quadricValue << endl;
// 	}
// 	outQEMFile.close();
	// ----- end error plot -----

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total optimization processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of iterations: " << iterationCount << endl;
	cout << "Total number of full updates: " << numFullUpdate << endl;
	cout << "Last improvement: " << lastImprovementValue << endl;
	cout << "Last improvement/bboxDiagonal: " << lastImprovementValue/bboxDiagonal << endl;

	avgSolvingTime /= numSolve;
	avgSetupTime /= numSetup;
	cout << "Max. setup time: " << maxSetupTime << endl;
	cout << "Avg. setup time: " << avgSetupTime << endl;
	cout << "Max. solving time: " << maxSolvingTime << endl;
	cout << "Avg. solving time: " << avgSolvingTime << endl;

	cout << "total linear search found = " << numLinSearchFound << endl;
	cout << "total linear search in feasible region = " << numLinSearchInFeasibleRegion << " (" << 100.0*numLinSearchInFeasibleRegion/numLinSearchFound << ")" << endl;

	return totalTime;
}

clock_t NonobtOptn::optimize_afterMove_noPriority(unsigned int numMaxIteration, 
																double quadricThreshold, 
																double improvementThreshold,
																double alpha, 
																unsigned int numOneRingSearch, 
																unsigned int numOneRingQuadric,
																double angleBound,
																bool bRecomputeQuadrics,
																bool bUpdateClosestPolygon,
																bool bCleanDirtyWhenStuck,
																NORegionType region)
{
//TODO: monitor the following dents
// vertices: 8076, 8342, 8329, 8225, 7418, 6843, 7294

	avgSolvingTime = 0;
	maxSolvingTime = 0;
	avgSetupTime = 0;
	maxSetupTime = 0;
	numSolve = 0;
	numSetup = 0;
	numClosestFaceChanged = 0;

	numLinSearchFound = 0;
	numLinSearchInFeasibleRegion = 0;

	unsigned int iterationCount = 0;
	unsigned int numMoved = 0;
	unsigned int numUnmoved = 0;
	bool continueOptimize(true);
	bool isRandom(false);
	double avgNumTries(0);

	clock_t b4Time = clock();
	clock_t afterTime = clock();

	vector<int> processOrders(numNoVertices);
	for (int i = 0; i < numNoVertices; ++i)
		processOrders[i] = i;
	// randomize process orders
	if (isRandom) {
		for (int i = 0; i < numNoVertices-1; ++i) {
			int r = i + (rand() % (numNoVertices-i)); // Random remaining position.
			swap(processOrders[i], processOrders[r]);
		}
	}
	
	// DEBUG ONLY
// 	unsigned int monitorV[7] = {8076, 8342, 8329, 8225, 7418, 6843, 7294};
// 	unsigned int numMonitorMoved[7] = {0,0,0,0,0,0,0};

	for (unsigned int i = 0; continueOptimize; i=(i+1)%numNoVertices)
	{
// 		cout << "optimizing ... at iteration " << iterationCount << endl;

// 		if (isRandom)
// 			i = rand() % numNoVertices;

		QuadricMove qm;
//  		qm.q = computeQuadric(i, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
		qm.q = computeQuadric(processOrders[i], alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
		for (unsigned int j = 0; j < 3; ++j)
			qm.newPos[j] = noVerticesList[3*i+j];
//  		qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound);
		unsigned int numTries(0);
 		qm.bCanMove = computeOptimalLocation_linearSearch(qm.q.vIdx, qm.q, qm.newPos, angleBound, numTries);
//   		qm.bCanMove = computeOptimalLocation_radiiRandomProbing(qm.q.vIdx, qm.q, qm.newPos, angleBound, numTries);
		avgNumTries += numTries;
//   		if (!qm.bCanMove)// || qm.q.vIdx == 8076 || qm.q.vIdx == 8342 || qm.q.vIdx == 8329 || qm.q.vIdx == 8225 || qm.q.vIdx == 7418 || qm.q.vIdx == 6843 || qm.q.vIdx == 7294)
//  			qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound);

		double minDist(0.0);
		double closestPt[3] = {0,0,0};
		if (isBoundaryVertex(qm.q.vIdx))
		{
			Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm.q.vIdx, qm.newPos, closestPt, numOneRingSearch, false);
			qm.qValue_afterMove = computeBoundaryQuadricValue(qm.newPos, closestPt, alpha, numOneRingQuadric);
		}
		else
		{
			int closestPolyIdx_afterMove = findClosestPolygon(qm.newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], numOneRingSearch);
			qm.qValue_afterMove = computeQuadricValue(qm.newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
		}
		qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;	

		
		// DEBUG ONLY
// 		for (unsigned int j = 0; j < 7; ++j)
// 			if (qm.q.vIdx == (int)monitorV[j] && qm.bCanMove)
// 				numMonitorMoved[j] += 1;

		if (qm.bCanMove && qm.qValueImprovement > 0)
// 		if (qm.bCanMove)
		{
			updateMesh(qm.q.vIdx, qm.newPos);
			lastMovedVertexIdx = qm.q.vIdx;
			//cout << "Moved vertex " << lastMovedVertexIdx << endl;
			++numMoved;
			numUnmoved = 0;
			if (numMoved % 1000 == 0) {
				afterTime = clock();
				cout << numMoved << " vertices moved...(" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
			}

// 			if (numMaxIteration > 0 && numMoved >= numMaxIteration)
// 				continueOptimize = false;
		}
		else
		{
			++numUnmoved;
			if ((int)numUnmoved >= numNoVertices)
				continueOptimize = false;
		}

		++iterationCount;

		if (numMaxIteration > 0 && iterationCount >= numMaxIteration)
			continueOptimize = false;
	}


	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total optimization processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of iterations: " << iterationCount << endl;
	cout << "Total number of moved vertices: " << numMoved << endl;
	avgNumTries /= iterationCount;
	cout << "Average number of tries in linear search scheme: " << avgNumTries << endl;
	
	avgSolvingTime /= numSolve;
	avgSetupTime /= numSetup;
	cout << "Max. setup time: " << maxSetupTime << endl;
	cout << "Avg. setup time: " << avgSetupTime << endl;
	cout << "Max. solving time: " << maxSolvingTime << endl;
	cout << "Avg. solving time: " << avgSolvingTime << endl;

	cout << "numClosestFaceChanged = " << numClosestFaceChanged << endl;

	cout << "total linear search found = " << numLinSearchFound << endl;
	cout << "total linear search in feasible region = " << numLinSearchInFeasibleRegion << " (" << 100.0*numLinSearchInFeasibleRegion/numLinSearchFound << ")" << endl;

// 	for (unsigned int j = 0; j < 7; ++j)
// 		cout << "total number of movements for vertex " << monitorV[j] << ": " << numMonitorMoved[j] << endl;

	return totalTime;
}

clock_t NonobtOptn::optimize_afterMove_MCA(unsigned int numMaxIteration, 
																double quadricThreshold, 
																double improvementThreshold,
																double alpha, 
																unsigned int numOneRingSearch, 
																unsigned int numOneRingQuadric,
																double angleBound,
																bool bRecomputeQuadrics,
																bool bUpdateClosestPolygon,
																bool bCleanDirtyWhenStuck,
																NORegionType region)
{
	avgSolvingTime = 0;
	maxSolvingTime = 0;
	avgSetupTime = 0;
	maxSetupTime = 0;
	numSolve = 0;
	numSetup = 0;
	numClosestFaceChanged = 0;

	numLinSearchFound = 0;
	numLinSearchInFeasibleRegion = 0;

	unsigned int iterationCount = 0;
	unsigned int numMoved = 0;
	unsigned int numUnmoved = 0;
	bool continueOptimize(true);
	double avgNumTries(0);

	const unsigned int NUMCHOICE = 10;

	clock_t b4Time = clock();
	clock_t afterTime = clock();
	
	if (qMovePtrList.empty()) {
		qMovePtrList.resize(numNoVertices);
		bRecomputeQuadrics = true;
	}

	if (bRecomputeQuadrics) {
		for (int i = 0; i < numNoVertices; ++i) {
			QuadricMove *qm = new QuadricMove;
			qm->dirtyImprovement = true;
			qMovePtrList[i] = qm;
		}
	}

	while (continueOptimize)
	{
// 		cout << "optimizing ... at iteration " << iterationCount << endl;
		
		QuadricMove *qm_best = NULL;
		double bestValueImprovement;
		for (int k = 0; k < (int)NUMCHOICE; ++k)
		{
			unsigned int i = rand() % numNoVertices;
				
			QuadricMove *qm = qMovePtrList[i];
			if (qm->dirtyImprovement)
			{
				qm->q = computeQuadric(i, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
				for (unsigned int j = 0; j < 3; ++j)
					qm->newPos[j] = noVerticesList[3*i+j];
				qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);
		// 		unsigned int numTries(0);
		//  		qm->bCanMove = computeOptimalLocation_linearSearch(qm->q.vIdx, qm->q, qm->newPos, angleBound, numTries);
		// 		avgNumTries += numTries;
		//   		if (!qm->bCanMove)
		//  			qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound);
		
				double minDist(0.0);
				double closestPt[3] = {0,0,0};
				if (isBoundaryVertex(qm->q.vIdx))
				{
					Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm->q.vIdx, qm->newPos, closestPt, numOneRingSearch, false);
					qm->qValue_afterMove = computeBoundaryQuadricValue(qm->newPos, closestPt, alpha, numOneRingQuadric);
				}
				else
				{
					int closestPolyIdx_afterMove = findClosestPolygon(qm->newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], numOneRingSearch);
					qm->qValue_afterMove = computeQuadricValue(qm->newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
				}
				qm->qValueImprovement = qm->q.quadricValue - qm->qValue_afterMove;
				qm->dirtyImprovement = false;
			}

			if (!qm->bCanMove || qm->qValueImprovement <= 0)
			{
				--k;
				continue;
			}

			if (qm_best == NULL || bestValueImprovement > qm->qValueImprovement)
			{
				bestValueImprovement = qm->qValueImprovement;
				qm_best = qm;
			}
		}

		if (qm_best->qValueImprovement/bboxDiagonal <= improvementThreshold)
			break;
		else if (qm_best->bCanMove)
		{
			updateMesh(qm_best->q.vIdx, qm_best->newPos);
			unsigned int lastMovedVertexIdx = qm_best->q.vIdx;
			//cout << "Moved vertex " << lastMovedVertexIdx << endl;
			++numMoved;
			numUnmoved = 0;
			if (numMoved % 1000 == 0) {
				afterTime = clock();
				cout << numMoved << " vertices moved...(" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
				cout << "Last Quadric: " << qm_best->q.quadricValue << endl;
				cout << "Last Quadric/bboxDiagonal: " << qm_best->q.quadricValue/bboxDiagonal << endl;
				cout << "Last improvement: " << qm_best->qValueImprovement << endl;
				cout << "Last improvement/bboxDiagonal: " << qm_best->qValueImprovement/bboxDiagonal << endl;
			}

			qm_best->dirtyImprovement = true;
			qm_best->qValue_afterMove = true;
			qm_best->q.dirtyQVal = true;

			// make dirty quadrics for the moved vertex and its one-ring
			unsigned int numOneRingTriangles = noVertexNeighbourList[lastMovedVertexIdx].size();
			for (unsigned int i = 0; i < numOneRingTriangles; ++i)
			{
				unsigned int currentPolyIdx = noVertexNeighbourList[lastMovedVertexIdx][i];
				for (unsigned int j = 0; j < 3; ++j)
				{
					unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
					if (!qMovePtrList[vIdx]->dirtyImprovement)
					{
						qMovePtrList[vIdx]->dirtyQVal_AfterMove = true;
						qMovePtrList[vIdx]->dirtyImprovement = true;
						qMovePtrList[vIdx]->q.dirtyQVal = true;		// update the smoothness term since centroid has moved
					}
				}
			}

// 			if (numMaxIteration > 0 && numMoved >= numMaxIteration)
// 				continueOptimize = false;
		}
		else
		{
			++numUnmoved;
			if ((int)numUnmoved >= numNoVertices)
				continueOptimize = false;
		}

		++iterationCount;

		if (numMaxIteration > 0 && iterationCount >= numMaxIteration)
			continueOptimize = false;
	}


	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total optimization processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of iterations: " << iterationCount << endl;
	cout << "Total number of moved vertices: " << numMoved << endl;
	avgNumTries /= iterationCount;
	cout << "Average number of tries in linear search scheme: " << avgNumTries << endl;
	
	avgSolvingTime /= numSolve;
	avgSetupTime /= numSetup;
	cout << "Max. setup time: " << maxSetupTime << endl;
	cout << "Avg. setup time: " << avgSetupTime << endl;
	cout << "Max. solving time: " << maxSolvingTime << endl;
	cout << "Avg. solving time: " << avgSolvingTime << endl;

	cout << "numClosestFaceChanged = " << numClosestFaceChanged << endl;

	cout << "total linear search found = " << numLinSearchFound << endl;
	cout << "total linear search in feasible region = " << numLinSearchInFeasibleRegion << " (" << 100.0*numLinSearchInFeasibleRegion/numLinSearchFound << ")" << endl;

	return totalTime;
}

/*
// OLD SORTING METHOD
void NonobtOptn::optimize_afterMove(unsigned int numMaxIteration, 
									double quadricThreshold, 
									double improvementThreshold,
									//double r,	// debug ------------------
									double alpha, 
									unsigned int numOneRingSearch, 
									unsigned int numOneRingQuadric,
									double angleBound,
									bool bRecomputeQuadrics,
									bool bUpdateClosestPolygon,
									NORegionType region)
{
	if (bRecomputeQuadrics)
		qMoveList.clear();

	unsigned int iterationCount = 0;
	int lastVertexMovedIdx = -1;
	bool continueOptimize(true);

	// ---- debug ----
	//double lastAvgQuadValue = -1.0;
	//int numAngleBoundViolated(0);
	// ---- end debug ----

	while (continueOptimize)
	{
		bool hasPassThreshold(false);	// true if at least one vertex has quadric value > quadricThreshold

		if (qMoveList.empty())
		{
			qMoveList.clear();
			cout << "Computing Quadrics for all vertices for the first time..." << endl;
			make_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);

			// compute quadrics for each vertex
			for (int i = 0; i < numNoVertices; ++i)
			{
				cout << "processing vertex " << i << "/" << numNoVertices << "..." << endl;

				Quadric q = computeQuadric(i, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
				
				QuadricMove qm;
				qm.q = q;
				qm.bCanMove = computeOptimalLocation(q.vIdx, q, qm.newPos, region, angleBound);

				double minDist(0.0);
				double closestPt[3] = {0,0,0};
				if (isBoundaryVertex(qm.q.vIdx))
				{
					Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm.q.vIdx, qm.newPos, closestPt, numOneRingSearch, false);
					qm.qValue_afterMove = computeBoundaryQuadricValue(qm.newPos, closestPt, alpha, numOneRingQuadric);
					qm.dirtyQVal_AfterMove = false;
				}
				else
				{
					int closestPolyIdx_afterMove = findClosestPolygon(qm.newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], numOneRingSearch);
					qm.qValue_afterMove = computeQuadricValue(qm.newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
					qm.dirtyQVal_AfterMove = false;
				}
				qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;	
				qm.dirtyImprovement = false;

				qMoveList.push_back(qm);
				push_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);
			}
			qMoveIndicesList.resize(qMoveList.size());
		}
		else
		{
			cout << "Updating Quadrics at iteration " << iterationCount << endl;
			if (lastVertexMovedIdx != -1)
			{
				// update quadrics
				qMoveList[qMoveIndicesList[lastVertexMovedIdx]].q = computeQuadric(lastVertexMovedIdx, alpha, numOneRingSearch, numOneRingQuadric);
				
				// update moved quadrics for the moved vertex and its one-ring
				unsigned int numOneRingTriangles = noVertexNeighbourList[lastVertexMovedIdx].size();
				vector<unsigned int> updatedIndices;
				for (unsigned int i = 0; i < numOneRingTriangles; ++i)
				{
					unsigned int currentPolyIdx = noVertexNeighbourList[lastVertexMovedIdx][i];
					for (unsigned int j = 0; j < 3; ++j)
					{
						unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
	
						// check if the vertex has been updated
						if (find(updatedIndices.begin(), updatedIndices.end(), vIdx) != updatedIndices.end())
							continue;

						// update moved quadric
						qMoveList[qMoveIndicesList[vIdx]].bCanMove = computeOptimalLocation(qMoveList[qMoveIndicesList[vIdx]].q.vIdx, 
																				qMoveList[qMoveIndicesList[vIdx]].q,
																				qMoveList[qMoveIndicesList[vIdx]].newPos, region, angleBound);
						
						double minDist(0.0);
						double closestPt[3] = {0,0,0};
						if (isBoundaryVertex(qMoveList[qMoveIndicesList[vIdx]].q.vIdx))
						{
							Edge closestBEdge_afterMove = findClosestBoundaryEdge(qMoveList[qMoveIndicesList[vIdx]].q.vIdx, qMoveList[qMoveIndicesList[vIdx]].newPos, closestPt, numOneRingSearch, false);
							qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove = computeBoundaryQuadricValue(qMoveList[qMoveIndicesList[vIdx]].newPos, closestPt, alpha, numOneRingQuadric);
							qMoveList[qMoveIndicesList[vIdx]].dirtyQVal_AfterMove = false;
						}
						else
						{
							int closestPolyIdx_afterMove = findClosestPolygon(qMoveList[qMoveIndicesList[vIdx]].newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[qMoveList[qMoveIndicesList[vIdx]].q.vIdx], numOneRingSearch);
							qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove = computeQuadricValue(qMoveList[qMoveIndicesList[vIdx]].newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
							qMoveList[qMoveIndicesList[vIdx]].dirtyQVal_AfterMove = false;
						}
						
						qMoveList[qMoveIndicesList[vIdx]].qValueImprovement = qMoveList[qMoveIndicesList[vIdx]].q.quadricValue - qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove;
						qMoveList[qMoveIndicesList[vIdx]].dirtyImprovement = false;

						updatedIndices.push_back(vIdx);
					}
				}
				
				make_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);
			}
		}

		// ---- debug ----
		//double largestQuadValue = -1.0;
		//unsigned int largestQuadIdx = 0;
		//double avgQuadValue = 0.0;
		// ---- end debug ----

		for (unsigned int i = 0; i < qMoveList.size(); ++i)
		{
			// update qMoveIndicesList
			qMoveIndicesList[qMoveList[i].q.vIdx] = i;

			// check if current vertex has quadrics exceeded the threshold
			if (!hasPassThreshold && qMoveList[i].q.quadricValue > quadricThreshold)
				hasPassThreshold = true;

			// ---- debug ----
			//avgQuadValue += qMoveList[i].q.quadricValue;
			//if (largestQuadValue == -1.0 || largestQuadValue < qMoveList[i].q.quadricValue)
			//{	largestQuadValue = qMoveList[i].q.quadricValue;
			//	largestQuadIdx = qMoveList[i].q.vIdx;
			//}
			//cout << "vertex " << qMoveList[i].q.vIdx << ": " << qMoveList[i].q.quadricValue << endl;
			// ---- end debug ----
		}
		// ---- debug ----
		//avgQuadValue /= qMoveList.size();
		//if (initAvgQuadValue == -1)
		//	initAvgQuadValue = avgQuadValue;
		//cout << "vertex " << largestQuadIdx << ": " << qMoveList[qMoveIndicesList[largestQuadIdx]].q.quadricValue << endl;
		//cout << "initial avg. quad. value: " << initAvgQuadValue << endl;
		//cout << "average quadraic value: " << avgQuadValue << endl;
		//cout << "change in quadric value: " << avgQuadValue - lastAvgQuadValue << endl;
		
		//if (lastAvgQuadValue != -1.0 && lastAvgQuadValue - avgQuadValue < 0.0000001)
		//	break;
		//lastAvgQuadValue = avgQuadValue;

		//if (avgQuadValue < r*initAvgQuadValue)
		//	break;
		// ---- end debug ----
		

		if (iterationCount != 0 && iterationCount == numMaxIteration) {	// max iterations reached
			break;
		}
		if (!hasPassThreshold) {	// all vertices have quadrics within the threshold
			break;
		}

		// --- debug
 		//vector<QuadricMove> qMoveList_debug = qMoveList;
 		//sort_heap(qMoveList_debug.begin(), qMoveList_debug.end(), my_quadMove_largestImprovementFirst);
		// --- end debug


		QuadricMove &qm_front = qMoveList.front();

		if (qm_front.qValueImprovement <= improvementThreshold)	{	// can't do anymore improvements
			break;
		}

		if (qm_front.bCanMove)
		{
			// ---- debug only ----
			lastMovedVertexOldPos[0] = noVerticesList[3*(qm_front.q.vIdx)];
			lastMovedVertexOldPos[1] = noVerticesList[3*(qm_front.q.vIdx)+1];
			lastMovedVertexOldPos[2] = noVerticesList[3*(qm_front.q.vIdx)+2];
			lastMovedVertexIdx = qm_front.q.vIdx;
			isLastMovedVertexSet = true;
			// ---- end debug -----


			// ---- debug only ----
// 			PlaneEqn *constraints = NULL;
// 			int numConstraints = getNonobtuseRegion(qm_front.q.vIdx, constraints, region, false, angleBound);
// 			for (int k = 0; k < numConstraints; k++)
// 			{
// 				double value = constraints[k].a * qm_front.newPos[0] + constraints[k].b * qm_front.newPos[1]
// 						+ constraints[k].c * qm_front.newPos[2]	+ constraints[k].d;
// 				if (value < -0.0001)
// 					int foo = 1;
// 			}
			// ---- end debug -----

			updateMesh(qm_front.q.vIdx, qm_front.newPos);
			lastVertexMovedIdx = qm_front.q.vIdx;
			cout << "Moved vertex " << lastVertexMovedIdx << endl;

			
			// ---- debug only ----
// 			for (unsigned int i = 0; i < noVertexNeighbourList[lastMovedVertexIdx].size(); i++)
// 			{
// 				unsigned int polyIdx = noVertexNeighbourList[lastMovedVertexIdx][i];
// 				for (unsigned j = 0; j < 3; j++)
// 				{
// 					int vIdx = noPolygonsList[polyIdx][j];
// 					int nextIdx = noPolygonsList[polyIdx][(j+1)%3];
// 					int prevIdx = noPolygonsList[polyIdx][(j+2)%3];
// 					double edge1[3] = {noVerticesList[3*nextIdx] - noVerticesList[3*vIdx],
// 										noVerticesList[3*nextIdx+1] - noVerticesList[3*vIdx+1],
// 										noVerticesList[3*nextIdx+2]- noVerticesList[3*vIdx+2]};
// 					double edge2[3] = {noVerticesList[3*prevIdx] - noVerticesList[3*vIdx],
// 										noVerticesList[3*prevIdx+1] - noVerticesList[3*vIdx+1],
// 										noVerticesList[3*prevIdx+2] - noVerticesList[3*vIdx+2]};
// 					double angle = getAngle(edge1, edge2) * (180.0/M_PI);
// 					double angle2 = getAngle(noVerticesList[3*vIdx], noVerticesList[3*vIdx+1], noVerticesList[3*vIdx+2], noVerticesList[3*prevIdx], noVerticesList[3*prevIdx+1], noVerticesList[3*prevIdx+2], noVerticesList[3*nextIdx], noVerticesList[3*nextIdx+1], noVerticesList[3*nextIdx+2]) * (180.0/M_PI);
// 					if (angle < 30.0)
// 					{
// 						cout << "angle-bound violated (" << angle << ") at iteration " << iterationCount << " when moving vertex " << lastMovedVertexIdx << endl;
// 						int foo = 1;
// 						if (vIdx != lastMovedVertexIdx)
// 							int foo = 1;
// 						if (prevIdx == lastMovedVertexIdx)
// 							int foo = 1;
// 
// 						edge1[0] *= -1.0;
// 						edge1[1] *= -1.0;
// 						edge1[2] *= -1.0;
// 						edge2[0] *= -1.0;
// 						edge2[1] *= -1.0;
// 						edge2[2] *= -1.0;
// 						double edgeVector[3] = {noVerticesList[3*prevIdx] - noVerticesList[3*nextIdx],
// 												noVerticesList[3*prevIdx+1] - noVerticesList[3*nextIdx+1],
// 												noVerticesList[3*prevIdx+2]- noVerticesList[3*nextIdx+2]};
// 						double revEdgeVector[3] = {-1.0 * edgeVector[0], -1.0 * edgeVector[1], -1.0 * edgeVector[2]};
// 
// 						double a1 = getAngle(edge1, edgeVector) * (180.0/M_PI);
// 						double a2 = getAngle(edge2, revEdgeVector) * (180.0/M_PI);
// 						int bar = 1;
// 						++numAngleBoundViolated;
// 					}
// 					if (angle > 90.0009)
// 					{
// 						cout << "nonobtusity violated (" << angle << ") at iteration " << iterationCount << " when moving vertex " << lastMovedVertexIdx << endl;
// 						int foo = 1;
// 					}
// 				}
// 			}
			// ---- end debug -----
			
		}
		else		// no vertices can move
			break;

		iterationCount++;
	}
}
*/

// non-obtuse smooth mesh
// numMaxIteration (in): max number of iteration to run
// smoothnessThreshold (in): minimum smoothness value the non-obtuse mesh can have before stopping the optimization
// angleBound (in): angle bound (in degrees)
// bRecomputeSmoothness (in): true - recompute all smoothness value
// region (in): determines how the non-obtuse region is to be approximated
// return value (out): time taken
clock_t NonobtOptn::optimize_smoothing(unsigned int numMaxIteration, 
									double smoothnessThreshold, 
									double angleBound,
									bool bRecomputeSmoothness,
									NORegionType region)
{/*
	if (bRecomputeSmoothness)
		qSmoothList.clear();

	unsigned int iterationCount = 0;
	int lastVertexMovedIdx = -1;
	bool continueOptimize(true);
	while (continueOptimize)
	{
		bool hasPassThreshold(false);	// true if at least one vertex has smoothness value > smoothnessThreshold

		if (qSmoothList.empty())
		{
			qSmoothList.clear();
			cout << "Computing Smoothness for all vertices for the first time..." << endl;
			make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
			//make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestQuadFirst);

			// compute smoothness for each vertex
			for (int i = 0; i < numNoVertices; ++i)
			{
				double centroid[3] = {0,0,0};
				Quadric q = computeSmoothness(i, centroid, false);
				
				QuadricMove qm;
				qm.q = q;
				qm.bCanMove = computeOptimalLocation(q.vIdx, q, qm.newPos, region);
				
				qm.qValue_afterMove = length(qm.newPos[0] - centroid[0], 
											qm.newPos[1] - centroid[1],
											qm.newPos[2] - centroid[2]);
				qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;	

				qSmoothList.push_back(qm);
				push_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
				//push_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestQuadFirst);

				// check if current vertex has smoothness exceeded the threshold
				if (!hasPassThreshold && q.quadricValue > smoothnessThreshold)
					hasPassThreshold = true;
			}

			qSmoothIndicesList.resize(qSmoothList.size());
		}
		else
		{
			cout << "Updating Smoothness at iteration " << iterationCount << endl;
			if (lastVertexMovedIdx != -1)
			{
				// for taubin smoothing
				updateCentroidList(lastVertexMovedIdx);

				// update smoothness and after-move-smoothness for the moved vertex and its one-ring
				unsigned int numOneRingTriangles = noVertexNeighbourList[lastVertexMovedIdx].size();
				vector<unsigned int> updatedIndices;
				for (unsigned int i = 0; i < numOneRingTriangles; ++i)
				{
					unsigned int currentPolyIdx = noVertexNeighbourList[lastVertexMovedIdx][i];
					for (unsigned int j = 0; j < 3; ++j)
					{
						unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
						
						// check if the vertex has been updated
						if (find(updatedIndices.begin(), updatedIndices.end(), vIdx) != updatedIndices.end())
							continue;

						// update smoothness
						double centroid[3] = {0,0,0};

						qSmoothList[qSmoothIndicesList[vIdx]].q = computeSmoothness(qSmoothList[qSmoothIndicesList[vIdx]].q.vIdx, centroid, false);
						
						// update after move smoothness
						qSmoothList[qSmoothIndicesList[vIdx]].bCanMove = computeOptimalLocation(qSmoothList[qSmoothIndicesList[vIdx]].q.vIdx, 
																				qSmoothList[qSmoothIndicesList[vIdx]].q,
																				qSmoothList[qSmoothIndicesList[vIdx]].newPos, region);
						qSmoothList[qSmoothIndicesList[vIdx]].qValue_afterMove = 
									length(qSmoothList[qSmoothIndicesList[vIdx]].newPos[0] - centroid[0], 
											qSmoothList[qSmoothIndicesList[vIdx]].newPos[1] - centroid[1],
											qSmoothList[qSmoothIndicesList[vIdx]].newPos[2] - centroid[2]);

						qSmoothList[qSmoothIndicesList[vIdx]].qValueImprovement = qSmoothList[qSmoothIndicesList[vIdx]].q.quadricValue - qSmoothList[qSmoothIndicesList[vIdx]].qValue_afterMove;

						updatedIndices.push_back(vIdx);
					}
				}
				
				make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
				//make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestQuadFirst);
			}

			// check threshold
			for (int i = 0; i < numNoVertices; ++i)
			{
				// check if current vertex has smoothness exceeded the threshold
				if (!hasPassThreshold && qSmoothList[i].q.quadricValue > smoothnessThreshold)
				{
					hasPassThreshold = true;
					break;
				}
			}
		}

		// update qSmoothIndicesList
		for (unsigned int i = 0; i < qSmoothList.size(); ++i)
			qSmoothIndicesList[qSmoothList[i].q.vIdx] = i;

		if (iterationCount != 0 && iterationCount == numMaxIteration)	// max iterations reached
			break;
		if (!hasPassThreshold)	// all vertices have quadrics within the threshold
			break;
		
		// --- debug
		//vector<QuadricMove> qSmoothList_debug = qSmoothList;
		//sort_heap(qSmoothList_debug.begin(), qSmoothList_debug.end(), my_quadMove_largestImprovementFirst);
		//sort_heap(qSmoothList_debug.begin(), qSmoothList_debug.end(), my_quadMove_largestQuadFirst);
		// --- end debug


		QuadricMove qm = qSmoothList.front();

		if (qm.qValueImprovement <= 0)		// can't do anymore improvements
			break;

		if (qm.bCanMove)
		{
			// ---- debug only ----
			lastMovedVertexOldPos[0] = noVerticesList[3*(qm.q.vIdx)];
			lastMovedVertexOldPos[1] = noVerticesList[3*(qm.q.vIdx)+1];
			lastMovedVertexOldPos[2] = noVerticesList[3*(qm.q.vIdx)+2];
			lastMovedVertexIdx = qm.q.vIdx;
			isLastMovedVertexSet = true;
			// ---- end debug -----

			updateMesh(qm.q.vIdx, qm.newPos);
			lastVertexMovedIdx = qm.q.vIdx;
			cout << "Smoothed vertex " << lastVertexMovedIdx << endl;
		}
		else		// no vertices can move
			break;
		
		iterationCount++;
	}*/

	numLinSearchFound = 0;
	numLinSearchInFeasibleRegion = 0;

	bool isRandom(false);
	bool continueOptimize(true);

	clock_t b4Time = clock();
	clock_t afterTime = clock();

	cout << "Smoothing vertices (" << numNoVertices << ")..." << flush;
	unsigned int numSmoothed = 0;
	unsigned int count = 0;
	double avgNumTries(0);
	for (int i = 0; continueOptimize; i=(i+1)%numNoVertices)
	{
		++count;
		if (isRandom)
			i = rand() % numNoVertices;

		double centroid[3] = {0,0,0};
		Quadric q = computeSmoothness(i, centroid, false);
		
		QuadricMove qm;
		qm.q = q;
		for (unsigned int j = 0; j < 3; ++j)
			qm.newPos[j] = noVerticesList[3*i+j];
// 		qm.bCanMove = computeOptimalLocation(q.vIdx, q, qm.newPos, region, angleBound);
		unsigned int numTries(0);
 		qm.bCanMove = computeOptimalLocation_linearSearch(q.vIdx, q, qm.newPos, angleBound, numTries);
//   		qm.bCanMove = computeOptimalLocation_radiiRandomProbing(q.vIdx, q, qm.newPos, angleBound, numTries);
		avgNumTries += numTries;
		
// 		qm.qValue_afterMove = length(qm.newPos[0] - centroid[0], 
// 									qm.newPos[1] - centroid[1],
// 									qm.newPos[2] - centroid[2]);
// 		qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;	
// 
// 		if (qm.bCanMove && qm.qValueImprovement > 0)
		if (qm.bCanMove)
		{
			updateMesh(qm.q.vIdx, qm.newPos);
			lastMovedVertexIdx = qm.q.vIdx;
			//cout << "Smoothed vertex " << lastMovedVertexIdx << endl;

			numSmoothed++;
		}

// 		if (numSmoothed >= numMaxIteration)
// 			continueOptimize = false;
		if ((count >= numMaxIteration && numMaxIteration > 0) || ((int)count >= numNoVertices && numMaxIteration <= 0))
			continueOptimize = false;
	}

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total smoothing processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of vertices smoothed: " << numSmoothed << endl;
	avgNumTries /= count;
	cout << "Average number of tries in linear search scheme: " << avgNumTries << endl;
	cout << "done!" << endl;

	cout << "total linear search found = " << numLinSearchFound << endl;
	cout << "total linear search in feasible region = " << numLinSearchInFeasibleRegion << " (" << 100.0*numLinSearchInFeasibleRegion/numLinSearchFound << ")" << endl;

	return totalTime;
}

// optimize mesh via alternating optimize_afterMove() and optimize_smoothing()
// numMaxIteration (in): max number of iteration to run
// numMoveIteration (in): max number of iteration to move before switching to smooth
// alpha (in): weighting alpha value for objective function
// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the optimization
// improvementThreshold (in): minimum improvement value before stopping the optimization
// numOneRingSearch (in): number of one-ring to search for closest face
// numOneRingQuadric (in): number of one-ring to use for computing quadric
// numSmoothIteration (in): max number of iteration to smooth before switching to move
// angleBound (in): angle bound (in degrees)
// bDoOptnFirst (in): true - do optimization before smoothing
// bRecompute (in): true - recompute quadrics and smoothness
// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated when bRecomputeQuadrics is true
// region (in): determines how the non-obtuse region is to be approximated
// *********** OBSOLETE ***********
// void NonobtOptn::optimize_alternate(unsigned int numMaxIteration,
// 									unsigned int numMoveIteration,
// 									double quadricThreshold, 
// 									double improvementThreshold,
// 									double alpha,
// 									unsigned int numOneRingSearch, 
// 									unsigned int numOneRingQuadric,
// 									unsigned int numSmoothIteration,
// 									double smoothnessThreshold,
// 									double angleBound,
// 									bool bDoOptnFirst,
// 									bool bRecompute,
// 									bool bUpdateClosestPolygon,
// 									NORegionType region)
// {
// 	unsigned int iterationCount = 0;
// 	unsigned int alterIterCount = 0;
// 	bool doMove(bDoOptnFirst);
// 	int lastVertexMovedIdx = -1;
// 	bool continueOptimize(true);
// 
// 	if (bRecompute)
// 	{	
// 		qMoveList.clear();
// 	}
// 
// 	while (continueOptimize)
// 	{
// 		// true if at least one vertex has quadric value > quadricThreshold or smoothness > smoothnessThreshold
// 		bool hasPassThreshold(false);	
// 		if (qMoveList.empty() || qSmoothList.empty())
// 		{
// 			qMoveList.clear();
// 			qSmoothList.clear();
// 			cout << "Computing Quadrics and Smoothness for all vertices for the first time..." << endl;
// 			make_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);
// 			make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
// 
// 			for (int i = 0; i < numNoVertices; ++i)
// 			{
// 				cout << "processing vertex " << i << "/" << numNoVertices << "..." << endl;
// 
// 				// compute quadrics for each vertex
// 				Quadric q = computeQuadric(i, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);
// 				
// 				QuadricMove qm;
// 				qm.q = q;
// 				qm.bCanMove = computeOptimalLocation(q.vIdx, q, qm.newPos, region, angleBound);
// 
// 				double minDist(0.0);
// 				double closestPt[3] = {0,0,0};
// 
// 				if (isBoundaryVertex(qm.q.vIdx))
// 				{
// 					Edge closestBEdge_afterMove = findClosestBoundaryEdge(qm.q.vIdx, qm.newPos, closestPt, numOneRingSearch, false);
// 					//qm.qValue_afterMove = computeBoundaryQuadricValue(qm.newPos, closestBEdge_afterMove, alpha, numOneRingQuadric);
// 					qm.qValue_afterMove = computeBoundaryQuadricValue(qm.newPos, closestPt, alpha, numOneRingQuadric);
// 				}
// 				else
// 				{
// 					int closestPolyIdx_afterMove = findClosestPolygon(qm.newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], numOneRingSearch);
// 					qm.qValue_afterMove = computeQuadricValue(qm.newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
// 				}
// 				qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;	
// 
// 				qMoveList.push_back(qm);
// 				push_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);
// 
// 				// compute smoothness for each vertex
// 				double centroid[3] = {0,0,0};
// 				Quadric qs = computeSmoothness(i, centroid, false);
// 				
// 				QuadricMove qms;
// 				qms.q = qs;
// 				qms.bCanMove = computeOptimalLocation(qs.vIdx, qs, qms.newPos, region, angleBound);
// 				
// 				qms.qValue_afterMove = length(qms.newPos[0] - centroid[0], 
// 											qms.newPos[1] - centroid[1],
// 											qms.newPos[2] - centroid[2]);
// 
// 				qms.qValueImprovement = qms.q.quadricValue - qms.qValue_afterMove;	
// 
// 				qSmoothList.push_back(qms);
// 				push_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
// 
// 				// check if current vertex has smoothness exceeded the threshold
// 				if (!hasPassThreshold && (qs.quadricValue > smoothnessThreshold || q.quadricValue > quadricThreshold))
// 					hasPassThreshold = true;				
// 			}
// 			
// 			qMoveIndicesList.resize(qMoveList.size());
// 			qSmoothIndicesList.resize(qSmoothList.size());
// 		}
// 		else
// 		{
// 			cout << "Updating Quadrics and Smoothness at iteration " << iterationCount << endl;
// 			if (lastVertexMovedIdx != -1)
// 			{
// 				// for taubin smoothing
// 				updateCentroidList(lastVertexMovedIdx);
// 
// 				// update quadrics for the moved vertex
// 				qMoveList[qMoveIndicesList[lastVertexMovedIdx]].q = computeQuadric(lastVertexMovedIdx, alpha, numOneRingSearch, numOneRingQuadric);
// 				
// 				// update moved quadrics for the moved vertex and its one-ring
// 				// update smoothness and after-move-smoothness for the moved vertex and its one-ring
// 				unsigned int numOneRingTriangles = noVertexNeighbourList[lastVertexMovedIdx].size();
// 				vector<unsigned int> updatedIndices;
// 				for (unsigned int i = 0; i < numOneRingTriangles; ++i)
// 				{
// 					unsigned int currentPolyIdx = noVertexNeighbourList[lastVertexMovedIdx][i];
// 					for (unsigned int j = 0; j < 3; ++j)
// 					{
// 						unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
// 	
// 						// check if the vertex has been updated
// 						bool isUpdated(false);
// 						for (unsigned int k = 0; k < updatedIndices.size(); ++k)
// 						{
// 							if (updatedIndices[k] == vIdx)
// 							{	
// 								isUpdated = true;
// 								break;
// 							}
// 						}
// 						if (isUpdated)
// 							continue;
// 	
// 						// update moved quadric
// 						qMoveList[qMoveIndicesList[vIdx]].bCanMove = computeOptimalLocation(qMoveList[qMoveIndicesList[vIdx]].q.vIdx, 
// 																							qMoveList[qMoveIndicesList[vIdx]].q,
// 																							qMoveList[qMoveIndicesList[vIdx]].newPos, region, angleBound);
// 						
// 						double minDist(0.0);
// 						double closestPt[3] = {0,0,0};
// 						if (isBoundaryVertex(qMoveList[qMoveIndicesList[vIdx]].q.vIdx))
// 						{
// 							Edge closestBEdge_afterMove = findClosestBoundaryEdge(qMoveList[qMoveIndicesList[vIdx]].q.vIdx, qMoveList[qMoveIndicesList[vIdx]].newPos, closestPt, numOneRingSearch, false);
// 							qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove = computeBoundaryQuadricValue(qMoveList[qMoveIndicesList[vIdx]].newPos, closestPt, alpha, numOneRingQuadric);
// 						}
// 						else
// 						{
// 							int closestPolyIdx_afterMove = findClosestPolygon(qMoveList[qMoveIndicesList[vIdx]].newPos, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[qMoveList[qMoveIndicesList[vIdx]].q.vIdx], numOneRingSearch);
// 							qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove = computeQuadricValue(qMoveList[qMoveIndicesList[vIdx]].newPos, closestPolyIdx_afterMove, alpha, numOneRingQuadric);
// 						}
// 
// 						qMoveList[qMoveIndicesList[vIdx]].qValueImprovement = qMoveList[qMoveIndicesList[vIdx]].q.quadricValue - qMoveList[qMoveIndicesList[vIdx]].qValue_afterMove;
// 
// 						// update smoothness
// 						double centroid[3] = {0,0,0};
// 						qSmoothList[qSmoothIndicesList[vIdx]].q = computeSmoothness(qSmoothList[qSmoothIndicesList[vIdx]].q.vIdx, centroid, false);
// 						
// 						// update after move smoothness
// 						qSmoothList[qSmoothIndicesList[vIdx]].bCanMove = computeOptimalLocation(qSmoothList[qSmoothIndicesList[vIdx]].q.vIdx, 
// 												qSmoothList[qSmoothIndicesList[vIdx]].q,
// 												qSmoothList[qSmoothIndicesList[vIdx]].newPos, region, angleBound);
// 
// 						qSmoothList[qSmoothIndicesList[vIdx]].qValue_afterMove = 
// 									length(qSmoothList[qSmoothIndicesList[vIdx]].newPos[0] - centroid[0], 
// 											qSmoothList[qSmoothIndicesList[vIdx]].newPos[1] - centroid[1],
// 											qSmoothList[qSmoothIndicesList[vIdx]].newPos[2] - centroid[2]);
// 						
// 						qSmoothList[qSmoothIndicesList[vIdx]].qValueImprovement = qSmoothList[qSmoothIndicesList[vIdx]].q.quadricValue - qSmoothList[qSmoothIndicesList[vIdx]].qValue_afterMove;
// 						
// 						updatedIndices.push_back(vIdx);
// 					}
// 				}
// 				
// 				make_heap(qMoveList.begin(), qMoveList.end(), my_quadMove_largestImprovementFirst);				
// 				make_heap(qSmoothList.begin(), qSmoothList.end(), my_quadMove_largestImprovementFirst);
// 			}
// 
// 			// check threshold
// 			for (int i = 0; i < numNoVertices; ++i)
// 			{
// 				// check if current vertex has quadrics exceeded the threshold
// 				if (!hasPassThreshold && (qMoveList[i].q.quadricValue > quadricThreshold || qSmoothList[i].q.quadricValue > smoothnessThreshold))
// 				{
// 					hasPassThreshold = true;
// 					break;
// 				}
// 			}
// 		}
// 
// 		// update qMoveIndicesList
// 		for (unsigned int i = 0; i < qMoveList.size(); ++i)
// 			qMoveIndicesList[qMoveList[i].q.vIdx] = i;
// 		// update qSmoothIndicesList
// 		for (unsigned int i = 0; i < qSmoothList.size(); ++i)
// 			qSmoothIndicesList[qSmoothList[i].q.vIdx] = i;
// 
// 		if (iterationCount != 0 && iterationCount == numMaxIteration)	// max iterations reached
// 			break;
// 		if (!hasPassThreshold)	// all vertices have quadrics within the threshold
// 			break;
// 		
// 		// --- debug
// 		//vector<QuadricMove> qMoveList_debug = qMoveList;
// 		//sort_heap(qMoveList_debug.begin(), qMoveList_debug.end(), my_quadMove_largestImprovementFirst);
// 		//vector<QuadricMove> qSmoothList_debug = qSmoothList;
// 		//sort_heap(qSmoothList_debug.begin(), qSmoothList_debug.end(), my_quadMove_largestImprovementFirst);
// 		// --- end debug
// 
// 
// 		QuadricMove qm;
// 		if (doMove)
// 			qm = qMoveList.front();
// 		else
// 			qm = qSmoothList.front();
// 
// 		if (qm.qValueImprovement > improvementThreshold && qm.bCanMove)
// 		{
// 			// ---- debug only ----
// 			lastMovedVertexOldPos[0] = noVerticesList[3*(qm.q.vIdx)];
// 			lastMovedVertexOldPos[1] = noVerticesList[3*(qm.q.vIdx)+1];
// 			lastMovedVertexOldPos[2] = noVerticesList[3*(qm.q.vIdx)+2];
// 			lastMovedVertexIdx = qm.q.vIdx;
// 			isLastMovedVertexSet = true;
// 			// ---- end debug -----
// 
// 			updateMesh(qm.q.vIdx, qm.newPos);
// 			lastVertexMovedIdx = qm.q.vIdx;
// 			if (doMove)
// 				cout << "Moved vertex " << lastVertexMovedIdx << endl;
// 			else
// 				cout << "Smoothed vertex " << lastVertexMovedIdx << endl;
// 
// 		}
// 		else		// no vertices can move or make improvment
// 		{
// 			if (doMove)
// 			{
// 				// can't move, so check if we can smooth
// 				qm = qSmoothList.front();
// 				if (numSmoothIteration > 0 && qm.qValueImprovement > 0 && qm.bCanMove)
// 				{
// 					doMove = false;
// 					alterIterCount = 0;
// 				}
// 				else
// 					break;
// 			}
// 			else
// 			{
// 				// can't smooth, so check if we can move
// 				qm = qMoveList.front();
// 				if (numMoveIteration > 0 && qm.qValueImprovement > 0 && qm.bCanMove)
// 				{
// 					doMove = true;
// 					alterIterCount = 0;
// 				}
// 				else
// 					break;
// 			}
// 		}
// 		
// 		iterationCount++;
//  		alterIterCount++;
// 
// 		if (doMove)
// 		{
// 			if (alterIterCount == numMoveIteration && numSmoothIteration > 0)
// 			{
// 				doMove = false;
// 				alterIterCount = 0;
// 			}
// 		}
// 		else
// 		{	
// 			if (alterIterCount == numSmoothIteration && numMoveIteration > 0)
// 			{
// 				doMove = true;
// 				alterIterCount = 0;
// 			}
// 		}
// 	}
// }

// non-obtuse decimate the non-obtuse mesh
// numRemaining (in): decimate until the mesh has numRemaining vertices
// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the decimation
// angleBound (in): angle bound (in degrees)
// degreeN (in): max. of min. degrees that we want to avoid
// alpha (in): weighting alpha value for objective function
// region (in): determines how the non-obtuse region is to be approximated
// return value (out): total time taken
// NEW SORTING METHOD
clock_t NonobtOptn::decimate(unsigned int numRemaining, double quadricThreshold, double angleBound, unsigned int degreeN, double alpha, NORegionType region)
{
	bool cleanAtEachIteration = true;
	bool cleanDirtyVertices = false;
	double lastChangeInError(0.0);
	double lastError(0.0);

	vector<unsigned int> newUnusedEdges;
	unsigned int iterationCount = 0;
	int lastNewVertexIdx = -1;	
	bool continueOptimize(true);

	clock_t b4Time = clock();
	clock_t afterTime = clock();

	while (continueOptimize)
	{
		if (qDecimatePtrList.empty())
		{
			qDecimatePtrList.clear();
			qDecimateSortPtrList.clear();
			dirtyDecimateQMs.clear();
			dirtyDecimateBackLinks.clear();
			make_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);

			// initialize vertex quadrics
			cout << "Computing Quadrics (decimate) for all vertices for the first time..." << flush;
			initializeVertexDecimateQuadric();
			cout << "done!" << endl;

			unsigned int lastPct = 0;
			// initialize edge quadrics
			unsigned int numEdges = noEdgeList.size();
			cout << "Computing Quadrics (decimate) for all edges (" << numEdges << ") for the first time" << flush;
			for (unsigned int i = 0; i < numEdges; ++i)
			{
				//cout << "Processing edge " << i << "/" << numEdges << "..." << endl;
				unsigned int currentPct = (unsigned int)(((float)i/numEdges) * 100);
				if (currentPct > lastPct && currentPct % 5 == 0)
				{
					cout << "." << flush;
				}
				lastPct = currentPct;

				QuadricMove* qm = new QuadricMove;
				qm->q.dirtyQVal = true;		// used as marking dirty quadric; quadric value isn't used for decimation
				qm->q.vIdx = i;
				qm->bCanMove = canDecimate(qm->q.vIdx, degreeN);
				if (qm->bCanMove)
				{
					qm->q = computeEdgeQuadric(qm->q.vIdx, alpha);
					qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound, true);	// q.vIdx is an edge index
					qm->qValue_afterMove = computeQuadricValue(qm->q, qm->newPos);
				}
				qm->dirtyQVal_AfterMove = false;

				qDecimatePtrList.push_back(qm);
				qDecimateSortPtrList.push_back(qm);
				push_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);

				dirtyDecimateBackLinks.push_back(NULL);
			}
			cout << "done!" << endl;
		}

		if (getNumNoVertices() <= (int)numRemaining)
			break;
		
		//--- debug
//		vector<QuadricMove*> qDecimateList_debug = qDecimatePtrList;
//		make_heap(qDecimateList_debug.begin(), qDecimateList_debug.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);
//		sort_heap(qDecimateList_debug.begin(), qDecimateList_debug.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);
		//--- end debug

		QuadricMove* qm_front = qDecimateSortPtrList.front();
		while (!cleanDirtyVertices && qm_front->dirtyQVal_AfterMove)
		{
			// top of heap is dirty, so pop it, clean it, then repush it back to heap
			pop_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);

			// clean 
			qm_front->bCanMove = canDecimate(qm_front->q.vIdx, degreeN);
			if (qm_front->bCanMove)
			{
				if (qm_front->q.dirtyQVal)
					qm_front->q = computeEdgeQuadric(qm_front->q.vIdx, alpha);
				qm_front->bCanMove = computeOptimalLocation(qm_front->q.vIdx, qm_front->q, qm_front->newPos, region, angleBound, true);	// qm.q.vIdx is an edge index
				qm_front->qValue_afterMove = computeQuadricValue(qm_front->q, qm_front->newPos);
			}
			qm_front->dirtyQVal_AfterMove = false;

			unsigned int swapIdx = dirtyDecimateQMs.back()->q.vIdx;
			swap(*(dirtyDecimateBackLinks[qm_front->q.vIdx]), dirtyDecimateQMs.back());
			swap(dirtyDecimateBackLinks[qm_front->q.vIdx], dirtyDecimateBackLinks[swapIdx]);

			dirtyDecimateQMs.pop_back();
			dirtyDecimateBackLinks[qm_front->q.vIdx] = NULL;

			push_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);
			qm_front = qDecimateSortPtrList.front();
		}

		double eps = 5.0;
		if (cleanAtEachIteration || (qm_front->qValue_afterMove - lastError) / lastChangeInError > eps)
			cleanDirtyVertices = true;

		// error is too big, stop decimate
		cout << "qVal: " << qm_front->qValue_afterMove << endl;
		cout << "qVal (normalized): " << qm_front->qValue_afterMove / bboxDiagonal << endl;
		if (qm_front->qValue_afterMove/bboxDiagonal > quadricThreshold)
			break;

		if (qm_front->bCanMove && 
				((cleanDirtyVertices && dirtyDecimateQMs.empty()) || !cleanDirtyVertices))
		{
			lastChangeInError = qm_front->qValue_afterMove - lastError;
			lastError = qm_front->qValue_afterMove;
			cout << "lastChangeInError = " << lastChangeInError << endl;
			cout << "lastError = " << lastError << endl;

			Edge e = noEdgeList[qm_front->q.vIdx];
			bool isBoundaryEdge = (isBoundaryVertex(e.vertex1Idx) || isBoundaryVertex(e.vertex2Idx));
			unsigned int numPlanesInNewQuadric = numPlanesInQuadricsList[e.vertex1Idx] + numPlanesInQuadricsList[e.vertex2Idx];

			// ------- edge collapse ----------
			unsigned int newVertexIdx = edgeCollapse(qm_front->q.vIdx, qm_front->newPos, newUnusedEdges);
			lastNewVertexIdx = newVertexIdx;

			//cout << "Iteration " << iterationCount << ": Removed edge " << qm_front->q.vIdx << " and added new vertex " << newVertexIdx << endl;

			// ------ update vertex quadrics ------
			if (isBoundaryEdge)		// boundary edge
			{
				qVertexDecimateList[newVertexIdx] = computeSmoothness(0, qm_front->newPos, true);	// new quadric is just distance to itself
				qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
				numPlanesInNewQuadric = 1;	
			}
			else	// normal interior edge
			{
				qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
				for (unsigned int i = 0; i < 9; ++i)
				{
					qVertexDecimateList[newVertexIdx].A[i] = qm_front->q.A[i];
					qVertexDecimateList[newVertexIdx].fn.A[i] = qm_front->q.fn.A[i];
					if (i < 3) {
						qVertexDecimateList[newVertexIdx].b[i] = qm_front->q.b[i];
						qVertexDecimateList[newVertexIdx].fn.b[i] = qm_front->q.fn.b[i];
					}
				}
				qVertexDecimateList[newVertexIdx].c = qm_front->q.c;
				qVertexDecimateList[newVertexIdx].fn.c = qm_front->q.fn.c;
			}

			qVertexDecimateList[newVertexIdx].quadricValue = computeQuadricValue(qVertexDecimateList[newVertexIdx], qm_front->newPos);
			numPlanesInQuadricsList[newVertexIdx] = numPlanesInNewQuadric;

			// -------- mark dirty edges ----------
			unsigned int numEdges = noVertexEdgeList[lastNewVertexIdx].size();
			for (unsigned int i = 0; i < numEdges; ++i)
			{
				unsigned int edgeIdx = noVertexEdgeList[lastNewVertexIdx][i];
				Edge e = noEdgeList[edgeIdx];

				// mark dirty for the edges connecting to vertices on the one-ring
				unsigned int vIdx = e.vertex1Idx;
				if ((int)vIdx == lastNewVertexIdx)
					vIdx = e.vertex2Idx;

				unsigned int vIdxDegree =  noVertexEdgeList[vIdx].size();
				for (unsigned int j = 0; j < vIdxDegree; ++j)
				{
					edgeIdx = noVertexEdgeList[vIdx][j];
					QuadricMove* qm = qDecimatePtrList[edgeIdx];
					if (!qm->dirtyQVal_AfterMove)
					{
						qm->dirtyQVal_AfterMove = true;
						qDecimatePtrList[edgeIdx]->q.dirtyQVal = true;		// smoothness term needs to be updated
						dirtyDecimateQMs.push_back(qm);
						dirtyDecimateBackLinks[qm->q.vIdx] = &(dirtyDecimateQMs.back());
					}
				}
			}

			// ------- remove the unused edges from the list ------
			for (unsigned int i = 0; i < newUnusedEdges.size(); ++i)
			{
				unsigned int unusedEdgeIdx = newUnusedEdges[i];
				unsigned int lastDecimateSortPtrSlot = qDecimateSortPtrList.size()-1;
				for (unsigned int j = 0; j < qDecimateSortPtrList.size(); ++j)
				{
					if (qDecimateSortPtrList[j]->q.vIdx == (int)unusedEdgeIdx)
					{
						swap(qDecimateSortPtrList[j], qDecimateSortPtrList[lastDecimateSortPtrSlot]);
						qDecimateSortPtrList.pop_back();

						// remove the ptr in dirtyQMs
						if (dirtyDecimateBackLinks[unusedEdgeIdx] != NULL)
						{
							unsigned int swapIdx = dirtyDecimateQMs.back()->q.vIdx;
							swap(*(dirtyDecimateBackLinks[unusedEdgeIdx]), dirtyDecimateQMs.back());
							swap(dirtyDecimateBackLinks[unusedEdgeIdx], dirtyDecimateBackLinks[swapIdx]);
				
							dirtyDecimateQMs.pop_back();
							dirtyDecimateBackLinks[unusedEdgeIdx] = NULL;
						}

						break;
					}
				}
			}
			newUnusedEdges.clear();
			make_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);
		}
		else if (!dirtyDecimateQMs.empty())	// time to clean up all dirty vertices
		{
			cout << "Cleaning up dirty vertices..." << flush;
			unsigned int numDirty = dirtyDecimateQMs.size();
			double numPctToClean = 1.0;
			bool bCanStop(false);
			unsigned int numCleaned(0);
			while (!dirtyDecimateQMs.empty())
			{
				// clean 
				QuadricMove* qm = dirtyDecimateQMs.back();
				qm->bCanMove = canDecimate(qm->q.vIdx, degreeN);
				if (qm->bCanMove)
				{
					if (qm->q.dirtyQVal)
						qm->q = computeEdgeQuadric(qm->q.vIdx, alpha);
					qm->bCanMove = computeOptimalLocation(qm->q.vIdx, qm->q, qm->newPos, region, angleBound, true);	// qm.q.vIdx is an edge index
					qm->qValue_afterMove = computeQuadricValue(qm->q, qm->newPos);
				}
				qm->dirtyQVal_AfterMove = false;

				dirtyDecimateQMs.pop_back();
				dirtyDecimateBackLinks[qm->q.vIdx] = NULL;

				++numCleaned;

				// check if the cleaned vertex is better than qm_front
 				if (my_quadMovePtr_smallestQuadAfterMoveFirst(qm_front, qm))
 					bCanStop = true;

				if (bCanStop && (numPctToClean * numDirty) <= numCleaned)
					break;
			}
			make_heap(qDecimateSortPtrList.begin(), qDecimateSortPtrList.end(), my_quadMovePtr_smallestQuadAfterMoveFirst);
			afterTime = clock();
			cout << "done! (" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
			cleanDirtyVertices = false;
		}
		else		// no edges can be collapsed
		{
			break;
		}

		iterationCount++;
		if (iterationCount % 1000 == 0) {
			afterTime = clock();
			cout << iterationCount << " edges collapsed...(" << (afterTime-b4Time)/CLOCKS_PER_SEC << " seconds elapsed)" << endl;
		}
	}

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of edge-collapses: " << iterationCount << endl;

	return totalTime;
}


// OLD SORTING METHOD
// void NonobtOptn::decimate(unsigned int numRemaining, double angleBound, unsigned int degreeN, double alpha, NORegionType region)
// {
// 	vector<unsigned int> newUnusedEdges;
// 	unsigned int iterationCount = 0;
// 	int lastNewVertexIdx = -1;	
// 	bool continueOptimize(true);
// 	while (continueOptimize)
// 	{
// 		if (qDecimateList.empty())
// 		{
// 			qDecimateList.clear();
// 			qDecimateIndicesList.clear();
// 			cout << "Computing Quadrics (decimate) for all edges for the first time..." << endl;
// 			make_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_smallestQuadAfterMoveFirst);
// 			//make_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_largestImprovementFirst);
// 
// 			// initialize vertex quadrics
// 			initializeVertexDecimateQuadric();
// 
// 			// initialize edge quadrics
// 			unsigned int numEdges = noEdgeList.size();
// 			qDecimateIndicesList.resize(numEdges);
// 			for (unsigned int i = 0; i < numEdges; ++i)
// 			{
// 				cout << "Processing edge " << i << "/" << numEdges << "..." << endl;
// 				QuadricMove qm;
// 				qm.q.vIdx = i;
// 				qm.bCanMove = canDecimate(qm.q.vIdx, degreeN);
// 				if (qm.bCanMove)
// 				{
// 					qm.q = computeEdgeQuadric(qm.q.vIdx, alpha);
// 					qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound, true);	// q.vIdx is an edge index
// 					qm.qValue_afterMove = computeQuadricValue(qm.q, qm.newPos);
// 					//qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;
// 				}
// 
// 				qDecimateList.push_back(qm);
// 				push_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_smallestQuadAfterMoveFirst);
// 				//push_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_largestImprovementFirst);
// 			}
// 		}
// 		else
// 		{
// 			cout << "Updating Quadrics (decimate) at iteration " << iterationCount << endl;
// 			if (lastNewVertexIdx != -1)
// 			{
// 				unsigned int numEdges = noVertexEdgeList[lastNewVertexIdx].size();
// 				for (unsigned int i = 0; i < numEdges; ++i)
// 				{
// 					unsigned int edgeIdx = noVertexEdgeList[lastNewVertexIdx][i];
// 					Edge e = noEdgeList[edgeIdx];
// 
// 					// update quadrics for the edges connecting to vertices on the one-ring
// 					unsigned int vIdx = e.vertex1Idx;
// 					if ((int)vIdx == lastNewVertexIdx)
// 						vIdx = e.vertex2Idx;
// 
// 					unsigned int vIdxDegree =  noVertexEdgeList[vIdx].size();
// 					for (unsigned int j = 0; j < vIdxDegree; ++j)
// 					{
// 						edgeIdx = noVertexEdgeList[vIdx][j];
// 						e = noEdgeList[edgeIdx];
// 
// 						QuadricMove &qm = qDecimateList[qDecimateIndicesList[edgeIdx]];
// 						qm.bCanMove = canDecimate(edgeIdx, degreeN);
// 						if (qm.bCanMove)
// 						{
// 							qm.q = computeEdgeQuadric(edgeIdx, alpha);
// 							qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound, true);	// qm.q.vIdx is an edge index
// 							qm.qValue_afterMove = computeQuadricValue(qm.q, qm.newPos);
// 							//qm.qValueImprovement = qm.q.quadricValue - qm.qValue_afterMove;
// 						}
// 					}
// 				}
// 
// 				// remove the unused edges from the list
// 				for (unsigned int i = 0; i < newUnusedEdges.size(); ++i)
// 				{
// 					unsigned int unusedEdgeIdx = newUnusedEdges[i];
// 					unsigned int lastDecimateSlot = qDecimateList.size()-1;
// 					unsigned int swapedEdgeIdx = qDecimateList[lastDecimateSlot].q.vIdx;
// 
// 					swap(qDecimateList[qDecimateIndicesList[unusedEdgeIdx]], qDecimateList[lastDecimateSlot]);
// 					swap(qDecimateIndicesList[unusedEdgeIdx], qDecimateIndicesList[swapedEdgeIdx]);
// 					qDecimateList.pop_back();
// 				}
// 				newUnusedEdges.clear();
// 				make_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_smallestQuadAfterMoveFirst);
// 				//make_heap(qDecimateList.begin(), qDecimateList.end(), my_quadMove_largestImprovementFirst);
// 			}
// 		}
// 
// 		// update qDecimateIndicesList
// 		for (unsigned int i = 0; i < qDecimateList.size(); ++i)
// 		{
// 			qDecimateIndicesList[qDecimateList[i].q.vIdx] = i;
// 
// 			// ----------------- debug only -----------------
// // 			if (find(unusedEdgeIdxList.begin(), unusedEdgeIdxList.end(), qDecimateList[i].q.vIdx) != unusedEdgeIdxList.end())
// // 				cout << "*** PROBLEM: Unused edge " << qDecimateList[i].q.vIdx << " is in the heap ***" << endl;
// 			// ----------------- end debug  -----------------
// 		}
// 
// 		if (getNumNoVertices() <= (int)numRemaining)
// 			break;
// 		
// 		//--- debug
//  		//vector<QuadricMove> qDecimateList_debug = qDecimateList;
//  		//sort_heap(qDecimateList_debug.begin(), qDecimateList_debug.end(), my_quadMove_smallestQuadAfterMoveFirst);
// 		//--- end debug
// 
// 		QuadricMove &qm_front = qDecimateList.front();
// 		if (qm_front.bCanMove)
// 		{
// 			Edge e = noEdgeList[qm_front.q.vIdx];
// 			bool isBoundaryEdge = (isBoundaryVertex(e.vertex1Idx) || isBoundaryVertex(e.vertex2Idx));
// 			unsigned int numPlanesInNewQuadric = numPlanesInQuadricsList[e.vertex1Idx] + numPlanesInQuadricsList[e.vertex2Idx];
// 
// 			// edge collapse
// 			unsigned int newVertexIdx = edgeCollapse(qm_front.q.vIdx, qm_front.newPos, newUnusedEdges);
// 			lastNewVertexIdx = newVertexIdx;
// 
// 			cout << "Removed edge " << qm_front.q.vIdx << " and added new vertex " << newVertexIdx << endl;
// 
// 			// update vertex quadrics
// 			if (isBoundaryEdge)		// boundary edge
// 			{
// 				qVertexDecimateList[newVertexIdx] = computeSmoothness(0, qm_front.newPos, true);	// new quadric is just distance to itself
// 				qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
// 				numPlanesInNewQuadric = 1;	
// 			}
// 			else	// normal interior edge
// 			{
// 				qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
// 				for (unsigned int i = 0; i < 9; ++i)
// 				{
// 					qVertexDecimateList[newVertexIdx].A[i] = qm_front.q.A[i];
// 					qVertexDecimateList[newVertexIdx].fn.A[i] = qm_front.q.fn.A[i];
// 					if (i < 3) {
// 						qVertexDecimateList[newVertexIdx].b[i] = qm_front.q.b[i];
// 						qVertexDecimateList[newVertexIdx].fn.b[i] = qm_front.q.fn.b[i];
// 					}
// 				}
// 				qVertexDecimateList[newVertexIdx].c = qm_front.q.c;
// 				qVertexDecimateList[newVertexIdx].fn.c = qm_front.q.fn.c;
// 			}
// 
// 			qVertexDecimateList[newVertexIdx].quadricValue = computeQuadricValue(qVertexDecimateList[newVertexIdx], qm_front.newPos);
// 			numPlanesInQuadricsList[newVertexIdx] = numPlanesInNewQuadric;
// 		}
// 		else		// no edges can be collapsed
// 		{
// 			break;
// 		}
// 
// 		// ------- debug only --------
// // 		for (int i = 0; i < numNoPolygons; i++)
// // 		{
// // 			for (int j = 0; j < 3; j++)
// // 			{
// // 				if (noPolygonsList[i][j] >= numNoVertices)
// // 					cout << "*** PROBLEM: Polygon " << i << " is indexing an invalid vertex " << noPolygonsList[i][j] << " ***" << endl;
// // 			}
// // 		}
// // 	
// // 		for (int i = 0; i < numNoVertices; i++)
// // 		{
// // 			int numNeighbour = noVertexNeighbourList[i].size();
// // 			for (int j = 0; j < numNeighbour; j++)
// // 			{
// // 				if (noVertexNeighbourList[i][j] >= numNoPolygons)
// // 					cout << "*** PROBLEM: Vertex " << i << " is indexing an invalid polygon " << noVertexNeighbourList[i][j] << " ***" << endl;
// // 			}
// // 		}
// 		// ------- end debug --------
// 
// 		iterationCount++;
// 	}
// }

// non-obtusely decimate the non-obtuse mesh using multiple choice + linear search scheme
// numRemaining (in): decimate until the mesh has numRemaining vertices
// quadricThreshold (in): minimum quadric value the non-obtuse mesh can have before stopping the decimation	*** UNUSED ***
// angleBound (in): angle bound (in degrees)
// degreeN (in): max. of min. degrees that we want to avoid
// alpha (in): weighting alpha value for objective function
// return value (out): total time taken
clock_t NonobtOptn::decimate_MC_LinearSearch(unsigned int numRemaining, double quadricThreshold, double angleBound, unsigned int degreeN, double alpha)
{
	vector<unsigned int> newUnusedEdges;	// stores the unused edges that were collapsed
	unsigned int iterationCount = 0;
	int lastNewVertexIdx = -1;
	bool continueOptimize(true);

	clock_t b4Time = clock();
	clock_t afterTime = clock();

	// initialize vertex quadrics
	cout << "Computing Quadrics (decimate) for all vertices for the first time..." << flush;
	initializeVertexDecimateQuadric();
	cout << "done!" << endl;

	const unsigned int PROBECOUNT = 10;
// 	const unsigned int MAXPROBEATTEMPT = 100;
	
	while (continueOptimize)
	{
		// check if we have enough vertices in the mesh
		if (getNumNoVertices() <= (int)numRemaining)
			break;

		// randomly choose d edges to collapse
		double lowestQValue(0.0);
		QuadricMove qm_decimate;
		qm_decimate.q.vIdx = -1;
		unsigned int numProbe(0);
		unsigned int numProbeAttempt(0);
		while (numProbe < PROBECOUNT)
		{
			int edgeIdx = rand() % noEdgeList.size();
			++numProbeAttempt;
			QuadricMove qm;
			qm.q.vIdx = edgeIdx;
			qm.bCanMove = canDecimate(qm.q.vIdx, degreeN);
			if (!qm.bCanMove) {	// can't decimate edge due to degreeN restrictions.
				continue;
			}
			qm.q = computeEdgeQuadric(qm.q.vIdx, alpha);

			// compute midpoint of edge to initialize an old point as qm.newPos
			unsigned int v1Idx = noEdgeList[qm.q.vIdx].vertex1Idx;
			unsigned int v2Idx = noEdgeList[qm.q.vIdx].vertex2Idx;
			midpoint(qm.newPos, noVerticesList[3*v1Idx], noVerticesList[3*v1Idx+1], noVerticesList[3*v1Idx+2], noVerticesList[3*v2Idx], noVerticesList[3*v2Idx+1], noVerticesList[3*v2Idx+2]);

// 			if (numProbeAttempt > MAXPROBEATTEMPT) {
// 	 			qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, ANGLEBOUND_7PLANES, angleBound, true);	// q.vIdx is an edge index
// 			} else {
				unsigned int numTries(0);
 				qm.bCanMove = computeOptimalLocation_linearSearch(qm.q.vIdx, qm.q, qm.newPos, angleBound, numTries, true);	// q.vIdx is an edge index
// 			}
			if (!qm.bCanMove) {	// can't decimate edge due to degreeN restrictions.
				continue;
			}
			qm.qValue_afterMove = computeQuadricValue(qm.q, qm.newPos);
			++numProbe;
			
			// choose the edge with the lowest qValue to decimate
			if (qm_decimate.q.vIdx == -1 || qm.qValue_afterMove < lowestQValue) {
				qm_decimate = qm;
				lowestQValue = qm.qValue_afterMove;
			}
		}

		// decimate
		Edge e = noEdgeList[qm_decimate.q.vIdx];
		bool isBoundaryEdge = (isBoundaryVertex(e.vertex1Idx) || isBoundaryVertex(e.vertex2Idx));
		unsigned int numPlanesInNewQuadric = numPlanesInQuadricsList[e.vertex1Idx] + numPlanesInQuadricsList[e.vertex2Idx];
 
		// ------- edge collapse ----------
		unsigned int newVertexIdx = edgeCollapse(qm_decimate.q.vIdx, qm_decimate.newPos, newUnusedEdges);
		lastNewVertexIdx = newVertexIdx;

		// make sure we remove the unused edges
		removeUnused(newUnusedEdges);
		newUnusedEdges.clear();
 
		cout << "Iteration " << iterationCount << ": Removed edge " << qm_decimate.q.vIdx << " and added new vertex " << newVertexIdx << endl;
 
		// ------ update vertex quadrics ------
		if (isBoundaryEdge)		// boundary edge
		{
			qVertexDecimateList[newVertexIdx] = computeSmoothness(0, qm_decimate.newPos, true);	// new quadric is just distance to itself
			qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
			numPlanesInNewQuadric = 1;	
		}
		else	// normal interior edge
		{
			qVertexDecimateList[newVertexIdx].vIdx = newVertexIdx;
			for (unsigned int i = 0; i < 9; ++i)
			{
				qVertexDecimateList[newVertexIdx].A[i] = qm_decimate.q.A[i];
				qVertexDecimateList[newVertexIdx].fn.A[i] = qm_decimate.q.fn.A[i];
				if (i < 3) {
					qVertexDecimateList[newVertexIdx].b[i] = qm_decimate.q.b[i];
					qVertexDecimateList[newVertexIdx].fn.b[i] = qm_decimate.q.fn.b[i];
				}
			}
			qVertexDecimateList[newVertexIdx].c = qm_decimate.q.c;
			qVertexDecimateList[newVertexIdx].fn.c = qm_decimate.q.fn.c;
		}

		qVertexDecimateList[newVertexIdx].quadricValue = computeQuadricValue(qVertexDecimateList[newVertexIdx], qm_decimate.newPos);
		numPlanesInQuadricsList[newVertexIdx] = numPlanesInNewQuadric;

		++iterationCount;
	}

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of edge-collapses: " << iterationCount << endl;

	return totalTime;
}


// compute quadric for novIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
// alpha (in): weighting alpha value for objective function
// numOneRingSearch (in): number of one-ring to search for closest face
// numOneRingQuadric (in): number of one-ring to use for computing quadric
// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated
Quadric NonobtOptn::computeQuadric(int novIdx, double alpha, unsigned int numOneRingSearch, unsigned int numOneRingQuadric, bool bUpdateClosestPolygon) //const
{
	// check if novIdx is a boundary vertex, if so, use boundaryQuadric
	if (isBoundaryVertex(novIdx))
		return computeBoundaryQuadric(novIdx, alpha, numOneRingSearch, numOneRingQuadric, bUpdateClosestPolygon);

	double minDist(0.0);
	double closestPt[3] = {0,0,0};
	
	// find closest face to vertex
	if (closestFaceList[novIdx] == -1)	// closest face is unassigned
	{
		// --- global search for closest face ---
		closestFaceList[novIdx] = findClosestPolygon(novIdx, noVerticesList, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, true, 0, numOneRingSearch);
		for (unsigned int i = 0; i < 3; ++i)
			closestPtList[3*novIdx+i] = closestPt[i];
	}
	else if (bUpdateClosestPolygon)
	{
		// --- local search for closest face ---
		unsigned int oldClosestFace = closestFaceList[novIdx];

		closestFaceList[novIdx] = findClosestPolygon(novIdx, noVerticesList, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[novIdx], numOneRingSearch);
		for (unsigned int i = 0; i < 3; ++i)
			closestPtList[3*novIdx+i] = closestPt[i];

		if (closestFaceList[novIdx] != (int)oldClosestFace)
			++numClosestFaceChanged;
	}

	// construct quadrics for the one-ring
	Quadric q;
	q.vIdx = novIdx;
	q.quadricValue = 0.0;
	for (unsigned int i = 0; i < 9; ++i)
		q.A[i] = 0.0;
	for (unsigned int i = 0; i < 3; ++i)
		q.b[i] = 0.0;
	q.c = 0.0;
	
	// stores the one ring vertices that will be used
	vector<int> oneRingToAdd;
	oneRingToAdd.push_back(dPolygonsList[closestFaceList[novIdx]][0]);
	oneRingToAdd.push_back(dPolygonsList[closestFaceList[novIdx]][1]);
	oneRingToAdd.push_back(dPolygonsList[closestFaceList[novIdx]][2]);

	unsigned int numTrianglesInQuadric(0);
	
	//vector<int> processedTriangles;	//stores the polygons that has been processed
	
	for (unsigned int i = 1; i <= numOneRingQuadric; ++i)
	{
		unsigned int numOneRingVertices = oneRingToAdd.size();
		for (unsigned int j = 0; j < numOneRingVertices; ++j)
		{
			int oneRingIdx = oneRingToAdd[j];
			for (unsigned int k = 0; k < dVertexNeighbourList[oneRingIdx].size(); ++k)
			{
				int currentPolyIdx = dVertexNeighbourList[oneRingIdx][k];

				//if (find(processedTriangles.begin(), processedTriangles.end(), currentPolyIdx) == processedTriangles.end())
				//{
				
				double normal[3] = {dFaceNormalsList[3*currentPolyIdx],
									dFaceNormalsList[3*currentPolyIdx+1],
									dFaceNormalsList[3*currentPolyIdx+2]};
				normalize(normal);
				double pt[3] = {dVerticesList[3*dPolygonsList[currentPolyIdx][0]],
								dVerticesList[3*dPolygonsList[currentPolyIdx][0]+1],
								dVerticesList[3*dPolygonsList[currentPolyIdx][0]+2]};
				double d = -1.0 * dotProduct(normal, pt);
				
				// c = d^2
				q.c += d*d;
				// b = d*normal
				q.b[0] += d*normal[0];
				q.b[1] += d*normal[1];
				q.b[2] += d*normal[2];
				// A = normal*normal^T
				q.A[0] += normal[0]*normal[0];
				q.A[1] += normal[0]*normal[1];
				q.A[2] += normal[0]*normal[2];
				q.A[3] += normal[0]*normal[1];
				q.A[4] += normal[1]*normal[1];
				q.A[5] += normal[1]*normal[2];
				q.A[6] += normal[0]*normal[2];
				q.A[7] += normal[1]*normal[2];
				q.A[8] += normal[2]*normal[2];

				++numTrianglesInQuadric;

				//processedTriangles.push_back(currentPolyIdx);
				//}

				// add vertices to add
				if (i != numOneRingQuadric)	// we haven't add the outer one-ring yet
				{
					// add the 3 vertices of the current polgyon into the oneRingToAdd list
					for (unsigned int l = 0; l < 3; ++l)
					{
						int triVIdx = dPolygonsList[currentPolyIdx][l];
						// make sure we don't add duplicates into the list
						bool isInList(false);
						for (unsigned int m = 0; m < oneRingToAdd.size(); ++m)
						{
							if (oneRingToAdd[m] == triVIdx)
							{
								isInList = true;
								break;
							}
						}
						if (isInList)
							continue;
						
						oneRingToAdd.push_back(triVIdx);
					}
				}
			}
		}
		
		// erase the added vertices
		for (unsigned int j = 0; j < numOneRingVertices; ++j)
			oneRingToAdd.erase(oneRingToAdd.begin());
	}
	
	// special case when we only use the closest triangle to compute the quadric
	if (numOneRingQuadric == 0)
	{
		int currentPolyIdx = closestFaceList[novIdx];
		double normal[3] = {dFaceNormalsList[3*currentPolyIdx],
							dFaceNormalsList[3*currentPolyIdx+1],
							dFaceNormalsList[3*currentPolyIdx+2]};
		normalize(normal);
		double pt[3] = {dVerticesList[3*dPolygonsList[currentPolyIdx][0]],
						dVerticesList[3*dPolygonsList[currentPolyIdx][0]+1],
						dVerticesList[3*dPolygonsList[currentPolyIdx][0]+2]};
		double d = -1.0 * dotProduct(normal, pt);
		
		// c = d^2
		q.c = d*d;
		// b = d*normal
		q.b[0] = d*normal[0];
		q.b[1] = d*normal[1];
		q.b[2] = d*normal[2];
		// A = normal*normal^T
		q.A[0] = normal[0]*normal[0];
		q.A[1] = normal[0]*normal[1];
		q.A[2] = normal[0]*normal[2];
		q.A[3] = q.A[1];
		q.A[4] = normal[1]*normal[1];
		q.A[5] = normal[1]*normal[2];
		q.A[6] = q.A[2];
		q.A[7] = q.A[5];
		q.A[8] = normal[2]*normal[2];
	}
	
	// compute quadric value
	double pt[3] = {noVerticesList[3*novIdx],
					noVerticesList[3*novIdx+1], 
					noVerticesList[3*novIdx+2]};
	double Av[3] = {q.A[0]*pt[0] + q.A[1]*pt[1] + q.A[2]*pt[2],
					q.A[3]*pt[0] + q.A[4]*pt[1] + q.A[5]*pt[2],
					q.A[6]*pt[0] + q.A[7]*pt[1] + q.A[8]*pt[2]};
	q.quadricValue = dotProduct(Av, pt) + 2.0*dotProduct(q.b, pt) + q.c;
	q.dirtyQVal = false;

	// smoothing term
	
	// get centroid first
	double oneRingCentroid[3] = {0,0,0};
	for (unsigned int i = 0; i < noVertexNeighbourList[novIdx].size(); ++i)
	{
		int polyIdx = noVertexNeighbourList[novIdx][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
				e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
				break;
			}
		}
		
		oneRingCentroid[0] += noVerticesList[3*(e.vertex1Idx)];
		oneRingCentroid[1] += noVerticesList[3*(e.vertex1Idx)+1];
		oneRingCentroid[2] += noVerticesList[3*(e.vertex1Idx)+2];
		oneRingCentroid[0] += noVerticesList[3*(e.vertex2Idx)];
		oneRingCentroid[1] += noVerticesList[3*(e.vertex2Idx)+1];
		oneRingCentroid[2] += noVerticesList[3*(e.vertex2Idx)+2];
	}
	
	// counted each one ring vertex twice
	oneRingCentroid[0] /= 2 * noVertexNeighbourList[novIdx].size();
	oneRingCentroid[1] /= 2 * noVertexNeighbourList[novIdx].size();
	oneRingCentroid[2] /= 2 * noVertexNeighbourList[novIdx].size();
	
	// quad. term
	for (unsigned int i = 0; i < 9; ++i)
	{
		q.fn.A[i] = (1.0/numTrianglesInQuadric) * q.A[i];
		if (alpha != 1.0)
			q.fn.A[i] = q.A[i] * alpha * (1.0/numTrianglesInQuadric);
		if (i == 0 || i == 4 || i == 8)
			q.fn.A[i] += (1 - alpha);
	}
	
	// linear term
	for (unsigned int i = 0; i < 3; ++i)
	{
		q.fn.b[i] =  (1.0/numTrianglesInQuadric) * q.b[i];
		if (alpha != 1.0)
			q.fn.b[i] =  (1.0/numTrianglesInQuadric) * alpha*(q.b[i]) - (1 - alpha)*oneRingCentroid[i];
	}
	
	// constant term
	q.fn.c = (1.0/numTrianglesInQuadric) * q.c;
	if (alpha != 1.0)
		q.fn.c =  (1.0/numTrianglesInQuadric) * alpha*(q.c) + (1 - alpha)*dotProduct(oneRingCentroid, oneRingCentroid);
		
	return q;	
}

// compute boundary quadric for novIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
// alpha (in): weighting alpha value for objective function
// numOneRingSearch (in): number of one-ring to search for closest face/edge
// numOneRingQuadric (in): number of one-ring/neighbour-edge to use for computing quadric
// bUpdateClosestPolygon (in): true - closest polygon for the vertex is updated
Quadric NonobtOptn::computeBoundaryQuadric(int novIdx, double alpha, unsigned int numOneRingSearch, unsigned int numOneRingQuadric, bool bUpdateClosestPolygon) const
{
	// ------------ update closest boundary edge to vertex ------------
	double pt[3] = {noVerticesList[3*novIdx], noVerticesList[3*novIdx+1], noVerticesList[3*novIdx+2]};
	double closestPt[3] = {0,0,0};
	if (closestBoundaryEdgeList[novIdx].vertex1Idx == -1 || closestBoundaryEdgeList[novIdx].vertex2Idx == -1) {
		closestBoundaryEdgeList[novIdx] = findClosestBoundaryEdge(novIdx, pt, closestPt, numOneRingSearch, true);
		for (unsigned int i = 0; i < 3; ++i)
			closestPtList[3*novIdx+i] = closestPt[i];
	}
	else if (bUpdateClosestPolygon) {
		closestBoundaryEdgeList[novIdx] = findClosestBoundaryEdge(novIdx, pt, closestPt, numOneRingSearch, false);
		for (unsigned int i = 0; i < 3; ++i)
			closestPtList[3*novIdx+i] = closestPt[i];
	}
	
	// ------------ construct quadric ----------------
	// Idea: quadric should measures the distance from the vertex to the set of vertices that make up the boundary edges
	//       giving more weights to points that connected to the boundary edge that is closer to the vertex
	//
	// Q(x) = xIx - 2px + pp
	// where I is the identity matrix, p is the point we want to get close to
	//
	
	Quadric q;
	q.vIdx = novIdx;

	// two vertices of closest boundary edge is added to quadric
	q.A[0] = 1;
	q.A[1] = 0;
	q.A[2] = 0;
	q.A[3] = 0;
	q.A[4] = 1;
	q.A[5] = 0;
	q.A[6] = 0;
	q.A[7] = 0;
	q.A[8] = 1;
	
	q.b[0] = -1*closestPtList[3*novIdx];
	q.b[1] = -1*closestPtList[3*novIdx+1];
	q.b[2] = -1*closestPtList[3*novIdx+2];

	q.c = dotProduct(closestPtList[3*novIdx], closestPtList[3*novIdx+1], closestPtList[3*novIdx+2], 
						closestPtList[3*novIdx], closestPtList[3*novIdx+1], closestPtList[3*novIdx+2]);

	// compute the quadric value
	q.quadricValue = computeQuadricValue(q, pt);
	q.dirtyQVal = false;

	// ------- construct smoothness term for objective function -------

	// smoothing term
	// get centroid first
	double oneRingCentroid[3] = {0,0,0};
	int numBoundaryNbr(0);
	for (unsigned int i = 0; i < noVertexEdgeList[novIdx].size(); ++i)
	{
		// find boundary neighbour
		int vIdx = noEdgeList[noVertexEdgeList[novIdx][i]].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[noVertexEdgeList[novIdx][i]].vertex2Idx;

		if (isBoundaryVertex(vIdx)) {
			oneRingCentroid[0] += noVerticesList[3*vIdx];
			oneRingCentroid[1] += noVerticesList[3*vIdx+1];
			oneRingCentroid[2] += noVerticesList[3*vIdx+2];
			++numBoundaryNbr;
		}
		
		if (numBoundaryNbr == 2)
			break;
	}
	oneRingCentroid[0] /= 2;
	oneRingCentroid[1] /= 2;
	oneRingCentroid[2] /= 2;
	
	
	int numEdgesInQuadric = 1;
	for (unsigned int i = 0; i < 9; ++i)
	{
		// quad. term
		q.fn.A[i] = (1.0/numEdgesInQuadric) * q.A[i];
		if (alpha != 1.0)
			q.fn.A[i] = q.A[i] * alpha * (1.0/numEdgesInQuadric);
		if (i == 0 || i == 4 || i == 8)
			q.fn.A[i] += (1 - alpha);

		// linear term
		if (i < 3)
		{
			q.fn.b[i] = (1.0/numEdgesInQuadric) * q.b[i];
			if (alpha != 1.0)
				q.fn.b[i] =  (1.0/numEdgesInQuadric) * alpha*(q.b[i]) - (1 - alpha)*oneRingCentroid[i];
		}
	}
	
	// constant term
	q.fn.c = (1.0/numEdgesInQuadric) * q.c;
	if (alpha != 1.0)
		q.fn.c =  (1.0/numEdgesInQuadric) * alpha*(q.c) + (1 - alpha)*dotProduct(oneRingCentroid, oneRingCentroid);

	return q;

}

// compute edge quadric for edgeIdx and returns it
// edgeIdx (in): edge index (of the nonobtuse mesh) of the quadric to be computed
// alpha (in): weighting alpha value for objective function
Quadric NonobtOptn::computeEdgeQuadric(int edgeIdx, double alpha) const
{
	Edge e = noEdgeList[edgeIdx];
	unsigned int numTrianglesInQuadric = numPlanesInQuadricsList[e.vertex1Idx] + numPlanesInQuadricsList[e.vertex2Idx];

	// get centroid first
	double oneRingCentroid[3] = {0,0,0};
	vector<unsigned int> oneRingVertices;

	// boundary edge uses different centroid; use the avg of the 2 immediate boundary neighbours as centroid
	if (isBoundaryVertex(e.vertex1Idx) && isBoundaryVertex(e.vertex2Idx))
		getEdgeBoundaryNeighbours(edgeIdx, oneRingVertices);
	else if (isBoundaryVertex(e.vertex1Idx))
	{
		getVertexBoundaryNeighbours(e.vertex1Idx, oneRingVertices);
		//numTrianglesInQuadric = 1;
	}
	else if (isBoundaryVertex(e.vertex2Idx))
	{
		getVertexBoundaryNeighbours(e.vertex2Idx, oneRingVertices);
		//numTrianglesInQuadric = 1;
	}
	else	// otherwise, use one-ring vertices to compute centroid
		if (!getEdgeOneRingVertices(edgeIdx, oneRingVertices))
			cout << "Warning: Nonobtoptn::computeEdgeQuadric() - edge does not have a collapsable one-ring." << endl;
	
	for (unsigned int i = 0; i < oneRingVertices.size(); ++i)
	{
		oneRingCentroid[0] += noVerticesList[3*(oneRingVertices[i])];
		oneRingCentroid[1] += noVerticesList[3*(oneRingVertices[i])+1];
		oneRingCentroid[2] += noVerticesList[3*(oneRingVertices[i])+2];
	}
	
	oneRingCentroid[0] /= oneRingVertices.size();
	oneRingCentroid[1] /= oneRingVertices.size();
	oneRingCentroid[2] /= oneRingVertices.size();

	// edge quadric = summation of the two vertices quadric
	Quadric q;
	q.vIdx = edgeIdx;
	for (unsigned int i = 0; i < 9; ++i)
	{
		q.A[i] = qVertexDecimateList[e.vertex1Idx].A[i] + qVertexDecimateList[e.vertex2Idx].A[i];	
		q.fn.A[i] = (1.0/numTrianglesInQuadric) * q.A[i];

		if (alpha != 1.0)
		{
			q.fn.A[i] = q.A[i] * alpha * (1.0/numTrianglesInQuadric);
			if (i == 0 || i == 4 || i == 8)
				q.fn.A[i] += (1 - alpha);
		}
	}
	for (unsigned int i = 0; i < 3; ++i)
	{
		q.b[i] = qVertexDecimateList[e.vertex1Idx].b[i] + qVertexDecimateList[e.vertex2Idx].b[i];
		q.fn.b[i] = (1.0/numTrianglesInQuadric) * q.b[i];
		
		if (alpha != 1.0)
		{
			q.fn.b[i] = (1.0/numTrianglesInQuadric) * alpha*(q.b[i]) - (1 - alpha)*oneRingCentroid[i];
		}
	}
	q.c = qVertexDecimateList[e.vertex1Idx].c + qVertexDecimateList[e.vertex2Idx].c;
	q.fn.c = (1.0/numTrianglesInQuadric) * q.c;
	if (alpha != 1.0)
		q.fn.c =  (1.0/numTrianglesInQuadric) * alpha*(q.c) + (1 - alpha)*dotProduct(oneRingCentroid, oneRingCentroid);

	q.dirtyQVal = false;

	return q;
}

// get the two immediate boundary vertex neighbour of edgeIdx and store in 'edgeNbr'
//
// edgeIdx (in): boundary edge index (of the nonobtuse mesh) of the quadric to be computed
// edgeNbr (out): stores the 2 immediate boundary vertex neighbour of edgeIdx
void NonobtOptn::getEdgeBoundaryNeighbours(int edgeIdx, vector<unsigned int> &edgeNbr) const
{
	Edge e = noEdgeList[edgeIdx];

	for (unsigned int i = 0; i < 2; ++i)
	{
		int vIdx = e.vertex1Idx;
		int otherVIdx = e.vertex2Idx;
		if (i == 1)
			swap(vIdx, otherVIdx);

		// go through all the neighbour vertices of vIdx
		for (unsigned int j = 0; j < noVertexEdgeList[vIdx].size(); ++j)
		{
			Edge nbrEdge = noEdgeList[noVertexEdgeList[vIdx][j]];
			int nbrIdx = nbrEdge.vertex1Idx;
			if (nbrIdx == vIdx)
				nbrIdx = nbrEdge.vertex2Idx;

			// add nbr vertex that is boundary vertex and also not the one in edgeIdx
			if (nbrIdx != otherVIdx && isBoundaryVertex(nbrIdx)) {
				edgeNbr.push_back(nbrIdx);
				break;
			}
		}
	}
}

// get the two immediate boundary vertex neighbour of boundary vertex novIdx and store in 'edgeNbr'
//
// novIdx (in): boundary vertex index (of the nonobtuse mesh) of the quadric to be computed
// edgeNbr (out): stores the 2 immediate boundary vertex neighbour of edgeIdx
void NonobtOptn::getVertexBoundaryNeighbours(int novIdx, vector<unsigned int> &edgeNbr) const
{
	int vIdx = novIdx;
	// go through all the neighbour vertices of vIdx
	for (unsigned int j = 0; j < noVertexEdgeList[vIdx].size(); ++j)
	{
		Edge nbrEdge = noEdgeList[noVertexEdgeList[vIdx][j]];
		int nbrIdx = nbrEdge.vertex1Idx;
		if (nbrIdx == vIdx)
			nbrIdx = nbrEdge.vertex2Idx;

		// add nbr vertex that is boundary vertex
		if (isBoundaryVertex(nbrIdx)) {
			edgeNbr.push_back(nbrIdx);
		}
	}
}

/*
// move vertex (novIdx) to a better position that minimizes quadric q
// ******* OBSOLETE *******
// returns true if successful
// novIdx (in): index of the vertex that is being moved
// q (in): quadric value of novIdx
// region (in): determines how the non-obtuse region is to be approximated
bool NonobtOptn::moveToOptimalLocation(int novIdx, Quadric q, NORegionType region)
{
	// --- compute the non-obtuse regions (set of plane constraints)
	PlaneEqn* constraints = NULL;
	unsigned int numConstraints = getNonobtuseRegion(novIdx, constraints, region, false, 0);
	
	// --- setup parameters for solver
	int nx = 3;		// number of variables to optimize
	double c[3] = {q.fn.b[0], q.fn.b[1], q.fn.b[2]};		// linear term in objective function
	double  xupp[] = {0, 0, 0};		// bounds on x
	char   ixupp[] = {0, 0, 0};
	double  xlow[] = {0, 0, 0};
	char   ixlow[] = {0, 0, 0};
	
	// quadratic term in objective function
	int nnzQ = 0;
	int numEntriesInQ = nx*nx;
	int irowQ_tmp[numEntriesInQ];
	int jcolQ_tmp[numEntriesInQ];
	double dQ_tmp[numEntriesInQ];
	for (int row = 0; row < nx; ++row)	
	{
		for (int col = 0; col < row+1; ++col)
		{
			if (q.fn.A[row*nx+col] != 0.0)
			{
				irowQ_tmp[nnzQ] = row;
				jcolQ_tmp[nnzQ] = col;
				dQ_tmp[nnzQ] = q.fn.A[row*nx+col];
				++nnzQ;
			}
		}
	}
	
	int irowQ[nnzQ]; 
	int jcolQ[nnzQ];
	double dQ[nnzQ];
	for (int i = 0; i < nnzQ; ++i)	
	{
		irowQ[i] = irowQ_tmp[i];
		jcolQ[i] = jcolQ_tmp[i];
		dQ[i] = dQ_tmp[i];
	}
	
	// linear equality constraints
	int my         = 0;
	double * b     = 0;
	int nnzA       = 0;
	int * irowA    = 0;
	int * jcolA    = 0;
	double * dA    = 0;

	// linear inequality constraints
	int mz = numConstraints;
	double clow[mz];
	char  iclow[mz];
	double cupp[mz];
	char  icupp[mz];
	
	int nnzC = 0;
	int irowC_tmp[mz*nx];
	int jcolC_tmp[mz*nx];
	double dC_tmp[mz*nx];
	for (int i = 0; i < mz; ++i)
	{
		clow[i] = -1.0 * constraints[i].d;	// ax + by + cz >= -d
		iclow[i] = 1;
		cupp[i] = 0;
		icupp[i] = 0;
		
		if (constraints[i].a != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 0;
			dC_tmp[nnzC] = constraints[i].a;
			++nnzC;
		}
		
		if (constraints[i].b != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 1;
			dC_tmp[nnzC] = constraints[i].b;
			++nnzC;
		}
		
		if (constraints[i].c != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 2;
			dC_tmp[nnzC] = constraints[i].c;
			++nnzC;
		}
	}
	
	int   irowC[nnzC];
	int   jcolC[nnzC];
	double   dC[nnzC];
	for (int i = 0; i < nnzC; ++i)
	{
		irowC[i] = irowC_tmp[i];
		jcolC[i] = jcolC_tmp[i];
		dC[i]    = dC_tmp[i];
	}
	
	// --- finish setting up params
	
	// solving optimization problem
	QpGenSparseMa27 *qp = new QpGenSparseMa27(nx, my, mz, nnzQ, nnzA, nnzC);
	
	QpGenData *prob = (QpGenData*) qp->copyDataFromSparseTriple(
										c,      irowQ,  nnzQ,   jcolQ,  dQ,
										xlow,   ixlow,  xupp,   ixupp,
										irowA,  nnzA,   jcolA,  dA,     b,
										irowC,  nnzC,   jcolC,  dC,
										clow,   iclow,  cupp,   icupp );
										
	QpGenVars *vars = (QpGenVars*) qp->makeVariables( prob );
	QpGenResiduals * resid = (QpGenResiduals *) qp->makeResiduals( prob );
  
	GondzioSolver *s = new GondzioSolver( qp, prob );
	
	// s->monitorSelf();	// print out solver info.
	int ierr = s->solve(prob, vars, resid);
	
	// ERROR CODE
	// 	enum TerminationCode
	// 	{
	// 		SUCCESSFUL_TERMINATION = 0,
	// 		NOT_FINISHED,
	// 		MAX_ITS_EXCEEDED,
	// 		INFEASIBLE,
	// 		UNKNOWN
	// 	};
	
	if (ierr == 0)
	{
		// found optimal location, move vertex and update mesh
		double newloc[nx];
		vars->x->copyIntoArray(newloc);
		updateMesh(novIdx, newloc);
	}
	
	// clean up
	delete qp;
	delete prob;
	delete vars;
	delete resid;
	delete s;
	delete[] constraints;
	
	return (ierr == 0);
}
*/
// updates vertex with index 'novIdx' on the nonobtuse mesh to position 'newloc'
// novIdx (in): index of the vertex that is being moved
// newloc (in): new location
void NonobtOptn::updateMesh(int novIdx, double *newloc)
{
	// update new location for vertex
	for (unsigned int i = 0; i < 3; ++i)
		noVerticesList[3*novIdx+i] = newloc[i];
	
	// update face normals of one-ring
	unsigned int numPolyInOneRing = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numPolyInOneRing; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		double normal[] = {0.0, 0.0, 0.0};
		getFaceNormal(polyIdx, noPolygonsList, noVerticesList, normal);
		
		for (unsigned int j = 0; j < 3; ++j)
			noFaceNormalsList[3*polyIdx+j] = normal[j];
	}
	
	// update vertex normals
	vector<unsigned int> updatedVerticesIdx;
	for (unsigned int i = 0; i < numPolyInOneRing; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[polyIdx][j];
		
			// check if vertex has been updated
			bool isUpdated(false);
			for (unsigned int k = 0; k < updatedVerticesIdx.size(); ++k)
			{
				if (vIdx == updatedVerticesIdx[k])
				{
					isUpdated = true;
					break;
				}
			}
			if (isUpdated)
				continue;
			
			// update vertex normal
			double vertexNormal[3];
			getVertexNormal(vIdx, noPolygonsList, noVerticesList, noVertexNeighbourList, noFaceNormalsList, vertexNormal);
			for (int k = 0; k < 3; ++k)
				noVertexNormalsList[3*vIdx+k] = vertexNormal[k];
			
			updatedVerticesIdx.push_back(vIdx);
		}
	}
}

// edge collapse the non-obtuse mesh
// edgeIdx (in): index of the edge that is being collapsed
// newloc (in): new location for the collapsed vertex
// newUnusedEdges (out): vector of new unused edges
// return val (out): index of the new vertex where the edge collapsed into
unsigned int NonobtOptn::edgeCollapse(int edgeIdx, double *newloc, vector<unsigned int> &newUnusedEdges)
{
	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;

	// check if edge is boundary edge
	bool isBoundaryEdge = (isBoundaryVertex(vIdx1) || isBoundaryVertex(vIdx2));

	// list of poly indices that are unused
	vector<int> unusedPolyIdxList;
	// list of vertex indices that are unused
	vector<int> unusedVertexIdxList;

	// stores the one-ring vertices (in the order based on right hand rule)
	vector<unsigned int> oneRingVertices;
	if (!getEdgeOneRingVertices(edgeIdx, oneRingVertices))
	{
		cout << "Warning: NonobtOptn::edgeCollapse() - trying to collapse an edge that may introduce non-manifold vertices/edge" << endl;
	}

	vector<int> v1NbrFaces = noVertexNeighbourList[vIdx1];
	vector<int> v2NbrFaces = noVertexNeighbourList[vIdx2];
	unsigned int numUnusedEdgesIntroduced(0);
	for (unsigned int i = 0; i < 2; ++i)
	{
		unsigned int currentVIdx = vIdx1;
		unsigned int otherVIdx = vIdx2;
		vector<int> *nbrFaces = &v1NbrFaces;
		if (i == 1)
		{
			currentVIdx = vIdx2;
			otherVIdx = vIdx1;
			nbrFaces = &v2NbrFaces;
		}

		// store unused edge index
		unsigned int numEdges = noVertexEdgeList[currentVIdx].size();
		for (unsigned int j = 0; j < numEdges; ++j)
		{
			unsigned unusedEdgeIdx = noVertexEdgeList[currentVIdx][j];
			
			// do not add duplicate edge(vIdx1, vIdx2) into the unused edge list
			if (i == 1 && 
					((noEdgeList[unusedEdgeIdx].vertex1Idx == (int)vIdx1 && noEdgeList[unusedEdgeIdx].vertex2Idx == (int)vIdx2) ||
					(noEdgeList[unusedEdgeIdx].vertex1Idx == (int)vIdx2 && noEdgeList[unusedEdgeIdx].vertex2Idx == (int)vIdx1)))
				continue;

			unusedEdgeIdxList.push_back(unusedEdgeIdx);
			++numUnusedEdgesIntroduced;
		}
		
		// store unused poly idx
		unsigned int numNeighbours = (*nbrFaces).size();
		for (unsigned int j = 0; j < numNeighbours; ++j)
		{
			unsigned int polyIdx = (*nbrFaces)[j];
			
			Edge e;
			e.vertex1Idx = 0;
			e.vertex2Idx = 0;
			for (unsigned int k = 0; k < 3; ++k)
			{
				if (noPolygonsList[polyIdx][k] == (int)currentVIdx)
				{
					e.vertex1Idx = noPolygonsList[polyIdx][(k+1)%3];	// v_i
					e.vertex2Idx = noPolygonsList[polyIdx][(k+2)%3];	// v_{i+1}
					break;
				}
			}

			for (unsigned int k = 0; k < 2; ++k)
			{
				unsigned int vIdx = e.vertex1Idx;
				if (k == 1)
					vIdx = e.vertex2Idx;

				// remove vertex-to-poly pointer for unused poly
				for (vector<int>::iterator iter = noVertexNeighbourList[vIdx].begin();
					iter != noVertexNeighbourList[vIdx].end(); iter++)
				{
					if (*iter == (int)polyIdx)
					{
						noVertexNeighbourList[vIdx].erase(iter);
						break;
					}
				}

				// remove vertex-to-edge pointer for unused edges
				for (vector<int>::iterator iter = noVertexEdgeList[vIdx].begin();
					iter != noVertexEdgeList[vIdx].end(); iter++)
				{
					if ((noEdgeList[*iter].vertex1Idx == (int)vIdx && noEdgeList[*iter].vertex2Idx == (int)currentVIdx) ||
						(noEdgeList[*iter].vertex1Idx == (int)currentVIdx && noEdgeList[*iter].vertex2Idx == (int)vIdx))
					{
						noVertexEdgeList[vIdx].erase(iter);
						break;
					}
				}
			}
	
			// skip triangles that has vIdx1 and vIdx2 as vertices
			if (i == 1)		// make sure we don't add duplicate triangles into unused poly list
			{
				if (e.vertex1Idx == (int)otherVIdx || e.vertex2Idx == (int)otherVIdx)
					continue;
			}

			// add polyIdx to unused poly list
			unusedPolyIdxList.push_back(polyIdx);
		}
	}

	// store unused vertex
	unusedVertexIdxList.push_back(vIdx1);
	unusedVertexIdxList.push_back(vIdx2);

	// add new vertex to mesh
	unsigned int newVertexIdx = unusedVertexIdxList.back();
	unusedVertexIdxList.pop_back();
	noVerticesList[3*newVertexIdx] = newloc[0];
	noVerticesList[3*newVertexIdx+1] = newloc[1];
	noVerticesList[3*newVertexIdx+2] = newloc[2];
	noVertexNeighbourList[newVertexIdx].clear();
	noVertexEdgeList[newVertexIdx].clear();

	unsigned int numOneRingVertices = oneRingVertices.size();
	for (unsigned int i = 0; i < numOneRingVertices; ++i)
	{
		unsigned int newPolyIdx = unusedPolyIdxList.back();
		unusedPolyIdxList.pop_back();
		
		unsigned int vIdx1 = oneRingVertices[i];
		unsigned int vIdx2 = oneRingVertices[(i+1)%numOneRingVertices];

		// add new polygons to mesh
		noPolygonsList[newPolyIdx][0] = vIdx1;
		noPolygonsList[newPolyIdx][1] = vIdx2;
		noPolygonsList[newPolyIdx][2] = newVertexIdx;

		// update vertex-to-neighbour relationship
		noVertexNeighbourList[vIdx1].push_back(newPolyIdx);
		noVertexNeighbourList[vIdx2].push_back(newPolyIdx);
		noVertexNeighbourList[newVertexIdx].push_back(newPolyIdx);

		// add new edges
		unsigned int newEdgeIdx = unusedEdgeIdxList.back();
		unusedEdgeIdxList.pop_back();
		--numUnusedEdgesIntroduced;

		noEdgeList[newEdgeIdx].vertex1Idx = vIdx1;
		noEdgeList[newEdgeIdx].vertex2Idx = newVertexIdx;
		if (newVertexIdx < vIdx1)
		{
			noEdgeList[newEdgeIdx].vertex1Idx = newVertexIdx;
			noEdgeList[newEdgeIdx].vertex2Idx = vIdx1;
		}

		// update vertex-to-edge relationship
		noVertexEdgeList[vIdx1].push_back(newEdgeIdx);
		noVertexEdgeList[newVertexIdx].push_back(newEdgeIdx);

		// if boundary edge, we stop at 2nd last one-ring vertex and add the remaining edge
		if (isBoundaryEdge && i == numOneRingVertices-2)
		{
			// add the remaining boundary edges
			unsigned int newEdgeIdx = unusedEdgeIdxList.back();
			unusedEdgeIdxList.pop_back();
			--numUnusedEdgesIntroduced;
	
			noEdgeList[newEdgeIdx].vertex1Idx = vIdx2;
			noEdgeList[newEdgeIdx].vertex2Idx = newVertexIdx;
			if (newVertexIdx < vIdx2)
			{
				noEdgeList[newEdgeIdx].vertex1Idx = newVertexIdx;
				noEdgeList[newEdgeIdx].vertex2Idx = vIdx2;
			}
	
			// update vertex-to-edge relationship
			noVertexEdgeList[vIdx2].push_back(newEdgeIdx);
			noVertexEdgeList[newVertexIdx].push_back(newEdgeIdx);
			break;
		}
	}

	// updates normals
	updateMesh(newVertexIdx, newloc);

	newUnusedEdges.clear();
	for (unsigned int i = 1; i <= numUnusedEdgesIntroduced; ++i)
		newUnusedEdges.push_back(unusedEdgeIdxList[unusedEdgeIdxList.size()-i]);

	// reorder arrays
	removeUnused(unusedPolyIdxList, unusedVertexIdxList, newVertexIdx);

	return newVertexIdx;
}

// remove unused vertices and polygons in noVerticesList and noPolygonsList
// it actually moves the unused to the end of the arrays and update numNoVertices and numNoPolygons, array size stays the same
// unusedPolyIndices (in): vector of unused polygons indices
// unusedVertexIndices (in): vector of unused vertices indices
// vertexIdx (in/out): index of a vertex to keep track of; returns the new index for the vertex if any idx swapping has been done on it
void NonobtOptn::removeUnused(const	vector<int> &unusedPolyIndices, const vector<int> &unusedVertexIndices, unsigned int &vertexIdx)
{
	// ---------------------------------------------
	// --------- reorder vertex array --------------
	// ---------------------------------------------
	
	unsigned int numUnusedVertices = unusedVertexIndices.size();
	for (unsigned int i = 0; i < numUnusedVertices; ++i)
	{
		unsigned int vIdx = unusedVertexIndices[i];
		unsigned int lastVIdx = numNoVertices - 1;

		// make sure lastVIdx is a valid vertex
		for (unsigned int j = i; j < numUnusedVertices; ++j)
		{
			if (unusedVertexIndices[j] == (int)lastVIdx)
			{
				--lastVIdx;
				break;
			}
		}

		if (vIdx < lastVIdx)
		{
			// make sure we return vertexIdx with the new index if 'vertexIdx' was changed
			if (lastVIdx == vertexIdx)	
				vertexIdx = vIdx;
			else if (vIdx == vertexIdx)	
				vertexIdx = lastVIdx;

			// swap position with the last vertex
			swap(noVerticesList[3*vIdx], noVerticesList[3*lastVIdx]);
			swap(noVerticesList[3*vIdx+1], noVerticesList[3*lastVIdx+1]);
			swap(noVerticesList[3*vIdx+2], noVerticesList[3*lastVIdx+2]);
			// swap vertex normals
			swap(noVertexNormalsList[3*vIdx], noVertexNormalsList[3*lastVIdx]);
			swap(noVertexNormalsList[3*vIdx+1], noVertexNormalsList[3*lastVIdx+1]);
			swap(noVertexNormalsList[3*vIdx+2], noVertexNormalsList[3*lastVIdx+2]);
			// swap vertex-neighbour relationship
			swap(noVertexNeighbourList[vIdx], noVertexNeighbourList[lastVIdx]);
			// swap vertex-edge relationship
			swap(noVertexEdgeList[vIdx], noVertexEdgeList[lastVIdx]);
			// make sure we also swap the vertex decimate list
			if (!qVertexDecimateList.empty())
				swap(qVertexDecimateList[vIdx], qVertexDecimateList[lastVIdx]);
			if (!numPlanesInQuadricsList.empty())
				swap(numPlanesInQuadricsList[vIdx], numPlanesInQuadricsList[lastVIdx]);

			// swap other related info
			swap(closestFaceList[vIdx], closestFaceList[lastVIdx]);
			swap(closestBoundaryEdgeList[vIdx], closestBoundaryEdgeList[lastVIdx]);
			swap(closestPtList[3*vIdx], closestPtList[3*lastVIdx]);
			swap(closestPtList[3*vIdx+1], closestPtList[3*lastVIdx+1]);
			swap(closestPtList[3*vIdx+2], closestPtList[3*lastVIdx+2]);

			// update polyList
			unsigned int numNeighbours = noVertexNeighbourList[vIdx].size();
			for (unsigned int j = 0; j < numNeighbours; ++j)
			{
				unsigned int polyIdx = noVertexNeighbourList[vIdx][j];

				for (unsigned int k = 0; k < 3; ++k)
				{
					if (noPolygonsList[polyIdx][k] == (int)lastVIdx)
					{
						noPolygonsList[polyIdx][k] = vIdx;
						break;
					}
				}
			}
	
			// update edge list
			unsigned int numEdges = noVertexEdgeList[vIdx].size();
			for (unsigned int j = 0; j < numEdges; ++j)
			{
				unsigned int edgeIdx = noVertexEdgeList[vIdx][j];
				if (noEdgeList[edgeIdx].vertex1Idx == (int)lastVIdx)
					noEdgeList[edgeIdx].vertex1Idx = vIdx;
				else if (noEdgeList[edgeIdx].vertex2Idx == (int)lastVIdx)
					noEdgeList[edgeIdx].vertex2Idx = vIdx;
			}
		}
		// update numNoVertices
		--numNoVertices;
	}

	// ---------------------------------------------
	// --------- reorder polygon array --------------
	// ---------------------------------------------
	unsigned int numUnusedPolygons = unusedPolyIndices.size();
	for (unsigned int i = 0; i < numUnusedPolygons; ++i)
	{
		unsigned int polyIdx = unusedPolyIndices[i];
		unsigned int lastPolyIdx = numNoPolygons - 1;

		// make sure lastVIdx is a valid polygon
		for (unsigned int j = i; j < numUnusedPolygons; ++j)
		{
			if (unusedPolyIndices[j] == (int)lastPolyIdx)
			{
				--lastPolyIdx;
				break;
			}
		}
		
		if (polyIdx < lastPolyIdx)
		{
			// swap positions with last polygon
			swap(noPolygonsList[polyIdx][0], noPolygonsList[lastPolyIdx][0]);
			swap(noPolygonsList[polyIdx][1], noPolygonsList[lastPolyIdx][1]);
			swap(noPolygonsList[polyIdx][2], noPolygonsList[lastPolyIdx][2]);
	
			// swap face normals
			swap(noFaceNormalsList[3*polyIdx], noFaceNormalsList[3*lastPolyIdx]);
			swap(noFaceNormalsList[3*polyIdx+1], noFaceNormalsList[3*lastPolyIdx+1]);
			swap(noFaceNormalsList[3*polyIdx+2], noFaceNormalsList[3*lastPolyIdx+2]);
	
			// update vertex-neighbour relationship
			for (unsigned int j = 0; j < 3; ++j)
			{
				unsigned int vIdx = noPolygonsList[polyIdx][j];
				unsigned int numNeighbours = noVertexNeighbourList[vIdx].size();

				for (unsigned int k = 0; k < numNeighbours; ++k)
				{
					if (noVertexNeighbourList[vIdx][k] == (int)lastPolyIdx)
					{
						noVertexNeighbourList[vIdx][k] = polyIdx;
						break;
					}
				}
			}
		}
		// update numNoPolygons
		--numNoPolygons;
	}
}

// remove unused edges in noEdgeList
// unusedEdgeIdxList (in): vector of unused edge indices
void NonobtOptn::removeUnused(vector<unsigned int> &unusedEdgeIdxList)
{
	while (!unusedEdgeIdxList.empty())
	{
		// check if last edge is unused
		vector<unsigned int>::iterator iter = find(unusedEdgeIdxList.begin(), unusedEdgeIdxList.end(), noEdgeList.size()-1);
		while (iter != unusedEdgeIdxList.end()) {
			noEdgeList.pop_back();
			unusedEdgeIdxList.erase(iter);
			iter = find(unusedEdgeIdxList.begin(), unusedEdgeIdxList.end(), noEdgeList.size()-1);
		}

		if (unusedEdgeIdxList.empty())
			break;

		// swap unused index with the last in-use edge

		Edge e = noEdgeList.back();
		vector<int>::iterator iter1 = find(noVertexEdgeList[e.vertex1Idx].begin(), noVertexEdgeList[e.vertex1Idx].end(), (int)(noEdgeList.size()-1));
		if (iter1 == noVertexEdgeList[e.vertex1Idx].end()) {
			cout << "Warning: NonoptOptn::removeAndTriangulate() - cannot find vertex-to-edge reference for vertex " << e.vertex1Idx << endl;
		}
		else
			*iter1 = unusedEdgeIdxList.back();

		vector<int>::iterator iter2 = find(noVertexEdgeList[e.vertex2Idx].begin(), noVertexEdgeList[e.vertex2Idx].end(), (int)(noEdgeList.size()-1));
		if (iter2 == noVertexEdgeList[e.vertex2Idx].end()) {
			cout << "Warning: NonoptOptn::removeAndTriangulate() - cannot find vertex-to-edge reference for vertex " << e.vertex2Idx << endl;
		}
		else
			*iter2 = unusedEdgeIdxList.back();

		swap(noEdgeList[unusedEdgeIdxList.back()], noEdgeList.back());
		noEdgeList.pop_back();
		unusedEdgeIdxList.pop_back();
	}
}

// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
// return val (out): number of planes in constraints
// novIdx (in): vertex index (or edge vertex if bEdgeOneRing is set to true)
// constratins (out): array of plane equations
// region (in): determines how the non-obtuse region is to be approximated
// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
//                           on the one-ring vertices for the edge
// angleBound (in): angle-bound (in degrees)
unsigned int NonobtOptn::getNonobtuseRegion(int novIdx, PlaneEqn* &constraints, NORegionType region, bool bEdgeOneRing, double angleBound) const
{
	if (region == SIMPLE3PLANES)
	{
		if (bEdgeOneRing)
			return getNonobtuseRegionForEdge_Simple3Planes(novIdx, constraints);
		else
			return getNonobtuseRegion_Simple3Planes(novIdx, constraints);
	}
	else if (region == SIMPLE3PLANES_TOWARD_CENTER)
	{
		if (bEdgeOneRing)
			return getNonobtuseRegionForEdge_Simple3Planes_TowardCenter(novIdx, constraints);
		else
			return getNonobtuseRegion_Simple3Planes_TowardCenter(novIdx, constraints);
	}
	else if (region == ANGLEBOUND_7PLANES)
	{
		angleBound = angleBound*(M_PI/180.0);	// convert angleBound to radian
		if (bEdgeOneRing)
			return getNonobtuseRegionForEdge_AngleBound_7Planes(novIdx, constraints, angleBound);
		else
			return getNonobtuseRegion_AngleBound_7Planes(novIdx, constraints, angleBound);
	}
	
	return 0;
}

// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
// by way of SIMPLE3PLANES method
// return val (out): number of planes in constraints
// novIdx (in): vertex index
// constratins (out): array of plane equations
unsigned int NonobtOptn::getNonobtuseRegion_Simple3Planes(int novIdx, PlaneEqn* &constraints) const
{
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	
	// Approximate non-obtuse region with 3 planes per edge
	// 2 for the side, 1 for facing toward center and perpendicular to avg. plane
	if (constraints != NULL)	// sanity checks
		delete[] constraints;
	
	unsigned int numConstraints = 3*numNbrFaces;
	//unsigned int numConstraints = 5*numNbrFaces;
	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);
	
	// process each edge in one-ring
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		// find edge in one-ring from the neighbour face
		int polyIdx = noVertexNeighbourList[novIdx][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
				e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
				break;
			}
		}
		
		// --- side plane 1 ---
		// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
		// d = -ax_i-by_i-cz_i
		double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		double offsetPct = 0.05;
		double offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
								noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
								noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
		
		// --- side plane 2 ---
		// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
		// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
		planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
		planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
		planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
		offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
		ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
		ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
		
		// --- plane facing toward one-ring ---
		// v_{i+1} - v_i
		double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		// edgeVector x vertexNormal
		double normal[3];
		crossProduct(normal[0], normal[1], normal[2],
					noVertexNormalsList[3*novIdx], noVertexNormalsList[3*novIdx+1], noVertexNormalsList[3*novIdx+2],
					edgeVector[0], edgeVector[1], edgeVector[2]);
		constraints[currentConstraint].a = normal[0];
		constraints[currentConstraint].b = normal[1];
		constraints[currentConstraint].c = normal[2];
		normalize(normal);
		
		double vm[3];	// midpoint of v_i and v_{i+1}
		midpoint(vm,
				noVerticesList[3*(e.vertex1Idx)], 
				noVerticesList[3*(e.vertex1Idx)+1], 
				noVerticesList[3*(e.vertex1Idx)+2],
				noVerticesList[3*(e.vertex2Idx)], 
				noVerticesList[3*(e.vertex2Idx)+1], 
				noVerticesList[3*(e.vertex2Idx)+2]);
		
		//double alpha = length(edgeVector) / 2.0;
		offset = offsetPct * length(edgeVector);
		double alpha = (length(edgeVector) / 2.0) + offset;
		
		// point on plane facing toward one-ring = vm + alpha*normal
		double pt[3] = {vm[0] + alpha*normal[0], vm[1] + alpha*normal[1], vm[2] + alpha*normal[2]};
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 pt[0],
															 pt[1],
															 pt[2]);
		++currentConstraint;
	}
	
	return numConstraints;
}

// computes the set of planes that approximates the nonobtuse region of the one-ring of edgeIdx
// by way of SIMPLE3PLANES method
// return val (out): number of planes in constraints
// edgeIdx (in): edge index
// constratins (out): array of plane equations
unsigned int NonobtOptn::getNonobtuseRegionForEdge_Simple3Planes(int edgeIdx, PlaneEqn* &constraints) const
{
	// Approximate non-obtuse region with 3 planes per edge
	// 2 for the side, 1 for facing toward center and perpendicular to avg. plane
	if (constraints != NULL)	// sanity checks
		delete[] constraints;

	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;
	
	// make sure we have enough room to store the number of constraints; not necessary the number of neighbour faces for edge
	unsigned int numNbrFaces = noVertexNeighbourList[vIdx1].size() + noVertexNeighbourList[vIdx2].size();

	// 3 planes per edge
	unsigned int numConstraints = 3*numNbrFaces;
	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);
	
	// process each edge in one-ring
	for (unsigned int k = 0; k < 2; ++k)
	{
		int currentVIdx = vIdx1;
		int otherVIdx = vIdx2;
		if (k == 1)
		{
			currentVIdx = vIdx2;
			otherVIdx = vIdx1;
		}

		unsigned int numNeighbours = noVertexNeighbourList[currentVIdx].size();
		for (unsigned int i = 0; i < numNeighbours; ++i)
		{
			// find edge in one-ring from the neighbour face
			int polyIdx = noVertexNeighbourList[currentVIdx][i];
			Edge e;
			e.vertex1Idx = 0;
			e.vertex2Idx = 0;
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (noPolygonsList[polyIdx][j] == currentVIdx)
				{
					e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
					e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
					break;
				}
			}

			// skip triangles that has vIdx1 and vIdx2 as vertices
			if (e.vertex1Idx == otherVIdx || e.vertex2Idx == otherVIdx)
				continue;
			
			// --- side plane 1 ---
			// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
			// d = -ax_i-by_i-cz_i
			double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double offsetPct = 0.0;
			double offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
									noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
									noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- side plane 2 ---
			// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
			// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
			planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
			planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
			planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
			offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
			ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
			ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- plane facing toward one-ring ---
			// v_{i+1} - v_i
			double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double oneRingAvgNormal[3];
			computeOneRingAvgNormalForEdge(edgeIdx, oneRingAvgNormal);
			
			// edgeVector x avgNormal
			double normal[3];	
			crossProduct(normal[0], normal[1], normal[2],
						oneRingAvgNormal[0], oneRingAvgNormal[1], oneRingAvgNormal[2],
						edgeVector[0], edgeVector[1], edgeVector[2]);
			constraints[currentConstraint].a = normal[0];
			constraints[currentConstraint].b = normal[1];
			constraints[currentConstraint].c = normal[2];
			normalize(normal);
			
			double vm[3];	// midpoint of v_i and v_{i+1}
			midpoint(vm,
					noVerticesList[3*(e.vertex1Idx)], 
					noVerticesList[3*(e.vertex1Idx)+1], 
					noVerticesList[3*(e.vertex1Idx)+2],
					noVerticesList[3*(e.vertex2Idx)], 
					noVerticesList[3*(e.vertex2Idx)+1], 
					noVerticesList[3*(e.vertex2Idx)+2]);
			
			//double alpha = length(edgeVector) / 2.0;
			offset = offsetPct * length(edgeVector);
			double alpha = (length(edgeVector) / 2.0) + offset;
			
			// point on plane facing toward one-ring = vm + alpha*normal
			double pt[3] = {vm[0] + alpha*normal[0], vm[1] + alpha*normal[1], vm[2] + alpha*normal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																pt[0],
																pt[1],
																pt[2]);
			++currentConstraint;
		}
	}

	return currentConstraint;
}

// computes the set of planes that approximates the nonobtuse region of the one-ring of novIdx
// by way of SIMPLE3PLANES_TOWARD_CENTER method
// return val (out): number of planes in constraints
// novIdx (in): vertex index
// constratins (out): array of plane equations
unsigned int NonobtOptn::getNonobtuseRegion_Simple3Planes_TowardCenter(int novIdx, PlaneEqn* &constraints) const
{
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	
	// Approximate non-obtuse region with 3 planes per edge
	// 2 for the side, 1 for facing toward the vertex inside one-ring
	if (constraints != NULL)	// sanity checks
		delete[] constraints;
	
	unsigned int numConstraints = 3*numNbrFaces;
	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);
	
	// process each edge in one-ring
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		// find edge in one-ring from the neighbour face
		int polyIdx = noVertexNeighbourList[novIdx][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
				e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
				break;
			}
		}
		
		// --- side plane 1 ---
		// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
		// d = -ax_i-by_i-cz_i
		double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		double offsetPct = 0.0;
		double offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
								noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
								noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
		
		// --- side plane 2 ---
		// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
		// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
		planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
		planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
		planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
		offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
		ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
		ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
															 
		++currentConstraint;
		
		// --- plane facing toward one-ring ---
		
		// v_{i+1} - v_i
		double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		double vm[3];	// midpoint of v_i and v_{i+1}
		midpoint(vm,
				noVerticesList[3*(e.vertex1Idx)], 
				noVerticesList[3*(e.vertex1Idx)+1], 
				noVerticesList[3*(e.vertex1Idx)+2],
				noVerticesList[3*(e.vertex2Idx)], 
				noVerticesList[3*(e.vertex2Idx)+1], 
				noVerticesList[3*(e.vertex2Idx)+2]);

		// vm to novIdx
		double normal[3] = {noVerticesList[3*novIdx] - vm[0],
							noVerticesList[3*novIdx+1] - vm[1],
							noVerticesList[3*novIdx+2] - vm[2]};

		constraints[currentConstraint].a = normal[0];
		constraints[currentConstraint].b = normal[1];
		constraints[currentConstraint].c = normal[2];
		normalize(normal);
		offset = offsetPct * length(edgeVector);
		double alpha = 0.5*length(edgeVector) + offset;

		// point on plane facing toward one-ring = vm + alpha*normal
		double pt[3] = {vm[0] + alpha*normal[0], vm[1] + alpha*normal[1], vm[2] + alpha*normal[2]};
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 pt[0],
															 pt[1],
															 pt[2]);
		++currentConstraint;
	}
	
	return numConstraints;
}

// computes the set of planes that approximates the nonobtuse region of the one-ring of edgeIdx
// by way of SIMPLE3PLANES_TOWARD_CENTER method
// return val (out): number of planes in constraints
// edgeIdx (in): edge index
// constratins (out): array of plane equations
unsigned int NonobtOptn::getNonobtuseRegionForEdge_Simple3Planes_TowardCenter(int edgeIdx, PlaneEqn* &constraints) const
{
	// Approximate non-obtuse region with 3 planes per edge
	// 2 for the side, 1 for facing toward the vertex inside the one-ring
	if (constraints != NULL)	// sanity checks
		delete[] constraints;

	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;
	// make sure we have enough room to store the number of constraints; not necessary the number of neighbour faces for edge
	unsigned int numNbrFaces = noVertexNeighbourList[vIdx1].size() + noVertexNeighbourList[vIdx2].size();

	// 3 planes per edge
	unsigned int numConstraints = 3*numNbrFaces;
	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);
	
	// process each edge in one-ring
	for (unsigned int k = 0; k < 2; ++k)
	{
		int currentVIdx = vIdx1;
		int otherVIdx = vIdx2;
		if (k == 1)
		{
			currentVIdx = vIdx2;
			otherVIdx = vIdx1;
		}

		unsigned int numNeighbours = noVertexNeighbourList[currentVIdx].size();
		for (unsigned int i = 0; i < numNeighbours; ++i)
		{
			// find edge in one-ring from the neighbour face
			int polyIdx = noVertexNeighbourList[currentVIdx][i];
			Edge e;
			e.vertex1Idx = 0;
			e.vertex2Idx = 0;
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (noPolygonsList[polyIdx][j] == currentVIdx)
				{
					e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
					e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
					break;
				}
			}

			// skip triangles that has vIdx1 and vIdx2 as vertices
			if (e.vertex1Idx == otherVIdx || e.vertex2Idx == otherVIdx)
				continue;
			
			// --- side plane 1 ---
			// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
			// d = -ax_i-by_i-cz_i
			double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double offsetPct = 0.0;
			double offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
									noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
									noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- side plane 2 ---
			// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
			// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
			planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
			planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
			planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
			offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
			ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
			ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- plane facing toward one-ring ---

			// v_{i+1} - v_i
			double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double vm[3];	// midpoint of v_i and v_{i+1}
			midpoint(vm,
					noVerticesList[3*(e.vertex1Idx)], 
					noVerticesList[3*(e.vertex1Idx)+1], 
					noVerticesList[3*(e.vertex1Idx)+2],
					noVerticesList[3*(e.vertex2Idx)], 
					noVerticesList[3*(e.vertex2Idx)+1], 
					noVerticesList[3*(e.vertex2Idx)+2]);

			double midpointOfEdgeIdx[3];
			midpoint(midpointOfEdgeIdx,
					noVerticesList[3*vIdx1], 
					noVerticesList[3*vIdx1+1], 
					noVerticesList[3*vIdx1+2],
					noVerticesList[3*vIdx2], 
					noVerticesList[3*vIdx2+1], 
					noVerticesList[3*vIdx2+2]);
	
			// vm to midpoint of edgeIdx
			double normal[3] = {midpointOfEdgeIdx[0] - vm[0],
								midpointOfEdgeIdx[1] - vm[1],
								midpointOfEdgeIdx[2] - vm[2]};
			constraints[currentConstraint].a = normal[0];
			constraints[currentConstraint].b = normal[1];
			constraints[currentConstraint].c = normal[2];
			normalize(normal);
			offset = offsetPct * length(edgeVector);
			double alpha = 0.5*length(edgeVector) + offset;
	
			// point on plane facing toward one-ring = vm + alpha*normal
			double pt[3] = {vm[0] + alpha*normal[0], vm[1] + alpha*normal[1], vm[2] + alpha*normal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																pt[0],
																pt[1],
																pt[2]);
			++currentConstraint;
		}
	}

	return currentConstraint;
}

// computes the set of planes that approximates the angle-bounded nonobtuse region of the one-ring of novIdx
// by way of ANGLEBOUND_7PLANES method
// return val (out): number of planes in constraints
// novIdx (in): vertex index
// constratins (out): array of plane equations
// angleBound (in): angle-bound (in radians)
unsigned int NonobtOptn::getNonobtuseRegion_AngleBound_7Planes(int novIdx, PlaneEqn* &constraints, double angleBound) const
{
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();

	if (constraints != NULL)	// sanity checks
		delete[] constraints;

	// Approximate angle-bounded non-obtuse region with n planes per edge
	unsigned int numDivision = 1;
	unsigned int numConstraints = (4*(numDivision+1)+5)*numNbrFaces;

	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);

	// process each edge in one-ring
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		// find edge in one-ring from the neighbour face
		int polyIdx = noVertexNeighbourList[novIdx][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
				e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
				break;
			}
		}
		
		// --- side plane 1 ---
		// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
		// d = -ax_i-by_i-cz_i
		double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		double offsetPct = 0.0;
		double offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
								noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
								noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
		
		// --- side plane 2 ---
		// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
		// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
		planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
		planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
		planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
		offset = offsetPct * length(planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);
		ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
		ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
		ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
															 
		++currentConstraint;
		
		// --- plane facing toward one-ring ---
		
		// v_{i+1} - v_i
		double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
								noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
								noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		double edgeLength = length(edgeVector);
		double vm[3];	// midpoint of v_i and v_{i+1}
		midpoint(vm,
				noVerticesList[3*(e.vertex1Idx)], 
				noVerticesList[3*(e.vertex1Idx)+1], 
				noVerticesList[3*(e.vertex1Idx)+2],
				noVerticesList[3*(e.vertex2Idx)], 
				noVerticesList[3*(e.vertex2Idx)+1], 
				noVerticesList[3*(e.vertex2Idx)+2]);

		// vm to novIdx
		planeNormal[0] = noVerticesList[3*novIdx] - vm[0];
		planeNormal[1] = noVerticesList[3*novIdx+1] - vm[1];
		planeNormal[2] = noVerticesList[3*novIdx+2] - vm[2];
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		double normalR[3] = {planeNormal[0], planeNormal[1], planeNormal[2]};

		normalize(planeNormal);
		offset = offsetPct * edgeLength;
		double alpha = 0.5*edgeLength + offset;
 
 		// point on plane facing toward one-ring = vm + alpha*normal
 		ptOnPlane[0] = vm[0] + alpha*planeNormal[0];
		ptOnPlane[1] = vm[1] + alpha*planeNormal[1];
		ptOnPlane[2] = vm[2] + alpha*planeNormal[2];
		double ptr[3] = {ptOnPlane[0], ptOnPlane[1], ptOnPlane[2]};

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
		
		

		// ------ plane T (angle bound at v_i) ------
		normalize(edgeVector);	// normalize the edgeVector for easier computation

		// find vector pointing to "angleBound" direction; ie. vi_to_x
		double viplus1_to_v[3] = {noVerticesList[3*novIdx] - noVerticesList[3*(e.vertex2Idx)],
								noVerticesList[3*novIdx+1] - noVerticesList[3*(e.vertex2Idx)+1],
								noVerticesList[3*novIdx+2] - noVerticesList[3*(e.vertex2Idx)+2]};
		normalize(viplus1_to_v);
		double revEdgeVector[3] = {-1.0*edgeVector[0], -1.0*edgeVector[1], -1.0*edgeVector[2]};
		normalize(revEdgeVector);

		// use sine law to find the distance from v_{i+1} to x
		double beta = M_PI - angleBound - acos(dotProduct(viplus1_to_v, revEdgeVector));
		double dist_viplus1_to_x = (edgeLength/sin(beta)) * sin(angleBound);

		// vi_to_x = edge + viplus1_to_v
		double vi_to_x[3] = {edgeLength*edgeVector[0] + dist_viplus1_to_x*viplus1_to_v[0],
							edgeLength*edgeVector[1] + dist_viplus1_to_x*viplus1_to_v[1],
							edgeLength*edgeVector[2] + dist_viplus1_to_x*viplus1_to_v[2]};
		normalize(vi_to_x);

		// get normal of plane orthogonal to ie. plane Y
		double normalY[3] = {0,0,0};
		crossProduct(normalY, edgeVector, vi_to_x);

		// normal of plane T
		crossProduct(planeNormal, normalY, vi_to_x);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		ptOnPlane[0] = noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0];
		ptOnPlane[1] = noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1];
		ptOnPlane[2] = noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2];
		
		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;

		// ------ plane S (angle bound at v_i+1) ------
		// find vector pointing to "angleBound" direction; ie. viplus1_to_x
		double vi_to_v[3] = {noVerticesList[3*novIdx] - noVerticesList[3*(e.vertex1Idx)],
							noVerticesList[3*novIdx+1] - noVerticesList[3*(e.vertex1Idx)+1],
							noVerticesList[3*novIdx+2] - noVerticesList[3*(e.vertex1Idx)+2]};
		normalize(vi_to_v);

		// use sine law to find the distance from v_{i+1} to x
		beta = M_PI - angleBound - acos(dotProduct(vi_to_v, edgeVector));
		double dist_vi_to_x = (edgeLength/sin(beta)) * sin(angleBound);

		// viplus1_to_x = vi_to_v - edge
		double viplus1_to_x[3] = {-1.0*edgeLength*edgeVector[0] + dist_vi_to_x*vi_to_v[0],
								-1.0*edgeLength*edgeVector[1] + dist_vi_to_x*vi_to_v[1],
								-1.0*edgeLength*edgeVector[2] + dist_vi_to_x*vi_to_v[2]};
		normalize(viplus1_to_x);

		// normal of plane S
		crossProduct(planeNormal, viplus1_to_x, normalY);
		
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
		ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
		ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;

		double pt[3] = {noVerticesList[3*novIdx], noVerticesList[3*novIdx+1], noVerticesList[3*novIdx+2]};
		getPlanesMN(pt, e, angleBound, normalY, normalR, ptr, numDivision, offset, constraints, currentConstraint);

	}

	return numConstraints;
}

// computes the set of planes that approximates the angle-bounded nonobtuse region of the one-ring of edgeIdx
// by way of ANGLEBOUND_7PLANES method
// return val (out): number of planes in constraints
// edgeIdx (in): edge index
// constratins (out): array of plane equations
// angleBound (in): angle-bound (in radians)
unsigned int NonobtOptn::getNonobtuseRegionForEdge_AngleBound_7Planes(int edgeIdx, PlaneEqn* &constraints, double angleBound) const
{
	// Approximate non-obtuse region with 7 planes per edge
	if (constraints != NULL)	// sanity checks
		delete[] constraints;

	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;

	// make sure we have enough room to store the number of constraints; not necessary the number of neighbour faces for edge
	unsigned int numNbrFaces = noVertexNeighbourList[vIdx1].size() + noVertexNeighbourList[vIdx2].size();
	
	// Approximate angle-bounded non-obtuse region with n planes per edge
	unsigned int numDivision = 1;
	unsigned int numConstraints = (4*(numDivision+1)+5)*numNbrFaces;
	constraints = new PlaneEqn[numConstraints];
	unsigned int currentConstraint(0);
	
	// process each edge in one-ring
	for (unsigned int k = 0; k < 2; ++k)
	{
		int currentVIdx = vIdx1;
		int otherVIdx = vIdx2;
		if (k == 1)
		{
			currentVIdx = vIdx2;
			otherVIdx = vIdx1;
		}

		unsigned int numNeighbours = noVertexNeighbourList[currentVIdx].size();
		for (unsigned int i = 0; i < numNeighbours; ++i)
		{
			// find edge in one-ring from the neighbour face
			int polyIdx = noVertexNeighbourList[currentVIdx][i];
			Edge e;
			e.vertex1Idx = 0;
			e.vertex2Idx = 0;
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (noPolygonsList[polyIdx][j] == currentVIdx)
				{
					e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
					e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
					break;
				}
			}

			// skip triangles that has vIdx1 and vIdx2 as vertices
			if (e.vertex1Idx == otherVIdx || e.vertex2Idx == otherVIdx)
				continue;
			
			// --- side plane 1 ---
			// n = (a, b, c) = (x_{i+1} -í°€x_i, y_{i+1} - y_i, z_{i+1} - z_i)
			// d = -ax_i-by_i-cz_i
			double planeNormal[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double offsetPct = 0.0;
			double offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			double ptOnPlane[3] = {noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0],
									noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1],
									noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- side plane 2 ---
			// n = (a, b, c) = (x_i - x_{i+1}, y_i - y_{i+1}, z_i - z_{i+1})
			// d = -ax_{i+1}-by_{i+1}-cz_{i+1}
			planeNormal[0] = noVerticesList[3*(e.vertex1Idx)] - noVerticesList[3*(e.vertex2Idx)];
			planeNormal[1] = noVerticesList[3*(e.vertex1Idx)+1] - noVerticesList[3*(e.vertex2Idx)+1];
			planeNormal[2] = noVerticesList[3*(e.vertex1Idx)+2] - noVerticesList[3*(e.vertex2Idx)+2];
			offset = offsetPct * length(planeNormal);
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
			ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
			ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptOnPlane[0],
																ptOnPlane[1],
																ptOnPlane[2]);
			++currentConstraint;
			
			// --- plane facing toward one-ring ---

			// v_{i+1} - v_i
			double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)],
									noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
									noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
			double edgeLength = length(edgeVector);
			double vm[3];	// midpoint of v_i and v_{i+1}
			midpoint(vm,
					noVerticesList[3*(e.vertex1Idx)], 
					noVerticesList[3*(e.vertex1Idx)+1], 
					noVerticesList[3*(e.vertex1Idx)+2],
					noVerticesList[3*(e.vertex2Idx)], 
					noVerticesList[3*(e.vertex2Idx)+1], 
					noVerticesList[3*(e.vertex2Idx)+2]);

			double midpointOfEdgeIdx[3];
			midpoint(midpointOfEdgeIdx,
					noVerticesList[3*vIdx1], 
					noVerticesList[3*vIdx1+1], 
					noVerticesList[3*vIdx1+2],
					noVerticesList[3*vIdx2], 
					noVerticesList[3*vIdx2+1], 
					noVerticesList[3*vIdx2+2]);
	
			// vm to midpoint of edgeIdx
			double normalR[3] = {midpointOfEdgeIdx[0] - vm[0],
								midpointOfEdgeIdx[1] - vm[1],
								midpointOfEdgeIdx[2] - vm[2]};
			planeNormal[0] = normalR[0];
			planeNormal[1] = normalR[1];
			planeNormal[2] = normalR[2];
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			normalize(planeNormal);
			offset = offsetPct * edgeLength;
			double alpha = 0.5*edgeLength + offset;
	
			// point on plane facing toward one-ring = vm + alpha*normal
			double ptr[3] = {vm[0] + alpha*planeNormal[0], vm[1] + alpha*planeNormal[1], vm[2] + alpha*planeNormal[2]};
			constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
																constraints[currentConstraint].b, 
																constraints[currentConstraint].c, 
																ptr[0],
																ptr[1],
																ptr[2]);
			++currentConstraint;

			// ------ plane T (angle bound at v_i) ------
			normalize(edgeVector);	// normalize the edgeVector for easier computation

			// find vector pointing to "angleBound" direction; ie. vi_to_x
			double viplus1_to_v[3] = {midpointOfEdgeIdx[0] - noVerticesList[3*(e.vertex2Idx)],
									midpointOfEdgeIdx[1] - noVerticesList[3*(e.vertex2Idx)+1],
									midpointOfEdgeIdx[2] - noVerticesList[3*(e.vertex2Idx)+2]};
			normalize(viplus1_to_v);
			double revEdgeVector[3] = {-1.0*edgeVector[0], -1.0*edgeVector[1], -1.0*edgeVector[2]};
			normalize(revEdgeVector);
	
			// use sine law to find the distance from v_{i+1} to x
			double beta = M_PI - angleBound - acos(dotProduct(viplus1_to_v, revEdgeVector));
			double dist_viplus1_to_x = (edgeLength/sin(beta)) * sin(angleBound);
	
			// vi_to_x = edge + viplus1_to_v
			double vi_to_x[3] = {edgeLength*edgeVector[0] + dist_viplus1_to_x*viplus1_to_v[0],
								edgeLength*edgeVector[1] + dist_viplus1_to_x*viplus1_to_v[1],
								edgeLength*edgeVector[2] + dist_viplus1_to_x*viplus1_to_v[2]};
			normalize(vi_to_x);
	
			// get normal of plane orthogonal to ie. plane Y
			double normalY[3] = {0,0,0};
			crossProduct(normalY, edgeVector, vi_to_x);
	
			// normal of plane T
			crossProduct(planeNormal, normalY, vi_to_x);
			normalize(planeNormal);
	
			ptOnPlane[0] = noVerticesList[3*(e.vertex1Idx)] + offset*planeNormal[0];
			ptOnPlane[1] = noVerticesList[3*(e.vertex1Idx)+1] + offset*planeNormal[1];
			ptOnPlane[2] = noVerticesList[3*(e.vertex1Idx)+2] + offset*planeNormal[2];
			
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			constraints[currentConstraint].d = -1.0 * dotProduct(planeNormal, ptOnPlane);
			++currentConstraint;
	
			// ------ plane S (angle bound at v_i+1) ------
			// find vector pointing to "angleBound" direction; ie. viplus1_to_x
			double vi_to_v[3] = {midpointOfEdgeIdx[0] - noVerticesList[3*(e.vertex1Idx)],
								midpointOfEdgeIdx[1] - noVerticesList[3*(e.vertex1Idx)+1],
								midpointOfEdgeIdx[2] - noVerticesList[3*(e.vertex1Idx)+2]};
			normalize(vi_to_v);
	
			// use sine law to find the distance from v_{i+1} to x
			beta = M_PI - angleBound - acos(dotProduct(vi_to_v, edgeVector));
			double dist_vi_to_x = (edgeLength/sin(beta)) * sin(angleBound);
	
			// viplus1_to_x = vi_to_v - edge
			double viplus1_to_x[3] = {-1.0*edgeLength*edgeVector[0] + dist_vi_to_x*vi_to_v[0],
									-1.0*edgeLength*edgeVector[1] + dist_vi_to_x*vi_to_v[1],
									-1.0*edgeLength*edgeVector[2] + dist_vi_to_x*vi_to_v[2]};
			normalize(viplus1_to_x);
	
			// normal of plane S
			crossProduct(planeNormal, viplus1_to_x, normalY);
			normalize(planeNormal);
	
			ptOnPlane[0] = noVerticesList[3*(e.vertex2Idx)] + offset*planeNormal[0];
			ptOnPlane[1] = noVerticesList[3*(e.vertex2Idx)+1] + offset*planeNormal[1];
			ptOnPlane[2] = noVerticesList[3*(e.vertex2Idx)+2] + offset*planeNormal[2];
			
			constraints[currentConstraint].a = planeNormal[0];
			constraints[currentConstraint].b = planeNormal[1];
			constraints[currentConstraint].c = planeNormal[2];
			constraints[currentConstraint].d = -1.0 * dotProduct(planeNormal, ptOnPlane);
			++currentConstraint;

			getPlanesMN(midpointOfEdgeIdx, e, angleBound, normalY, normalR, ptr, numDivision, offset, constraints, currentConstraint);

		}
	}

	return currentConstraint;
}

// construct all constraints for setting up the planes M_i and N_i; for angle bound region
//
// inRingPos (in): position of the in-ring vertex y
// e (in): the edge where planes M_i and N_i are constructing for
// angleBound (in): angle bound requirement for the orientation of planes M_i and N_i (in radians)
// normalY (in): normal for plane Y
// normalR (in): normal for plane R
// ptr (in): a pt in plane R
// numDivision (in): number of division used to construct the planes M_i and N_i
// offset (in): offsets for the constructed planes along the normal
// constraints (in/out): array of plane equations
// currentConstraint (in/out): indices of the first available slot to store the constraints
// return value (out): true - if all planes constructed successfully
bool NonobtOptn::getPlanesMN(double inRingPos[3], Edge e, double angleBound, double normalY[3], double normalR[3], double ptr[3], unsigned int numDivision, double offset, PlaneEqn* &constraints, unsigned int &currentConstraint) const
{
	double vi[3] = {noVerticesList[3*(e.vertex1Idx)], noVerticesList[3*(e.vertex1Idx)+1], noVerticesList[3*(e.vertex1Idx)+2]};
	double viplus1[3] = {noVerticesList[3*(e.vertex2Idx)], noVerticesList[3*(e.vertex2Idx)+1], noVerticesList[3*(e.vertex2Idx)+2]};
	double vm[3];	// midpoint of v_i and v_{i+1}
	midpoint(vm, noVerticesList[3*(e.vertex1Idx)], noVerticesList[3*(e.vertex1Idx)+1], noVerticesList[3*(e.vertex1Idx)+2], noVerticesList[3*(e.vertex2Idx)], noVerticesList[3*(e.vertex2Idx)+1], noVerticesList[3*(e.vertex2Idx)+2]);
	double edgeVector[3] = {noVerticesList[3*(e.vertex2Idx)] - noVerticesList[3*(e.vertex1Idx)], 
							noVerticesList[3*(e.vertex2Idx)+1] - noVerticesList[3*(e.vertex1Idx)+1],
							noVerticesList[3*(e.vertex2Idx)+2] - noVerticesList[3*(e.vertex1Idx)+2]};
	double edgeLength = length(edgeVector);
	normalize(edgeVector);
	double revEdgeVector[3] = {-1*edgeVector[0], -1*edgeVector[1], -1*edgeVector[2]};

	// --------------------------------------------------------------------------
	// --------------------------- Compute w0, m0, n0 ---------------------------
	// --------------------------------------------------------------------------

	double viplus1_to_v[3] = {inRingPos[0] - viplus1[0], inRingPos[1] - viplus1[1], inRingPos[2] - viplus1[2]};
	normalize(viplus1_to_v);

	// find w0 first; the point at which it creates a triangle with edge has angle = 'anglebound' and orthogonal to normalY
	double beta = M_PI_2 + angleBound/2.0 - acos(dotProduct(viplus1_to_v, revEdgeVector));
	double dist_viplus1_to_x = (edgeLength/sin(beta)) * sin(M_PI_2 - (angleBound/2.0));
	double vi_to_w0[3] = {edgeLength * edgeVector[0] + dist_viplus1_to_x * viplus1_to_v[0],
 							edgeLength * edgeVector[1] + dist_viplus1_to_x * viplus1_to_v[1],
 							edgeLength * edgeVector[2] + dist_viplus1_to_x * viplus1_to_v[2]};
	normalize(vi_to_w0);

	double dist_vi_to_w0 = (edgeLength/2.0) / sin(angleBound/2.0);
	vi_to_w0[0] = dist_vi_to_w0 * vi_to_w0[0];
	vi_to_w0[1] = dist_vi_to_w0 * vi_to_w0[1];
	vi_to_w0[2] = dist_vi_to_w0 * vi_to_w0[2];
	double w0[3] = {vi[0] + vi_to_w0[0], vi[1] + vi_to_w0[1], vi[2] + vi_to_w0[2]};

	// find m0; the point at which plane U (ie. 1st nonobtuse constraint) intersects with circumcircle orthogonal to normalY
	double dist_vi_to_m0 = edgeLength / tan(angleBound);
	double vi_to_m0[3] = {0,0,0};
	crossProduct(vi_to_m0, normalY, edgeVector);
	normalize(vi_to_m0);
	vi_to_m0[0] = dist_vi_to_m0 * vi_to_m0[0];
	vi_to_m0[1] = dist_vi_to_m0 * vi_to_m0[1];
	vi_to_m0[2] = dist_vi_to_m0 * vi_to_m0[2];
	double m0[3] = {vi[0] + vi_to_m0[0], vi[1] + vi_to_m0[1], vi[2] + vi_to_m0[2]};

	// find n0; the point at which plane V (ie. 2nd nonobtuse constraint) intersects with circumcircle orthogonal to normalY
	double n0[3] = {viplus1[0] + vi_to_m0[0], viplus1[1] + vi_to_m0[1], viplus1[2] + vi_to_m0[2]};

	// --------------------------------------------------------------------------
	// ----------- Compute r_plus_u, r_minus_u, r_plus_v, r_minus_v -------------
	// --------------------------------------------------------------------------

	double r0u[3] = {0,0,0};
	if (line_plane_intersection(r0u, normalR, ptr, vi, m0) != 1) {
		cout << "Error: NonobtOptn::getPlanesMN() - cannot intersect vi_to_m0 with plane R" << endl;
		return false;
	}
	
	double vi_to_r0u[3] = {r0u[0] - vi[0], r0u[1] - vi[1], r0u[2] - vi[2]};
	double dist_r0u_to_rplusu = sqrt(dotProduct(vi_to_m0, vi_to_m0) - dotProduct(vi_to_r0u, vi_to_r0u));
	normalize(normalY);
	double r_plus_u[3] = {r0u[0] - dist_r0u_to_rplusu * normalY[0], r0u[1] - dist_r0u_to_rplusu * normalY[1], r0u[2] - dist_r0u_to_rplusu * normalY[2]};
	double r_minus_u[3] = {r0u[0] + dist_r0u_to_rplusu * normalY[0], r0u[1] + dist_r0u_to_rplusu * normalY[1], r0u[2] + dist_r0u_to_rplusu * normalY[2]};

	double r0v[3] = {0,0,0};
	if (line_plane_intersection(r0v, normalR, ptr, viplus1, n0) != 1) {
		cout << "Error: NonobtOptn::getPlanesMN() - cannot intersect viplus1_to_n0 with plane R" << endl;
		return false;
	}

	double viplus1_to_n0[3] = {n0[0] - viplus1[0], n0[1] - viplus1[1], n0[2] - viplus1[2]};
	double viplus1_to_r0v[3] = {r0v[0] - viplus1[0], r0v[1] - viplus1[1], r0v[2] - viplus1[2]};
	double dist_r0v_to_rplusv = sqrt(dotProduct(viplus1_to_n0, viplus1_to_n0) - dotProduct(viplus1_to_r0v, viplus1_to_r0v));
	double r_plus_v[3] = {r0v[0] - dist_r0v_to_rplusv * normalY[0], r0v[1] - dist_r0v_to_rplusv * normalY[1], r0v[2] - dist_r0v_to_rplusv * normalY[2]};
	double r_minus_v[3] = {r0v[0] + dist_r0v_to_rplusv * normalY[0], r0v[1] + dist_r0v_to_rplusv * normalY[1], r0v[2] + dist_r0v_to_rplusv * normalY[2]};

	// --------------------------------------------------------------------------
	// ----------- Compute m_plus_k, m_minus_k, n_plus_k, n_minus_k -------------
	// --------------------------------------------------------------------------

	double m0_to_rplusu[3] = {r_plus_u[0] - m0[0], r_plus_u[1] - m0[1], r_plus_u[2] - m0[2]};
	double n0_to_rplusv[3] = {r_plus_v[0] - n0[0], r_plus_v[1] - n0[1], r_plus_v[2] - n0[2]};
	double m_plus_k[3] = {0,0,0};
	double m_minus_k[3] = {0,0,0};
	double n_plus_k[3] = {0,0,0};
	double n_minus_k[3] = {0,0,0};
	if (dotProduct(m0_to_rplusu, m0_to_rplusu) > dotProduct(n0_to_rplusv, n0_to_rplusv))
	{
		// m_plus_k := r_plus_u, m_minus_k := r_minus_u
		for (unsigned int i = 0; i < 3; ++i) {
			m_plus_k[i] = r_plus_u[i];
			m_minus_k[i] = r_minus_u[i];
			n_plus_k[i] = m_plus_k[i] + edgeLength * edgeVector[i];
			n_minus_k[i] = m_minus_k[i] + edgeLength * edgeVector[i];
		}
	}
	else
	{
		// n_plus_k := r_plus_v, n_minus_k := r_minus_v
		for (unsigned int i = 0; i < 3; ++i) {
			n_plus_k[i] = r_plus_v[i];
			n_minus_k[i] = r_minus_v[i];
			m_plus_k[i] = n_plus_k[i] - edgeLength * edgeVector[i];
			m_minus_k[i] = n_minus_k[i] - edgeLength * edgeVector[i];
		}
	}

	// --------------------------------------------------------------------------
	// --------------------- Compute w_plus_k, w_minus_k ------------------------
	// --------------------------------------------------------------------------

	// find w_plus_k
	double viplus1_to_mplusk[3] = {m_plus_k[0] - viplus1[0], m_plus_k[1] - viplus1[1], m_plus_k[2] - viplus1[2]};
	double dist_viplus1_to_mplusk = length(viplus1_to_mplusk);
	beta = M_PI_2 + (angleBound/2.0) - acos(dotProduct(viplus1_to_mplusk, revEdgeVector) / dist_viplus1_to_mplusk);
	dist_viplus1_to_x = (edgeLength/sin(beta)) * sin(M_PI_2 - (angleBound/2.0));
	double vi_to_wplusk[3] = {edgeLength * edgeVector[0] + (dist_viplus1_to_x/dist_viplus1_to_mplusk) * viplus1_to_mplusk[0],
								edgeLength * edgeVector[1] + (dist_viplus1_to_x/dist_viplus1_to_mplusk) * viplus1_to_mplusk[1],
								edgeLength * edgeVector[2] + (dist_viplus1_to_x/dist_viplus1_to_mplusk) * viplus1_to_mplusk[2]};
	normalize(vi_to_wplusk);
	double dist_vi_to_wplusk = (edgeLength/2.0) / sin(M_PI_2 - (angleBound/2.0));
	vi_to_wplusk[0] = dist_vi_to_wplusk * vi_to_wplusk[0];
	vi_to_wplusk[1] = dist_vi_to_wplusk * vi_to_wplusk[1];
	vi_to_wplusk[2] = dist_vi_to_wplusk * vi_to_wplusk[2];
	double w_plus_k[3] = {vi[0] + vi_to_wplusk[0], vi[1] + vi_to_wplusk[1], vi[2] + vi_to_wplusk[2]};

	// find w_minus_k
	double viplus1_to_mminusk[3] = {m_minus_k[0] - viplus1[0], m_minus_k[1] - viplus1[1], m_minus_k[2] - viplus1[2]};
	double dist_viplus1_to_mminusk = length(viplus1_to_mminusk);
	beta = M_PI_2 + (angleBound/2.0) - acos(dotProduct(viplus1_to_mminusk, revEdgeVector) / dist_viplus1_to_mminusk);
	dist_viplus1_to_x = (edgeLength/sin(beta)) * sin(M_PI_2 - (angleBound/2.0));
	double vi_to_wminusk[3] = {edgeLength * edgeVector[0] + (dist_viplus1_to_x/dist_viplus1_to_mminusk) * viplus1_to_mminusk[0],
								edgeLength * edgeVector[1] + (dist_viplus1_to_x/dist_viplus1_to_mminusk) * viplus1_to_mminusk[1],
								edgeLength * edgeVector[2] + (dist_viplus1_to_x/dist_viplus1_to_mminusk) * viplus1_to_mminusk[2]};
	normalize(vi_to_wminusk);
	vi_to_wminusk[0] = dist_vi_to_wplusk * vi_to_wminusk[0];
	vi_to_wminusk[1] = dist_vi_to_wplusk * vi_to_wminusk[1];
	vi_to_wminusk[2] = dist_vi_to_wplusk * vi_to_wminusk[2];
	double w_minus_k[3] = {vi[0] + vi_to_wminusk[0], vi[1] + vi_to_wminusk[1], vi[2] + vi_to_wminusk[2]};


	// --------------------------------------------------------------------------
	// ------------------- Setting up the planes M_i, N_i -----------------------
	// --------------------------------------------------------------------------
	double viplus1_to_nplusk[3] = {n_plus_k[0] - viplus1[0], n_plus_k[1] - viplus1[1], n_plus_k[2] - viplus1[2]};
	double dist_viplus1_to_nplusk = length(viplus1_to_nplusk);
	double dist_viplus1_to_n0 = length(viplus1_to_n0);
	double spanAngle = acos(dotProduct(viplus1_to_n0, viplus1_to_nplusk) / (dist_viplus1_to_n0 * dist_viplus1_to_nplusk));
	double subdivideAngle(0);
	if (numDivision > 0)
		subdivideAngle = spanAngle / numDivision;
	double *m_plus_i = m0;
	double *n_plus_i = n0;
	double *w_plus_i = w0;
	double *m_minus_i = m0;
	double *n_minus_i = n0;
	double *w_minus_i = w0;
	for (unsigned int i = 0; i <= numDivision; ++i)
	{
		double *m_plus_iplus1 = m_plus_k;
		double *n_plus_iplus1 = n_plus_k;
		double *w_plus_iplus1 = NULL;
		double *m_minus_iplus1 = m_minus_k;
		double *n_minus_iplus1 = n_minus_k;
		double *w_minus_iplus1 = NULL;

		double m_plus_iplus1_array[3] = {0,0,0};
		double n_plus_iplus1_array[3] = {0,0,0};
		double w_plus_iplus1_array[3] = {0,0,0};
		double m_minus_iplus1_array[3] = {0,0,0};
		double n_minus_iplus1_array[3] = {0,0,0};
		double w_minus_iplus1_array[3] = {0,0,0};
		if (i < numDivision)
		{
			double angle = (i+1)*subdivideAngle;

			// ---- compute m_plus_i, n_plus_i ----
			double n0_to_nplusk[3] = {n_plus_k[0] - n0[0], n_plus_k[1] - n0[1], n_plus_k[2] - n0[2]};
			double n0_to_viplus1[3] = {-1.0 * viplus1_to_n0[0], -1.0 * viplus1_to_n0[1], -1.0 * viplus1_to_n0[2]};
			double dist_n0_to_nplusk = length(n0_to_nplusk);
			beta = M_PI - angle - acos(dotProduct(n0_to_nplusk, n0_to_viplus1) / (dist_n0_to_nplusk * dist_viplus1_to_n0));
			double dist_n0_to_x = (dist_viplus1_to_n0/sin(beta)) * sin(angle);
			double viplus1_to_n_plus_iplus1[3] = {viplus1_to_n0[0] + (dist_n0_to_x/dist_n0_to_nplusk) * n0_to_nplusk[0],
													viplus1_to_n0[1] + (dist_n0_to_x/dist_n0_to_nplusk) * n0_to_nplusk[1],
													viplus1_to_n0[2] + (dist_n0_to_x/dist_n0_to_nplusk) * n0_to_nplusk[2]};
			normalize(viplus1_to_n_plus_iplus1);
			viplus1_to_n_plus_iplus1[0] = dist_viplus1_to_n0 * viplus1_to_n_plus_iplus1[0];
			viplus1_to_n_plus_iplus1[1] = dist_viplus1_to_n0 * viplus1_to_n_plus_iplus1[1];
			viplus1_to_n_plus_iplus1[2] = dist_viplus1_to_n0 * viplus1_to_n_plus_iplus1[2];
			n_plus_iplus1_array[0] = viplus1[0] + viplus1_to_n_plus_iplus1[0];
			n_plus_iplus1_array[1] = viplus1[1] + viplus1_to_n_plus_iplus1[1];
			n_plus_iplus1_array[2] = viplus1[2] + viplus1_to_n_plus_iplus1[2];
			m_plus_iplus1_array[0] = n_plus_iplus1_array[0] - edgeLength*edgeVector[0];
			m_plus_iplus1_array[1] = n_plus_iplus1_array[1] - edgeLength*edgeVector[1];
			m_plus_iplus1_array[2] = n_plus_iplus1_array[2] - edgeLength*edgeVector[2];

			// ---- compute w_plus_i ----
			double w0_to_wplusk[3] = {w_plus_k[0] - w0[0], w_plus_k[1] - w0[1], w_plus_k[2] - w0[2]};
			double dist_w0_to_wplusk = length(w0_to_wplusk);
			double w0_to_vm[3] = {vm[0] - w0[0], vm[1] - w0[1], vm[2] - w0[2]};
			double dist_w0_to_vm = length(w0_to_vm);
			beta = M_PI - angle - acos(dotProduct(w0_to_wplusk, w0_to_vm) / (dist_w0_to_wplusk * dist_w0_to_vm));
			double dist_w0_to_x = (dist_w0_to_vm/sin(beta)) * sin(angle);
			double vm_to_w_plus_iplus1[3] = {-1.0*w0_to_vm[0] + (dist_w0_to_x/dist_w0_to_wplusk) * w0_to_wplusk[0],
											-1.0*w0_to_vm[1] + (dist_w0_to_x/dist_w0_to_wplusk) * w0_to_wplusk[1],
											-1.0*w0_to_vm[2] + (dist_w0_to_x/dist_w0_to_wplusk) * w0_to_wplusk[2]};
			normalize(vm_to_w_plus_iplus1);
			vm_to_w_plus_iplus1[0] = dist_w0_to_vm * vm_to_w_plus_iplus1[0];
			vm_to_w_plus_iplus1[1] = dist_w0_to_vm * vm_to_w_plus_iplus1[1];
			vm_to_w_plus_iplus1[2] = dist_w0_to_vm * vm_to_w_plus_iplus1[2];
			w_plus_iplus1_array[0] = vm[0] + vm_to_w_plus_iplus1[0];
			w_plus_iplus1_array[1] = vm[1] + vm_to_w_plus_iplus1[1];
			w_plus_iplus1_array[2] = vm[2] + vm_to_w_plus_iplus1[2];

			// ---- compute m_minus_i, n_minus_i ----
			double n0_to_nminusk[3] = {n_minus_k[0] - n0[0], n_minus_k[1] - n0[1], n_minus_k[2] - n0[2]};
			double dist_n0_to_nminusk = length(n0_to_nminusk);
			beta = M_PI - angle - acos(dotProduct(n0_to_nminusk, n0_to_viplus1) / (dist_n0_to_nminusk * dist_viplus1_to_n0));
			dist_n0_to_x = (dist_viplus1_to_n0/sin(beta)) * sin(angle);
			double viplus1_to_n_minus_iplus1[3] = {viplus1_to_n0[0] + (dist_n0_to_x/dist_n0_to_nminusk) * n0_to_nminusk[0],
													viplus1_to_n0[1] + (dist_n0_to_x/dist_n0_to_nminusk) * n0_to_nminusk[1],
													viplus1_to_n0[2] + (dist_n0_to_x/dist_n0_to_nminusk) * n0_to_nminusk[2]};
			normalize(viplus1_to_n_minus_iplus1);
			viplus1_to_n_minus_iplus1[0] = dist_viplus1_to_n0 * viplus1_to_n_minus_iplus1[0];
			viplus1_to_n_minus_iplus1[1] = dist_viplus1_to_n0 * viplus1_to_n_minus_iplus1[1];
			viplus1_to_n_minus_iplus1[2] = dist_viplus1_to_n0 * viplus1_to_n_minus_iplus1[2];
			n_minus_iplus1_array[0] = viplus1[0] + viplus1_to_n_minus_iplus1[0];
			n_minus_iplus1_array[1] = viplus1[1] + viplus1_to_n_minus_iplus1[1];
			n_minus_iplus1_array[2] = viplus1[2] + viplus1_to_n_minus_iplus1[2];
			m_minus_iplus1_array[0] = n_minus_iplus1_array[0] - edgeLength*edgeVector[0];
			m_minus_iplus1_array[1] = n_minus_iplus1_array[1] - edgeLength*edgeVector[1];
			m_minus_iplus1_array[2] = n_minus_iplus1_array[2] - edgeLength*edgeVector[2];

			// ---- compute w_minus_i ----
			double w0_to_wminusk[3] = {w_minus_k[0] - w0[0], w_minus_k[1] - w0[1], w_minus_k[2] - w0[2]};
			double dist_w0_to_wminusk = length(w0_to_wminusk);
			beta = M_PI - angle - acos(dotProduct(w0_to_wminusk, w0_to_vm) / (dist_w0_to_wminusk * dist_w0_to_vm));
			dist_w0_to_x = (dist_w0_to_vm/sin(beta)) * sin(angle);
			double vm_to_w_minus_iplus1[3] = {-1.0*w0_to_vm[0] + (dist_w0_to_x/dist_w0_to_wminusk) * w0_to_wminusk[0],
											-1.0*w0_to_vm[1] + (dist_w0_to_x/dist_w0_to_wminusk) * w0_to_wminusk[1],
											-1.0*w0_to_vm[2] + (dist_w0_to_x/dist_w0_to_wminusk) * w0_to_wminusk[2]};
			normalize(vm_to_w_minus_iplus1);
			vm_to_w_minus_iplus1[0] = dist_w0_to_vm * vm_to_w_minus_iplus1[0];
			vm_to_w_minus_iplus1[1] = dist_w0_to_vm * vm_to_w_minus_iplus1[1];
			vm_to_w_minus_iplus1[2] = dist_w0_to_vm * vm_to_w_minus_iplus1[2];
			w_minus_iplus1_array[0] = vm[0] + vm_to_w_minus_iplus1[0];
			w_minus_iplus1_array[1] = vm[1] + vm_to_w_minus_iplus1[1];
			w_minus_iplus1_array[2] = vm[2] + vm_to_w_minus_iplus1[2];

			// assign to pointer
			m_plus_iplus1 = m_plus_iplus1_array;
			n_plus_iplus1 = n_plus_iplus1_array;
			w_plus_iplus1 = w_plus_iplus1_array;
			m_minus_iplus1 = m_minus_iplus1_array;
			n_minus_iplus1 = n_minus_iplus1_array;
			w_minus_iplus1 = w_minus_iplus1_array;
		}

		// construct plane M_plus_i
		double planeNormal[3] = {0,0,0};
		::computeFaceNormal(w_plus_i[0], w_plus_i[1], w_plus_i[2], m_plus_i[0], m_plus_i[1], m_plus_i[2], m_plus_iplus1[0], m_plus_iplus1[1], m_plus_iplus1[2], planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		double ptOnPlane[3] = {0,0,0};
		ptOnPlane[0] = w_plus_i[0] + offset*planeNormal[0];
		ptOnPlane[1] = w_plus_i[1] + offset*planeNormal[1];
		ptOnPlane[2] = w_plus_i[2] + offset*planeNormal[2];

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;


		// construct plane M_minus_i
		::computeFaceNormal(m_minus_i[0], m_minus_i[1], m_minus_i[2], w_minus_i[0], w_minus_i[1], w_minus_i[2], m_minus_iplus1[0], m_minus_iplus1[1], m_minus_iplus1[2], planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		ptOnPlane[0] = w_minus_i[0] + offset*planeNormal[0];
		ptOnPlane[1] = w_minus_i[1] + offset*planeNormal[1];
		ptOnPlane[2] = w_minus_i[2] + offset*planeNormal[2];

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;

		// construct plane N_plus_i
		::computeFaceNormal(n_plus_i[0], n_plus_i[1], n_plus_i[2], w_plus_i[0], w_plus_i[1], w_plus_i[2], n_plus_iplus1[0], n_plus_iplus1[1], n_plus_iplus1[2], planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		ptOnPlane[0] = w_plus_i[0] + offset*planeNormal[0];
		ptOnPlane[1] = w_plus_i[1] + offset*planeNormal[1];
		ptOnPlane[2] = w_plus_i[2] + offset*planeNormal[2];

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;

		// construct plane N_minus_i
		::computeFaceNormal(w_minus_i[0], w_minus_i[1], w_minus_i[2], n_minus_i[0], n_minus_i[1], n_minus_i[2], n_minus_iplus1[0], n_minus_iplus1[1], n_minus_iplus1[2], planeNormal);
		constraints[currentConstraint].a = planeNormal[0];
		constraints[currentConstraint].b = planeNormal[1];
		constraints[currentConstraint].c = planeNormal[2];
		normalize(planeNormal);

		ptOnPlane[0] = w_minus_i[0] + offset*planeNormal[0];
		ptOnPlane[1] = w_minus_i[1] + offset*planeNormal[1];
		ptOnPlane[2] = w_minus_i[2] + offset*planeNormal[2];

		constraints[currentConstraint].d = -1.0 * dotProduct(constraints[currentConstraint].a, 
															 constraints[currentConstraint].b, 
															 constraints[currentConstraint].c, 
															 ptOnPlane[0],
															 ptOnPlane[1],
															 ptOnPlane[2]);
		++currentConstraint;
	}

	return true;
}

// computes the avg. normal of one-ring of an edge
// edgeIdx (in): index of edge
// normal (out): avg. normal
void NonobtOptn::computeOneRingAvgNormalForEdge(int edgeIdx, double* normal) const
{
	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;
	// 2 sides of each one-ring are not part of the edge's one-ring
	unsigned int numNbrFaces = noVertexNeighbourList[vIdx1].size() + noVertexNeighbourList[vIdx2].size() - 2;
	
	vector<double> normalList(3 * numNbrFaces);
	vector<double> weightList(numNbrFaces);
	double areaSum = 0.0;

	unsigned int count = 0;
	// process each edge in one-ring
	for (unsigned int k = 0; k < 2; ++k)
	{
		int currentVIdx = vIdx1;
		int otherVIdx = vIdx2;
		if (k == 1)
		{
			currentVIdx = vIdx2;
			otherVIdx = vIdx1;
		}

		unsigned int numNeighbours = noVertexNeighbourList[currentVIdx].size();
		for (unsigned int i = 0; i < numNeighbours; ++i)
		{
			// find edge in one-ring from the neighbour face
			int polyIdx = noVertexNeighbourList[currentVIdx][i];
			Edge e;
			e.vertex1Idx = 0;
			e.vertex2Idx = 0;
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (noPolygonsList[polyIdx][j] == currentVIdx)
				{
					e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];	// v_i
					e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];	// v_{i+1}
					break;
				}
			}

			// skip triangles that has vIdx1 and vIdx2 as vertices in the 2nd run
			if (k == 1 && (e.vertex1Idx == otherVIdx || e.vertex2Idx == otherVIdx))
				continue;

			double pt1[3] = {noVerticesList[3*currentVIdx], noVerticesList[3*currentVIdx+1], noVerticesList[3*currentVIdx+2]};
			double pt2[3] = {noVerticesList[3*(e.vertex1Idx)], noVerticesList[3*(e.vertex1Idx)+1], noVerticesList[3*(e.vertex1Idx)+2]};
			double pt3[3] = {noVerticesList[3*(e.vertex2Idx)], noVerticesList[3*(e.vertex2Idx)+1], noVerticesList[3*(e.vertex2Idx)+2]};

			weightList[count] = areaTriangle(pt1, pt2, pt3);
			areaSum += weightList[count];
			normalList[3*count] = noFaceNormalsList[3*polyIdx];
			normalList[3*count+1] = noFaceNormalsList[3*polyIdx+1];
			normalList[3*count+2] = noFaceNormalsList[3*polyIdx+2];
			
			++count;
		}
	}

	normal[0] = 0;
	normal[1] = 0;
	normal[2] = 0;
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		weightList[i] /= areaSum;
		normal[0] += weightList[i]*normalList[3*i];
		normal[1] += weightList[i]*normalList[3*i+1];
		normal[2] += weightList[i]*normalList[3*i+2];
	}
}

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
unsigned int NonobtOptn::findClosestPolygon(int vIdx, double* vList, 
											double* search_vList, int** search_polyList, 
											vector<int>* search_vertexNeighbourList, unsigned int numPolygons, 
											double &minDist, double* closestPt, bool doFullSearch, int closestPolyIdx, 
											unsigned int numOneRingSearch) const
{
	double pt[3] = {vList[3*vIdx], vList[3*vIdx+1], vList[3*vIdx+2]};
	return findClosestPolygon(pt, search_vList, search_polyList, search_vertexNeighbourList, numPolygons, minDist, closestPt, doFullSearch, closestPolyIdx, numOneRingSearch);
}

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
unsigned int NonobtOptn::findClosestPolygon(double *pt, 
											double* search_vList, int** search_polyList, 
											vector<int>* search_vertexNeighbourList, unsigned int numPolygons, 
											double &minDist, double* closestPt, bool doFullSearch, int closestPolyIdx, 
											unsigned int numOneRingSearch) const
{
	minDist = 0.0;
	closestPt[0] = 0.0;
	closestPt[1] = 0.0;
	closestPt[2] = 0.0;
	unsigned int minDist_polyIdx(0);
	
	//if (doFullSearch || closestPolyIdx < 0 || closestPolyIdx >= numPolygons)
	if (doFullSearch)
	{
		// loop through all triangles in mesh and check for closest distance
		for (unsigned int i = 0; i < numPolygons; ++i)
		{
			double triPt1[3] = {search_vList[3*search_polyList[i][0]], 
								search_vList[3*search_polyList[i][0]+1], 
								search_vList[3*search_polyList[i][0]+2]};
			double triPt2[3] = {search_vList[3*search_polyList[i][1]], 
								search_vList[3*search_polyList[i][1]+1], 
								search_vList[3*search_polyList[i][1]+2]};
			double triPt3[3] = {search_vList[3*search_polyList[i][2]], 
								search_vList[3*search_polyList[i][2]+1], 
								search_vList[3*search_polyList[i][2]+2]};
			
			double closestPt_tri[3];
			double dist = pt_triangle_distance(pt, triPt1, triPt2, triPt3, closestPt_tri);
			
			if (i == 0 || dist < minDist)
			{
				minDist = dist;
				closestPt[0] = closestPt_tri[0];
				closestPt[1] = closestPt_tri[1];
				closestPt[2] = closestPt_tri[2];
				minDist_polyIdx = i;
			}
		}
	}
	else
	{
		// vertices of the last closest face are our initial one-ring search space
		vector<int> oneRingToSearch;
		oneRingToSearch.push_back(search_polyList[closestPolyIdx][0]);
		oneRingToSearch.push_back(search_polyList[closestPolyIdx][1]);
		oneRingToSearch.push_back(search_polyList[closestPolyIdx][2]);
		
		vector<int> searchedFaces;	// stores the polygon indices that has been searched
		int minPolyIdx(-1);
		for (unsigned int i = 1; i <= numOneRingSearch; ++i)
		{
			unsigned int numToSearch = oneRingToSearch.size();
			for (unsigned int j = 0; j < numToSearch; ++j)
			{
				int oneRingIdx = oneRingToSearch[j];
				for (unsigned int k = 0; k < search_vertexNeighbourList[oneRingIdx].size(); ++k)
				{
					int currentPolyIdx = search_vertexNeighbourList[oneRingIdx][k];
					
					// check if the current polygon has been searched
					bool isSearched(false);
					for (unsigned int l = 0; l < searchedFaces.size(); ++l)
					{
						if (searchedFaces[l] == currentPolyIdx)
						{
							isSearched = true;
							break;
						}
					}
					
					if (isSearched)
						continue;					
					
					double triPt1[3] = {search_vList[3*search_polyList[currentPolyIdx][0]],
										search_vList[3*search_polyList[currentPolyIdx][0]+1],
										search_vList[3*search_polyList[currentPolyIdx][0]+2]};
					double triPt2[3] = {search_vList[3*search_polyList[currentPolyIdx][1]],
										search_vList[3*search_polyList[currentPolyIdx][1]+1],
										search_vList[3*search_polyList[currentPolyIdx][1]+2]};
					double triPt3[3] = {search_vList[3*search_polyList[currentPolyIdx][2]],
										search_vList[3*search_polyList[currentPolyIdx][2]+1],
										search_vList[3*search_polyList[currentPolyIdx][2]+2]};
					double closestPt_tri[3];
					double dist = pt_triangle_distance(pt, triPt1, triPt2, triPt3, closestPt_tri);
					
					if (dist < minDist || minPolyIdx == -1)
					{
						minDist = dist;
						minPolyIdx = currentPolyIdx;
						closestPt[0] = closestPt_tri[0];
						closestPt[1] = closestPt_tri[1];
						closestPt[2] = closestPt_tri[2];
					}
					
					// add polygon to the list of searched faces
					searchedFaces.push_back(currentPolyIdx);
					
					// add vertices to search for
					if (i != numOneRingSearch)	// we haven't search to the outer one-ring yet
					{
						// add the 3 vertices of the searched polgyon into the oneRingToSearch list
						for (unsigned int l = 0; l < 3; ++l)
						{
							int triVIdx = search_polyList[currentPolyIdx][l];
							// make sure we don't add duplicates into the list
							bool isInList(false);
							for (unsigned int m = 0; m < oneRingToSearch.size(); ++m)
							{
								if (oneRingToSearch[m] == triVIdx)
								{
									isInList = true;
									break;
								}
							}
							if (isInList)
								continue;
							
							oneRingToSearch.push_back(triVIdx);
						}
					}
				}
			}
			
			// erase the searched vertices
			for (unsigned int j = 0; j < numToSearch; ++j)
				oneRingToSearch.erase(oneRingToSearch.begin());
		}
		
		minDist_polyIdx = minPolyIdx;
	}
	
	return minDist_polyIdx;
}

// returns the quadric value of a point (x, y, z) based on some quadric q
// q (in): Quadric
// pt (in): coord of the point
double NonobtOptn::computeQuadricValue(Quadric q, double *pt) const
{
	// compute quadric value
	double Av[3] = {q.A[0]*pt[0] + q.A[1]*pt[1] + q.A[2]*pt[2],
					q.A[3]*pt[0] + q.A[4]*pt[1] + q.A[5]*pt[2],
					q.A[6]*pt[0] + q.A[7]*pt[1] + q.A[8]*pt[2]};
	return (dotProduct(Av, pt) + 2.0*dotProduct(q.b, pt) + q.c);
}

// returns the quadric value of a point pt based on some quadric q computed by the polygon dPolyIdx
// no_pt (in): vertex location (of the nonobtuse mesh) of the quadric to be computed
// dPolyIdx (in): index of polygon used for computing the quadric
// alpha (in): weighting alpha value for objective function
// numOneRingQuadric (in): number of one-ring to use for computing quadric
double NonobtOptn::computeQuadricValue(double *no_pt, int dPolyIdx, double alpha, unsigned int numOneRingQuadric) const
{
	Quadric q;
	for (unsigned int i = 0; i < 9; ++i)
		q.A[i] = 0.0;
	for (unsigned int i = 0; i < 3; ++i)
		q.b[i] = 0.0;
	q.c = 0.0;
	
	// stores the one ring vertices that will be used
	vector<int> oneRingToAdd;
	oneRingToAdd.push_back(dPolygonsList[dPolyIdx][0]);
	oneRingToAdd.push_back(dPolygonsList[dPolyIdx][1]);
	oneRingToAdd.push_back(dPolygonsList[dPolyIdx][2]);
		
	//vector<int> processedTriangles;	//stores the polygons that has been processed

	for (unsigned int i = 1; i <= numOneRingQuadric; ++i)
	{
		unsigned int numOneRingVertices = oneRingToAdd.size();
		for (unsigned int j = 0; j < numOneRingVertices; ++j)
		{
			int oneRingIdx = oneRingToAdd[j];
			for (unsigned int k = 0; k < dVertexNeighbourList[oneRingIdx].size(); ++k)
			{
				int currentPolyIdx = dVertexNeighbourList[oneRingIdx][k];
				
				//if (find(processedTriangles.begin(), processedTriangles.end(), currentPolyIdx) == processedTriangles.end())
				//{
				
				double normal[3] = {dFaceNormalsList[3*currentPolyIdx],
									dFaceNormalsList[3*currentPolyIdx+1],
									dFaceNormalsList[3*currentPolyIdx+2]};
				normalize(normal);
				double pt[3] = {dVerticesList[3*dPolygonsList[currentPolyIdx][0]],
								dVerticesList[3*dPolygonsList[currentPolyIdx][0]+1],
								dVerticesList[3*dPolygonsList[currentPolyIdx][0]+2]};
				double d = -1.0 * dotProduct(normal, pt);
				
				// c = d^2
				q.c += d*d;
				// b = d*normal
				q.b[0] += d*normal[0];
				q.b[1] += d*normal[1];
				q.b[2] += d*normal[2];
				// A = normal*normal^T
				q.A[0] += normal[0]*normal[0];
				q.A[1] += normal[0]*normal[1];
				q.A[2] += normal[0]*normal[2];
				q.A[3] += normal[0]*normal[1];
				q.A[4] += normal[1]*normal[1];
				q.A[5] += normal[1]*normal[2];
				q.A[6] += normal[0]*normal[2];
				q.A[7] += normal[1]*normal[2];
				q.A[8] += normal[2]*normal[2];

				//processedTriangles.push_back(currentPolyIdx);
				//}

				// add vertices to add
				if (i != numOneRingQuadric)	// we haven't add the outer one-ring yet
				{
					// add the 3 vertices of the current polgyon into the oneRingToAdd list
					for (unsigned int l = 0; l < 3; ++l)
					{
						int triVIdx = dPolygonsList[currentPolyIdx][l];
						// make sure we don't add duplicates into the list
						bool isInList(false);
						for (unsigned int m = 0; m < oneRingToAdd.size(); ++m)
						{
							if (oneRingToAdd[m] == triVIdx)
							{
								isInList = true;
								break;
							}
						}
						if (isInList)
							continue;
						
						oneRingToAdd.push_back(triVIdx);
					}
				}
			}
		}
		
		// erase the added vertices
		for (unsigned int j = 0; j < numOneRingVertices; ++j)
			oneRingToAdd.erase(oneRingToAdd.begin());
	}
	
	// special case when we only use the closest triangle to compute the quadric
	if (numOneRingQuadric == 0)
	{
		int currentPolyIdx = dPolyIdx;
		double normal[3] = {dFaceNormalsList[3*currentPolyIdx],
							dFaceNormalsList[3*currentPolyIdx+1],
							dFaceNormalsList[3*currentPolyIdx+2]};
		normalize(normal);
		double pt[3] = {dVerticesList[3*dPolygonsList[currentPolyIdx][0]],
						dVerticesList[3*dPolygonsList[currentPolyIdx][0]+1],
						dVerticesList[3*dPolygonsList[currentPolyIdx][0]+2]};
		double d = -1.0 * dotProduct(normal, pt);
		
		// c = d^2
		q.c = d*d;
		// b = d*normal
		q.b[0] = d*normal[0];
		q.b[1] = d*normal[1];
		q.b[2] = d*normal[2];
		// A = normal*normal^T
		q.A[0] = normal[0]*normal[0];
		q.A[1] = normal[0]*normal[1];
		q.A[2] = normal[0]*normal[2];
		q.A[3] = q.A[1];
		q.A[4] = normal[1]*normal[1];
		q.A[5] = normal[1]*normal[2];
		q.A[6] = q.A[2];
		q.A[7] = q.A[5];
		q.A[8] = normal[2]*normal[2];
	}

	return computeQuadricValue(q, no_pt);
}

// compute optimal location for vertex (novIdx) that minimizes quadric q
// returns true if successful
// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
// q (in): quadric value of novIdx
// pt (out): moved location
// region (in): determines how the non-obtuse region is to be approximated
// angleBound (in): angle bound (in degrees)
// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
//                           on the one-ring vertices for the edge
bool NonobtOptn::computeOptimalLocation(int novIdx, Quadric q, double *pt, NORegionType region, double angleBound, bool bEdgeOneRing)
{
	avgSolvingTime = 0;
	maxSolvingTime = 0;
	avgSetupTime = 0;
	maxSetupTime = 0;
	numSolve = 0;
	numSetup = 0;

	// --- compute the non-obtuse regions (set of plane constraints)
	PlaneEqn* constraints = NULL;
	clock_t begin = clock();
	unsigned int numConstraints = getNonobtuseRegion(novIdx, constraints, region, bEdgeOneRing, angleBound);
	//clock_t totalTime = (clock() - begin) / CLOCKS_PER_SEC;
	clock_t totalTime = (clock() - begin);
	if (maxSetupTime < totalTime)
		maxSetupTime = totalTime;
	avgSetupTime += totalTime;
	++numSetup;

	begin = clock();
	
	// --- setup parameters for solver
	int nx = 3;		// number of variables to optimize
	double c[3] = {q.fn.b[0], q.fn.b[1], q.fn.b[2]};		// linear term in objective function
	double  xupp[] = {0, 0, 0};		// bounds on x
	char   ixupp[] = {0, 0, 0};
	double  xlow[] = {0, 0, 0};
	char   ixlow[] = {0, 0, 0};
	
	// quadratic term in objective function
	int nnzQ = 0;
	int numEntriesInQ = nx*nx;
	vector<int> irowQ_tmp(numEntriesInQ);
	vector<int> jcolQ_tmp(numEntriesInQ);
	vector<double> dQ_tmp(numEntriesInQ);
	for (int row = 0; row < nx; ++row)	
	{
		for (int col = 0; col < row+1; ++col)
		{
			if (q.fn.A[row*nx+col] != 0.0)
			{
				irowQ_tmp[nnzQ] = row;
				jcolQ_tmp[nnzQ] = col;
				dQ_tmp[nnzQ] = q.fn.A[row*nx+col];
				++nnzQ;
			}
		}
	}
	
	vector<int> irowQ(nnzQ); 
	vector<int> jcolQ(nnzQ);
	vector<double> dQ(nnzQ);
	for (int i = 0; i < nnzQ; ++i)	
	{
		irowQ[i] = irowQ_tmp[i];
		jcolQ[i] = jcolQ_tmp[i];
		dQ[i] = dQ_tmp[i];
	}
	
	// linear equality constraints
	int my         = 0;
	double * b     = 0;
	int nnzA       = 0;
	int * irowA    = 0;
	int * jcolA    = 0;
	double * dA    = 0;

	// linear inequality constraints
	int mz = numConstraints;
	vector<double> clow(mz);
	vector<char>  iclow(mz);
	vector<double> cupp(mz);
	vector<char>  icupp(mz);
	
	int nnzC = 0;
	vector<int> irowC_tmp(mz*nx);
	vector<int> jcolC_tmp(mz*nx);
	vector<double> dC_tmp(mz*nx);
	for (int i = 0; i < mz; ++i)
	{
		clow[i] = -1.0 * constraints[i].d;	// ax + by + cz >= -d
		iclow[i] = 1;
		cupp[i] = 0;
		icupp[i] = 0;
		
		if (constraints[i].a != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 0;
			dC_tmp[nnzC] = constraints[i].a;
			++nnzC;
		}
		
		if (constraints[i].b != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 1;
			dC_tmp[nnzC] = constraints[i].b;
			++nnzC;
		}
		
		if (constraints[i].c != 0.0)
		{
			irowC_tmp[nnzC] = i;
			jcolC_tmp[nnzC] = 2;
			dC_tmp[nnzC] = constraints[i].c;
			++nnzC;
		}
	}
	
	vector<int>   irowC(nnzC);
	vector<int>   jcolC(nnzC);
	vector<double>   dC(nnzC);
	for (int i = 0; i < nnzC; ++i)
	{
		irowC[i] = irowC_tmp[i];
		jcolC[i] = jcolC_tmp[i];
		dC[i]    = dC_tmp[i];
	}

	// --- finish setting up params
	
	// solving optimization problem
	QpGenSparseMa27 *qp = new QpGenSparseMa27(nx, my, mz, nnzQ, nnzA, nnzC);
	
	QpGenData *prob = (QpGenData*) qp->copyDataFromSparseTriple(
										c,          &irowQ[0],  nnzQ,       &jcolQ[0],  &dQ[0],
										xlow,       ixlow,      xupp,       ixupp,
										irowA,      nnzA,       jcolA,      dA,         b,
										&irowC[0],  nnzC,       &jcolC[0],  &dC[0],
										&clow[0],   &iclow[0],  &cupp[0],   &icupp[0]);
										
	QpGenVars *vars = (QpGenVars*) qp->makeVariables( prob );
	QpGenResiduals * resid = (QpGenResiduals *) qp->makeResiduals( prob );
  
	GondzioSolver *s = new GondzioSolver( qp, prob );
	
	// s->monitorSelf();	// print out solver info.
	int ierr = s->solve(prob, vars, resid);
	
	// ERROR CODE
	// 	enum TerminationCode
	// 	{
	// 		SUCCESSFUL_TERMINATION = 0,
	// 		NOT_FINISHED,
	// 		MAX_ITS_EXCEEDED,
	// 		INFEASIBLE,
	// 		UNKNOWN
	// 	};
	
	if (ierr == 0)
	{
		// found optimal location, move vertex and update mesh
		vars->x->copyIntoArray(pt);
	}
	
	// ---- debug only ----
// 	for (unsigned int i = 0; i < numConstraints; ++i)
// 	{
// 		double value = constraints[i].a * pt[0] + constraints[i].b * pt[1] + constraints[i].c * pt[2] + constraints[i].d;
// 		if (value < 0)
// 			int foo = 1;
// 	}
	// ---- end debug -----

	// clean up
	delete qp;
	delete prob;
	delete vars;
	delete resid;
	delete s;
	delete[] constraints;
	
// 	totalTime = (clock() - begin) / CLOCKS_PER_SEC;
 	totalTime = (clock() - begin);
	if (maxSolvingTime < totalTime)
		maxSolvingTime = totalTime;
	avgSolvingTime += totalTime;
	++numSolve;

	return (ierr == 0);
}

// compute smoothness for vIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
// centroid (out): centroid position for 'novIdx' one-ring
// useCentroid (in): true - use centroid to compute smoothness
// smtype (in): determines how smoothness is computed
Quadric NonobtOptn::computeSmoothness(int novIdx, double *centroid, bool useCentroid, SmoothType smtype) const
{
	if (useCentroid)
	{
		Quadric q;
		q.vIdx = novIdx;
		q.quadricValue = length(centroid[0] - noVerticesList[3*novIdx],
								centroid[1] - noVerticesList[3*novIdx+1],
								centroid[2] - noVerticesList[3*novIdx+2]);
	
		for (unsigned int i = 0; i < 9; ++i)
		{
			q.A[i] = 0;
			q.fn.A[i] = 0;
			if (i == 0 || i == 4 || i == 8)
			{
				q.A[i] = 1;
				q.fn.A[i] = 1;
			}

			if (i < 3)
			{
				q.b[i] = -1*centroid[i];
				q.fn.b[i] = -1*centroid[i];
			}
		}
		q.c = dotProduct(centroid, centroid);
		q.fn.c = dotProduct(centroid, centroid);

		return q;
	}

	if (smtype == LAPLACIAN)
		return computeLaplacianSmoothness(novIdx, centroid);
	
	return computeTaubinSmoothness(novIdx, centroid);
}

// compute smoothness using LAPLACIAN for vIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
// centroid (out): centroid position for 'novIdx' one-ring
Quadric NonobtOptn::computeLaplacianSmoothness(int novIdx, double *centroid) const
{
	centroid[0] = 0;
	centroid[1] = 0;
	centroid[2] = 0;

	if (isBoundaryVertex(novIdx))
		return computeBoundaryLaplacianSmoothness(novIdx, centroid);

	unsigned int numNeighbours = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNeighbours; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				unsigned int vIdx1 = noPolygonsList[polyIdx][(j+1)%3];
				unsigned int vIdx2 = noPolygonsList[polyIdx][(j+2)%3];
	
				centroid[0] += noVerticesList[3*vIdx1];
				centroid[1] += noVerticesList[3*vIdx1+1];
				centroid[2] += noVerticesList[3*vIdx1+2];

				centroid[0] += noVerticesList[3*vIdx2];
				centroid[1] += noVerticesList[3*vIdx2+1];
				centroid[2] += noVerticesList[3*vIdx2+2];
				break;
			}
		}
	}

	centroid[0] /= (2*numNeighbours);	// added each vertex twice, so divide 2*numNeighbours
	centroid[1] /= (2*numNeighbours);
	centroid[2] /= (2*numNeighbours);

	Quadric q;
	q.vIdx = novIdx;
	q.quadricValue = length(centroid[0] - noVerticesList[3*novIdx],
							centroid[1] - noVerticesList[3*novIdx+1],
							centroid[2] - noVerticesList[3*novIdx+2]);

	for (unsigned int i = 0; i < 9; ++i)
	{
		q.A[i] = 0;
		q.fn.A[i] = 0;
		if (i == 0 || i == 4 || i == 8)
		{
			q.A[i] = 1;
			q.fn.A[i] = 1;
		}
	}

	for (unsigned int i = 0; i < 3; ++i)
	{
		q.b[i] = -1*centroid[i];
		q.fn.b[i] = -1*centroid[i];
	}

	q.c = dotProduct(centroid, centroid);
	q.fn.c = dotProduct(centroid, centroid);

	return q;
}

// compute smoothness using LAPLACIAN for boundary vertex vIdx and returns it
// novIdx (in): boundary vertex index (of the nonobtuse mesh) of the smoothness to be computed
// centroid (out): centroid position for 'novIdx' one-ring
Quadric NonobtOptn::computeBoundaryLaplacianSmoothness(int novIdx, double *centroid) const
{
	centroid[0] = 0;
	centroid[1] = 0;
	centroid[2] = 0;

	int numBoundaryNbr(0);
	for (unsigned int i = 0; i < noVertexEdgeList[novIdx].size(); ++i)
	{
		// find boundary neighbour
		int vIdx = noEdgeList[noVertexEdgeList[novIdx][i]].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[noVertexEdgeList[novIdx][i]].vertex2Idx;

		if (isBoundaryVertex(vIdx)) {
			centroid[0] += noVerticesList[3*vIdx];
			centroid[1] += noVerticesList[3*vIdx+1];
			centroid[2] += noVerticesList[3*vIdx+2];
			++numBoundaryNbr;
		}
		
		if (numBoundaryNbr == 2)
			break;
	}
	centroid[0] /= 2;
	centroid[1] /= 2;
	centroid[2] /= 2;

	Quadric q;
	q.vIdx = novIdx;
	q.quadricValue = length(centroid[0] - noVerticesList[3*novIdx],
							centroid[1] - noVerticesList[3*novIdx+1],
							centroid[2] - noVerticesList[3*novIdx+2]);

	for (unsigned int i = 0; i < 9; ++i)
	{
		q.A[i] = 0;
		q.fn.A[i] = 0;
		if (i == 0 || i == 4 || i == 8)
		{
			q.A[i] = 1;
			q.fn.A[i] = 1;
		}
	}

	for (unsigned int i = 0; i < 3; ++i)
	{
		q.b[i] = -1*centroid[i];
		q.fn.b[i] = -1*centroid[i];
	}

	q.c = dotProduct(centroid, centroid);
	q.fn.c = dotProduct(centroid, centroid);

	return q;
}

// compute smoothness using TAUBIN for vIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the smoothness to be computed
// centroid (out): centroid position for 'novIdx' one-ring
Quadric NonobtOptn::computeTaubinSmoothness(int novIdx, double *centroid) const
{
	// compute centroid based on centroidList
	
	centroid[0] = 0;
	centroid[1] = 0;
	centroid[2] = 0;
	unsigned int numNeighbours = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNeighbours; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == novIdx)
			{
				unsigned int vIdx1 = noPolygonsList[polyIdx][(j+1)%3];
				unsigned int vIdx2 = noPolygonsList[polyIdx][(j+2)%3];
	
				centroid[0] += centroidList[3*vIdx1];
				centroid[1] += centroidList[3*vIdx1+1];
				centroid[2] += centroidList[3*vIdx1+2];

				centroid[0] += centroidList[3*vIdx2];
				centroid[1] += centroidList[3*vIdx2+1];
				centroid[2] += centroidList[3*vIdx2+2];
				break;
			}
		}
	}

	centroid[0] /= (2*numNeighbours);	// added each vertex twice, so divide 2*numNeighbours
	centroid[1] /= (2*numNeighbours);
	centroid[2] /= (2*numNeighbours);

	centroid[0] = (1-TAUBIN_MU)*centroidList[3*novIdx] + TAUBIN_MU*centroid[0];
	centroid[1] = (1-TAUBIN_MU)*centroidList[3*novIdx+1] + TAUBIN_MU*centroid[1];
	centroid[2] = (1-TAUBIN_MU)*centroidList[3*novIdx+2] + TAUBIN_MU*centroid[2];

	// construct quadric based on computed centroid
	Quadric q;
	q.vIdx = novIdx;
	q.quadricValue = length(centroid[0] - noVerticesList[3*novIdx],
							centroid[1] - noVerticesList[3*novIdx+1],
							centroid[2] - noVerticesList[3*novIdx+2]);

	for (unsigned int i = 0; i < 9; ++i)
	{
		q.A[i] = 0;
		q.fn.A[i] = 0;
		if (i == 0 || i == 4 || i == 8)
		{
			q.A[i] = 1;
			q.fn.A[i] = 1;
		}
	}

	for (unsigned int i = 0; i < 3; ++i)
	{
		q.b[i] = -1*centroid[i];
		q.fn.b[i] = -1*centroid[i];
	}

	q.c = dotProduct(centroid, centroid);
	q.fn.c = dotProduct(centroid, centroid);

	return q;
}

// update centroidList after novIdx has been moved
// novIdx (in): index of vertex that moved
void NonobtOptn::updateCentroidList(int novIdx)
{
	// update centroidList for the moved vertex and its one-ring
	unsigned int numOneRingTriangles = noVertexNeighbourList[novIdx].size();
	vector<unsigned int> updatedIndices;
	for (unsigned int i = 0; i < numOneRingTriangles; ++i)
	{
		unsigned int currentPolyIdx = noVertexNeighbourList[novIdx][i];
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[currentPolyIdx][j];
			
			// check if the vertex has been updated
			if (find(updatedIndices.begin(), updatedIndices.end(), vIdx) != updatedIndices.end())
				continue;

			// update centroid
			double centroid[3] = {0,0,0};
			unsigned int numNeighbours = noVertexNeighbourList[vIdx].size();
			for (unsigned int k = 0; k < numNeighbours; ++k)
			{
				unsigned int polyIdx = noVertexNeighbourList[vIdx][k];
				for (unsigned int m = 0; m < 3; ++m)
				{
					if (noPolygonsList[polyIdx][m] == (int)vIdx)
					{
						unsigned int vIdx1 = noPolygonsList[polyIdx][(m+1)%3];
						unsigned int vIdx2 = noPolygonsList[polyIdx][(m+2)%3];
			
						centroid[0] += noVerticesList[3*vIdx1];
						centroid[1] += noVerticesList[3*vIdx1+1];
						centroid[2] += noVerticesList[3*vIdx1+2];
		
						centroid[0] += noVerticesList[3*vIdx2];
						centroid[1] += noVerticesList[3*vIdx2+1];
						centroid[2] += noVerticesList[3*vIdx2+2];
						break;
					}
				}
			}
			centroid[0] /= (2*numNeighbours);	// added each vertex twice, so divide 2*numNeighbours
			centroid[1] /= (2*numNeighbours);
			centroid[2] /= (2*numNeighbours);

			centroidList[3*vIdx] = (1-TAUBIN_LAMBDA)*noVerticesList[3*vIdx] + TAUBIN_LAMBDA*centroid[0];
			centroidList[3*vIdx+1] = (1-TAUBIN_LAMBDA)*noVerticesList[3*vIdx+1] + TAUBIN_LAMBDA*centroid[1];
			centroidList[3*vIdx+2] = (1-TAUBIN_LAMBDA)*noVerticesList[3*vIdx+2] + TAUBIN_LAMBDA*centroid[2];

			updatedIndices.push_back(vIdx);
		}
	}
}

// initialize vertex decimate quadric
void NonobtOptn::initializeVertexDecimateQuadric()
{
	qVertexDecimateList.clear();
	numPlanesInQuadricsList.clear();

	for (int i = 0; i < numNoVertices; ++i)
	{
		if (isBoundaryVertex(i))
		{
			qVertexDecimateList.push_back(computeInitialBoundaryDecimateQuadric(i));
			numPlanesInQuadricsList.push_back(1);
		}
		else
		{
			qVertexDecimateList.push_back(computeInitialDecimateQuadric(i));
			numPlanesInQuadricsList.push_back(noVertexNeighbourList[i].size());
		}
	}
}

// compute initial decimate quadric for vIdx and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
Quadric NonobtOptn::computeInitialDecimateQuadric(int novIdx) const
{
	Quadric q;
	q.vIdx = novIdx;
	q.quadricValue = 0.0;
	for (unsigned int i = 0; i < 9; ++i)
		q.A[i] = 0.0;
	for (unsigned int i = 0; i < 3; ++i)
		q.b[i] = 0.0;
	q.c = 0.0;
	
	unsigned int numNeighbours = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNeighbours; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];

		double normal[3] = {noFaceNormalsList[3*polyIdx],
							noFaceNormalsList[3*polyIdx+1],
							noFaceNormalsList[3*polyIdx+2]};
		normalize(normal);
		double pt[3] = {noVerticesList[3*noPolygonsList[polyIdx][0]],
						noVerticesList[3*noPolygonsList[polyIdx][0]+1],
						noVerticesList[3*noPolygonsList[polyIdx][0]+2]};
		double d = -1.0 * dotProduct(normal, pt);
		
		// c = d^2
		q.c += d*d;
		// b = d*normal
		q.b[0] += d*normal[0];
		q.b[1] += d*normal[1];
		q.b[2] += d*normal[2];
		// A = normal*normal^T
		q.A[0] += normal[0]*normal[0];
		q.A[1] += normal[0]*normal[1];
		q.A[2] += normal[0]*normal[2];
		q.A[3] += normal[0]*normal[1];
		q.A[4] += normal[1]*normal[1];
		q.A[5] += normal[1]*normal[2];
		q.A[6] += normal[0]*normal[2];
		q.A[7] += normal[1]*normal[2];
		q.A[8] += normal[2]*normal[2];
	}
	
	// compute quadric value
	double pt[3] = {noVerticesList[3*novIdx],
					noVerticesList[3*novIdx+1], 
					noVerticesList[3*novIdx+2]};
	double Av[3] = {q.A[0]*pt[0] + q.A[1]*pt[1] + q.A[2]*pt[2],
					q.A[3]*pt[0] + q.A[4]*pt[1] + q.A[5]*pt[2],
					q.A[6]*pt[0] + q.A[7]*pt[1] + q.A[8]*pt[2]};
	q.quadricValue = dotProduct(Av, pt) + 2.0*dotProduct(q.b, pt) + q.c;

	for (unsigned int i = 0; i < 9; ++i)
		q.fn.A[i] = q.A[i];
	for (unsigned int i = 0; i < 3; ++i)
		q.fn.b[i] = q.b[i];
	q.fn.c = q.c;
	
	return q;
}

// compute initial decimate quadric for boundary vertex 'vIdx' and returns it
// novIdx (in): vertex index (of the nonobtuse mesh) of the quadric to be computed
Quadric NonobtOptn::computeInitialBoundaryDecimateQuadric(int novIdx) const
{
	double pt[3] = {noVerticesList[3*novIdx], noVerticesList[3*novIdx+1], noVerticesList[3*novIdx+2]};
	return computeSmoothness(novIdx, pt, true, LAPLACIAN);
}

// remove arbitrary elements from heap
// heap (in): heap
// idx (in): index in 'heap'
// func (in): comparison function
// TODO: test this
void NonobtOptn::remove_from_heap(vector<QuadricMove> &heap, unsigned int idx, bool (*func)(QuadricMove, QuadricMove))
{
	//sanity checks
	if (idx >= heap.size() || idx < 0)
		return;

	// swap with last element and remove the one we don't want
	swap(heap[idx], heap[heap.size()-1]);
	heap.pop_back();

	// update heap tree
	update_heap(heap, idx, func);
}

// update heap by moving element 'idx' to the correct location
// heap (in): heap
// idx (in): index in "heap"
// func (in): comparison function
// TODO: test this
void NonobtOptn::update_heap(vector<QuadricMove> &heap, unsigned int idx, bool (*func)(QuadricMove, QuadricMove))
{
	unsigned int parentIdx = (idx-1)/2;
	unsigned int leftIdx = 2*(idx+1) - 1;
	unsigned int rightIdx = 2*(idx+1);
	if ((*func)(heap[parentIdx], heap[idx]))			// parent < idx
	{
		swap(heap[idx], heap[parentIdx]);
		update_heap(heap, parentIdx, func);
	}
	else if ((*func)(heap[leftIdx], heap[rightIdx]))	// left < right
	{
		if ((*func)(heap[idx], heap[leftIdx]))			// idx < left
		{
			swap(heap[idx], heap[leftIdx]);
			update_heap(heap, leftIdx, func);
		}
	}
	else												// right <= left
	{
		if ((*func)(heap[idx], heap[rightIdx]))			// idx < right
		{
			swap(heap[idx], heap[rightIdx]);
			update_heap(heap, rightIdx, func);
		} 
	}
}

// returns the list of one-ring vertices for an edge into oneRingVertices (ordered by right hand rule)
// edgeIdx (in): index of edge
// oneRingVertices (out): list of onering vertices
// return value (out): true - if no duplicates in one-ring
bool NonobtOptn::getEdgeOneRingVertices(int edgeIdx, vector<unsigned int> &oneRingVertices) const
{
	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;

	bool result(true);

	// always process non-boundary vertex first if there's one
	if (isBoundaryVertex(vIdx1) && !isBoundaryVertex(vIdx2))
		swap(vIdx1, vIdx2);

	vector<int> v1NbrFaces = noVertexNeighbourList[vIdx1];
	vector<int> v2NbrFaces = noVertexNeighbourList[vIdx2];
	
	unsigned int countLeftOvers(0);
	oneRingVertices.clear();
	for (int i = 0; !v1NbrFaces.empty(); i=(i+1)%v1NbrFaces.size())
	{
		if (i == 0)		// recount left overs if i == 0
			countLeftOvers = 0;	

		unsigned int polyIdx = v1NbrFaces[i];
		if (oneRingVertices.empty())
		{
			for (unsigned int k = 0; k < 3; ++k) {
				if (noPolygonsList[polyIdx][k] == (int)vIdx1) {
					if (noPolygonsList[polyIdx][(k+1)%3] != (int)vIdx2) {
						if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(noPolygonsList[polyIdx][(k+1)%3])) != oneRingVertices.end())
							result = false;
						oneRingVertices.push_back(noPolygonsList[polyIdx][(k+1)%3]);
					}
					if (noPolygonsList[polyIdx][(k+2)%3] != (int)vIdx2) {
						if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(noPolygonsList[polyIdx][(k+2)%3])) != oneRingVertices.end())
							result = false;
						oneRingVertices.push_back(noPolygonsList[polyIdx][(k+2)%3]);
					}
					break;
				}
			}

			v1NbrFaces.erase(v1NbrFaces.begin() + i);
			if (v1NbrFaces.empty())
				break;
			i = -1;
			continue;
		}

		vector<int> verticesToCheck;
		for (unsigned int k = 0; k < 3; ++k) {
			if (noPolygonsList[polyIdx][k] == (int)vIdx1) {
				if (noPolygonsList[polyIdx][(k+1)%3] != (int)vIdx2)
					verticesToCheck.push_back(noPolygonsList[polyIdx][(k+1)%3]);
				if (noPolygonsList[polyIdx][(k+2)%3] != (int)vIdx2)
					verticesToCheck.push_back(noPolygonsList[polyIdx][(k+2)%3]);
				break;
			}
		}

		if (verticesToCheck.size() != 2) {
			v1NbrFaces.erase(v1NbrFaces.begin() + i);
			if (v1NbrFaces.empty())
				break;
			i = -1;
			continue;
		}

		// check if we can add to the one-ring vertices in order
		if (verticesToCheck.front() == (int)oneRingVertices.back()) {
			if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(verticesToCheck.back())) != oneRingVertices.end())
				result = false;
			oneRingVertices.push_back(verticesToCheck.back());
			v1NbrFaces.erase(v1NbrFaces.begin() + i);
			if (v1NbrFaces.empty())
				break;
			i = -1;
			continue;
		}
		else if (verticesToCheck.back() == (int)oneRingVertices.front()) {
			if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(verticesToCheck.front())) != oneRingVertices.end())
				result = false;
			oneRingVertices.insert(oneRingVertices.begin(), verticesToCheck.front());
			v1NbrFaces.erase(v1NbrFaces.begin() + i);
			if (v1NbrFaces.empty())
				break;
			i = -1;
			continue;
		}

		++countLeftOvers;
		if (countLeftOvers == v1NbrFaces.size())	// there are some leftovers that cannot be processed
			return false;
	}

	for (int i = 0; !v2NbrFaces.empty(); i=(i+1)%v2NbrFaces.size())
	{
		if (i == 0)		// recount left overs if i == 0
			countLeftOvers = 0;	

		unsigned int polyIdx = v2NbrFaces[i];
		vector<int> verticesToCheck;
		for (unsigned int k = 0; k < 3; ++k) {
			if (noPolygonsList[polyIdx][k] == (int)vIdx2) {
				if (noPolygonsList[polyIdx][(k+1)%3] != (int)vIdx1)
					verticesToCheck.push_back(noPolygonsList[polyIdx][(k+1)%3]);
				if (noPolygonsList[polyIdx][(k+2)%3] != (int)vIdx1)
					verticesToCheck.push_back(noPolygonsList[polyIdx][(k+2)%3]);
				break;
			}
		}

		if (verticesToCheck.size() != 2) {
			v2NbrFaces.erase(v2NbrFaces.begin() + i);
			if (v2NbrFaces.empty())
				break;
			i = -1;
			continue;
		}

		// check if we can add to the one-ring vertices in order
		if (!isBoundaryVertex(vIdx2) && 
			verticesToCheck.front() == (int)oneRingVertices.back() && 
			verticesToCheck.back() == (int)oneRingVertices.front())
			break;
		else if (verticesToCheck.front() == (int)oneRingVertices.back()) {
			if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(verticesToCheck.back())) != oneRingVertices.end())
				result = false;
			oneRingVertices.push_back(verticesToCheck.back());
			v2NbrFaces.erase(v2NbrFaces.begin() + i);
			if (v2NbrFaces.empty())
				break;
			i = -1;
			continue;
		}
		else if (verticesToCheck.back() == (int)oneRingVertices.front()) {
			if (find(oneRingVertices.begin(), oneRingVertices.end(), (unsigned int)(verticesToCheck.front())) != oneRingVertices.end())
				result = false;
			oneRingVertices.insert(oneRingVertices.begin(), verticesToCheck.front());
			v2NbrFaces.erase(v2NbrFaces.begin() + i);
			if (v2NbrFaces.empty())
				break;
			i = -1;
			continue;
		}

		++countLeftOvers;
		if (countLeftOvers == v2NbrFaces.size())	// there are some leftovers that cannot be processed
			return false;
	}

	return result;
}

// returns true if an edge can be decimated
// an edge can be decimated as long as the neighbour vertices don't have degree-degreeN
// edgeIdx (in): index of edge
// degreeN (in): max. of min. degrees that we want to avoid
// return val (out): true if edge can be decimated
bool NonobtOptn::canDecimate(unsigned int edgeIdx, unsigned int degreeN) const
{
	// index of the 2 vertices of the edge
	unsigned int vIdx1 = noEdgeList[edgeIdx].vertex1Idx;
	unsigned int vIdx2 = noEdgeList[edgeIdx].vertex2Idx;

	bool bCanDecimate = true;

	// check for min valence
	unsigned int numVIdx1Neighbours = noVertexNeighbourList[vIdx1].size();
	for (unsigned int i = 0; i < numVIdx1Neighbours; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[vIdx1][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		{
			if (noPolygonsList[polyIdx][j] == (int)vIdx1)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j+1)%3];
				e.vertex2Idx = noPolygonsList[polyIdx][(j+2)%3];
				break;
			}
		}

		// see if current polygon is the one that has edgeIdx as an edge
		if (e.vertex1Idx == (int)vIdx2)
		{
			// if so, then check the degree of the 3rd vertex
			// make sure it has does not have degree <= degreeN + 1
			if (noVertexEdgeList[e.vertex2Idx].size() <= degreeN + 1)
			{	
				bCanDecimate = false;
				break;
			}
		}
		else if (e.vertex2Idx == (int)vIdx2)
		{
			// if so, then check the degree of the 3rd vertex
			// make sure it has does not have degree <= degreeN + 1
			if (noVertexEdgeList[e.vertex1Idx].size() <= degreeN + 1)
			{
				bCanDecimate = false;
				break;
			}
		}
	}

	// check for non-manifold cases
	if (bCanDecimate)
	{
		vector<unsigned int> oneRingVertices;
		bCanDecimate = getEdgeOneRingVertices(edgeIdx, oneRingVertices);
	}

	return bCanDecimate;
}

// returns true if novIdx is at boundary
// novIdx (in): index of vertex in nonobtuse mesh to check
// return value (out): true if novIdx is at boundary
bool NonobtOptn::isBoundaryVertex(int novIdx) const
{
	// sanity checks
	if (novIdx < 0 || novIdx > numNoVertices)
		return false;

	return (noVertexEdgeList[novIdx].size() != noVertexNeighbourList[novIdx].size());
}

// locating the closest boundary edge to vertex 'novIdx'
//
// novIdx (in): index of the vertex
// pt (in): location of novIdx
// closestPt (out): location on desired mesh closest to pt
// numOneRingSearch (in): depth of search
// doFullSearch (in): true - if search exhaustively in a breadth-first manner starting with the closest triangle of 'novIdx'
// return value (out): the boundary edge closest to 'novIdx'; (-1,-1) if none found
Edge NonobtOptn::findClosestBoundaryEdge(int novIdx, double *pt, double* closestPt, unsigned int numOneRingSearch, bool doFullSearch) const
{
	closestPt[0] = 0;
	closestPt[1] = 0;
	closestPt[2] = 0;

	Edge closestBoundaryEdge;
	// start searching from last closest boundary edge
	closestBoundaryEdge.vertex1Idx = closestBoundaryEdgeList[novIdx].vertex1Idx;
	closestBoundaryEdge.vertex2Idx = closestBoundaryEdgeList[novIdx].vertex2Idx;

	if (doFullSearch || closestBoundaryEdge.vertex1Idx < 0 || closestBoundaryEdge.vertex2Idx < 0)
	{
		int startSearchTri = closestFaceList[novIdx];
		if (closestFaceList[novIdx] == -1)	// closest face is unassigned
		{
			// --- global search for closest face ---
			double minDist(0.0);
			double closestPt[3] = {0,0,0};
			startSearchTri = findClosestPolygon(novIdx, noVerticesList, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, true, 0, numOneRingSearch);
		}

		// ----- do breadth-first search starting form closest face -----
		vector<bool> toIgnoreChecking(numDPolygons, false);	// stores the triangles that have been checked
		queue<int> toCheckList;		// stores the next triangles to check for boundary edge
		toCheckList.push(startSearchTri);
		while (!toCheckList.empty())
		{
			int triIdx = toCheckList.front();
			toCheckList.pop();
			toIgnoreChecking[triIdx] = true;

			// sanity check
			if (triIdx < 0 || triIdx >= numDPolygons)
				continue;

			// check if 'triIdx' has a boundary edge
			if (dPolygonNeighbourList[triIdx].size() < 3)
			{
				// check which boundary edge of 'triIdx' is closest to 'novIdx'
				double closestDist(-1.0);
				for (unsigned int i = 0; i < 3; ++i)
				{
					// skip checking distance from 'novIdx' for non-boundary edge
					int edgeV1 = dPolygonsList[triIdx][i];
					int edgeV2 = dPolygonsList[triIdx][(i+1)%3];
					bool isBoundaryEdge(false);
					for (vector<EdgeNeighbours>::iterator iter = dEdgeNeighbourList[edgeV1].begin(); iter != dEdgeNeighbourList[edgeV1].end(); iter++)
					{
						if (iter->e.vertex2Idx == edgeV2) {
							if (iter->polyNeighbour2Idx < 0 || iter->polyNeighbour1Idx < 0)
								isBoundaryEdge = true;
							break;
						}
					}
					if (!isBoundaryEdge)
						continue;

					// compute distance from 'novIdx'
					double edgePt1[3] = {dVerticesList[3*edgeV1], dVerticesList[3*edgeV1+1], dVerticesList[3*edgeV1+2]};
					double edgePt2[3] = {dVerticesList[3*edgeV2], dVerticesList[3*edgeV2+1], dVerticesList[3*edgeV2+2]};
					double dist = pt_segment_distance(pt, edgePt1, edgePt2, closestPt);

					// update the closest boundary edge
					if (closestDist < 0 || dist < closestDist)
					{
						closestDist = dist;
						closestBoundaryEdge.vertex1Idx = edgeV1;
						closestBoundaryEdge.vertex2Idx = edgeV2;
					}
				}

				break;	// break for looping toCheckList
			}
			else
			{		
				// 'triIdx' does not have boundary edge, add its neighbours for testing
				for (unsigned int i = 0; i < dPolygonNeighbourList[triIdx].size(); ++i)
					if (!toIgnoreChecking[dPolygonNeighbourList[triIdx][i]])
						toCheckList.push(dPolygonNeighbourList[triIdx][i]);
			}
		}
	}

	// ----- look for closest boundary edge starting from the last closest boundary edge -----

	// starting boundary edge to start looking
	int boundV1 = closestBoundaryEdge.vertex1Idx;
	int boundV2 = closestBoundaryEdge.vertex2Idx;
	// the neighbour vertices of boundV1, boundV2; this is to keep track of which way to search
	int nbrV1 = boundV2;
	int nbrV2 = boundV1;

	double edgePt1[3] = {dVerticesList[3*boundV1], dVerticesList[3*boundV1+1], dVerticesList[3*boundV1+2]};
	double edgePt2[3] = {dVerticesList[3*boundV2], dVerticesList[3*boundV2+1], dVerticesList[3*boundV2+2]};
	double closestDist = pt_segment_distance(pt, edgePt1, edgePt2, closestPt);

	bool stopSearchV1(false);
	bool stopSearchV2(false);
	double closestPt_Tri[3] = {0,0,0};
	for (unsigned int i = 0; i < numOneRingSearch; ++i)
	{
		// find next closest pair of vertices along the boundary
		for (unsigned int j = 0; j < 2; ++j)
		{
			if ((j == 0 && stopSearchV1) || (j == 1 && stopSearchV2))
				continue;

			int* vIdx = &boundV1;
			int* vNbr = &nbrV1;
			if (j > 0) {
				vIdx = &boundV2;
				vNbr = &nbrV2;
			}

			// find the next open edge adjacent to v1 and v2
			unsigned int numEdges = dEdgeNeighbourList[*vIdx].size();
			for (unsigned int k = 0; k < numEdges; ++k)
			{
				if (dEdgeNeighbourList[*vIdx][k].e.vertex2Idx != *vNbr && (dEdgeNeighbourList[*vIdx][k].polyNeighbour2Idx < 0 || dEdgeNeighbourList[*vIdx][k].polyNeighbour1Idx < 0))
				{
					*vNbr = *vIdx;
					*vIdx = dEdgeNeighbourList[*vIdx][k].e.vertex2Idx;
					break;
				}

				if (k == numEdges-1) {
					if (j == 0)
						stopSearchV1 = true;
					else
						stopSearchV2 = true;
				}
			}
		}

		// no more boundary edge to search
		if (stopSearchV1 && stopSearchV2)
			break;

		// check distance from vertex
		if (!stopSearchV1) 
		{
			edgePt1[0] = dVerticesList[3*boundV1];
			edgePt1[1] = dVerticesList[3*boundV1+1];
			edgePt1[2] = dVerticesList[3*boundV1+2];
			edgePt2[0] = dVerticesList[3*nbrV1];
			edgePt2[1] = dVerticesList[3*nbrV1+1];
			edgePt2[2] = dVerticesList[3*nbrV1+2];
			double dist = pt_segment_distance(pt, edgePt1, edgePt2, closestPt_Tri);
			if (dist < closestDist) {
				closestDist = dist;
				closestPt[0] = closestPt_Tri[0];
				closestPt[1] = closestPt_Tri[1];
				closestPt[2] = closestPt_Tri[2];
				closestBoundaryEdge.vertex1Idx = boundV1;
				closestBoundaryEdge.vertex2Idx = nbrV1;
			}
		}

		if (!stopSearchV2)
		{
			edgePt1[0] = dVerticesList[3*boundV2];
			edgePt1[1] = dVerticesList[3*boundV2+1];
			edgePt1[2] = dVerticesList[3*boundV2+2];
			edgePt2[0] = dVerticesList[3*nbrV2];
			edgePt2[1] = dVerticesList[3*nbrV2+1];
			edgePt2[2] = dVerticesList[3*nbrV2+2];
			double dist = pt_segment_distance(pt, edgePt1, edgePt2, closestPt_Tri);
			if (dist < closestDist) {
				closestDist = dist;
				closestPt[0] = closestPt_Tri[0];
				closestPt[1] = closestPt_Tri[1];
				closestPt[2] = closestPt_Tri[2];
				closestBoundaryEdge.vertex1Idx = boundV2;
				closestBoundaryEdge.vertex2Idx = nbrV2;
			}
		}
	}

	return closestBoundaryEdge;
}

// returns the quadric value of a point pt based on some quadric q computed by the polygon dPolyIdx
// no_pt (in): vertex location (of the nonobtuse mesh) of the quadric to be computed
// closestEdge (in): closest boundary edge used for computing the quadric
// alpha (in): weighting alpha value for objective function
// numOneRingQuadric (in): number of one-ring to use for computing quadric
//double NonobtOptn::computeBoundaryQuadricValue(double *no_pt, Edge closestEdge, double alpha, unsigned int numOneRingQuadric) const
double NonobtOptn::computeBoundaryQuadricValue(double *no_pt, double* closestPt, double alpha, unsigned int numOneRingQuadric) const
{
	// ------------ construct quadric ----------------
	// Idea: quadric should measures the distance from the vertex to the set of vertices that make up the boundary edges
	//       giving more weights to points that connected to the boundary edge that is closer to the vertex
	//
	// Q(x) = xIx - 2px + pp
	// where I is the identity matrix, p is the point we want to get close to
	//
	
	Quadric q;

	q.A[0] = 1;
	q.A[1] = 0;
	q.A[2] = 0;
	q.A[3] = 0;
	q.A[4] = 1;
	q.A[5] = 0;
	q.A[6] = 0;
	q.A[7] = 0;
	q.A[8] = 1;
	
	q.b[0] = -1*closestPt[0];
	q.b[1] = -1*closestPt[1];
	q.b[2] = -1*closestPt[2];

	q.c = dotProduct(closestPt[0], closestPt[1], closestPt[2], 
						closestPt[0], closestPt[1], closestPt[2]);

	// compute the quadric value
	return computeQuadricValue(q, no_pt);
}

// remove all vertices that have bad valence on the nonobtuse mesh
//
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in degrees)
void NonobtOptn::removeBadValence(NORegionType region, double angleBound)
{
	double angleBound_Rad = angleBound*(M_PI/180.0);	// convert angleBound to radian

	cout << "Removing bad valence vertices..." << flush;
	bool hasRemoved(true);
	clock_t b4Time = clock();
	while (hasRemoved)
	{
		hasRemoved = false;
		for (int i = 0; i < numNoVertices; ++i)
		{
			if (isBoundaryVertex(i) && noVertexEdgeList[i].size() == 2)		// boundary valence-2 vertices are bad
			{
				if (removeBoundary2(i)) {
					--i;
					hasRemoved = true;
				}
			}
		}
	
		for (int i = 0; i < numNoVertices; ++i)
		{
			if (!isBoundaryVertex(i) && noVertexEdgeList[i].size() == 3)	// interior valence-3 vertices are bad
			{
				if (removeInterior3AndTriangulate(i, region, angleBound_Rad)) {
					--i;
					hasRemoved = true;
				}
			}
		}
	
		for (int i = 0; i < numNoVertices; ++i)
		{
			if (!isBoundaryVertex(i) && noVertexEdgeList[i].size() == 4)		// interior valence-4 vertices are bad
			{
				if (removeInterior4AndTriangulate(i, region, angleBound_Rad)) {		// 1st attempt
					hasRemoved = true;
				}
				else if (removeInterior4ByEdgeCollapse(i, region, angleBound)) {	// 2nd attempt
					hasRemoved = true;
				}
// 				else if (false)														// 3rd attempt
// 					hasRemoved = true;
			}
		}
	
		for (int i = 0; i < numNoVertices; ++i)
		{
			if (isBoundaryVertex(i) && noVertexEdgeList[i].size() == 3)		// boundary valence-3 vertices are bad
			{
				if (removeBoundary3AndTriangulate(i, region, angleBound_Rad)) {		// 1st attempt
					hasRemoved = true;
				}
				else if (removeBoundary3ByEdgeCollapse(i, region, angleBound)) {	// 2nd attempt
					hasRemoved = true;
				}
	// 			else if (false)														// 3rd attempt
	// 				hasRemoved = true;
			}
		}
	}

	cout << "done! (in " << (clock()-b4Time) / CLOCKS_PER_SEC << " seconds)" << endl;
}

// remove the boundary valence-3 vertex 'novIdx' by way of edge collapse
//
// novIdx (in): index of vertex to be removed (boundary vertex)
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in degrees)
// return value (out): true if successful
bool NonobtOptn::removeBoundary3ByEdgeCollapse(unsigned int novIdx, NORegionType region, double angleBound)
{
	unsigned int numNbrs = noVertexEdgeList[novIdx].size();
	if (numNbrs != 3) {
		cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - boundary vertex " << novIdx << " is not of valence-3" << endl;
		return false;
	}

	// Get indices of neighbour vertices
	vector<unsigned int> nbrVertIndices;
	if (!getOneRingVertices(novIdx, nbrVertIndices)) {
		cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - cannot get one-ring vertices for boundary vertex " << novIdx << endl;
		return false;
	}
	// sanity check
	if (nbrVertIndices.size() != 3) {
		cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - number of one-ring vertices for boundary vertex " << novIdx << " is not 3." << endl;
		return false;
	}

	// check neighbour valences
	bool isOnBoundary[3] = {isBoundaryVertex(nbrVertIndices[0]), isBoundaryVertex(nbrVertIndices[1]), isBoundaryVertex(nbrVertIndices[2])};
 	vector<unsigned int> collapsedAt;
	if (isOnBoundary[0]) {
		if (noVertexEdgeList[nbrVertIndices[0]].size() > 2 &&
			((!isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 5) ||
			(isOnBoundary[1] &&	noVertexEdgeList[nbrVertIndices[1]].size() > 4)))
			collapsedAt.push_back(0);
	}
	else {
		cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - neighbour vertex " << nbrVertIndices[0] << " is not at the boundary." << endl;
		return false;
	}

	if (!isOnBoundary[1]) {
		if (noVertexEdgeList[nbrVertIndices[1]].size() > 4 &&
			isOnBoundary[0] && noVertexEdgeList[nbrVertIndices[0]].size() > 4 &&
			isOnBoundary[2] && noVertexEdgeList[nbrVertIndices[2]].size() > 4)
			collapsedAt.push_back(1);
	}
	else {
		// middle edge is connecting to a boundary vertex; collapsing it will cause non-manifold vertices to occur
		return false;
	}

	if (isOnBoundary[2]) {
		if (noVertexEdgeList[nbrVertIndices[2]].size() > 2 &&
			((!isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 5) ||
			(isOnBoundary[1] &&	noVertexEdgeList[nbrVertIndices[1]].size() > 4)))
			collapsedAt.push_back(2);
	}
	else {
		cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - neighbour vertex " << nbrVertIndices[2] << " is not at the boundary." << endl;
		return false;
	}

 	if (collapsedAt.empty())
 		return false;

	// try to collapse at the possible edges
	QuadricMove qm;
	bool collapsable(false);
	unsigned int chosenCollapseAt = 0;
	for (unsigned int i = 0; i < collapsedAt.size(); ++i)
	{
		// find the edge index for the edge to be collapsed
		int edgeIdx = -1;
		for (vector<int>::iterator iter = noVertexEdgeList[novIdx].begin(); iter != noVertexEdgeList[novIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)nbrVertIndices[collapsedAt[i]] && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)nbrVertIndices[collapsedAt[i]]))
			{
				edgeIdx = *iter;
				break;
			}
		}
		if (edgeIdx == -1) {
			cout << "Error: NonoptOptn::removeBoundary3ByEdgeCollapse() - cannot find edge (" << novIdx << ", " << nbrVertIndices[collapsedAt[i]] << ")." << endl;
			return false;
		}
	
		// try to find a position for the unifed vertex
		double midpointOfEdge[3];
		midpoint(midpointOfEdge,
				noVerticesList[3*novIdx], 
				noVerticesList[3*novIdx+1], 
				noVerticesList[3*novIdx+2],
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]], 
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]+1], 
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]+2]);
	
		// compute a quadric that tries to move the unifed vertex as close to the edge midpoint as possible
		qm.q = computeSmoothness(novIdx, midpointOfEdge, true);	// pass 'novIdx' is just for making sure the function can compute a quadricValue
		qm.q.vIdx = edgeIdx;	// make sure we set qm->q.vIdx back to the edge index
	
		qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound, true);	// q.vIdx is an edge index
		if (qm.bCanMove) {	// can find a position for the unified vertex
			chosenCollapseAt = collapsedAt[i];
			collapsable = true;
			break;
		}
	}

	if (!collapsable)
		return false;
	
	// edge collapse
	vector<unsigned int> newUnusedEdges;
	edgeCollapse(qm.q.vIdx, qm.newPos, newUnusedEdges);

	// remove unused edges
	removeUnused(newUnusedEdges);

	return true;
}

// remove the interior valence-4 vertex 'novIdx' by way of edge collapse
//
// novIdx (in): index of vertex to be removed (interior vertex)
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in degrees)
// return value (out): true if successful
bool NonobtOptn::removeInterior4ByEdgeCollapse(unsigned int novIdx, NORegionType region, double angleBound)
{
	unsigned int numNbrs = noVertexEdgeList[novIdx].size();
	if (numNbrs != 4) {
		cout << "Error: NonoptOptn::removeInterior4ByEdgeCollapse() - interior vertex " << novIdx << " is not of valence-4" << endl;
		return false;
	}

	// Get indices of neighbour vertices
	vector<unsigned int> nbrVertIndices;
	if (!getOneRingVertices(novIdx, nbrVertIndices)) {
		cout << "Error: NonoptOptn::removeInterior4ByEdgeCollapse() - cannot get one-ring vertices for interior vertex " << novIdx << endl;
		return false;
	}
	// sanity check
	if (nbrVertIndices.size() != 4) {
		cout << "Error: NonoptOptn::removeInterior4ByEdgeCollapse() - number of one-ring vertices for interior vertex " << novIdx << " is not 4." << endl;
		return false;
	}

	// check neighbour valences
	bool isOnBoundary[4] = {isBoundaryVertex(nbrVertIndices[0]), isBoundaryVertex(nbrVertIndices[1]), isBoundaryVertex(nbrVertIndices[2]), isBoundaryVertex(nbrVertIndices[3])};
	vector<unsigned int> collapsedAt;
	for (unsigned int i = 0; i < 4; ++i)
	{
		unsigned int nextIdx = (i+1)%4;
		unsigned int next2Idx = (i+2)%4;

		if (((isOnBoundary[i] && noVertexEdgeList[nbrVertIndices[i]].size() > 4) ||
			(!isOnBoundary[i] && noVertexEdgeList[nbrVertIndices[i]].size() > 5))
			&&
			((isOnBoundary[nextIdx] && noVertexEdgeList[nbrVertIndices[nextIdx]].size() > 3) ||
			(!isOnBoundary[nextIdx] && noVertexEdgeList[nbrVertIndices[nextIdx]].size() > 4))
			&&
			((isOnBoundary[next2Idx] && noVertexEdgeList[nbrVertIndices[next2Idx]].size() > 4) ||
			(!isOnBoundary[next2Idx] && noVertexEdgeList[nbrVertIndices[next2Idx]].size() > 5)))
		{
			collapsedAt.push_back(nextIdx);
		}	
	}

	if (collapsedAt.empty())
		return false;

	// attempt to edge collapse at possible edges
	QuadricMove qm;
	bool collapsable(false);
	unsigned int chosenCollapseAt = 0;
	for (unsigned int i = 0; i < collapsedAt.size(); ++i)
	{
		// find the edge index for the edge to be collapsed
		int edgeIdx = -1;
		for (vector<int>::iterator iter = noVertexEdgeList[novIdx].begin(); iter != noVertexEdgeList[novIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)nbrVertIndices[collapsedAt[i]] && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)nbrVertIndices[collapsedAt[i]]))
			{
				edgeIdx = *iter;
				break;
			}
		}
		if (edgeIdx == -1) {
			cout << "Error: NonoptOptn::removeInterior4ByEdgeCollapse() - cannot find edge (" << novIdx << ", " << nbrVertIndices[collapsedAt[i]] << ")." << endl;
			return false;
		}
	
		// try to find a position for the unifed vertex
		double midpointOfEdge[3];
		midpoint(midpointOfEdge,
				noVerticesList[3*novIdx], 
				noVerticesList[3*novIdx+1], 
				noVerticesList[3*novIdx+2],
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]], 
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]+1], 
				noVerticesList[3*nbrVertIndices[collapsedAt[i]]+2]);
	
		// compute a quadric that tries to move the unifed vertex as close to the edge midpoint as possible
		qm.q = computeSmoothness(novIdx, midpointOfEdge, true);	// pass 'novIdx' is just for making sure the function can compute a quadricValue
		qm.q.vIdx = edgeIdx;	// make sure we set qm->q.vIdx back to the edge index
	
		qm.bCanMove = computeOptimalLocation(qm.q.vIdx, qm.q, qm.newPos, region, angleBound, true);	// q.vIdx is an edge index
		if (qm.bCanMove) {	// can find a position for the unified vertex
			collapsable = true;
			chosenCollapseAt = collapsedAt[i];
			break;
		}
	}

	if (!collapsable)
		return false;

	// edge collapse
	vector<unsigned int> newUnusedEdges;
	edgeCollapse(qm.q.vIdx, qm.newPos, newUnusedEdges);

	// remove unused edges
	removeUnused(newUnusedEdges);

	return true;
}

// remove the boundary valence-2 vertex 'novIdx'
//
// novIdx (in): index of vertex to be removed (boundary vertex)
// return value (out): true if successful
bool NonobtOptn::removeBoundary2(unsigned int novIdx)
{
	unsigned int numEdges = noVertexEdgeList[novIdx].size();
	if (numEdges != 2)
		return false;

	// get neighbour vertices
	unsigned int nbrVertices[2] = {0,0};
	for (unsigned int i = 0; i < numEdges; ++i)
	{
		Edge &e = noEdgeList[noVertexEdgeList[novIdx][i]];
		nbrVertices[i] = e.vertex1Idx;
		if (e.vertex1Idx == (int)novIdx)
			nbrVertices[i] = e.vertex2Idx;
	}

	// check if it will introduce any new bad valence vertex
	if (noVertexEdgeList[nbrVertices[0]].size() <= 3 || noVertexEdgeList[nbrVertices[1]].size() <= 3)
		return false;

	// list of poly indices that are unused
	vector<int> unusedPolyIdxList;
	// list of vertex indices that are unused
	vector<int> unusedVertexIdxList;
	// list of edge indices that are unused
	vector<unsigned int> unusedEdgeIdxList;

	unusedVertexIdxList.push_back(novIdx);

	// remove vertex-to-poly references
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	if (numNbrFaces != 1)
		return false;
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		unusedPolyIdxList.push_back(polyIdx);
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[polyIdx][j];
			if (vIdx == novIdx)		// skip removing for novIdx
				continue;

			vector<int>::iterator iter = find(noVertexNeighbourList[vIdx].begin(), noVertexNeighbourList[vIdx].end(), (int)polyIdx);
			if (iter != noVertexNeighbourList[vIdx].end())
				noVertexNeighbourList[vIdx].erase(iter);
		}
	}
	noVertexNeighbourList[novIdx].clear();

	// remove vertex-to-edge references
	unsigned int numNbrEdges = noVertexEdgeList[novIdx].size();
	for (unsigned int i = 0; i < numNbrEdges; ++i)
	{
		unsigned int edgeIdx = noVertexEdgeList[novIdx][i];
		unsigned int vIdx = noEdgeList[edgeIdx].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[edgeIdx].vertex2Idx;

		for (vector<int>::iterator iter = noVertexEdgeList[vIdx].begin(); iter != noVertexEdgeList[vIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)vIdx && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)vIdx))
			{
				noVertexEdgeList[vIdx].erase(iter);
				break;
			}
		}

		unusedEdgeIdxList.push_back(edgeIdx);
	}
	noVertexEdgeList[novIdx].clear();

	// remove unused vertices and polys
	//unsigned int old_novIdx = novIdx;
	removeUnused(unusedPolyIdxList, unusedVertexIdxList, novIdx);

	// remove unused edges
	removeUnused(unusedEdgeIdxList);

	return true;
}

// remove the boundary valence-3 vertex 'novIdx' and triangulate
//
// novIdx (in): index of vertex to be removed (boundary vertex)
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in radians)
// return value (out): true if successful
// TODO: test this!
bool NonobtOptn::removeBoundary3AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound)
{
	unsigned int numNbrs = noVertexEdgeList[novIdx].size();

	if (numNbrs != 3)
		return false;

	// Get indices of neighbour vertices
	vector<unsigned int> nbrVertIndices;
	if (!getOneRingVertices(novIdx, nbrVertIndices)) {
		cout << "Error: NonoptOptn::removeBoundary3AndTriangulate() - cannot get one-ring vertices for interior vertex " << novIdx << endl;
		return false;
	}

	// sanity check
	if (nbrVertIndices.size() != 3) {
		cout << "Error: NonoptOptn::removeBoundary3AndTriangulate() - number of one-ring vertices for interior vertex " << novIdx << " is not 3." << endl;
		return false;
	}
	
	// check valence of interior neighbour vertex
		
	if (!
		((isBoundaryVertex(nbrVertIndices[1]) && noVertexEdgeList[nbrVertIndices[1]].size() > 4) ||
		(!isBoundaryVertex(nbrVertIndices[1]) && noVertexEdgeList[nbrVertIndices[1]].size() > 5))
		)
		return false;

	// check constraint violations
	double angle = getAngle(noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
								noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
								noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;
	angle = getAngle(noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
					noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
					noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;
	angle = getAngle(noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
					noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
					noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;

	// ----- begin remove + retriangulate -----
	// list of poly indices that are unused
	vector<int> unusedPolyIdxList;
	// list of vertex indices that are unused
	vector<int> unusedVertexIdxList;
	// list of edge indices that are unused
	vector<unsigned int> unusedEdgeIdxList;

	unusedVertexIdxList.push_back(novIdx);

	// remove vertex-to-poly references
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		unusedPolyIdxList.push_back(polyIdx);
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[polyIdx][j];
			if (vIdx == novIdx)		// skip removing for novIdx
				continue;

			vector<int>::iterator iter = find(noVertexNeighbourList[vIdx].begin(), noVertexNeighbourList[vIdx].end(), (int)polyIdx);
			if (iter != noVertexNeighbourList[vIdx].end())
				noVertexNeighbourList[vIdx].erase(iter);
		}
	}
	noVertexNeighbourList[novIdx].clear();

	// remove vertex-to-edge references
	unsigned int numNbrEdges = noVertexEdgeList[novIdx].size();
	for (unsigned int i = 0; i < numNbrEdges; ++i)
	{
		unsigned int edgeIdx = noVertexEdgeList[novIdx][i];
		unsigned int vIdx = noEdgeList[edgeIdx].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[edgeIdx].vertex2Idx;

		for (vector<int>::iterator iter = noVertexEdgeList[vIdx].begin(); iter != noVertexEdgeList[vIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)vIdx && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)vIdx))
			{
				noVertexEdgeList[vIdx].erase(iter);
				break;
			}
		}

		unusedEdgeIdxList.push_back(edgeIdx);
	}
	noVertexEdgeList[novIdx].clear();
	
	// create the new polys
	unsigned int newPolyIdx = unusedPolyIdxList.back();
	unusedPolyIdxList.pop_back();
	noPolygonsList[newPolyIdx][0] = nbrVertIndices[0];
	noPolygonsList[newPolyIdx][1] = nbrVertIndices[1];
	noPolygonsList[newPolyIdx][2] = nbrVertIndices[2];

	// assign vertex-to-poly refs
	for (unsigned int j = 0; j < 3; ++j)
		noVertexNeighbourList[noPolygonsList[newPolyIdx][j]].push_back(newPolyIdx);

	// assign vertex-to-edge refs
	unsigned int newEdgeIdx = unusedEdgeIdxList.back();
	unusedEdgeIdxList.pop_back();
	Edge &e = noEdgeList[newEdgeIdx];
	e.vertex1Idx = nbrVertIndices[0];
	e.vertex2Idx = nbrVertIndices[2];
	if (e.vertex1Idx > e.vertex2Idx)
		swap(e.vertex1Idx, e.vertex2Idx);

	noVertexEdgeList[e.vertex1Idx].push_back(newEdgeIdx);
	noVertexEdgeList[e.vertex2Idx].push_back(newEdgeIdx);

	// remove unused vertices and polys
	//unsigned int old_novIdx = novIdx;
	removeUnused(unusedPolyIdxList, unusedVertexIdxList, novIdx);

	// remove unused edges
	removeUnused(unusedEdgeIdxList);

	return true;
}

// remove the interior valence-3 vertex 'novIdx'
//
// novIdx (in): index of vertex to be removed (interior vertex)
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in radians)
// return value (out): true if successful
// TODO: test this!
bool NonobtOptn::removeInterior3AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound)
{
	unsigned int numNbrs = noVertexEdgeList[novIdx].size();

	if (numNbrs != 3)
		return false;

	// Get indices of neighbour vertices
	vector<unsigned int> nbrVertIndices;
	if (!getOneRingVertices(novIdx, nbrVertIndices)) {
		cout << "Error: NonoptOptn::removeInterior3AndTriangulate() - cannot get one-ring vertices for interior vertex " << novIdx << endl;
		return false;
	}

	// sanity check
	if (nbrVertIndices.size() != 3) {
		cout << "Error: NonoptOptn::removeInterior3AndTriangulate() - number of one-ring vertices for interior vertex " << novIdx << " is not 3." << endl;
		return false;
	}
	
	// check valence of diagonal vertices
	bool isOnBoundary[3] = {isBoundaryVertex(nbrVertIndices[0]), isBoundaryVertex(nbrVertIndices[1]), isBoundaryVertex(nbrVertIndices[2])};
	if (!(((isOnBoundary[0] && noVertexEdgeList[nbrVertIndices[0]].size() > 3) ||
		(!isOnBoundary[0] && noVertexEdgeList[nbrVertIndices[0]].size() > 5))
		&&
		((isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 3) ||
		(!isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 5))
		&&
		((isOnBoundary[2] && noVertexEdgeList[nbrVertIndices[2]].size() > 3) ||
		(!isOnBoundary[2] && noVertexEdgeList[nbrVertIndices[2]].size() > 5))))
		return false;

	// check constraint violations
	double angle = getAngle(noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
								noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
								noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;
	angle = getAngle(noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
					noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
					noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;
	angle = getAngle(noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
					noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
					noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2]);
	if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
		return false;


	// ----- begin remove + retriangulate -----
	// list of poly indices that are unused
	vector<int> unusedPolyIdxList;
	// list of vertex indices that are unused
	vector<int> unusedVertexIdxList;
	// list of edge indices that are unused
	vector<unsigned int> unusedEdgeIdxList;

	unusedVertexIdxList.push_back(novIdx);

	// remove vertex-to-poly references
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		unusedPolyIdxList.push_back(polyIdx);
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[polyIdx][j];
			if (vIdx == novIdx)		// skip removing for novIdx
				continue;

			vector<int>::iterator iter = find(noVertexNeighbourList[vIdx].begin(), noVertexNeighbourList[vIdx].end(), (int)polyIdx);
			if (iter != noVertexNeighbourList[vIdx].end())
				noVertexNeighbourList[vIdx].erase(iter);
		}
	}
	noVertexNeighbourList[novIdx].clear();

	// remove vertex-to-edge references
	unsigned int numNbrEdges = noVertexEdgeList[novIdx].size();
	for (unsigned int i = 0; i < numNbrEdges; ++i)
	{
		unsigned int edgeIdx = noVertexEdgeList[novIdx][i];
		unsigned int vIdx = noEdgeList[edgeIdx].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[edgeIdx].vertex2Idx;

		for (vector<int>::iterator iter = noVertexEdgeList[vIdx].begin(); iter != noVertexEdgeList[vIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)vIdx && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)vIdx))
			{
				noVertexEdgeList[vIdx].erase(iter);
				break;
			}
		}

		unusedEdgeIdxList.push_back(edgeIdx);
	}
	noVertexEdgeList[novIdx].clear();
	
	// create the new polys
	unsigned int newPolyIdx = unusedPolyIdxList.back();
	unusedPolyIdxList.pop_back();
	noPolygonsList[newPolyIdx][0] = nbrVertIndices[0];
	noPolygonsList[newPolyIdx][1] = nbrVertIndices[1];
	noPolygonsList[newPolyIdx][2] = nbrVertIndices[2];

	// assign vertex-to-poly refs
	for (unsigned int j = 0; j < 3; ++j)
		noVertexNeighbourList[noPolygonsList[newPolyIdx][j]].push_back(newPolyIdx);

	// remove unused vertices and polys
	//unsigned int old_novIdx = novIdx;
	removeUnused(unusedPolyIdxList, unusedVertexIdxList, novIdx);

	// remove unused edges
	removeUnused(unusedEdgeIdxList);

	return true;
}

// remove the interior vertex 'novIdx' and retriangulate region
//
// novIdx (in): index of vertex to be removed (interior vertex)
// region (in): determines how the non-obtuse region is to be approximated 
// angleBound (in): angle bound (in radians)
// return value (out): true if successful
// TODO: test this!
bool NonobtOptn::removeInterior4AndTriangulate(unsigned int novIdx, NORegionType region, double angleBound)
{
	unsigned int numNbrs = noVertexEdgeList[novIdx].size();

	if (numNbrs != 4)
		return false;

	// Get indices of neighbour vertices
	vector<unsigned int> nbrVertIndices;
	if (!getOneRingVertices(novIdx, nbrVertIndices)) {
		cout << "Error: NonoptOptn::removeInterior4AndTriangulate() - cannot get one-ring vertices for interior vertex " << novIdx << endl;
		return false;
	}

	// sanity check
	if (nbrVertIndices.size() != 4) {
		cout << "Error: NonoptOptn::removeInterior4AndTriangulate() - number of one-ring vertices for interior vertex " << novIdx << " is not 4." << endl;
		return false;
	}
	
	// check valence of diagonal vertices
	bool isOnBoundary[4] = {isBoundaryVertex(nbrVertIndices[0]), isBoundaryVertex(nbrVertIndices[1]), isBoundaryVertex(nbrVertIndices[2]), isBoundaryVertex(nbrVertIndices[3])};
	int splitAt = -1;
	if (((isOnBoundary[0] && noVertexEdgeList[nbrVertIndices[0]].size() > 4) || (!isOnBoundary[0] && noVertexEdgeList[nbrVertIndices[0]].size() > 5))
		&&
		((isOnBoundary[2] && noVertexEdgeList[nbrVertIndices[2]].size() > 4) || (!isOnBoundary[2] && noVertexEdgeList[nbrVertIndices[2]].size() > 5)))
	{
		bool angleViolated(false);
		double angle = getAngle(noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
								noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
								noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
		if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
			angleViolated = true;

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
							noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2],
							noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
							noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
							noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2],
							noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
							noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
							noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
							noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			splitAt = 1;
		}
	}

	if (splitAt == -1 &&
			((isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 4) || (!isOnBoundary[1] && noVertexEdgeList[nbrVertIndices[1]].size() > 5))
			&&
			((isOnBoundary[3] && noVertexEdgeList[nbrVertIndices[3]].size() > 4) || (!isOnBoundary[3] && noVertexEdgeList[nbrVertIndices[3]].size() > 5)))
	{
		bool angleViolated(false);
		double angle = getAngle(noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
								noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
								noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
		if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
			angleViolated = true;

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2],
							noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
							noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[1]], noVerticesList[3*nbrVertIndices[1]+1], noVerticesList[3*nbrVertIndices[1]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2],
							noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
							noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			angle = getAngle(noVerticesList[3*nbrVertIndices[0]], noVerticesList[3*nbrVertIndices[0]+1], noVerticesList[3*nbrVertIndices[0]+2],
							noVerticesList[3*nbrVertIndices[2]], noVerticesList[3*nbrVertIndices[2]+1], noVerticesList[3*nbrVertIndices[2]+2],
							noVerticesList[3*nbrVertIndices[3]], noVerticesList[3*nbrVertIndices[3]+1], noVerticesList[3*nbrVertIndices[3]+2]);
			if (!(angle <= M_PI_2 && (region != ANGLEBOUND_7PLANES || angle >= angleBound)))
				angleViolated = true;
		}

		if (!angleViolated) {
			splitAt = 0;
		}
	}
	
	if (splitAt == -1)
		return false;

	// ----- begin remove + retriangulate -----
	// list of poly indices that are unused
	vector<int> unusedPolyIdxList;
	// list of vertex indices that are unused
	vector<int> unusedVertexIdxList;
	// list of edge indices that are unused
	vector<unsigned int> unusedEdgeIdxList;

	unusedVertexIdxList.push_back(novIdx);

	// remove vertex-to-poly references
	unsigned int numNbrFaces = noVertexNeighbourList[novIdx].size();
	for (unsigned int i = 0; i < numNbrFaces; ++i)
	{
		unsigned int polyIdx = noVertexNeighbourList[novIdx][i];
		unusedPolyIdxList.push_back(polyIdx);
		for (unsigned int j = 0; j < 3; ++j)
		{
			unsigned int vIdx = noPolygonsList[polyIdx][j];
			if (vIdx == novIdx)		// skip removing for novIdx
				continue;

			vector<int>::iterator iter = find(noVertexNeighbourList[vIdx].begin(), noVertexNeighbourList[vIdx].end(), (int)polyIdx);
			if (iter != noVertexNeighbourList[vIdx].end())
				noVertexNeighbourList[vIdx].erase(iter);
		}
	}
	noVertexNeighbourList[novIdx].clear();

	// remove vertex-to-edge references
	unsigned int numNbrEdges = noVertexEdgeList[novIdx].size();
	for (unsigned int i = 0; i < numNbrEdges; ++i)
	{
		unsigned int edgeIdx = noVertexEdgeList[novIdx][i];
		unsigned int vIdx = noEdgeList[edgeIdx].vertex1Idx;
		if (vIdx == novIdx)
			vIdx = noEdgeList[edgeIdx].vertex2Idx;

		for (vector<int>::iterator iter = noVertexEdgeList[vIdx].begin(); iter != noVertexEdgeList[vIdx].end(); iter++)
		{
			if ((noEdgeList[*iter].vertex1Idx == (int)vIdx && noEdgeList[*iter].vertex2Idx == (int)novIdx) ||
				(noEdgeList[*iter].vertex1Idx == (int)novIdx && noEdgeList[*iter].vertex2Idx == (int)vIdx))
			{
				noVertexEdgeList[vIdx].erase(iter);
				break;
			}
		}

		unusedEdgeIdxList.push_back(edgeIdx);
	}
	noVertexEdgeList[novIdx].clear();
	
	// create the new polys
	for (unsigned int i = 0; i < 2; ++i)
	{
		unsigned int newPolyIdx = unusedPolyIdxList.back();
		unusedPolyIdxList.pop_back();
		noPolygonsList[newPolyIdx][0] = nbrVertIndices[splitAt];
		noPolygonsList[newPolyIdx][1] = nbrVertIndices[(splitAt+1)%4];
		noPolygonsList[newPolyIdx][2] = nbrVertIndices[(splitAt+2)%4];
		if (i == 1) {
			noPolygonsList[newPolyIdx][1] = nbrVertIndices[(splitAt+2)%4];
			noPolygonsList[newPolyIdx][2] = nbrVertIndices[(splitAt+3)%4];
		}

		// assign vertex-to-poly refs
		for (unsigned int j = 0; j < 3; ++j)
			noVertexNeighbourList[noPolygonsList[newPolyIdx][j]].push_back(newPolyIdx);
	}

	// assign vertex-to-edge refs
	unsigned int newEdgeIdx = unusedEdgeIdxList.back();
	unusedEdgeIdxList.pop_back();
	Edge &e = noEdgeList[newEdgeIdx];
	e.vertex1Idx = nbrVertIndices[splitAt];
	e.vertex2Idx = nbrVertIndices[(splitAt+2)%4];
	if (e.vertex1Idx > e.vertex2Idx)
		swap(e.vertex1Idx, e.vertex2Idx);

	noVertexEdgeList[e.vertex1Idx].push_back(newEdgeIdx);
	noVertexEdgeList[e.vertex2Idx].push_back(newEdgeIdx);

	// remove unused vertices and polys
	//unsigned int old_novIdx = novIdx;
	removeUnused(unusedPolyIdxList, unusedVertexIdxList, novIdx);

	// remove unused edges
	removeUnused(unusedEdgeIdxList);

	return true;
}

// returns the list of one-ring vertices for a vertex on the nonobtuse mesh into oneRingVertices (ordered by right hand rule)
// novIdx (in): index of Vertex
// oneRingVertices (out): list of one-ring vertices
// return value (out): true - if no duplicates in one-ring
bool NonobtOptn::getOneRingVertices(int novIdx, vector<unsigned int> &oneRingVertices) const
{
	vector<int> nbrFaces = noVertexNeighbourList[novIdx];
	vector<int>::iterator iter = nbrFaces.begin();
	while (iter != nbrFaces.end())
	{
		unsigned int polyIdx = *iter;
		Edge e;
		for (unsigned int i = 0; i < 3; ++i) {
			if (noPolygonsList[polyIdx][i] == novIdx) {
				e.vertex1Idx = noPolygonsList[polyIdx][(i+1)%3];
				e.vertex2Idx = noPolygonsList[polyIdx][(i+2)%3];
				break;
			}
		}

		if (oneRingVertices.empty()) {
			oneRingVertices.push_back(e.vertex1Idx);
			oneRingVertices.push_back(e.vertex2Idx);
			nbrFaces.erase(iter);
			iter = nbrFaces.begin();
		}
		else if (e.vertex1Idx == (int)oneRingVertices.back() && e.vertex2Idx == (int)oneRingVertices.front()) {
			nbrFaces.erase(iter);
			iter = nbrFaces.begin();
		}
		else if (e.vertex1Idx == (int)oneRingVertices.back()) {
			oneRingVertices.push_back(e.vertex2Idx);
			nbrFaces.erase(iter);
			iter = nbrFaces.begin();
		}
		else if (e.vertex2Idx == (int)oneRingVertices.front()) {
			oneRingVertices.insert(oneRingVertices.begin(), e.vertex1Idx);
			nbrFaces.erase(iter);
			iter = nbrFaces.begin();
		}
		else {
			iter++;
		}
	}

	return nbrFaces.empty();
}

// compute optimal location for vertex (novIdx) that minimizes quadric q using the linear search method
// returns true if successful
// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
// q (in): quadric value of novIdx
// pt (in/out): location before/location after
// angleBound (in): angle bound
// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
//                           on the one-ring vertices for the edge
bool NonobtOptn::computeOptimalLocation_linearSearch(int novIdx, Quadric q, double *pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing)
{
// 	PlaneEqn* constraints = NULL;
// 	unsigned int numConstraints = getNonobtuseRegion(novIdx, constraints, ANGLEBOUND_7PLANES, bEdgeOneRing, angleBound);

// 	angleBound = angleBound*(M_PI/180.0);	// convert angleBound to radian
	double old_pt[3] = {pt[0], pt[1], pt[2]};

	// compute optimal location based on quadric q
	// Ax = -b
	unsigned int matrixDim = 3;
	for (unsigned int i = 0; i < matrixDim; ++i)
		pt[i] = -1.0*q.fn.b[i];
	double *L = new double[(matrixDim*(matrixDim+1))/2];
	cholesky(q.fn.A, matrixDim, L);
	L_backSub(L, matrixDim, pt, 1);
	LT_backSub(L, matrixDim, pt, 1);
	delete[] L;

	// do linear search to find best position that satisfy the constraints
	double optimal_pt[3] = {pt[0], pt[1], pt[2]};
	double len = length(optimal_pt[0]-old_pt[0], optimal_pt[1]-old_pt[1], optimal_pt[2]-old_pt[2]);
	
	double bestPt[3] = {0,0,0};
	double bestImprovementValue = -1;
	bool foundGoodPt(false);

	const unsigned int NUMDIR = 10;
	for (unsigned int i = 0; i < NUMDIR; ++i)
	{
		if (i > 0)
		{
			double randDir[3] = {rand(), rand(), rand()};
			normalize(randDir);
			old_pt[0] = optimal_pt[0] + randDir[0] * len;
			old_pt[1] = optimal_pt[1] + randDir[1] * len;
			old_pt[2] = optimal_pt[2] + randDir[2] * len;
		}

		if (computeOptimalLocation_linearSearch_aux(novIdx, old_pt, optimal_pt, angleBound, numTries, bEdgeOneRing))
		{
			double minDist(0.0);
			double closestPt[3] = {0,0,0};
			double qValue_afterMove(0);
			double qValueImprovement(0);
			if (isBoundaryVertex(q.vIdx))
			{
				Edge closestBEdge_afterMove = findClosestBoundaryEdge(q.vIdx, old_pt, closestPt, 1, false);
				qValue_afterMove = computeBoundaryQuadricValue(old_pt, closestPt, 0.5, 1);
			}
			else
			{
				int closestPolyIdx_afterMove = findClosestPolygon(old_pt, dVerticesList, dPolygonsList, dVertexNeighbourList, numDPolygons, minDist, closestPt, false, closestFaceList[i], 1);
				qValue_afterMove = computeQuadricValue(old_pt, closestPolyIdx_afterMove, 0.5, 1);
			}
			qValueImprovement = q.quadricValue - qValue_afterMove;

			if (!foundGoodPt || qValueImprovement > bestImprovementValue)
			{
				bestPt[0] = old_pt[0];
				bestPt[1] = old_pt[1];
				bestPt[2] = old_pt[2];
				bestImprovementValue = qValueImprovement;
				foundGoodPt = true;
			}
		}
	}

	return foundGoodPt;

// 	const unsigned int NUMTRIES = 10;
// 	const double INTERVAL = 1.0/NUMTRIES;
// 	bool foundGoodPt(false);
// 	numTries = NUMTRIES;
// 	for (unsigned int i = 0; i < NUMTRIES; ++i)
// 	{
// 		for (unsigned int j = 0; j < 3; ++j)
// 			pt[j] = old_pt[j] * (INTERVAL * i) + optimal_pt[j] * (1 - INTERVAL * i);
// 
// 		// test if current pt satisfies constraints, if so, stop
// 		foundGoodPt = true;	
// 
// 		// storage for the two edges
// 		double edgeA[3] = {0,0,0};
// 		double edgeB[3] = {0,0,0};
// 		
// 		// vertex idx for the endpoints of edge
// 		unsigned int edgeVIdx1(0);
// 		unsigned int edgeVIdx2(0);
// 		if (bEdgeOneRing) {
// 			edgeVIdx1 = noEdgeList[novIdx].vertex1Idx;
// 			edgeVIdx2 = noEdgeList[novIdx].vertex2Idx;
// 		}
// 
// 
// 
// 		// DEBUG ONLY: check conservative region only
// // 		for (unsigned int j = 0; j < numConstraints; ++j) {
// // 			if (constraints[j].a * pt[0] + constraints[j].b * pt[1] + constraints[j].c * pt[2] + constraints[j].d < 0.0) {
// // 				foundGoodPt = false;
// // 				break;
// // 			}
// // 		}
// // 		if (foundGoodPt)
// // 			break;
// 		// END DEBUG
// 
// 
// 		for (unsigned int k = 0; k < 2; ++k)
// 		{
// 			if (k == 1 && !bEdgeOneRing)	// stop looping if we are considering vertexIdx
// 				break;
// 	
// 			unsigned int vIdx = novIdx;
// 			unsigned int otherIdx = novIdx;
// 			if (bEdgeOneRing && k == 0) {
// 				vIdx = edgeVIdx1;
// 				otherIdx = edgeVIdx2;
// 			} else if (bEdgeOneRing && k == 1) {
// 				vIdx = edgeVIdx2;
// 				otherIdx = edgeVIdx1;
// 			}
// 
// 			for (vector<int>::const_iterator iter = noVertexNeighbourList[vIdx].begin();
// 				iter != noVertexNeighbourList[vIdx].end(); iter++)
// 			{
// 				// get the two other vertex that makes up the two edges
// 				int v1Idx(-1);
// 				int v2Idx(-1);
// 				for (int i = 0; i < 3; ++i)
// 				{
// 					if (noPolygonsList[*iter][i] == (int)vIdx)
// 					{
// 						int nextIdx = (i+1)%3;
// 						int prevIdx = i-1;
// 						if (i == 0)
// 							prevIdx = 2;
// 						v1Idx = noPolygonsList[*iter][prevIdx];
// 						v2Idx = noPolygonsList[*iter][nextIdx];
// 						break;
// 					}
// 				}
// 
// 				// ignore triangle that share both edge endpoints
// 				// only check if we are considering edges
// 				if (bEdgeOneRing && (v1Idx == (int)otherIdx || v2Idx == (int)otherIdx))
// 					continue;
// 
// 				// sanity checks
// 				if (v1Idx > -1 && v2Idx > -1)
// 				{
// 					double v[3][3] = {{pt[0], pt[1], pt[2]}, {noVerticesList[3*v1Idx], noVerticesList[3*v1Idx+1], noVerticesList[3*v1Idx+2]}, {noVerticesList[3*v2Idx], noVerticesList[3*v2Idx+1], noVerticesList[3*v2Idx+2]}};
// 	
// 					for (unsigned int i = 0; i < 3; ++i)
// 					{
// 						// form the 2 edges
// 						vectorSub(edgeA, 
// 								v[(i+1)%3][0], v[(i+1)%3][1], v[(i+1)%3][2],
// 								v[i][0], v[i][1], v[i][2]);
// 	
// 						vectorSub(edgeB, 
// 								v[(i+2)%3][0], v[(i+2)%3][1], v[(i+2)%3][2],
// 								v[i][0], v[i][1], v[i][2]);
// 			
// 						// found an obtuse angle
// 						double angle = getAngle(edgeA, edgeB);
// 						if (angle > M_PI_2 || angle < angleBound) {
// 							foundGoodPt = false;
// 							break;
// 						}
// 					}
// 	
// 					if (!foundGoodPt)
// 						break;
// 				}
// 			}
// 
// 			if (!foundGoodPt)
// 				break;
// 		}
// 
// 		if (foundGoodPt) {
// 			numTries = i+1;
// 
// 			++numLinSearchFound;
// 			bool inFeasibleRegion(true);
// 			for (unsigned int j = 0; j < numConstraints; ++j) {
// 				if (constraints[j].a * pt[0] + constraints[j].b * pt[1] + constraints[j].c * pt[2] + constraints[j].d < 0.0) {
// 					inFeasibleRegion = false;
// 					break;
// 				}
// 			}
// 
// 			if (inFeasibleRegion)
// 				++numLinSearchInFeasibleRegion;
// 
//  			break;
// 		}
// 	}
// 
//  	return foundGoodPt;
}


// aux. function for computeOptimalLocation_linearSearch; does linear search between old_pt and optimal_pt
// returns true if successful
// novIdx (in): index of the vertex that is being moved (or edge index if bEdgeOneRing is set to true
// old_pt (in/out): one end of linear search / best search point
// optimal_pt (in): the other end of linear search
// angleBound (in): angle bound
// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
//                           on the one-ring vertices for the edge
bool NonobtOptn::computeOptimalLocation_linearSearch_aux(int novIdx, double *old_pt, double *optimal_pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing)
{
	PlaneEqn* constraints = NULL;
	unsigned int numConstraints = getNonobtuseRegion(novIdx, constraints, ANGLEBOUND_7PLANES, bEdgeOneRing, angleBound);

	angleBound = angleBound*(M_PI/180.0);	// convert angleBound to radian

	const unsigned int NUMTRIES = 10;
	const double INTERVAL = 1.0/NUMTRIES;
	bool foundGoodPt(false);
	numTries = NUMTRIES;
	double pt[3] = {optimal_pt[0], optimal_pt[1], optimal_pt[2]};
	for (unsigned int i = 0; i < NUMTRIES; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
			pt[j] = old_pt[j] * (INTERVAL * i) + optimal_pt[j] * (1 - INTERVAL * i);

		// test if current pt satisfies constraints, if so, stop
		foundGoodPt = true;	

		// storage for the two edges
		double edgeA[3] = {0,0,0};
		double edgeB[3] = {0,0,0};
		
		// vertex idx for the endpoints of edge
		unsigned int edgeVIdx1(0);
		unsigned int edgeVIdx2(0);
		if (bEdgeOneRing) {
			edgeVIdx1 = noEdgeList[novIdx].vertex1Idx;
			edgeVIdx2 = noEdgeList[novIdx].vertex2Idx;
		}



		// DEBUG ONLY: check conservative region only
// 		for (unsigned int j = 0; j < numConstraints; ++j) {
// 			if (constraints[j].a * pt[0] + constraints[j].b * pt[1] + constraints[j].c * pt[2] + constraints[j].d < 0.0) {
// 				foundGoodPt = false;
// 				break;
// 			}
// 		}
// 		if (foundGoodPt)
// 			break;
		// END DEBUG


		for (unsigned int k = 0; k < 2; ++k)
		{
			if (k == 1 && !bEdgeOneRing)	// stop looping if we are considering vertexIdx
				break;
	
			unsigned int vIdx = novIdx;
			unsigned int otherIdx = novIdx;
			if (bEdgeOneRing && k == 0) {
				vIdx = edgeVIdx1;
				otherIdx = edgeVIdx2;
			} else if (bEdgeOneRing && k == 1) {
				vIdx = edgeVIdx2;
				otherIdx = edgeVIdx1;
			}

			for (vector<int>::const_iterator iter = noVertexNeighbourList[vIdx].begin();
				iter != noVertexNeighbourList[vIdx].end(); iter++)
			{
				// get the two other vertex that makes up the two edges
				int v1Idx(-1);
				int v2Idx(-1);
				for (int i = 0; i < 3; ++i)
				{
					if (noPolygonsList[*iter][i] == (int)vIdx)
					{
						int nextIdx = (i+1)%3;
						int prevIdx = i-1;
						if (i == 0)
							prevIdx = 2;
						v1Idx = noPolygonsList[*iter][prevIdx];
						v2Idx = noPolygonsList[*iter][nextIdx];
						break;
					}
				}

				// ignore triangle that share both edge endpoints
				// only check if we are considering edges
				if (bEdgeOneRing && (v1Idx == (int)otherIdx || v2Idx == (int)otherIdx))
					continue;

				// sanity checks
				if (v1Idx > -1 && v2Idx > -1)
				{
					double v[3][3] = {{pt[0], pt[1], pt[2]}, {noVerticesList[3*v1Idx], noVerticesList[3*v1Idx+1], noVerticesList[3*v1Idx+2]}, {noVerticesList[3*v2Idx], noVerticesList[3*v2Idx+1], noVerticesList[3*v2Idx+2]}};
	
					for (unsigned int i = 0; i < 3; ++i)
					{
						// form the 2 edges
						vectorSub(edgeA, 
								v[(i+1)%3][0], v[(i+1)%3][1], v[(i+1)%3][2],
								v[i][0], v[i][1], v[i][2]);
	
						vectorSub(edgeB, 
								v[(i+2)%3][0], v[(i+2)%3][1], v[(i+2)%3][2],
								v[i][0], v[i][1], v[i][2]);
			
						// found an obtuse angle
						double angle = getAngle(edgeA, edgeB);
						if (angle > M_PI_2 || angle < angleBound) {
							foundGoodPt = false;
							break;
						}
					}
	
					if (!foundGoodPt)
						break;
				}
			}

			if (!foundGoodPt)
				break;
		}

		if (foundGoodPt) {
			numTries = i+1;

			++numLinSearchFound;
			bool inFeasibleRegion(true);
			for (unsigned int j = 0; j < numConstraints; ++j) {
				if (constraints[j].a * pt[0] + constraints[j].b * pt[1] + constraints[j].c * pt[2] + constraints[j].d < 0.0) {
					inFeasibleRegion = false;
					break;
				}
			}

			if (inFeasibleRegion)
				++numLinSearchInFeasibleRegion;

			old_pt[0] = pt[0];
			old_pt[1] = pt[1];
			old_pt[2] = pt[2];

 			break;
		}
	}

 	return foundGoodPt;
}

// compute optimal location for vertex (novIdx) that minimizes quadric q using the radii random probing method
// returns true if successful
// novIdx (in): index of the vertex that is being moved (or edge index) if bEdgeOneRing is set to true
// q (in): quadric value of novIdx
// pt (in/out): location before/location after
// angleBound (in): angle bound
// bEdgeOneRing (in): true - novIdx is treated as an edge index, then non-obtuse region are computed based
//                           on the one-ring vertices for the edge
// TODO: add code for edges
bool NonobtOptn::computeOptimalLocation_radiiRandomProbing(int novIdx, Quadric q, double *pt, double angleBound, unsigned int &numTries, bool bEdgeOneRing)
{
	angleBound = angleBound*(M_PI/180.0);	// convert angleBound to radian
	double old_pt[3] = {pt[0], pt[1], pt[2]};

	// compute optimal location based on quadric q
	// Ax = -b
	unsigned int matrixDim = 3;
	for (unsigned int i = 0; i < matrixDim; ++i)
		pt[i] = -1.0*q.fn.b[i];
	double *L = new double[(matrixDim*(matrixDim+1))/2];
	cholesky(q.fn.A, matrixDim, L);
	L_backSub(L, matrixDim, pt, 1);
	LT_backSub(L, matrixDim, pt, 1);
	delete[] L;

	// do radii random probing to find best position that satisfy the constraints
	double optimal_pt[3] = {pt[0], pt[1], pt[2]};
	const unsigned int NUMRADII = 10;
	const unsigned int NUMTRIESPERRADII = 3;
	const unsigned int NUMTRIES = NUMRADII * NUMTRIESPERRADII;
	const double MAXRADII = length(optimal_pt[0]-old_pt[0], optimal_pt[1]-old_pt[1], optimal_pt[2]-old_pt[2]);
	const double RADII = MAXRADII / NUMRADII;
 	
	bool foundGoodPt(true);
 	numTries = NUMTRIES;
	double curRadii = 0;
	for (unsigned int i = 0; i < NUMTRIES; ++i)
	{
		// ---- probe for next point ----
		// random probe a direction
		double dirVect[3] = {rand() / RAND_MAX, rand() / RAND_MAX, rand() / RAND_MAX};
		normalize(dirVect);
		if (i % NUMTRIESPERRADII == 0)
			curRadii += RADII;
		
		// random probe distance
		double t = rand() / RAND_MAX;

		// random point then becomes ...
		for (unsigned int j = 0; j < 3; ++j)
 			pt[j] = optimal_pt[j] + dirVect[j]*curRadii*t;

		// test if current pt satisfies constraints, if so, stop
		foundGoodPt = true;	

		// storage for the two edges
		double* edgeA = new double[3];
		double* edgeB = new double[3];
		
		for (vector<int>::const_iterator iter = noVertexNeighbourList[novIdx].begin();
			iter != noVertexNeighbourList[novIdx].end(); iter++)
		{
			// get the two other vertex that makes up the two edges
			int v1Idx(-1);
			int v2Idx(-1);
			for (int i = 0; i < 3; ++i)
			{
				if (noPolygonsList[*iter][i] == novIdx)
				{
					int nextIdx = (i+1)%3;
					int prevIdx = i-1;
					if (i == 0)
						prevIdx = 2;
					v1Idx = noPolygonsList[*iter][prevIdx];
					v2Idx = noPolygonsList[*iter][nextIdx];
					break;
				}
			}
			// sanity checks
			if (v1Idx > -1 && v2Idx > -1)
			{
				double v[3][3] = {{pt[0], pt[1], pt[2]}, {noVerticesList[3*v1Idx], noVerticesList[3*v1Idx+1], noVerticesList[3*v1Idx+2]}, {noVerticesList[3*v2Idx], noVerticesList[3*v2Idx+1], noVerticesList[3*v2Idx+2]}};

				for (unsigned int i = 0; i < 3; ++i)
				{
					// form the 2 edges
					vectorSub(edgeA, 
							v[(i+1)%3][0], v[(i+1)%3][1], v[(i+1)%3][2],
							v[i][0], v[i][1], v[i][2]);

					vectorSub(edgeB, 
							v[(i+2)%3][0], v[(i+2)%3][1], v[(i+2)%3][2],
							v[i][0], v[i][1], v[i][2]);
		
					// found an obtuse angle
					double angle = getAngle(edgeA, edgeB);
					if (angle > M_PI_2 || angle < angleBound) {
						foundGoodPt = false;
						break;
					}
				}

				if (!foundGoodPt)
					break;
			}
		}
	
		// clean up
		delete[] edgeA;
		delete[] edgeB;

		if (foundGoodPt) {
			numTries = i+1;
			break;
		}
	}
	
	return foundGoodPt;
}
