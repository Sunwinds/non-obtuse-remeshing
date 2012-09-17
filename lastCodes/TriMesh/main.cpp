#include "TriMesh.h"
#include "TriMesh_algo.h"
#include "My_algo.h"
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>


/*
int main()
{
    TriMesh *trimesh, *normal;
	
	trimesh = TriMesh::read("D:\\mesh.OFF");
	normal = TriMesh::read("D:\\normal.OFF");

	std::vector<int> featureIndex;
	std::vector<point> featurePoiint;
    subdivMy(trimesh, normal, SUBDIV_PLANAR, featureIndex, featurePoiint);

	trimesh->write("D:\\test_sub.OFF");
	
	std::ofstream out("D:\\feature.txt");
	for(int i = 0;i < featureIndex.size();i++)
	{
		out << featureIndex.at(i) << " ";
		
		point p = featurePoiint.at(i);
		out << p[0] << " " << p[1] << " " << p[2] << std::endl; 
	}
	return 0;
}*/

/*
int main()
{
   TriMesh *trimesh;

   trimesh = TriMesh::read("D:\\mesh.OFF");

   trimesh->need_neighbors();
   trimesh->need_adjacentfaces();

   std::vector<int> ringsNeighbours;

   ringsNeighbours = getRingsNeighbors(trimesh, 100, 2);
  
   for(int i = 0; i < ringsNeighbours.size(); i++)
	   std::cout << ringsNeighbours[i] << std::endl;

   std::set<int> facesNeighbors;
   facesNeighbors = get1ringFaceNeighbors(trimesh, ringsNeighbours);

   for(std::set<int>::iterator iter = facesNeighbors.begin(); iter != facesNeighbors.end(); iter++)
	   std::cout << *iter << std::endl;

   std::vector<ThreePointsIndex> threePointIndex;
   std::vector<TwoPointsIndex> twoPointIndex;
   std::vector<OnePointsIndex> onePointIndex;
   getConstraints(trimesh, ringsNeighbours, threePointIndex, twoPointIndex, onePointIndex);

   std::cout << threePointIndex.size() << " " << twoPointIndex.size() << " " << onePointIndex.size() << std::endl;
   return 0;
}*/

int main()
{
	TriMesh *trimesh, *correspondence;

	trimesh = TriMesh::read("D:\\mesh.OFF");
	correspondence = TriMesh::read("D:\\correspondence.OFF");

	trimesh->need_neighbors();
	trimesh->need_adjacentfaces();

	int i;
	clock_t start, end;
	start = clock();
	for(i = 0; i < trimesh->faces.size(); i++)
	{
		//if(i == 185)
			//std::cout << i;
		std::vector<int> movedIndex;
        //movedIndex = getRingsNeighbors(trimesh, i, 1);
		movedIndex.push_back(trimesh->faces[i][0]);
		movedIndex.push_back(trimesh->faces[i][1]);
		movedIndex.push_back(trimesh->faces[i][2]);

		std::vector<Point> startPoints;
		startPoints = getPoints(trimesh, movedIndex);

		std::vector<Point> targetPoints;
		targetPoints = getPoints(correspondence, movedIndex);

		std::vector<ThreePointsIndex> threePointIndex;
		std::vector<TwoPointsIndex> twoPointIndex;
		std::vector<OnePointsIndex> onePointIndex;
		getConstraints(trimesh, movedIndex, threePointIndex, twoPointIndex, onePointIndex);

		//std::cout << threePointIndex.size() << " " << twoPointIndex.size() << " " << onePointIndex.size() << std::endl;

		std::vector<Point> resultPoint; 
		if(moveAnyStencile(startPoints, targetPoints, resultPoint, threePointIndex, twoPointIndex, onePointIndex))
			updatePoints(trimesh, movedIndex, resultPoint);

		end = clock();
		if(i % 100 == 0)
		std::cout << i << "vertices times" << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

		movedIndex.clear();
		startPoints.clear();
		targetPoints.clear();
		threePointIndex.clear();
		twoPointIndex.clear();
		onePointIndex.clear();
	}

	trimesh->write("D:\\result.OFF");
}