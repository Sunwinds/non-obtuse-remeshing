#include "meshio.h"

namespace DUT
{
    void MeshIO::readOBJ(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::in);
        std::string temp;
        char type;
        cv::Point3d vertex;
        cv::Point3i face;
        while(!stream.eof())
        {
            stream >> type;
            if(type == 'v')
            {
                stream >> vertex.x >> vertex.y >> vertex.z;
                vertexList.push_back(vertex);
            }
            else if(type == 'f')
            {
                stream >> face.x >> face.y >> face.z;
                --face.x;
                --face.y;
                --face.z;
                faceList.push_back(face);
            }
            else
            {
                stream >> temp;
            }
            type = '#';
        }
        stream.close();
    }

    void MeshIO::writeOBJ(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::out);
        for(std::vector<cv::Point3d>::const_iterator vIter = vertexList.begin(); vIter != vertexList.end(); ++vIter)
        {
            stream << 'v' << ' ' << vIter->x << ' ' << vIter->y << ' ' << vIter->z << std::endl;
        }
        for(std::vector<cv::Point3_<unsigned int>>::const_iterator fIter = faceList.begin(); fIter != faceList.end(); ++fIter)
        {
            stream << 'f' << ' ' << fIter->x + 1 << ' ' << fIter->y + 1 << ' ' << fIter->z + 1 << std::endl;
        }
        stream.close();
    }

    void MeshIO::readOFF(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::in);
        int vCount = 0;
        int fCount = 0;
        int eCount = 0;
        std::string temp;
        stream >> temp; // OFF
        stream >> vCount >> fCount >> eCount;
        cv::Point3d vertex;
        for(int vIdx = 0; vIdx < vCount; ++vIdx)
        {
            stream >> vertex.x >> vertex.y >> vertex.z;
            vertexList.push_back(vertex);
        }
        cv::Point3i face;
        int count;
        for(int fIdx = 0; fIdx < fCount; ++fIdx)
        {
            stream >> count;
            if(count == 3)
            {
                stream >> face.x >> face.y >> face.z;
            }
            else
            {
                stream >> temp;
            }
            faceList.push_back(face);
        }
        stream.close();
    }

    void MeshIO::writeOFF(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::out);
        stream << "OFF" << std::endl;
        stream << vertexList.size() << ' ' << faceList.size() << ' ' << '0' << std::endl;
        for(std::vector<cv::Point3d>::const_iterator vIter = vertexList.begin(); vIter != vertexList.end(); ++vIter)
        {
            stream << vIter->x << ' ' << vIter->y << ' ' << vIter->z << std::endl;
        }
        for(std::vector<cv::Point3_<unsigned int>>::const_iterator fIter = faceList.begin(); fIter != faceList.end(); ++fIter)
        {
            stream << '3' << ' ' << fIter->x << ' ' << fIter->y << ' ' << fIter->z << std::endl;
        }
        stream.close();
    }

    void MeshIO::readSMF(const std::string& filename, std::vector<cv::Point3d>& vertexList, std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::in);
        std::string temp;
        char type;
        cv::Point3d vertex;
        cv::Point3i face;
        while(!stream.eof())
        {
            stream >> type;
            if(type == 'v')
            {
                stream >> vertex.x >> vertex.y >> vertex.z;
                vertexList.push_back(vertex);
            }
            else if(type == 'f')
            {
                stream >> face.x >> face.y >> face.z;
                --face.x;
                --face.y;
                --face.z;
                faceList.push_back(face);
            }
            else
            {
                stream >> temp;
            }
            type = '#';
        }
        stream.close();
    }

    void MeshIO::writeSMF(const std::string& filename, const std::vector<cv::Point3d>& vertexList, const std::vector<cv::Point3_<unsigned int>>& faceList)
    {
        std::fstream stream(filename.c_str(), std::ios::out);
        stream << "#$SMF 1.0" << std::endl;
        stream << "#$vertices " << vertexList.size() << std::endl;
        stream << "#$faces " << faceList.size() << std::endl;
        for(std::vector<cv::Point3d>::const_iterator vIter = vertexList.begin(); vIter != vertexList.end(); ++vIter)
        {
            stream << 'v' << ' ' << vIter->x << ' ' << vIter->y << ' ' << vIter->z << std::endl;
        }
        for(std::vector<cv::Point3_<unsigned int>>::const_iterator fIter = faceList.begin(); fIter != faceList.end(); ++fIter)
        {
            stream << 'f' << ' ' << fIter->x + 1 << ' ' << fIter->y + 1 << ' ' << fIter->z + 1 << std::endl;
        }
        stream.close();
    }
}
