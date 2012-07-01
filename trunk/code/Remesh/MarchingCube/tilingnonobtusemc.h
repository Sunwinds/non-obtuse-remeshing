#ifndef DUT_TILINGNONOBTUSEMC_H
#define DUT_TILINGNONOBTUSEMC_H

#include "datadef.h"
#include "nonobtusemc.h"
#include <time.h>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <vector>

namespace DUT
{
    typedef std::map<uint, Edge> Id2Edge; // id to edge

    typedef std::map<uint, std::vector<uint>> V2E; // vertex to edge
    typedef std::map<uint, std::vector<uint>> V2Tri; // vertex to triangle
    typedef std::map<uint, std::vector<uint>> E2Tri; // edge to triangle

    typedef std::pair<uint, uint> VPair; // vertex pair
    typedef std::map<VPair, uint> VPair2E; // vertex pair to edge

    typedef std::map<uint, uint> V2EofCube; // vertex to edge of cube
    typedef std::map<uint, uint> E2F; // edge to face

    struct EdgePosInfo
    {
        uint iEdgeIdx; // edge index
        uint iV1Edge; // indicates which edge v1 lies on
        uint iV2Edge; // indicates which edge v1 lies on
        bool iIsV1First; // determine the direction of the edge on the face
    };

    typedef cv::Point3_<uint> CubeId;

    struct IntersectTri
    {
        IntersectTri() : iTriIdx(-1), iPatchNo(-1) {}
        uint iTriIdx; // index of the intersecting triangle
        uint iPatchNo; // patch no. that it belongs to
        bool iInsideCube; // true if triangle is completely inside the cube
    };

    struct Intersection
    {
        cv::Point3d iIntPt[3]; // intersected pt
        uint iTriIdx; // index of triangle that caused the intersection
        bool iIsFacingPos; // true if intersected triangle is facing toward the positive direction of the axis
    };

    struct IntersectedCube
    {
        std::vector<IntersectTri> iIntTriVec; // list of intersected triangles
        std::vector<std::vector<uint> > iTriGroups; // list of connected components formed by the intersected triangles; values are indices to intersectedTriangle
        std::vector<std::vector<int>> iSigns; // signs used for triangulating the patch; determines inside-outside test
        std::vector<bool> iIsConsistent; // true if patch has a consistent sign assignment
        std::vector<bool> iIsEmptyTriangulation; // true if patch introduces no triangles
    };

    bool operator == (const IntersectTri& a, const IntersectTri& b)
    {
        return a.iTriIdx == b.iTriIdx;
    }

    bool operator == (const EdgePosInfo& a, const EdgePosInfo& b)
    {
        return a.iEdgeIdx == b.iEdgeIdx;
    }

    template <class T>
    class TilingNonobtuseMC : public NonobtuseMC<T>
    {
    public:
        TilingNonobtuseMC();
        ~TilingNonobtuseMC();

    public:
        // clean up all members
        virtual void deleteSurface();
        // Generates the isosurface from the source mesh
        // Modified from the base class to give nonobtuse marching cube without using a scalarfield
        // surfaceType = MIDPOINT
        //
        // verticeList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // isoLevel (in): isolevel
        // cellsX (in): number of cells in x-dim
        // cellsY (in): number of cells in y-dim
        // cellsZ (in): number of cells in z-dim
        // cellLengthX (in): cell length in x-dim
        // cellLengthY (in): cell length in y-dim
        // cellLengthZ (in): cell length in z-dim
        // return value (out): time taken
        virtual clock_t generateSurface(
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* polygonNeighbourList,
            T isoLevel,
            uint cellsX,
            uint cellsY,
            uint cellsZ,
            double cellLengthX,
            double cellLengthY,
            double cellLengthZ);
        // Generates the isosurface from the source mesh (using simple MC)
        // surfaceType = MIDPOINT
        //
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
        // isoLevel (in): isolevel
        // cellsX (in): number of cells in x-dim
        // cellsY (in): number of cells in y-dim
        // cellsZ (in): number of cells in z-dim
        // cellLengthX (in): cell length in x-dim
        // cellLengthY (in): cell length in y-dim
        // cellLengthZ (in): cell length in z-dim
        // return value (out): time taken
        virtual clock_t generateSurfaceSimpleMC(
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* vertexNeighbourList,
            T isoLevel,
            uint cellsX,
            uint cellsY,
            uint cellsZ,
            double cellLengthX,
            double cellLengthY,
            double cellLengthZ);
        // returns the list of closest triangle for each vertex; -1 if no closest triangle
        //
        // this->iClosestTriList (out): array of associated triangles to return
        // return value (out): number of slots in array
        virtual const std::vector<uint>& getClosestTriList() const;

