typedef unsigned int uint;

//Testing, Hui Wang, May 30, 2012

#include <vector>
#include "datadef.h"
#include "isosurface.h"
#include "nonobtusemc.h"
#include <fstream>

int main()
{   
	//Part 1: the data, two choices
	//Data1: just have one cube
	uint cellsX = 1, cellsY = 1, cellsZ = 1;
	double cellLengthX = 0.25, cellLengthY = 0.25, cellLengthZ = 0.25; 

	//case2: a sphere
	//uint cellsX = 10, cellsY = 10, cellsZ = 10;
	//double cellLengthX = 0.11, cellLengthY = 0.11, cellLengthZ = 0.11;

	double *scalarField = new double[11 * 11 * 11];
	for(uint z = 0; z <= cellsZ; z++)
	  for(uint y = 0; y <= cellsY; y++)
		for(uint x = 0; x <= cellsX; x++)
		{   
			uint index = z * ((cellsX + 1) * (cellsY + 1)) + y * (cellsX + 1) + x;
			double xx = x * cellLengthX, yy = y * cellLengthY, zz = z * cellLengthZ;

			//Data 1:
			scalarField[index] = xx - 0.2;

			//Data 2
			//scalarField[index] = (xx - 1) * (xx - 1) + (yy - 1) * (yy - 1) + (zz - 1) * (zz - 1) - 1.0;
		}
    


	//Part 2: the marching cube, two choices
	//Use original marching cube
	DUT::IsoSurface<double> marchingCube;
	marchingCube.generateSurface(scalarField, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ);

	//Use the nonobtuse marching cube
	//DUT::NonobtuseMC<double> marchingCube;
    //std::vector<DUT::LineMeshIntType> xIntPtList;
	//xIntPtList.reserve(0);
	//std::vector<DUT::Triangle> ignoreCubes;
	//marchingCube.generateSurface(scalarField, &xIntPtList, &xIntPtList, &xIntPtList, 0.0, 0.0, 0.0, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ, 1, ignoreCubes);

	



	//Part 3: get the result, save in the file "result.OFF"
	std::vector<cv::Point3d> vertexList;
	DUT::TriangleVec triangleList;

	marchingCube.getVertices(vertexList);
	marchingCube.getTriangles(triangleList);

	std::ofstream  out("result.OFF");
    int i;
	out << "OFF" << std::endl;
	out << vertexList.size() << " " << triangleList.size() << " " << 0 << std::endl;
	for(i = 0; i < vertexList.size(); i++)
		out << vertexList.at(i).x << " " << vertexList.at(i).y << " " << vertexList.at(i).z << std::endl;
	for(i = 0; i < triangleList.size(); i++)
		out << "3 " << triangleList.at(i).x << " " << triangleList.at(i).y << " " << triangleList.at(i).z << std::endl;

	delete scalarField;
	return 0;
}