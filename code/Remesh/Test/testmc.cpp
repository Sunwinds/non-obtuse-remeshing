#include "testmc.h"
#include "../MarchingCube/isosurface.h"
#include "../MarchingCube/nonobtusemc.h"
#include "../MeshIO/meshio.h"

namespace DUT
{
    const static std::string fileDir = "../data/";

    int testmc(int argc, char* argv[])
    {
        std::vector<cv::Point3d> vertexList;
        std::vector<cv::Point3_<unsigned int>> faceList;
        MeshIO::readSMF(fileDir + "horse_4k.smf", vertexList, faceList);
        double isoLevel = 0.0f;
        NonobtuseMC<double> mc;
        std::string filename = fileDir + "scalarFileFileName.txt";
        mc.generateSurface(filename.c_str(), isoLevel, MIDPOINT);
        return 0;
    }
}
