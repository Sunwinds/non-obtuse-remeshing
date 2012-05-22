#ifndef DUT_ISOSURFACE_H
#define DUT_ISOSURFACE_H

#include "../config.h"
#include <map>
#include <vector>
#include <opencv2/opencv.hpp>

namespace DUT
{
    typedef unsigned int uint;
    struct Point3dId
    {
        uint iId;
        cv::Point3d iPt;
    };
    typedef std::map<uint, Point3dId> Id2Point3dId;
    typedef cv::Point3_<uint> Triangle;
    typedef std::vector<Triangle> TriangleVec;

    template <class T>
    class IsoSurface
    {
    public:
        IsoSurface();
        ~IsoSurface();

    public:
        // Generate the isosurface from the scalar field contained in the buffer scalarField.
        virtual void generateSurface(const T* scalarField, T isoLevel, uint cellsX, uint cellsY, uint cellsZ, double cellLengthX, double cellLengthY, double cellLengthZ);
        // Return true if a valid surface has been generated.
        virtual bool isSurfaceValid();
        // Delete the isosurface.
        virtual void deleteSurface();
        // Return the length, width and height of the volume in which the isosurface in enclosed in. Returns -1 if the surface is not valid.
        virtual int getVolumeLengths(double& volLengthX, double& volLengthY, double& volLengthZ);
        // return the isosurface vertices
        // return value: number of vertices
        virtual int getVertices(std::vector<cv::Point3d>& vertexList);
        // return the isosurface triangles
        // return value: number of triangles
        virtual int getTriangles(TriangleVec& triangleList);
        // return the isosurface normals
        // return value: number of normals
        virtual int getNormals(std::vector<cv::Point3d>& normalList);
        // return iCellsX
        virtual uint getCellsX() const;
        // return iCellsY
        virtual uint getCellsY() const;
        // return iCellsZ
        virtual uint getCellsZ() const;
        // return iCellLengthX
        virtual double getCellLengthX() const;
        // return iCellLengthY
        virtual double getCellLengthY() const;
        // return iCellLengthZ
        virtual double getCellLengthZ() const;
        // return m_ptScalarField
        virtual const T* getScalarField() const;
        // return m_tIsoLevel
        virtual T getIsoLevel() const;

        //protected:
    public:
        // The vertices which make up the isosurface.
        std::vector<cv::Point3d> iVertices;
        // The indices of the vertices which make up the triangles.
        TriangleVec iTriangleIndices;
        // The normals.
        std::vector<cv::Point3d> iNormals;
        // List of POINT3Ds which form the isosurface.
        Id2Point3dId iId2Point3dId;
        // List of TRIANGLEs which form the triangulation of the isosurface.
        TriangleVec iTriangleVec;

    public:
        // Return the edge id.
        uint getEdgeId(uint x, uint y, uint z, uint edgeNo);
        // Return the vertex id.
        uint getVertexId(uint x, uint y, uint z);
        // Calculate the intersection point of the isosurface with an edge.
        Point3dId calculateIntersection(uint x, uint y, uint z, uint edgeNo);
        // Interpolate between two grid points to produce the point at which the isosurface intersects an edge.
        Point3dId interpolate(const cv::Point3d& point1, const cv::Point3d& point2, T val1, T val2);
        // Rename vertices and triangles so that they can be accessed more efficiently.
        void renameVerticesAndTriangles();
        // Calculate the normals.
        void calculateNormals();

        //protected:
    public:
        // No. of cells in x, y, and z directions.
        uint iCellsX;
        uint iCellsY;
        uint iCellsZ;
        // Cell length in x, y, and z directions.
        double iCellLengthX;
        double iCellLengthY;
        double iCellLengthZ;
        // The buffer holding the scalar field.
        const T* iScalarField;
        // The isosurface value.
        T iIsoLevel;
        // Indicate whether a valid surface is present.
        bool iValidSurface;

    public:
        // Lookup tables used in the construction of the isosurface.
        static const uint iEdgeTable[256];
        static const int iTriTable[256][16];
    };
}

#endif // DUT_ISOSURFACE_H