    //protected:
    public:
        // triangulate the intersected cubes using simple MC
        //
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
        virtual void triangulateSimpleMC(
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* vertexNeighbourList);
        // returns the edge index
        //
        // x (in): cube x-coord
        // y (in): cube y-coord
        // z (in): cube z-coord
        // edgeIdx (in): edge index of cube (x,y,z)
        // return value (out): edge index being query
        virtual uint edgeIndexing(uint x, uint y, uint z, uint edgeIdx) const;
        // post-processing for eliminating non-manifold vertices introduced during construction using simpleMC
        //
        virtual void postProcessSimpleMC();
        // returns the scalar of grid point at (x,y,z); also return the closest triangle in the original mesh
        //
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // closestTriIdx (out): index of closest triangle in original mesh 
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
        // return val (out): scalar value of the grid point
        virtual double getScalar(
            uint x,
            uint y,
            uint z,
            uint& closestTriIdx,
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* vertexNeighbourList) const;
        // triangulate the cube(x,y,z) based on the intersecting triangles and the sorted groups
        //
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual void triangulate(
            uint x,
            uint y,
            uint z,
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<int>* polygonNeighbourList);
        // triangulate patch on cube(x,y,z) based on the signs given in 'signs'
        //
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // patchNo (in): patch number we are triangulating
        // edgeInt (in): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual void triangulatePatch(
            uint x,
            uint y,
            uint z,
            uint patchNo,
            const std::vector<Intersection> (&edgeInt)[12],
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* polygonNeighbourList);
        // based on edge intersections, determine all signs for the cube
        // array of signs for all 8 corners of the cube; 1=outside, -1=inside
        //
        // edgeInt (in): array of 12 slots; stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // patchNo (in): patch number we are triangulating
        // 
        // return value (out): true if all signs can be assigned without conflict
        virtual bool assignSigns(const std::vector<Intersection> (&edgeInt)[12], uint x, uint y, uint z, uint patchNo);
        // find edge intersection of groupIdx on cube(x,y,z)
        //
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // groupIdx (in): group index to check
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // edgeInt (out): array of 12 slots; stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
        virtual void findEdgeIntersection(
            uint x,
            uint y,
            uint z,
            uint groupIdx,
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList,
            const std::vector<uint>* polygonNeighbourList,
            std::vector<Intersection> (&edgeInt)[12]);
        // determine which cubes are intersected by which triangle in source mesh
        //
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        virtual void markIntersectedCubes(
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList);
        // determine which cubes are intersected by the given triangle
        //
        // triangleIdx (in): index of triangle
        // pt1x (in): x-coord of vertex 1 of triangle
        // pt1y (in): y-coord of vertex 1 of triangle
        // pt1z (in): z-coord of vertex 1 of triangle
        // pt2x (in): x-coord of vertex 2 of triangle
        // pt2y (in): y-coord of vertex 2 of triangle
        // pt2z (in): z-coord of vertex 2 of triangle
        // pt3x (in): x-coord of vertex 3 of triangle
        // pt3y (in): y-coord of vertex 3 of triangle
        // pt3z (in): z-coord of vertex 3 of triangle
        virtual void markIntersectedCubes(int triangleIdx, const cv::Point3d& pt1, const cv::Point3d& pt2, const cv::Point3d& pt3);
        // determine which cubes between the minSliceCube and maxSliceCube that are intersected by the given triangle
        //
        // triangleIdx (in): index of triangle
        // triPts (in): vertex positions of the triangle
        // minSliceCube (in:) starting slice index
        // maxSliceCube (in:) ending slice index
        // largestProjCoord (in): determines which coordinate has the largest projection area
        // scanSliceCoord (in): determines which coordinate scan slice takes place along
        // scanLineCoord (in): determines which coordinate scan line takes place along
        // cellLength_PSLCoord (in): CellLenth in terms of Proj,Slice,Line coordinate
        // nCells_PSLCoord (in): number of Cells in terms of Proj,Slice,Line coordinate
        virtual void markIntersectedCubes(
            uint triangleIdx,
            cv::Point3d triPts[3],
            uint minSliceCube,
            uint maxSliceCube,
            uint largestProjCoord,
            uint scanSliceCoord,
            uint scanLineCoord,
            double cellLength_PSLCoord[3],
            uint cells_PSLCoord[3]);
        // determine which cubes are intersected by the given line segment
        //
        // triangleIdx (in): index of triangle
        // pt1ScanLineCoord (in): scan-line coord. of vertex 1 of line segment
        // pt1ProjCoord (in): projection coord. of vertex 1 of line segment
        // pt2ScanLineCoord (in): scan-line coord. of vertex 2 of line segment
        // pt2ProjCoord (in): projection coord. of vertex 2 of line segment
        // scanSliceCubeIdx (in): slice index; determines which slice the line is at
        // projCoord (in): determines which coordinate has the largest projection area
        // scanLineCoord (in): determines which coordinate scan line takes place along
        virtual void markIntersectedCubes(
            uint triangleIdx,
            double pt1ScanLineCoord,
            double pt1ProjCoord,
            double pt2ScanLineCoord,
            double pt2ProjCoord,
            uint scanSliceCubeIdx,
            uint projCoord,
            uint scanLineCoord);
        // add triangle 'triIdx' to be intersecting cube 'cubeIdx'
        //
        // triangleIdx (in): index of intersecting triangle
        // cubeIdx (in): cube index
        // insideCube (in): true if triangle is completely inside the cube
        virtual void addIntersectingTriangle(uint triangleIdx, uint cubeIdx, bool insideCube);
        // sort the intersected triangles of cubeIdx into connected components
        //
        // cubeIdx (in): index of cube to be sorted
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual void sortIntersectingTriangle(uint cubeIdx, const std::vector<int>* polygonNeighbourList);
        // expand a connected component group by adding triIdx into groupIdx and expand it
        //
        // cubeIdx (in): index of cube to be sorted
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // groupIdx (in): index of group to expand
        // triIdx (in): index of intersecting triangle in the needSortedList array to add to the group and expand
        // needSortedList (in/out): index of intersecting triangles in intersectedTriangle need to be sorted
        virtual void expandGroup(uint cubeIdx, const std::vector<int>* polygonNeighbourList, uint groupIdx, uint triIdx, std::vector<uint>& needSortedList);
        // compute the intersecting point on the specified edge of the cube
        //
        // nX (in): x-coord of cube
        // nY (in): y-coord of cube
        // nZ (in): z-coord of cube
        // nEdgeNo (in): edge number the vertex is to be put on
        virtual Point3dId computeVertexLocation(uint x, uint y, uint z, uint edgeNo);
        // Returns the edge ID.
        virtual uint getEdgeId(uint x, uint y, uint z, uint edgeNo) const;
        // modified from base class
        // this class: each cube has 11 vertices
        // edge 12: (b,e) in special case 5b
        // edge 13: (b,e) in special case 9b
        // edge 14: (a,d) in special case 11c
        // edge 15: (b,e) in special case 12b
        // edge 16: (a,d) in special case 14c
        // edge 17: (b,e) in special case -7
        // edge 18: (a,d) in special case -3
        // edge 19: center of cube in special case -6
        virtual uint getVertexId(uint x, uint y, uint z) const;
        // Returns the face ID of cube.
        virtual uint getFaceId(uint x, uint y, uint z, uint faceNo) const;
        // unused vertices are moved to end of array;
        //
        // vertexList (in): source mesh's vertices list
        // polygonsList (in): source mesh's polygons list
        virtual void renameVerticesAndTriangles(
            const std::vector<cv::Point3d>& vertexList,
            const std::vector<std::vector<uint>>& polygonList);
        // check the triangulated cube(x,y,z) if it needs any stitching, if so, stitch it
        //
        // x (in): x-coord of cube
        // y (in): y-coord of cube
        // z (in): z-coord of cube
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual void checkStitching(uint x, uint y, uint z, const std::vector<int>* polygonNeighbourList);
        // returns true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
        //
        // edge1Idx (in): edge index 1
        // edge2Idx (in): edge index 2
        // return value (out): true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // x,y,z (in): index of cube where stitching was invoked
        // faceIdx (in): face index of where stitching occurs on cube
        virtual bool shouldStitch(uint edge1Idx, uint edge2Idx, uint x, uint y, uint z, uint faceIdx, const std::vector<int>* polygonNeighbourList);
        // returns true if patch is connected with some nontriangulable patch in adjcent cube
        // 
        // x, y, z (in): index of cube where patch is in
        // patchNo (in): patch number
        // helperCube (in): index of helper cube
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual bool isConnectedSamePatch(uint x, uint y, uint z, uint patchNo, CubeId helperCube, const std::vector<int>* polygonNeighbourList);
        // returns true if patch 1 and patch 2 are connected by a sequence of untriangulable patches
        // 
        // x1, y1, z1 (in): index of cube where patch 1 is in
        // patch1No (in): patch 1
        // x2, y2, z2 (in): index of cube where patch 2 is in
        // patch2No (in): patch 2
        // helperCube1 (in): index of helper cube 1
        // helperCube2 (in): index of helper cube 2
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        virtual bool isConnected(uint x1, uint y1, uint z1, uint patch1No, uint x2, uint y2, uint z2, uint patch2No, CubeId helperCube1, CubeId helperCube2, const std::vector<int>* polygonNeighbourList);
        // check if the triangles in checkTriList are all connected by the helper triangles in helperList
        //
        // checkTriList (in/out): index of triangles to see if one patch can be formed
        // helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // triIdx (in): index of needSortedList array to add to the patch and expand
        // return value (out): true if the triangles in needSortedList are all connected into one big patch
        virtual bool isOnePatch(std::vector<uint>& checkTriList, std::vector<uint>& helperList, const std::vector<int>* polygonNeighbourList);
        // check if the triangles in needSortedList are all connected into one big patch
        //
        // checkTriList (in/out): index of triangles to see if one patch can be formed
        // helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // index (in): index of needSortedList array to add to the patch and expand
        // indexFromCheckTriList (in): true if index is for checkTriList
        // return value (out): true if the triangles in needSortedList are all connected into one big patch
        virtual void isOnePatchAux(std::vector<uint>& checkTriList, std::vector<uint>& helperList, const std::vector<int>* polygonNeighbourList, uint index, bool indexFromCheckTriList);
        // stitch the two edges e1 and e2 together
        //
        // e1Info (in): edge info 1
        // e2Info (in): edge info 2
        // x,y,z (in): index of cube we are stitching at
        // faceIdx (in): face index of where stitching occurs
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // return value (out): true if stitching successful
        virtual bool stitch(EdgePosInfo e1Info, EdgePosInfo e2Info, uint x, uint y, uint z, uint faceIdx, const std::vector<int>* polygonNeighbourList);
        // Stitch overlapping edges; only call from stitch() to ensure we dont' introduce many overlapping vertices and run into infinite loops
        //
        // eInfo (in): info of edge that we want to stitch up with
        // x,y,z (in): index of cube where stitching was invoked
        // faceIdx (in): face index of where stitching occurs on cube
        // polygonNeighbourList (in): polygon neighbour information for source mesh
        // return value (out): true if edge is stitched
        virtual bool stitchOverlappingEdges(EdgePosInfo eInfo, uint x, uint y, uint z, uint faceIdx, const std::vector<int>* polygonNeighbourList);
        // join vertex v1Idx and v2Idx together
        //
        // v1Idx (in): index of vertex 1
        // v2Idx (in): index of vertex 2
        // return value (out): vertex ID of the joined vertex
        virtual uint stitchVertices(uint v1Idx, uint v2Idx);
        // check if vertex v1Idx and v2Idx can be joined together
        //
        // v1Idx (in): index of vertex 1
        // v2Idx (in): index of vertex 2
        // return value (out): true if can join
        virtual uint canVerticesJoin(uint v1Idx, uint v2Idx);

