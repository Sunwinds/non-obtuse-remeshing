/***************************************************************************
 *   Copyright (C) 2005 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/
#include "nonobtuse.h"

Nonobtuse::Nonobtuse()
{
}


Nonobtuse::~Nonobtuse()
{
}

// remove all old obtuse angles
//
void Nonobtuse::clearObtuseAngles()
{
	obtuseList.clear();
}

// returns angles histogram
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// stepSize (in): size of each bin in the histogram
// histogram (out): angles histogram
void Nonobtuse::getAngleHistogram(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, float stepSize, vector<int> &histogram)
{
	double radPerBin = stepSize * M_PI / 180.0;
	int numBins = (int)ceil(M_PI/radPerBin);
	histogram.resize(numBins,0);
	for (int i = 0; i < numBins; ++i)
		histogram[i] = 0;

	double minAngle = -1.0;

	for (int i = 0; i < numPolygons; ++i)
	{
		double edgeA[3] = {0,0,0};
		double edgeB[3] = {0,0,0};
		for (int j = 0; j < 3; ++j)
		{
			int prevIdx = j-1;
			if (j == 0)
				prevIdx = 2;
			
			vectorSub(edgeA, verticesList[3*polygonsList[i][prevIdx]], 
					verticesList[3*polygonsList[i][prevIdx]+1],
					verticesList[3*polygonsList[i][prevIdx]+2],
					verticesList[3*polygonsList[i][j]], 
					verticesList[3*polygonsList[i][j]+1],
					verticesList[3*polygonsList[i][j]+2]);
					
			int nextIdx = (j+1)%3;			
			vectorSub(edgeB, verticesList[3*polygonsList[i][nextIdx]], 
					verticesList[3*polygonsList[i][nextIdx]+1],
					verticesList[3*polygonsList[i][nextIdx]+2],
					verticesList[3*polygonsList[i][j]], 
					verticesList[3*polygonsList[i][j]+1],
					verticesList[3*polygonsList[i][j]+2]);
					
			double angle = getAngle(edgeA, edgeB);
			int bin = (int)floor(angle / radPerBin);
			histogram[bin] += 1;

			if (minAngle == -1.0 || minAngle > angle)
				minAngle = angle;
		}
	}

	cout << "min. angle = " << minAngle << "rad (" << minAngle*180/M_PI << " degrees)" << endl;
}

// find all obtuse angles in mesh and set it to obtuseList
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
void Nonobtuse::findObtuseAngles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons)
{
	// remove old obtuse angles entries
	if (!obtuseList.empty())
		clearObtuseAngles();

	// storage for the two edges
	double* edgeA = new double[3];
	double* edgeB = new double[3];
	// go through each polygon and check its angles
	for (int i = 0; i < numPolygons; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int prevIdx = j-1;
			if (j == 0)
				prevIdx = 2;
			
			vectorSub(edgeA, verticesList[3*polygonsList[i][prevIdx]], 
					verticesList[3*polygonsList[i][prevIdx]+1],
					verticesList[3*polygonsList[i][prevIdx]+2],
					verticesList[3*polygonsList[i][j]], 
					verticesList[3*polygonsList[i][j]+1],
					verticesList[3*polygonsList[i][j]+2]);
					
			int nextIdx = (j+1)%3;			
			vectorSub(edgeB, verticesList[3*polygonsList[i][nextIdx]], 
					verticesList[3*polygonsList[i][nextIdx]+1],
					verticesList[3*polygonsList[i][nextIdx]+2],
					verticesList[3*polygonsList[i][j]], 
					verticesList[3*polygonsList[i][j]+1],
					verticesList[3*polygonsList[i][j]+2]);
					
			// found an obtuse angle
			if (getAngle(edgeA, edgeB) > M_PI_2)
			//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
			{
				ObtuseAngle obAngle;
				obAngle.vertexIdx = polygonsList[i][j];
 				obAngle.neighbour1Idx = polygonsList[i][prevIdx];
 				obAngle.neighbour2Idx = polygonsList[i][nextIdx];
				obAngle.polyIdx = i;
				
				obtuseList.push_back(obAngle);
			}
		}
	}
	
	// clean up
	delete[] edgeA;
	delete[] edgeB;
}

// find all obtuse angles in a polygon
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// polyIndex (in): index of the polygon
// obAngleList (out): vector of ObtuseAngles found in polygon polyIndex
void Nonobtuse::findObtuseAngles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, int polyIndex, vector<ObtuseAngle> &obAngleList)
{
	// sanity check
	if (polyIndex >= numPolygons)
		return;
	
	// storage for the two edges
	double* edgeA = new double[3];
	double* edgeB = new double[3];
	
	for (int j = 0; j < 3; ++j)
	{
		int prevIdx = j-1;
		if (j == 0)
			prevIdx = 2;
		
		vectorSub(edgeA, verticesList[3*polygonsList[polyIndex][prevIdx]], 
				verticesList[3*polygonsList[polyIndex][prevIdx]+1],
				verticesList[3*polygonsList[polyIndex][prevIdx]+2],
				verticesList[3*polygonsList[polyIndex][j]], 
				verticesList[3*polygonsList[polyIndex][j]+1],
				verticesList[3*polygonsList[polyIndex][j]+2]);
				
		int nextIdx = (j+1)%3;			
		vectorSub(edgeB, verticesList[3*polygonsList[polyIndex][nextIdx]], 
				verticesList[3*polygonsList[polyIndex][nextIdx]+1],
				verticesList[3*polygonsList[polyIndex][nextIdx]+2],
				verticesList[3*polygonsList[polyIndex][j]], 
				verticesList[3*polygonsList[polyIndex][j]+1],
				verticesList[3*polygonsList[polyIndex][j]+2]);
				
		// found an obtuse angle
		if (getAngle(edgeA, edgeB) > M_PI_2)
		//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
		{
			ObtuseAngle obAngle;
			obAngle.vertexIdx = polygonsList[polyIndex][j];
			obAngle.neighbour1Idx = polygonsList[polyIndex][prevIdx];
			obAngle.neighbour2Idx = polygonsList[polyIndex][nextIdx];
			obAngle.polyIdx = polyIndex;
			
			obAngleList.push_back(obAngle);
		}
	}
	
	// clean up
	delete[] edgeA;
	delete[] edgeB;
}

// find all obtuse angles in a polygon
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// v1Idx (in): index of first vertex in triangle
// v2Idx (in): index of second vertex in triangle
// v3Idx (in): index of third vertex in triangle
// obAngleList (out): vector of ObtuseAngles found in polygon polyIndex
void Nonobtuse::findObtuseAngles(const double* verticesList, int numVertices, int v1Idx, int v2Idx, int v3Idx, vector<ObtuseAngle> &obAngleList)
{
	// sanity check
	if (v1Idx >= numVertices || v2Idx >= numVertices || v3Idx >= numVertices)
		return;
	
	// storage for the two edges
	double* edgeA = new double[3];
	double* edgeB = new double[3];
	
	// check angle v1Idx,v2Idx,v3Idx
	vectorSub(edgeA, verticesList[3*v1Idx], 
			verticesList[3*v1Idx+1],
			verticesList[3*v1Idx+2],
			verticesList[3*v2Idx], 
			verticesList[3*v2Idx+1],
			verticesList[3*v2Idx+2]);
				
	vectorSub(edgeB, verticesList[3*v3Idx], 
			verticesList[3*v3Idx+1],
			verticesList[3*v3Idx+2],
			verticesList[3*v2Idx], 
			verticesList[3*v2Idx+1],
			verticesList[3*v2Idx+2]);
				
	// found an obtuse angle
	if (getAngle(edgeA, edgeB) > M_PI_2)
	//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
	{
		ObtuseAngle obAngle;
		obAngle.vertexIdx = v2Idx;
		obAngle.neighbour1Idx = v1Idx;
		obAngle.neighbour2Idx = v3Idx;
		
		obAngleList.push_back(obAngle);
	}
		
	// check angle v2Idx,v3Idx,v1Idx
	vectorSub(edgeA, verticesList[3*v2Idx], 
			verticesList[3*v2Idx+1],
			verticesList[3*v2Idx+2],
			verticesList[3*v3Idx], 
			verticesList[3*v3Idx+1],
			verticesList[3*v3Idx+2]);
				
	vectorSub(edgeB, verticesList[3*v1Idx], 
			verticesList[3*v1Idx+1],
			verticesList[3*v1Idx+2],
			verticesList[3*v3Idx], 
			verticesList[3*v3Idx+1],
			verticesList[3*v3Idx+2]);
				
	// found an obtuse angle
	if (getAngle(edgeA, edgeB) > M_PI_2)
	//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
	{
		ObtuseAngle obAngle;
		obAngle.vertexIdx = v3Idx;
		obAngle.neighbour1Idx = v2Idx;
		obAngle.neighbour2Idx = v1Idx;
		
		obAngleList.push_back(obAngle);
	}
	
	// check angle v3Idx,v1Idx,v2Idx
	vectorSub(edgeA, verticesList[3*v3Idx], 
			verticesList[3*v3Idx+1],
			verticesList[3*v3Idx+2],
			verticesList[3*v1Idx], 
			verticesList[3*v1Idx+1],
			verticesList[3*v1Idx+2]);
				
	vectorSub(edgeB, verticesList[3*v2Idx], 
			verticesList[3*v2Idx+1],
			verticesList[3*v2Idx+2],
			verticesList[3*v1Idx], 
			verticesList[3*v1Idx+1],
			verticesList[3*v1Idx+2]);
				
	// found an obtuse angle
	if (getAngle(edgeA, edgeB) > M_PI_2)
	//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
	{
		ObtuseAngle obAngle;
		obAngle.vertexIdx = v1Idx;
		obAngle.neighbour1Idx = v3Idx;
		obAngle.neighbour2Idx = v2Idx;
		
		obAngleList.push_back(obAngle);
	}
	
	// clean up
	delete[] edgeA;
	delete[] edgeB;
}

// find all obtuse angles within the one ring of vertexIdx
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// vertexIdx (in): index of the vertex we are checking
// vertexNeighbourList (in): list of vertex neighbours for the mesh
// obAngleList (out): vector of ObtuseAngles found in the one ring
void Nonobtuse::findObtuseAngles(const double* verticesList, int numVertices, 
				const int* const* polygonsList, int numPolygons,
				int vertexIdx, const vector<int>* vertexNeighbourList, vector<ObtuseAngle> &obAngleList)
{
	// sanity checks
	if (vertexIdx >= numVertices)
		return;

	// storage for the two edges
	double* edgeA = new double[3];
	double* edgeB = new double[3];
	
	for (vector<int>::const_iterator iter = vertexNeighbourList[vertexIdx].begin();
	     iter != vertexNeighbourList[vertexIdx].end(); iter++)
	{
		// get the two other vertex that makes up the two edges
		int v1Idx(-1);
		int v2Idx(-1);
		for (int i = 0; i < 3; ++i)
		{
			if (polygonsList[*iter][i] == vertexIdx)
			{
				int nextIdx = (i+1)%3;
				int prevIdx = i-1;
				if (i == 0)
					prevIdx = 2;
				v1Idx = polygonsList[*iter][prevIdx];
				v2Idx = polygonsList[*iter][nextIdx];
				break;
			}
		}
		// sanity checks
		if (v1Idx > -1 && v2Idx > -1)
		{
			// form the 2 edges
			vectorSub(edgeA, verticesList[3*v1Idx], 
					verticesList[3*v1Idx+1],
					verticesList[3*v1Idx+2],
					verticesList[3*vertexIdx], 
					verticesList[3*vertexIdx+1],
					verticesList[3*vertexIdx+2]);
			vectorSub(edgeB, verticesList[3*v2Idx], 
					verticesList[3*v2Idx+1],
					verticesList[3*v2Idx+2],
					verticesList[3*vertexIdx], 
					verticesList[3*vertexIdx+1],
					verticesList[3*vertexIdx+2]);

			// found an obtuse angle
			if (getAngle(edgeA, edgeB) > M_PI_2)
			//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
			{

				ObtuseAngle obAngle;
				obAngle.vertexIdx = vertexIdx;
				obAngle.neighbour1Idx = v1Idx;
				obAngle.neighbour2Idx = v2Idx;
				
				obAngleList.push_back(obAngle);
			}
		}
	}

	// clean up
	delete[] edgeA;
	delete[] edgeB;
}

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
void Nonobtuse::findObtuseAngles(const double* verticesList, int numVertices, 
				const int* const* polygonsList, int numPolygons,
				double *vertexPos, int vertexIdx, const vector<int>* vertexNeighbourList,
				vector<ObtuseAngle> &obAngleList)
{
	// sanity checks
	if (vertexIdx >= numVertices)
		return;

	// storage for the two edges
	double* edgeA = new double[3];
	double* edgeB = new double[3];
	
	for (vector<int>::const_iterator iter = vertexNeighbourList[vertexIdx].begin();
	     iter != vertexNeighbourList[vertexIdx].end(); iter++)
	{
		// get the two other vertex that makes up the two edges
		int v1Idx(-1);
		int v2Idx(-1);
		for (int i = 0; i < 3; ++i)
		{
			if (polygonsList[*iter][i] == vertexIdx)
			{
				int nextIdx = (i+1)%3;
				int prevIdx = i-1;
				if (i == 0)
					prevIdx = 2;
				v1Idx = polygonsList[*iter][prevIdx];
				v2Idx = polygonsList[*iter][nextIdx];
				break;
			}
		}
		// sanity checks
		if (v1Idx > -1 && v2Idx > -1)
		{
			// form the 2 edges
			vectorSub(edgeA, verticesList[3*v1Idx], 
					verticesList[3*v1Idx+1],
					verticesList[3*v1Idx+2],
					vertexPos[0], 
					vertexPos[1],
					vertexPos[2]);
			vectorSub(edgeB, verticesList[3*v2Idx], 
					verticesList[3*v2Idx+1],
					verticesList[3*v2Idx+2],
					vertexPos[0], 
					vertexPos[1],
					vertexPos[2]);

			// found an obtuse angle
			if (getAngle(edgeA, edgeB) > M_PI_2)
			//if (getAngle(edgeA, edgeB) - M_PI_2 > 0.0001)
			{

				ObtuseAngle obAngle;
				obAngle.vertexIdx = vertexIdx;
				obAngle.neighbour1Idx = v1Idx;
				obAngle.neighbour2Idx = v2Idx;
				
				obAngleList.push_back(obAngle);
			}
		}
	}

	// clean up
	delete[] edgeA;
	delete[] edgeB;
}

// return the list of obtuse angles in mesh
//
// return value: list of obtuse angles in mesh
const vector<ObtuseAngle>& Nonobtuse::getObtuseAngles()
{
	return obtuseList;
}

// remove obtuse angles by blindly removing one at a time
//
// rdr (in): renderer that stores the mesh information

// TODO: There are still triangles that are equivalent to a straight line
// TODO: THere are still cases where an edge cannot be found
void Nonobtuse::blindEdgeFlips(Renderer &rdr)
{
	cout << "--- Starting blind edge-flips ---\n";
	
// 	uint numPrevRemaining(0);
//	uint numNoImprovement(0);
	
	int counter(0);
	
	vector<ObtuseAngle>::iterator iter = obtuseList.begin();
	while(iter != obtuseList.end() && counter < 10000)
	{
		cout << "------------ Iterator " << counter++ << " ------------\n";
// 		cout << "----Edge Neighbour Information----\n";
// 		for (int i = 0; i < rdr.numVertices; ++i)
// 		{
// 			cout << "Vertex " << i << ":\n";
// 			for (vector<EdgeNeighbours>::iterator iter = rdr.edgeNeighbourList[i].begin();
// 			iter != rdr.edgeNeighbourList[i].end(); iter++)
// 			{
// 				cout << "\tEdge connects with vertex " << iter->e.vertex2Idx << endl;
// 				cout << "\tEdge Neighbour 1: " << iter->polyNeighbour1Idx << endl;
// 				cout << "\tEdge Neighbour 2: " << iter->polyNeighbour2Idx << endl;
// 			}	
// 		}
// 		
// 		cout << "----Polygon Information----\n";
// 		for (int i = 0; i < rdr.numPolygons; ++i)
// 		{
// 			cout << "Polygon " << i << ": (";
// 			for (int j = 0; j < 3; ++j)
// 			{	
// 				if (j == 2)
// 					cout << rdr.polygonsList[i][j] << ")\n";
// 				else
// 					cout << rdr.polygonsList[i][j] << ", ";
// 			}
// 		}
// 		
// 		cout << "----Vertex Neighbour Information----\n";
// 		for (int i = 0; i < rdr.numVertices; ++i)
// 		{
// 			cout << "Vertex " << i << ":\n";
// 			for (uint j = 0; j < rdr.vertexNeighbourList[i].size(); ++j)
// 			{
// 				cout << "\tNeighbour " << j << ": " << rdr.vertexNeighbourList[i][j] << endl;
// 			}
// 		}
		
// 		if (numPrevRemaining == obtuseList.size())
// 		{
// 			++numNoImprovement;
// 			if (numNoImprovement > obtuseList.size())
// 				break;
// 		}
// 		else
// 		{
// 			numNoImprovement = 0;
// 		}
		
		if (!isValidObtuseAngle(*iter, rdr.getPolygons(), rdr.getNumPolygons()))
		{
			// remove the invalid obtuse angle
			iter = obtuseList.erase(iter);
			continue;
		}

// 		if (counter == 1328)
// 		{
// 			int foo = 1;
// 			cout << "Capacity = " << obtuseList.capacity() << endl;
// 			cout << "Max Size = " << obtuseList.max_size() << endl;
// 		}
			
		int endPt1Idx(iter->neighbour1Idx);
		int endPt2Idx(iter->neighbour2Idx);
		
		int poly1Idx, poly2Idx;
		int oppositePolyIdx(-1);	// stores the vertex idx on the other side of the obtuse angle
		
		if (rdr.findEdgeNeighbour(endPt1Idx, endPt2Idx, poly1Idx, poly2Idx))
		{
			oppositePolyIdx = poly1Idx;
			if (iter->polyIdx == poly1Idx)
			{
				oppositePolyIdx = poly2Idx;	
			}
		}
		else
		{
// 			int foo = 1;
// 			rdr.findEdgeNeighbour(endPt1Idx, endPt2Idx, poly1Idx, poly2Idx);
		}
	
// 		numPrevRemaining = obtuseList.size();
		//cout << "Number of obtuse angles remaining: " << obtuseList.size() << endl;
		
		// do edge flip
		if (rdr.edgeFlip(endPt1Idx, endPt2Idx, poly1Idx, poly2Idx))
		{
			// ---- update obtuse angle informations ----
			
			vector<ObtuseAngle> obAngleList;
			
			// check if we introduced new obtuse angles
			findObtuseAngles(rdr.getVertices(), rdr.getNumVertices(), rdr.getPolygons(), rdr.getNumPolygons(), poly1Idx, obAngleList);
			findObtuseAngles(rdr.getVertices(), rdr.getNumVertices(), rdr.getPolygons(), rdr.getNumPolygons(), poly2Idx, obAngleList);
			
			// add the newly introduced obtuse angles into the obtuse angles list
			for (vector<ObtuseAngle>::iterator i = obAngleList.begin();
			i != obAngleList.end(); i++)
			{
				obtuseList.push_back(*i);
			}
			
			//cout << "introducing " << obAngleList.size() << " new obtuse angles.\n";
		}			
		
		// remove the opposite angle if exist
		if (oppositePolyIdx != -1)
		{
			for (vector<ObtuseAngle>::iterator iter2 = obtuseList.begin();
			iter2 != obtuseList.end(); iter2++)
			{
				if (iter2->polyIdx == oppositePolyIdx
					&& ((iter2->neighbour1Idx == endPt1Idx && iter2->neighbour2Idx == endPt2Idx)
						||
					(iter2->neighbour2Idx == endPt1Idx && iter2->neighbour1Idx == endPt2Idx)))
				{
					obtuseList.erase(iter2);
					break;
				}
			}
		}
		
		// remove the current obtuse angle
		obtuseList.erase(obtuseList.begin());
		iter = obtuseList.begin();
	}
	
	/*
	cout << "------------ Finished Blind Edge Flip ------------\n";
	cout << "----Edge Neighbour Information----\n";
	for (int i = 0; i < rdr.numVertices; ++i)
	{
		cout << "Vertex " << i << ":\n";
		for (vector<EdgeNeighbours>::iterator iter = rdr.edgeNeighbourList[i].begin();
		iter != rdr.edgeNeighbourList[i].end(); iter++)
		{
			cout << "\tEdge connects with vertex " << iter->e.vertex2Idx << endl;
			cout << "\tEdge Neighbour 1: " << iter->polyNeighbour1Idx << endl;
			cout << "\tEdge Neighbour 2: " << iter->polyNeighbour2Idx << endl;
		}	
	}
	
	cout << "----Polygon Information----\n";
	for (int i = 0; i < rdr.numPolygons; ++i)
	{
		cout << "Polygon " << i << ": (";
		for (int j = 0; j < 3; ++j)
		{	
			if (j == 2)
				cout << rdr.polygonsList[i][j] << ")\n";
			else
				cout << rdr.polygonsList[i][j] << ", ";
		}
	}
	
	cout << "----Vertex Neighbour Information----\n";
	for (int i = 0; i < rdr.numVertices; ++i)
	{
		cout << "Vertex " << i << ":\n";
		for (uint j = 0; j < rdr.vertexNeighbourList[i].size(); ++j)
		{
			cout << "\tNeighbour " << j << ": " << rdr.vertexNeighbourList[i][j] << endl;
		}
	}
	*/
}

