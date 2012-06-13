typedef unsigned int uint;

//Testing, Hui Wang, May 30, 2012

#include "../MarchingCube/CIsoSurface.h"
#include "../MarchingCube/cnonobtusemc.h"
#include "../MarchingCube/tilingnonobtusemc.h"
#include <fstream>
#include <vector>

int main()
{   
    //Part 1: the data, two choices
    //Data1: just have one cube
    uint cellsX = 1, cellsY = 1, cellsZ = 1;
    double cellLengthX = 0.25, cellLengthY = 0.25, cellLengthZ = 0.25; 

    //case2: a sphere
    //uint cellsX = 10, cellsY = 10, cellsZ = 10;
    //double cellLengthX = 0.11, cellLengthY = 0.11, cellLengthZ = 0.11;

    double* scalarField = new double[11 * 11 * 11];
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
            CIsoSurface<double> marchingCube;
            marchingCube.GenerateSurface(scalarField, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ);

            //Use the nonobtuse marching cube
            //DUT::NonobtuseMC<double> marchingCube;
            //std::vector<DUT::LineMeshIntType> xIntPtList;
            //xIntPtList.reserve(0);
            //std::vector<DUT::Triangle> ignoreCubes;
            //marchingCube.generateSurface(scalarField, &xIntPtList, &xIntPtList, &xIntPtList, 0.0, 0.0, 0.0, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ, 1, ignoreCubes);





            //Part 3: get the result, save in the file "result.OFF"
            double* vertexList = NULL;
            int** triangleList = NULL;

            int vCount = marchingCube.getVertices(vertexList);
            int tCount = marchingCube.getTriangles(triangleList);

            std::ofstream out("result.off");
            int i;
            out << "OFF" << std::endl;
            out << vCount << " " << tCount << " " << 0 << std::endl;
            for(i = 0; i < vCount; i++)
                out << vertexList[i * 3 + 0] << " " << vertexList[i * 3 + 1] << " " << vertexList[i * 3 + 2] << std::endl;
            for(i = 0; i < tCount; i++)
                out << "3 " << triangleList[i][0] << " " << triangleList[i][1] << " " << triangleList[i][2] << std::endl;

            delete [] scalarField;
            delete [] vertexList;
            for(int i = 0; i < tCount; i++)
            {
                delete [] triangleList[i];
            }
            delete [] triangleList;

            return 0;
}