    public:
        // list of cube properties
        std::vector<IntersectedCube> iCubesProperty;
        // list of vertices created on edges of cube
        std::vector<std::vector<uint>> iVonE;
        // stores the relationship between v and which edge e it is on
        V2EofCube iV2EofCube;
        // list of edges created on faces of cube
        std::vector<std::vector<EdgePosInfo>> iEonF;
        // stores the relationship between e and which face f it is on
        E2F iE2F;
        // list of edges in the constructed mesh
        Id2Edge iEdgeList;
        // stores relationship of (v1,v2) to edge index
        VPair2E iVPair2E;
        // stores relationship of what edges are connected to v
        V2E iV2E;
        // stores relationship of what triangles are connected to v
        V2Tri iV2Tri;
        // stores relationship of what triangles are connected to e
        E2Tri iE2Tri;
        // stores relationship of what patch is responsible for creating the triangle
        std::vector<std::vector<uint>> iTri2Patch;
        // stores relationship of which cube the triangle is in
        std::vector<std::vector<CubeId>> iTri2CubeId;
        // the next unused vertex ID; used for adding new vertices
        uint iNextUnusedVertexId;
        // the next unused Edge ID; usd for adding new edges
        uint iNextUnusedEdgeId;
        // stores the associated triangles for the renamed vertices
        std::vector<uint> iClosestTriList;
        // number of cubes with multiple patches
        uint iNumCubeMultiPatch;
        // number of patches ignored
        uint iNumPatchIgnored;
        // number of faces needed for stitching
        uint iNumFaceStitching;
        // number of times non-manifold vertices were fixed
        uint iNumNonManifoldFix;
    };
}

#endif // DUT_TILINGNONOBTUSEMC_H
