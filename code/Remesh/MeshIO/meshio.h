#ifndef DUT_MESHIO_H
#define DUT_MESHIO_H

#include "../config.h"
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

namespace DUT
{
    class MeshIO
    {
    private:
        MeshIO() {}
        ~MeshIO() {}

    public:
        static void readOBJ(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList);
        static void writeOBJ(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList);
        static void readOFF(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList);
        static void writeOFF(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList);
        static void readSMF(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList);
        static void writeSMF(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList);
    };
}

#endif // DUT_MESHIO_H