// remove obtuse angles by flipping an edge that will not cause new obtuse angles
//
// rdr (in): renderer that stores the mesh information
void Nonobtuse::greedyEdgeFlips(Renderer &rdr)
{
	vector<ObtuseAngle>::iterator iter = obtuseList.begin();
	while(iter != obtuseList.end())
	{
		// edge's end pt
		int endPt1Idx(iter->neighbour1Idx);
		int endPt2Idx(iter->neighbour2Idx);
		
		// affected polygon indices
		int poly1Idx, poly2Idx;
		int oppositePolyIdx(-1);	// stores the vertex idx on the other side of the obtuse angle
		
		// find neighbour polgyon index
		if (rdr.findEdgeNeighbour(endPt1Idx, endPt2Idx, poly1Idx, poly2Idx))
		{
			oppositePolyIdx = poly1Idx;
			if (iter->polyIdx == poly1Idx)
			{
				oppositePolyIdx = poly2Idx;	
			}
		}
	
		if (oppositePolyIdx != -1)
		{
			// find the other vertex within the quad
			int oppositeVertexIdx(-1);
			for (int i = 0; i < 3; ++i)
			{
				if (rdr.getPolygons()[oppositePolyIdx][i] != endPt1Idx && 
				rdr.getPolygons()[oppositePolyIdx][i] != endPt2Idx)
				{
					oppositeVertexIdx = rdr.getPolygons()[oppositePolyIdx][i];
					break;
				}
			}
			
			// check if it will introduce new obtuse angles after edge flip
			if (oppositeVertexIdx != -1 && !obtuseAnglesExist(rdr.getVertices(), rdr.getNumVertices(), iter->vertexIdx, oppositeVertexIdx, endPt1Idx, endPt2Idx))
			{
				// do edge flip
				rdr.edgeFlip(endPt1Idx, endPt2Idx, poly1Idx, poly2Idx);
			}
			
			// remove the opposite angle if exist
			for (vector<ObtuseAngle>::iterator iter2 = obtuseList.begin();
			iter2 != obtuseList.end(); iter2++)
			{
				if (iter2->polyIdx == oppositePolyIdx
					&& ((iter2->neighbour1Idx == endPt1Idx && iter2->neighbour2Idx == endPt2Idx)
						||
					(iter2->neighbour2Idx == endPt1Idx && iter2->neighbour1Idx == endPt2Idx)))
				{
					obtuseList.erase(iter2);
					break;
				}
			}
		}
		// remove the current obtuse angle
		obtuseList.erase(obtuseList.begin());
		iter = obtuseList.begin();
	}
}

