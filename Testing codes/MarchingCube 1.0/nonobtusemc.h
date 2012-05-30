#ifndef DUT_NONOBTUSEMC_H
#define DUT_NONOBTUSEMC_H

//#include "../config.h"
#include "datadef.h"
#include "isosurface.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>


#define MIDPOINT 0
#define INTERPOLATE 1
#define NONOBTUSE_INTERPOLATE 2

#define M_PI_2     1.57079632679489661923

namespace DUT
{
	template <class T>
	class NonobtuseMC : public IsoSurface<T>
	{
	public: 
		NonobtuseMC();
		~NonobtuseMC();

	public:
		// Generate the isosurface from the scalar field contained in the buffer scalarField.
		// Modified from the base class to give nonobtuse marching cube.
		virtual void generateSurface(
			const T* scalarField,
			const std::vector<LineMeshIntType>* xIntPtList,
			const std::vector<LineMeshIntType>* yIntPtList,
			const std::vector<LineMeshIntType>* zIntPtList,
			double xOffsets,
			double yOffsets,
			double zOffsets,
			T isoLevel,
			uint cellsX,
			uint cellsY,
			uint cellsZ,
			double cellLengthX,
			double cellLengthY,
			double cellLengthZ,
			uint surfaceType,
			const std::vector<Triangle>& ignoreCubes);
		// Generate the isosurface from the scalar field contained in the file scalarFieldFileName
		// Modified from the base class to give nonobtuse marching cube.
		// return value (out): true if success
		virtual bool generateSurface(const char* scalarFieldFileName, T isoLevel, uint surfaceType);
		// given a list of triangles, return a list of cubes that intersects these triangles
		virtual void getIgnoreCubes(
			uint cellsX,
			uint cellsY,
			uint cellsZ,
			double cellLengthX,
			double cellLengthY,
			double cellLengthZ,
			double xOffsets,
			double yOffsets,
			double zOffsets,
			const std::vector<cv::Point3d>& exceptTriList,
			uint numExceptTri,
			std::vector<Triangle>& ignoreCubes);
		// Delete the isosurface.
		virtual void deleteSurface();
		// returns xOffsets
		virtual double getXOffset() const;
		// returns yOffsets
		virtual double getYOffset() const;
		// returns zOffsets
		virtual double getZOffset() const;

		//protected:
	public:
		// Rename vertices and triangles so that they can be accessed more efficiently.
		// unused vertices are moved to end of array
		virtual void renameVerticesAndTriangles();
		// Returns the edge Id.
		virtual uint getEdgeId(uint x, uint y, uint z, uint edgeNo);
		// Returns the vertex Id.
		virtual uint getVertexId(uint x, uint y, uint z);
		// Calculates the intersection point of the isosurface with an edge.
		virtual Point3dId calculateIntersection(uint x, uint y, uint z, uint edgeNo, uint surfaceType);
		// returns true if all angles incident to testPoint is nonobtuse
		virtual bool checkNonobtuse(const cv::Point3d& testPoint, uint x, uint y, uint z, uint edgeNo);
		// returns true if all angles in cube x, y, z at edge edgeNo incident to testPoint is nonobtuse
		virtual bool checkNonobtuse_singleCube(const cv::Point3d& testPoint, uint x, uint y, uint z, uint edgeNo);

		//protected:
	public:
		// intersecting points from casting rays at x,y,z directions
		const std::vector<LineMeshIntType>* iIntPtList[3];
		// offset from the origin of the mesh
		double iOffsets[3];
		// tableIndex for all cubes
		// ONLY VALID DURING generateSurface()
		uint* iAllTableIndex;
	};
}

#endif // DUT_NONOBTUSEMC_H
