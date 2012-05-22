#include "testmc.h"

namespace DUT
{
    const static std::string fileDir = "../data/";

    int testmc(int argc, char* argv[])
    {
        std::vector<cv::Point3d> vertexList;
        std::vector<cv::Point3_<unsigned int>> faceList;
        MeshIO::readSMF(fileDir + "horse_4k.smf", vertexList, faceList);
        double isoLevel = 0.0f;
        IsoSurface<double> mc;
        mc.generateSurface(NULL, isoLevel, 100, 100, 100, 0.0025, 0.0025, 0.0025);
        return 0;
    }
}