// remove obtuse angles by lifting a vertex upwards along the vertex normal direction
//
// rdr (out): renderer that stores the mesh information
void Nonobtuse::liftVertices(Renderer &rdr)
{
	vector<ObtuseAngle>::iterator iter = obtuseList.begin();
	while(iter != obtuseList.end())
	{
	
		liftVertex(iter->vertexIdx, rdr);
	
		// remove the current obtuse angle
		obtuseList.erase(obtuseList.begin());
		iter = obtuseList.begin();
	}
}

// try to find a new vertex position for vertex with index "vertexIdx" such that all neighbour angles of vertexIdx is nonobtuse
//
// newPos (out): returns the new position for the vertex; must be an array of 3 elements
// vertexIdx (in): index of the vertex we are trying to move
// rdr (in): renderer that stores the mesh information
// return value: true if we found a new position, false otherwise
//bool Nonobtuse::findLiftPosition(double *newPos, int vertexIdx, Renderer rdr)
bool Nonobtuse::liftVertex(int vertexIdx, Renderer &rdr)
{
	// sanity check
	if (vertexIdx >= rdr.getNumVertices())
		return false;

	bool result(false);	// stores the result of this function
	
	// find direction to lift vertex
	double* liftDir = new double[3];
	findLiftDirection(vertexIdx, liftDir, rdr);

	// check if we need to lift at all
	bool continueLifting(false);
	vector<ObtuseAngle> obAngleList;
	findObtuseAngles(rdr.getVertices(), rdr.getNumVertices(), 
			rdr.getPolygons(), rdr.getNumPolygons(),
			vertexIdx, rdr.getVertexNeighbourList(), obAngleList);
	if (!obAngleList.empty())
		continueLifting = true;
	

	double *oldVertexPosition = new double[3];
	oldVertexPosition[0] = rdr.getVertices()[3*vertexIdx];
	oldVertexPosition[1] = rdr.getVertices()[3*vertexIdx+1];
	oldVertexPosition[2] = rdr.getVertices()[3*vertexIdx+2];
	double *newVertexPosition = new double[3];
	newVertexPosition[0] = rdr.getVertices()[3*vertexIdx];
	newVertexPosition[1] = rdr.getVertices()[3*vertexIdx+1];
	newVertexPosition[2] = rdr.getVertices()[3*vertexIdx+2];
	
	double liftIncrement(0.001);

	int count(0);	
	// keep lifting the vertex until no obtuse angles exist
	while (continueLifting)
	{

		// find lift position
		newVertexPosition[0] += liftDir[0] * liftIncrement;
		newVertexPosition[1] += liftDir[1] * liftIncrement;
		newVertexPosition[2] += liftDir[2] * liftIncrement;
				
		// check all neighbour angles to see if we still need to lift
		obAngleList.clear();
		findObtuseAngles(rdr.getVertices(), rdr.getNumVertices(), 
				rdr.getPolygons(), rdr.getNumPolygons(),
				newVertexPosition, vertexIdx, rdr.getVertexNeighbourList(), obAngleList);
		if (!obAngleList.empty())
			continueLifting = true;
		else
		{	
			// lift vertex
			rdr.moveVertex(vertexIdx, newVertexPosition[0], newVertexPosition[1], newVertexPosition[2]);
			continueLifting = false;
			result = true;
		}
		
		++count;
		if (count >= 10)
		{
			// lift vertex
			rdr.moveVertex(vertexIdx, newVertexPosition[0], newVertexPosition[1], newVertexPosition[2]);
			continueLifting = false;
		}
	}
	
	// clean up
	delete[] liftDir;
	delete[] oldVertexPosition;
	delete[] newVertexPosition;

	return result;
}

