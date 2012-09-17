#include "My_algo.h"

extern int findLocation(std::vector<int> vertexIndexs, int index)
{
   int location;

   location = -1;
   for(int i = 0; i < vertexIndexs.size(); i++)
     if(index == vertexIndexs[i])
	 {
		 location = i;
         break;
	 }

   return location;
}

extern std::vector<int> getRingsNeighbors(TriMesh *trimesh, int index, int rings)
{
	std::vector<int> ringsNeighbours;

	int i;
	for(i = 0; i < trimesh->neighbors[index].size();i++)
		ringsNeighbours.push_back(trimesh->neighbors[index][i]);

	int j;
	for(j = 1; j < rings; j++)
	{
		int currentNei = ringsNeighbours.size();
		for(i = 0; i < currentNei; i++)
		{
			for(int k = 0; k < trimesh->neighbors[ringsNeighbours[i]].size(); k++)
				if(find(ringsNeighbours.begin(), ringsNeighbours.end(), trimesh->neighbors[ringsNeighbours[i]][k]) == ringsNeighbours.end())
					ringsNeighbours.push_back(trimesh->neighbors[ringsNeighbours[i]][k]);
		}
	}

	return ringsNeighbours;
}

extern std::set<int>  get1ringFaceNeighbors(TriMesh *trimesh, std::vector<int> vertexIndexs)
{
	std::set<int> facesNeighbors;

	int i,j;
	for(i = 0; i < vertexIndexs.size(); i++)
	{   
		int index;

		index = vertexIndexs[i]; 
		for(j = 0; j < trimesh->adjacentfaces[index].size(); j++)
			facesNeighbors.insert(trimesh->adjacentfaces[index][j]);
	}

	return facesNeighbors;
}

extern void getConstraints(TriMesh *trimesh, std::vector<int> vertexIndexs, std::vector<ThreePointsIndex> &threePointIndex, std::vector<TwoPointsIndex> &twoPointIndex, std::vector<OnePointsIndex> &onePointIndex)
{
   std::set<int> facesNeighbors;

   facesNeighbors = get1ringFaceNeighbors(trimesh, vertexIndexs);
   for(std::set<int>::iterator iter = facesNeighbors.begin(); iter != facesNeighbors.end(); iter++)
   {
       int faceIndex = *iter;
	   int v0,v1,v2;

	   v0 = trimesh->faces[faceIndex][0];
	   v1 = trimesh->faces[faceIndex][1];
	   v2 = trimesh->faces[faceIndex][2];
      
       int l0, l1, l2;
	   l0 = findLocation(vertexIndexs, v0);
       l1 = findLocation(vertexIndexs, v1);
	   l2 = findLocation(vertexIndexs, v2);
       
	   //Three points
	   if((l0 != -1) && (l1 != -1) && (l2 != -1))
	   {
          ThreePointsIndex TPI;

		  TPI.n1 = l0;
		  TPI.n2 = l1;
		  TPI.n3 = l2;

		  threePointIndex.push_back(TPI);
	   }

	   //Two points
	   TwoPointsIndex TPI1;
	   if((l0 == -1) && (l1 != -1) && (l2 != -1))
	   {
		   TPI1.n1 = l1;
		   TPI1.n2 = l2;
		   TPI1.x = trimesh->vertices[v0][0];
		   TPI1.y = trimesh->vertices[v0][1];
		   TPI1.z = trimesh->vertices[v0][2];

		   twoPointIndex.push_back(TPI1);
	   }
	   if((l0 != -1) && (l1 == -1) && (l2 != -1))
	   {
		   TPI1.n1 = l0;
		   TPI1.n2 = l2;
		   TPI1.x = trimesh->vertices[v1][0];
		   TPI1.y = trimesh->vertices[v1][1];
		   TPI1.z = trimesh->vertices[v1][2];

		   twoPointIndex.push_back(TPI1);
	   }
	   if((l0 != -1) && (l1 != -1) && (l2 == -1))
	   {
		  TPI1.n1 = l0;
		  TPI1.n2 = l1;
		  TPI1.x = trimesh->vertices[v2][0];
          TPI1.y = trimesh->vertices[v2][1];
		  TPI1.z = trimesh->vertices[v2][2];

		  twoPointIndex.push_back(TPI1);
	   }

	   //One point
	   OnePointsIndex OPI;
	   if((l0 != -1) && (l1 == -1) && (l2 == -1))
	   {
          OPI.n = l0;
		  OPI.x1 = trimesh->vertices[v1][0];
		  OPI.y1 = trimesh->vertices[v1][1];
		  OPI.z1 = trimesh->vertices[v1][2];
		  OPI.x2 = trimesh->vertices[v2][0];
		  OPI.y2 = trimesh->vertices[v2][1];
		  OPI.z2 = trimesh->vertices[v2][2];

		  onePointIndex.push_back(OPI);
	   }
	   if((l0 == -1) && (l1 != -1) && (l2 == -1))
	   {
		   OPI.n = l1;
		   OPI.x1 = trimesh->vertices[v0][0];
		   OPI.y1 = trimesh->vertices[v0][1];
		   OPI.z1 = trimesh->vertices[v0][2];
		   OPI.x2 = trimesh->vertices[v2][0];
		   OPI.y2 = trimesh->vertices[v2][1];
		   OPI.z2 = trimesh->vertices[v2][2];

		   onePointIndex.push_back(OPI);
	   }
	   if((l0 == -1) && (l1 == -1) && (l2 != -1))
	   {
		   OPI.n = l2;
		   OPI.x1 = trimesh->vertices[v0][0];
		   OPI.y1 = trimesh->vertices[v0][1];
		   OPI.z1 = trimesh->vertices[v0][2];
		   OPI.x2 = trimesh->vertices[v1][0];
		   OPI.y2 = trimesh->vertices[v1][1];
		   OPI.z2 = trimesh->vertices[v1][2];

		   onePointIndex.push_back(OPI);
	   }
   }
}

extern std::vector<Point> getPoints(TriMesh *trimesh, std::vector<int> vertexIndexs)
{
	std::vector<Point> points;

	int i, numberOfPoint;
	numberOfPoint = vertexIndexs.size();
	points.resize(numberOfPoint);
	for(i = 0; i < numberOfPoint; i++)
	{
		points[i].x = trimesh->vertices[vertexIndexs[i]][0];
		points[i].y = trimesh->vertices[vertexIndexs[i]][1];
        points[i].z = trimesh->vertices[vertexIndexs[i]][2];
	}

	return points;
}

extern void updatePoints(TriMesh *trimesh, std::vector<int> vertexIndexs, std::vector<Point> points)
{
	int i, numberOfPoint;
	numberOfPoint = vertexIndexs.size();
	for(i = 0; i < numberOfPoint; i++)
	{
	    trimesh->vertices[vertexIndexs[i]][0] = points[i].x;
		trimesh->vertices[vertexIndexs[i]][1] = points[i].y;
		trimesh->vertices[vertexIndexs[i]][2] = points[i].z;
	}
}