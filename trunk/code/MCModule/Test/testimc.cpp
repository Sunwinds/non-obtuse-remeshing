#include "testimc.h"

int testimc(int argc, char* argv[])
{
    //Part 1: the data, two choices
    //Data1: just have one cube
 /*   uint cellsX = 1, cellsY = 1, cellsZ = 1;
    double cellLengthX = 0.25, cellLengthY = 0.25, cellLengthZ = 0.25; */

    //case2: a sphere
    int cellsX = 20, cellsY = 20, cellsZ = 20;
    double cellLengthX = 0.11, cellLengthY = 0.11, cellLengthZ = 0.11;

    double* scalarField = new double[(cellsX + 1) * (cellsY + 1)* (cellsX + 1)];
    for(int z = 0; z <= cellsZ; z++)
    {
        for(int y = 0; y <= cellsY; y++)
        {
            for(int x = 0; x <= cellsX; x++)
            {   
                uint index = z * ((cellsX + 1) * (cellsY + 1)) + y * (cellsX + 1) + x;
                double xx = (x-cellsX/2) * cellLengthX, yy = (y-cellsY/2) * cellLengthY, zz = (z-cellsZ/2) * cellLengthZ;

                //Data 1:
               // scalarField[index] = xx - 0.2;

                //Data 2
                scalarField[index] = xx * xx+ yy*yy + zz*zz - 1.0;
            }
        }
    }

    //Part 2: the marching cube, two choices
    //Use original marching cube
    CIsoSurface<double> marchingCube;
    marchingCube.GenerateSurface(scalarField, 0.0, cellsX, cellsY, cellsZ, cellLengthX, cellLengthY, cellLengthZ);

    //Part 3: get the result, save in the file "result.OFF"
    double* vertexList = NULL;
    int** triangleList = NULL;

    int vCount = marchingCube.getVertices(vertexList);
    int tCount = marchingCube.getTriangles(triangleList);

    std::ofstream out("./mesh/imc.off");
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