// find a direction for the vertex "vertexIdx" to be lifted
// determined by the position of the centroid of the 1-ring and the position of the vertex
//
// vertexIdx (in): index of the vertex we are trying to move
// liftDir (out): vector pointing to the direction vertex should be lifted; must be an array of 3
// rdr (in): renderer that stores the mesh information
void Nonobtuse::findLiftDirection(int vertexIdx, double *liftDir, const Renderer &rdr)
{
	
	double *oneRingCentroid = new double[3];
	rdr.getOneRingCentroid(vertexIdx, oneRingCentroid);
	
	liftDir[0] = rdr.getVertexNormals()[3*vertexIdx];
	liftDir[1] = rdr.getVertexNormals()[3*vertexIdx+1];
	liftDir[2] = rdr.getVertexNormals()[3*vertexIdx+2];
	
	normalize(liftDir);
	
	// determine where to lift
	// ax + by + cz + d = 0
	if (rdr.getVertexNormals()[3*vertexIdx] * rdr.getVertices()[3*vertexIdx] 
		+ rdr.getVertexNormals()[3*vertexIdx+1] * rdr.getVertices()[3*vertexIdx+1]
		+ rdr.getVertexNormals()[3*vertexIdx+2] * rdr.getVertices()[3*vertexIdx+2]
		- rdr.getVertexNormals()[3*vertexIdx] * oneRingCentroid[0]
		- rdr.getVertexNormals()[3*vertexIdx+1] * oneRingCentroid[1]
		- rdr.getVertexNormals()[3*vertexIdx+2] * oneRingCentroid[2] < 0)
	{
		liftDir[0] *= -1;
		liftDir[1] *= -1;
		liftDir[2] *= -1;
	}
	
	// clean up
	delete[] oneRingCentroid;
}

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
bool Nonobtuse::obtuseAnglesExist(const double* verticesList, int numVertices, int endPt1Idx, int endPt2Idx, int v1Idx, int v2Idx)
{
	vector<ObtuseAngle> obAngleList;
		
	// test triangle v1Idx,endPt1Idx,endPt2Idx
	findObtuseAngles(verticesList, numVertices, v1Idx, endPt1Idx, endPt2Idx, obAngleList);
	if (obAngleList.size() > 0)
		return true;
	
	obAngleList.clear();
	// test triangle v2Idx,endPt1Idx,endPt2Idx
	findObtuseAngles(verticesList, numVertices, v2Idx, endPt1Idx, endPt2Idx, obAngleList);
	if (obAngleList.size() > 0)
		return true;
		
	return false;
}

