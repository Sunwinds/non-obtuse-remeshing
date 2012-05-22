#include "testmeshio.h"

namespace DUT
{
    const static std::string fileDir = "../data/";

    int testmeshio(int argc, char* argv[])
    {
        std::vector<cv::Point3d> vertexList;
        std::vector<cv::Point3_<unsigned int>> faceList;
        MeshIO::readSMF(fileDir + "horse_4k.smf", vertexList, faceList);
        MeshIO::writeOBJ(fileDir + "horse_4k.obj", vertexList, faceList);
        return 0;
    }
}
