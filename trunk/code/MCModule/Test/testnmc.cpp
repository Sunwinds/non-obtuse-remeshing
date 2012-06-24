#include "testnmc.h"

int testnmc(int argc, char* argv[])
{
    //Part 1: the data, two choices
    //Data 1: just have one cube
    //uint cellsX = 1, cellsY = 1, cellsZ = 1;
    //double cellLengthX = 0.25, cellLengthY = 0.25, cellLengthZ = 0.25; 

    //Data 2: a sphere
    uint cellsX = 10, cellsY = 10, cellsZ = 10;
    double cellLengthX = 0.11, cellLengthY = 0.11, cellLengthZ = 0.11;

    double* scalarField = new double[11 * 11 * 11];
    for(uint z = 0; z <= cellsZ; z++)
    {
        for(uint y = 0; y <= cellsY; y++)
        {
            for(uint x = 0; x <= cellsX; x++)
            {   
                uint index = z * ((cellsX + 1) * (cellsY + 1)) + y * (cellsX + 1) + x;
                double xx = x * cellLengthX, yy = y * cellLengthY, zz = z * cellLengthZ;

                //Data 1:
                //scalarField[index] = xx - 0.2;

                //Data 2
                scalarField[index] = (xx - 1) * (xx - 1) + (yy - 1) * (yy - 1) + (zz - 1) * (zz - 1) - 1.0;
            }
        }
    }

    //Part 2: the marching cube, two choices
    //Use the nonobtuse marching cube
    CNonobtuseMC<double> marchingCube;
    std::vector<lineMeshIntType> xIntPtList;
    std::vector<lineMeshIntType> yIntPtList;
    std::vector<lineMeshIntType> zIntPtList;
    //xIntPtList.reserve(0);
    std::vector<int*> ignoreCubes;
    marchingCube.GenerateSurface(scalarField, &xIntPtList, &yIntPtList, &zIntPtList, 0.0, 0.0, 0.0, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ, 0, ignoreCubes);

    //Part 3: get the result, save in the file "result.OFF"
    double* vertexList = NULL;
    int** triangleList = NULL;

    int vCount = marchingCube.getVertices(vertexList);
    int tCount = marchingCube.getTriangles(triangleList);

    std::ofstream out("./mesh/nmc.off");
    int i;
    out << "OFF" << std::endl;
    out << vCount << " " << tCount << " " << 0 << std::endl;
    for(i = 0; i < vCount; i++)
    {
        out << vertexList[i * 3 + 0] << " " << vertexList[i * 3 + 1] << " " << vertexList[i * 3 + 2] << std::endl;
    }
    for(i = 0; i < tCount; i++)
    {
        out << "3 " << triangleList[i][0] << " " << triangleList[i][1] << " " << triangleList[i][2] << std::endl;
    }

    delete [] scalarField;
    delete [] vertexList;
    for(int i = 0; i < tCount; i++)
    {
        delete [] triangleList[i];
    }
    delete [] triangleList;

    return 0;
}