// check if obAngle is a valid obtuse angle
//
// obAngle (in): obtuse angle to be checked
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// return value: result of the check
bool Nonobtuse::isValidObtuseAngle(const ObtuseAngle &obAngle, const int* const* polygonsList, int numPolygons)
{
	if (obAngle.polyIdx >= numPolygons)
		return false;
	
	for (int i = 0; i < 3; ++i)
	{
		// find the obtuse angle vertex in the polygon
		if (polygonsList[obAngle.polyIdx][i] == obAngle.vertexIdx)
		{
			// setup indices			
			int nextIdx = (i+1)%3;
			int prevIdx = i-1;
			if (i == 0)
				prevIdx = 2;
			
			// check if the edges are still valid
			return ((polygonsList[obAngle.polyIdx][prevIdx] == obAngle.neighbour1Idx 
					&& polygonsList[obAngle.polyIdx][nextIdx] == obAngle.neighbour2Idx)
				||
				(polygonsList[obAngle.polyIdx][nextIdx] == obAngle.neighbour1Idx
					&& polygonsList[obAngle.polyIdx][prevIdx] == obAngle.neighbour2Idx));
		}
	}
	
	return false;
}

// remove obtuse angles by moving a vertex to the centroid of the one ring
//
// rdr (out): renderer that stores the mesh information
void Nonobtuse::moveVerticesToCentroid(Renderer &rdr)
{
	double *oneRingCentroid = new double[3];

	vector<ObtuseAngle>::iterator iter = obtuseList.begin();
	while(iter != obtuseList.end())
	{
		if (rdr.getVertexDegree(iter->vertexIdx) > 3)
		{
			rdr.getOneRingCentroid(iter->vertexIdx , oneRingCentroid);
			rdr.moveVertex(iter->vertexIdx, oneRingCentroid[0], oneRingCentroid[1], oneRingCentroid[2]);
		}
		// remove the current obtuse angle
		obtuseList.erase(obtuseList.begin());
		iter = obtuseList.begin();
	}
	
	// clean up
	delete[] oneRingCentroid;
}

