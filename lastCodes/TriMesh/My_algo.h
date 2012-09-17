#ifndef MY_ALGO_H
#define MY_ALGO_H

//Hui Wang, September. 17, 2012

#include <vector>
#include <set>
#include "TriMesh.h"
#include "nonlinearSolver.h"

extern int findLocation(std::vector<int> vertexIndexs, int index);

extern std::vector<int> getRingsNeighbors(TriMesh *trimesh, int index, int rings);

extern std::set<int>    get1ringFaceNeighbors(TriMesh *trimesh, std::vector<int> vertexIndexs);

extern std::vector<Point> getPoints(TriMesh *trimesh, std::vector<int> vertexIndexs);

extern void updatePoints(TriMesh *trimesh, std::vector<int> vertexIndexs, std::vector<Point> points);

extern void getConstraints(TriMesh *trimesh, std::vector<int> vertexIndexs, std::vector<ThreePointsIndex> &threePointIndex, std::vector<TwoPointsIndex> &twoPointIndex, std::vector<OnePointsIndex> &onePointIndex);

#endif