// remove obtuse angles by moving a vertex to the median of the one ring
// median is computed by taking the average of min(x) and max(x) of the one ring vertices
// respectively for y and z coordinate
//
// rdr (out): renderer that stores the mesh information
void Nonobtuse::moveVerticesToMedian(Renderer &rdr)
{
	double *oneRingMedian = new double[3];

	vector<ObtuseAngle>::iterator iter = obtuseList.begin();
	while(iter != obtuseList.end())
	{
		if (rdr.getVertexDegree(iter->vertexIdx) > 3)
		{
			rdr.getOneRingMedian(iter->vertexIdx , oneRingMedian);
			rdr.moveVertex(iter->vertexIdx, oneRingMedian[0], oneRingMedian[1], oneRingMedian[2]);
		}
		// remove the current obtuse angle
		obtuseList.erase(obtuseList.begin());
		iter = obtuseList.begin();
	}
	
	// clean up
	delete[] oneRingMedian;
}

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
int Nonobtuse::getScalarField(const double* verticesList, int numVertices,
                    const int* const* polygonsList, int numPolygons,
		    float* &scalarField, 
		    vector<lineMeshIntType>* &xIntPtList,
		    vector<lineMeshIntType>* &yIntPtList,
		    vector<lineMeshIntType>* &zIntPtList,
		    double &xoffsets, double &yoffsets, double &zoffsets,
		    unsigned int nCellsX, unsigned int nCellsY,  unsigned int nCellsZ,
                    float fCellLengthX, float fCellLengthY, float fCellLengthZ)
{
	// sanity checks
	if (scalarField != NULL)
		return -1;
	if (xIntPtList != NULL)
		return -1;
	if (yIntPtList != NULL)
		return -1;
	if (zIntPtList != NULL)
		return -1;
	
	cout << endl;
	cout << "Computing Scalar Field ...... (this may take awhile) ...... " << endl;

	// instantiate scalar field array
	int numScalars = (nCellsX+1)*(nCellsY+1)*(nCellsZ+1);
	scalarField = new float[numScalars];
	

	// ---- compute centroid, minX, maxX, minY, maxY, minZ, maxZ ----

	double meshCentroid[3] = {0,0,0};
	double meshMinX(-1.0);	// default min X if mesh is not available
	double meshMaxX(1.0);	// default max X if mesh is not available
	double meshMinY(-1.0);	// default min Y if mesh is not available
	double meshMaxY(1.0);	// default max Y if mesh is not available
	double meshMinZ(-1.0);	// default min Z if mesh is not available
	double meshMaxZ(1.0);	// default max Z if mesh is not available
	if (numVertices > 0)
	{
		// mesh is available, set default min & max
		meshMinX = verticesList[0];
		meshMaxX = verticesList[0];
		meshMinY = verticesList[1];
		meshMaxY = verticesList[1];
		meshMinZ = verticesList[2];
		meshMaxZ = verticesList[2];
	}

	
	for (int i = 0; i < numVertices; ++i)
	{
		// compute centroid of mesh
		for (int j = 0; j < 3; ++j)
		{
			meshCentroid[j] += verticesList[3*i+j];
		}
		
		// check for min x and max x
		if (meshMinX > verticesList[3*i])
			meshMinX = verticesList[3*i];
		if (meshMaxX < verticesList[3*i])
			meshMaxX = verticesList[3*i];
			
		// check for min y and max y
		if (meshMinY > verticesList[3*i+1])
			meshMinY = verticesList[3*i+1];
		if (meshMaxY < verticesList[3*i+1])
			meshMaxY = verticesList[3*i+1];
			
		// check for min z and max z
		if (meshMinZ > verticesList[3*i+2])
			meshMinZ = verticesList[3*i+2];
		if (meshMaxZ < verticesList[3*i+2])
			meshMaxZ = verticesList[3*i+2];
	}
	
	for (unsigned int i = 0; i < 3 && numVertices > 0; i++)
		meshCentroid[i] /= numVertices;
	
	
	// calculate offset for the bounding box, so it fits the mesh
	float mcBoxLength[3] = {(nCellsX*fCellLengthX),(nCellsY*fCellLengthY),(nCellsZ*fCellLengthZ)};
	xoffsets = meshCentroid[0] - (mcBoxLength[0] / 2.0);
	yoffsets = meshCentroid[1] - (mcBoxLength[1] / 2.0);
	zoffsets = meshCentroid[2] - (mcBoxLength[2] / 2.0);
	unsigned int nPointsInXDirection = (nCellsX + 1);
	unsigned int nPointsInSlice = nPointsInXDirection*(nCellsY + 1);

	//    1----2
	//   /|  / |
	// 5----6  |
	// | /0-|--3
	// |/   |/
	// 4----7
	
	xIntPtList = new vector<lineMeshIntType>[(nCellsY+1) * (nCellsZ+1)];
	// loop through scalar field casting rays parallel to x-axis
	for (unsigned int k = 0; k <= nCellsZ; ++k)
	{
		for (unsigned int j = 0; j <= nCellsY; ++j)
		{
			// get start and end point for ray
			double y = (fCellLengthY * j) + yoffsets;
			double z = (fCellLengthZ * k) + zoffsets;

			double startPt[3] = {meshMinX-mcBoxLength[0], y, z};
			double endPt[3] = {meshMaxX+mcBoxLength[0], y, z};
			
			// find all intersection points of ray and mesh
			// sort intersection points by x-component
			//vector<lineMeshIntType> intPtList;
			unsigned int rayIdx = k*(nCellsY+1) + j;
			line_mesh_intersection(xIntPtList[rayIdx], startPt, endPt, verticesList, numVertices, polygonsList, numPolygons);
			
			unsigned int numIntPt = xIntPtList[rayIdx].size();
			unsigned int currentIntPt = 0;
			int countFrontInt = 0;
			bool isInside(false);
			for (unsigned int i = 0; i <= nCellsX; ++i)
			{
				// loop through each sample point on ray and assign IsoValue
				double x = (fCellLengthX * i) + xoffsets;
				
				// check if we have sampled pass an intersecting point
				if (currentIntPt < numIntPt && x > xIntPtList[rayIdx][currentIntPt].pt[0])
				{
					// update to next intersecting point
					
					// TODO: problems with cases where ray intersects vertex or edge (ie. test it on bunny.smf 40x40x35 @ 0.005)
					//             make sure it is fixed even for self-intersecting mesh (ie. cow.smf)
					while (currentIntPt < numIntPt && x > xIntPtList[rayIdx][currentIntPt].pt[0])
					{
						
						switch (xIntPtList[rayIdx][currentIntPt].intType)
						{
							case 1:		// intersection is inside the triangle (ray hits frontside)
								++countFrontInt;
								break;
							case -1:	// intersection is inside the triangle (ray hits backside)
								--countFrontInt;
								break;
							case 2:		// intersection is at a vertex (ray hits frontside)
								++countFrontInt;
								break;
							case -2:	// intersection is at a vertex (ray hits backside)
								--countFrontInt;
								break;
							case 3:		// intersection is on an edge (ray hits frontside)
								++countFrontInt;
								break;
							case -3:	// intersection is on an edge (ray hits backside)
								--countFrontInt;
								break;
							case 4:		// intersection contains an edge (coplanar)
								break;
							case 5:		// intersection through 2 edges (coplanar)
								break;
							case 6:		// intersection through one vertex and an edge (coplanar)
								break;
							case 7:		// intersection is at a vertex (coplanar)
								break;
						}
					
						++currentIntPt;
					}
					/*
					int countPrevFrontInt = 0;
					for (unsigned int j = currentIntPt-1; j >= 0; --j)
					{
						switch (xIntPtList[rayIdx][j].intType)
						{
							case 1:		// intersection is inside the triangle (ray hits frontside)
								++countPrevFrontInt;
								break;
							case -1:	// intersection is inside the triangle (ray hits backside)
								--countPrevFrontInt;
								break;
							case 2:		// intersection is at a vertex (ray hits frontside)
								++countPrevFrontInt;
								break;
							case -2:	// intersection is at a vertex (ray hits backside)
								--countPrevFrontInt;
								break;
							case 3:		// intersection is on an edge (ray hits frontside)
								++countPrevFrontInt;
								break;
							case -3:	// intersection is on an edge (ray hits backside)
								--countPrevFrontInt;
								break;
							case 4:		// intersection contains an edge (coplanar)
								break;
							case 5:		// intersection through 2 edges (coplanar)
								break;
							case 6:		// intersection through one vertex and an edge (coplanar)
								break;
							case 7:		// intersection is at a vertex (coplanar)
								break;
						}
					}
					
					int countNextBackInt = 0;
					for (unsigned int j = currentIntPt; j < numIntPt; ++j)
					{
						switch (xIntPtList[rayIdx][j].intType)
						{
							case 1:		// intersection is inside the triangle (ray hits frontside)
								++countNextBackInt;
								break;
							case -1:	// intersection is inside the triangle (ray hits backside)
								--countNextBackInt;
								break;
							case 2:		// intersection is at a vertex (ray hits frontside)
								++countNextBackInt;
								break;
							case -2:	// intersection is at a vertex (ray hits backside)
								--countNextBackInt;
								break;
							case 3:		// intersection is on an edge (ray hits frontside)
								++countNextBackInt;
								break;
							case -3:	// intersection is on an edge (ray hits backside)
								--countNextBackInt;
								break;
							case 4:		// intersection contains an edge (coplanar)
								break;
							case 5:		// intersection through 2 edges (coplanar)
								break;
							case 6:		// intersection through one vertex and an edge (coplanar)
								break;
							case 7:		// intersection is at a vertex (coplanar)
								break;
						}
					}
					
					bool prevVote = (countPrevFrontInt > 0);
					bool nextVote = (countNextBackInt > 0);
					if ((countPrevFrontInt == 0) && (countPrevFrontInt == 0))
					{
					}
					else if ((countPrevFrontInt != 0) && (countPrevFrontInt != 0))
					{
						
					}
					else
					{
					}
					*/
					// check if we are inside or outside
					isInside = (countFrontInt > 0);
					/*
					switch (xIntPtList[rayIdx][currentIntPt-1].intType)
					{
						case 1:		// intersection is inside the triangle (ray hits frontside)
							isInside = true;
							break;
						case -1:	// intersection is inside the triangle (ray hits backside)
							isInside = false;
							break;
						case 2:		// intersection is at a vertex (ray hits frontside)
							isInside = true;
							break;
						case -2:	// intersection is at a vertex (ray hits backside)
							isInside = false;
							break;
						case 3:		// intersection is on an edge (ray hits frontside)
							isInside = true;
							break;
						case -3:	// intersection is on an edge (ray hits backside)
							isInside = false;
							break;
						case 4:		// intersection contains an edge (coplanar)
							break;
						case 5:		// intersection through 2 edges (coplanar)
							break;
						case 6:		// intersection through one vertex and an edge (coplanar)
							break;
						case 7:		// intersection is at a vertex (coplanar)
							break;
					}
					*/
				}
				
				// check if point is inside the mesh
				if (isInside)	// point is inside the mesh
				{
					// TODO: compute isovalue
					scalarField[k*nPointsInSlice + j*nPointsInXDirection + i] = -1.0;
				}
				else	// point is outside the mesh
				{
					// TODO: compute isovalue
					scalarField[k*nPointsInSlice + j*nPointsInXDirection + i] = 1.0;
				}
			}
		}
	}

	yIntPtList = new vector<lineMeshIntType>[(nCellsX+1) * (nCellsZ+1)];
	// loop through scalar field casting rays parallel to y-axis
	for (unsigned int k = 0; k <= nCellsZ; ++k)
	{
		for (unsigned int i = 0; i <= nCellsX; ++i)
		{
			// get start and end point for ray
			double x = (fCellLengthX * i) + xoffsets;
			double z = (fCellLengthZ * k) + zoffsets;

			double startPt[3] = {x, meshMinY-mcBoxLength[1], z};
			double endPt[3] = {x, meshMaxY+mcBoxLength[1], z};
			
			// find all intersection points of ray and mesh
			// sort intersection points by y-component
			unsigned int rayIdx = k*(nCellsX+1) + i;
			line_mesh_intersection(yIntPtList[rayIdx], startPt, endPt, verticesList, numVertices, polygonsList, numPolygons, 1);
		}
	}
	
	zIntPtList = new vector<lineMeshIntType>[(nCellsX+1) * (nCellsY+1)];
	// loop through scalar field casting rays parallel to y-axis
	for (unsigned int j = 0; j <= nCellsY; ++j)
	{
		for (unsigned int i = 0; i <= nCellsX; ++i)
		{
			// get start and end point for ray
			double x = (fCellLengthX * i) + xoffsets;
			double y = (fCellLengthY * j) + yoffsets;

			double startPt[3] = {x, y, meshMinZ-mcBoxLength[2]};
			double endPt[3] = {x, y, meshMaxZ+mcBoxLength[2]};
			
			// find all intersection points of ray and mesh
			// sort intersection points by z-component
			unsigned int rayIdx = j*(nCellsX+1) + i;
			line_mesh_intersection(zIntPtList[rayIdx], startPt, endPt, verticesList, numVertices, polygonsList, numPolygons, 2);
		}
	}
	
/*	
	// --------- DEBUG ---------
	// following code returns scalar field for a particular
	// shape defined by the user-defined equation
	
	// sphere x^2 + y^2 + z^2 = r^2
	unsigned int radius = 3;
	
	double xoffsets = ((nCellsX*fCellLengthX) / 2.0);
	double yoffsets = ((nCellsY*fCellLengthY) / 2.0);
	double zoffsets = ((nCellsZ*fCellLengthZ) / 2.0);
	
	unsigned int nPointsInXDirection = (nCellsX + 1);
	unsigned int nPointsInSlice = nPointsInXDirection*(nCellsY + 1);
	
	// loop through scalar field 
	for (unsigned int k = 0; k <= nCellsZ; ++k)
	{
		for (unsigned int j = 0; j <= nCellsY; ++j)
		{
			for (unsigned int i = 0; i <= nCellsX; ++i)
			{
				double x = (fCellLengthX * i) - xoffsets;
				double y = (fCellLengthY * j) - yoffsets;
				double z = (fCellLengthZ * k) - zoffsets;
			
				// check if point is inside the mesh
				if (x*x + y*y + z*z <= radius*radius)
					scalarField[k*nPointsInSlice + j*nPointsInXDirection + i] = -1.0;
				else
					scalarField[k*nPointsInSlice + j*nPointsInXDirection + i] = 1.0;
			}
		}
	}
	
	// ------ END DEBUG --------
*/	

	cout << "done!\n";
	return numScalars;
}

// Given a mesh (vertices + faces), writes the scalar field of it to a file
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// filename (in): filename to write the scalar field to
// nCellsX (in): number of cells across the x direction
// nCellsY (in): number of cells across the y direction
// nCellsZ (in): number of cells across the z direction
// fCellLengthX (in): length of each cell across x direction
// fCellLengthY (in): length of each cell across y direction
// fCellLengthZ (in): length of each cell across z direction
// ignorePolyIdx (in): start index for polygons to be ignored
// return value (out): true if success
bool Nonobtuse::writeScalarField(const double* verticesList, int numVertices,
                    const int* const* polygonsList, int numPolygons,
                    char* filename, unsigned int nCellsX,
                    unsigned int nCellsY,  unsigned int nCellsZ,
                    float fCellLengthX, float fCellLengthY, float fCellLengthZ,
					int ignorePolyIdx)
{
	float *scalarField = NULL;
	vector<lineMeshIntType>* xIntPtList = NULL;
	vector<lineMeshIntType>* yIntPtList = NULL;
	vector<lineMeshIntType>* zIntPtList = NULL;
	double xoffsets;
	double yoffsets;
	double zoffsets;
	if (getScalarField(verticesList, numVertices, polygonsList, numPolygons, 
			scalarField, xIntPtList, yIntPtList, zIntPtList,
			xoffsets, yoffsets, zoffsets,
			nCellsX, nCellsY, nCellsZ, fCellLengthX, fCellLengthY, fCellLengthZ) == -1)
		return false;
	
	ofstream outFile;
	outFile.open(filename, ofstream::out);
	
	if (!outFile.is_open())
		return false;
		
	unsigned int numScalars = (nCellsX+1)*(nCellsY+1)*(nCellsZ+1);
	outFile << "# " << numScalars << endl;
	outFile << "nCellsX " << nCellsX << endl;
	outFile << "nCellsY " << nCellsY << endl;
	outFile << "nCellsZ " << nCellsZ << endl;
	outFile << "fCellLengthX " << fCellLengthX << endl;
	outFile << "fCellLengthY " << fCellLengthY << endl;
	outFile << "fCellLengthZ " << fCellLengthZ << endl;
	outFile << "xoffsets " << xoffsets << endl;
	outFile << "yoffsets " << yoffsets << endl;
	outFile << "zoffsets " << zoffsets << endl;
	if (ignorePolyIdx != -1)
	{
		unsigned int numIgnorePoly = numPolygons - ignorePolyIdx;
		outFile << "numIgnorePoly " << numIgnorePoly << endl;
		for (unsigned int i = 0; i < numIgnorePoly; ++i)
		{
			unsigned int idx = i+ignorePolyIdx;
			outFile << verticesList[3*(polygonsList[idx][0])] << endl;
			outFile << verticesList[3*(polygonsList[idx][0])+1] << endl;
			outFile << verticesList[3*(polygonsList[idx][0])+2] << endl;
			outFile << verticesList[3*(polygonsList[idx][1])] << endl;
			outFile << verticesList[3*(polygonsList[idx][1])+1] << endl;
			outFile << verticesList[3*(polygonsList[idx][1])+2] << endl;
			outFile << verticesList[3*(polygonsList[idx][2])] << endl;
			outFile << verticesList[3*(polygonsList[idx][2])+1] << endl;
			outFile << verticesList[3*(polygonsList[idx][2])+2] << endl;
		}
	}
	else
		outFile << "numIgnorePoly 0" << endl;

	for (unsigned int i = 0; i < numScalars; ++i)
	{
		outFile << scalarField[i] << endl;
	}
	
	// ---- the following intersection info. are for Marching Cubes interpolation only ----

	unsigned int numXRays = (nCellsY+1)*(nCellsZ+1);
	for (unsigned int i = 0; i < numXRays; ++i)
	{
		unsigned int numIntPts = xIntPtList[i].size();
		outFile << numIntPts << endl;	// number of intersecting points found in current ray
		for (unsigned int j = 0; j < numIntPts; ++j)
		{
			outFile << xIntPtList[i][j].intType << endl;	// intersecting type
			outFile << xIntPtList[i][j].pt[0] << endl;	// intersecting pt x-coord
			outFile << xIntPtList[i][j].pt[1] << endl;	// intersecting pt y-coord
			outFile << xIntPtList[i][j].pt[2] << endl;	// intersecting pt z-coord
		}
	}
	
	unsigned int numYRays = (nCellsX+1)*(nCellsZ+1);
	for (unsigned int i = 0; i < numYRays; ++i)
	{
		unsigned int numIntPts = yIntPtList[i].size();
		outFile << numIntPts << endl;	// number of intersecting points found in current ray
		for (unsigned int j = 0; j < numIntPts; ++j)
		{
			outFile << yIntPtList[i][j].intType << endl;	// intersecting type
			outFile << yIntPtList[i][j].pt[0] << endl;	// intersecting pt x-coord
			outFile << yIntPtList[i][j].pt[1] << endl;	// intersecting pt y-coord
			outFile << yIntPtList[i][j].pt[2] << endl;	// intersecting pt z-coord
		}
	}
	
	unsigned int numZRays = (nCellsX+1)*(nCellsY+1);
	for (unsigned int i = 0; i < numZRays; ++i)
	{
		unsigned int numIntPts = zIntPtList[i].size();
		outFile << numIntPts << endl;	// number of intersecting points found in current ray
		for (unsigned int j = 0; j < numIntPts; ++j)
		{
			outFile << zIntPtList[i][j].intType << endl;	// intersecting type
			outFile << zIntPtList[i][j].pt[0] << endl;	// intersecting pt x-coord
			outFile << zIntPtList[i][j].pt[1] << endl;	// intersecting pt y-coord
			outFile << zIntPtList[i][j].pt[2] << endl;	// intersecting pt z-coord
		}
	}
	
	outFile.close();
	
	cout << "\nScalar field successfully saved to: " << filename << endl;
		
	// clean up
	delete[] scalarField;
	delete[] xIntPtList;
	delete[] yIntPtList;
	delete[] zIntPtList;
	
	return true;
}

// Given a mesh (vertices + faces), return all one-ring vertices that are holes
//
// verticesList (in): list of vertices in mesh
// numVertices (in): number of vertices in verticesList
// polygonsList (in): list of polygons in mesh
// numPolygons (in): number of polygons in polygonsList
// edgeNeighbourList (in): edge neighbour information
// holesOneRingList (out): list of one-ring vertices seperated by delimiter -1
// return value (out): number of holes found
unsigned int Nonobtuse::getHoles(const double* verticesList, int numVertices,
								const int* const* polygonsList, int numPolygons,
								const vector<EdgeNeighbours>* edgeNeighbourList,
								vector<int> &holesOneRingList)
{
	// sanity checks
	holesOneRingList.clear();

	unsigned int numHoles(0);
	vector<Edge> boundaryEdges;

	for (int i = 0; i < numVertices; ++i)
	{
		unsigned int vDeg = edgeNeighbourList[i].size();
		for (unsigned int j = 0; j < vDeg; ++j)	// check each edge connecting to vertex i
		{
			const EdgeNeighbours &eNbr = edgeNeighbourList[i][j];
			if (eNbr.polyNeighbour2Idx == -1)	// edge is at boundary
			{
				// check if we have processed the edge already
	 			if (find(boundaryEdges.begin(), boundaryEdges.end(), eNbr.e) != boundaryEdges.end())
					continue;

				Edge e = eNbr.e;

				// make sure e is pointing the left hand direction
				// so the one-ring will be anti-clockwise
				for (unsigned int k = 0; k < 3; ++k)
				{
					if (polygonsList[eNbr.polyNeighbour1Idx][k] == i)
					{
						if (polygonsList[eNbr.polyNeighbour1Idx][(k+1)%3] == e.vertex2Idx)
						{
							e.vertex1Idx = e.vertex2Idx;
							e.vertex2Idx = i;
						}
						break;
					}
				}

				// add edge to list of boundary edges
				boundaryEdges.push_back(e);

				// -------------------------------------------
				// ----- construct one-ring for the hole -----
				// -------------------------------------------

				vector<int> oneRing;
				oneRing.push_back(e.vertex1Idx);
				oneRing.push_back(e.vertex2Idx);
				while (oneRing.back() != -1)	// keep finding the next vertex in one-ring until we marked -1 as finish finding
				{
					// search for next boundary edge
					unsigned int vDeg2 = edgeNeighbourList[oneRing.back()].size();

					bool nextVertexFound(false);
					for (unsigned int k = 0; k < vDeg2; ++k)
					{
						const EdgeNeighbours &eNbr2 = edgeNeighbourList[oneRing.back()][k];
						if (eNbr2.polyNeighbour2Idx == -1)
						{
							// make sure we don't add edges that has been processed
							if (find(boundaryEdges.begin(), boundaryEdges.end(), eNbr2.e) != boundaryEdges.end())
								continue;

							// add edge to list of boundary edges
							boundaryEdges.push_back(eNbr2.e);

							// add vertex to oneRing
							oneRing.push_back(eNbr2.e.vertex2Idx);

							// check if we finish finding the one-ring
							if (oneRing.back() == oneRing.front())
								oneRing[oneRing.size()-1] = -1;
							
							nextVertexFound = true;
							break;
						}
					}

					if (!nextVertexFound)	// boundary does not form a hole
						break;
				}

				if (oneRing.back() == -1)	// successfully found a hole
				{
					// copy into holesOneRingList
					unsigned int numVerticesInHole = oneRing.size();
					for (unsigned int k = 0; k < numVerticesInHole; ++k)
					{
						holesOneRingList.push_back(oneRing[k]);
					}
					++numHoles;
				}

				break;
			}
		}
	}

	return numHoles;
}

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
bool Nonobtuse::fillHoles(const double* verticesList, int numVertices,
						const int* const* polygonsList, int numPolygons,
						const vector<int> &holesOneRingList, unsigned int numHoles,
						double* &newVList, int &numNewVertices,
						int** &newPList, int &numNewPolygons)
{
	//sanity checks
	if (newVList != NULL)
		return false;
	if (newPList != NULL)
		return false;
	if (holesOneRingList.size() <= 0)
		return false;
	
	numNewVertices = numVertices + numHoles;	// one new vertex for each hole
	newVList = new double[3*numNewVertices];
	for (int i = 0; i < 3*numVertices; ++i)
		newVList[i] = verticesList[i];
	numNewPolygons =numPolygons + holesOneRingList.size() - numHoles;	// numHoles amount of -1 delimiter in array
	newPList = new int*[numNewPolygons];
	for (int i = 0; i < numPolygons; ++i)
	{
		newPList[i] = new int[3];
		newPList[i][0] = polygonsList[i][0];
		newPList[i][1] = polygonsList[i][1];
		newPList[i][2] = polygonsList[i][2];
	}
	
	unsigned int firstIdx = 0;	// array location of first vertex of current one-ring
	unsigned int firstUnusedVIdx = 3*numVertices;
	unsigned int firstUnusedPolyIdx = numPolygons;
	for (unsigned int i = 0; i < numHoles; ++i)
	{
		// find centroid of one-ring
		double newVertex[3] = {0,0,0};
		for (unsigned int j = firstIdx; j < holesOneRingList.size(); ++j)
		{
			if (holesOneRingList[j] == -1)
			{
				unsigned int numNewTriangles = j - firstIdx;
				newVertex[0] /= numNewTriangles;
				newVertex[1] /= numNewTriangles;
				newVertex[2] /= numNewTriangles;

				newVList[firstUnusedVIdx] = newVertex[0];
				newVList[firstUnusedVIdx+1] = newVertex[1];
				newVList[firstUnusedVIdx+2] = newVertex[2];
				firstUnusedVIdx += 3;
				break;
			}

			newVertex[0] += verticesList[3*holesOneRingList[j]];
			newVertex[1] += verticesList[3*holesOneRingList[j]+1];
			newVertex[2] += verticesList[3*holesOneRingList[j]+2];
		}

		// create new triangles for the hole
		for (unsigned int j = firstIdx; j < holesOneRingList.size(); ++j)
		{
			if (holesOneRingList[j] == -1)
			{
				firstIdx = j+1;
				break;
			}

			newPList[firstUnusedPolyIdx] = new int[3];
			newPList[firstUnusedPolyIdx][0] = (firstUnusedVIdx-3) / 3;
			newPList[firstUnusedPolyIdx][1] = holesOneRingList[j];
			newPList[firstUnusedPolyIdx][2] = holesOneRingList[j+1];
			if (holesOneRingList[j+1] == -1)
				newPList[firstUnusedPolyIdx][2] = holesOneRingList[firstIdx];
			++firstUnusedPolyIdx;
		}
	}

	return true;
}


