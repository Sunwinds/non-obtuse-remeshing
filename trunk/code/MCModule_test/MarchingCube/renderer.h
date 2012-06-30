#ifndef RENDERER_H
#define RENDERER_H

#include "datadef.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

class Renderer {
public: 
    Renderer();
    ~Renderer();

    // Load Mesh Info
    bool loadMeshInfo(const float* vList, int numV,
        const int* const* fList, int numF,
        const float* vNormalsList = 0,
        const float* fNormalsList = 0);
    bool loadMeshInfo(const double* vList, int numV,
        const int* const* fList, int numF,
        const float* vNormalsList = 0,
        const float* fNormalsList = 0);
    // Load file to render
    bool loadFile(char* filename);
    // Save file
    bool saveFile(const char* filename);
    // unload models and reset all members to default
    void destroy();
    // render model
    // renderMode = 0 <--> Gouraud Shading
    // renderMode = 1 <--> Flat Shading
    // renderMode = 2 <--> Select Vertices Mode
    void render(int renderMode = 0) const;
    // highlight the special points in red
    void renderPts(int numSpecialPts, int* indices) const;
    // highlight the special points in blue
    void renderPts(const vector<int> &specialPts) const;
    // highlight one point in green
    void renderOnePt(int verySpecialPt) const;
    // highlight one triangle in green
    void renderOneTriangle(int verySpecialTriangle) const;
    // highlight special edges
    // length specifies the length of edge to be drawn
    void renderEdges(const vector<Edge> &edgeList, float length = 1.0) const;
    // get centroid of mesh
    // for better rotation
    bool getCentroid(double* centroid) const;
    // get the list of vertices coordinates; returns the number of vertices in mesh
    int getVertices(double* &vList) const;
    // returns the const list of vertices coordinates
    const double* getVertices() const;
	// return the list of vertices coordinates (Added by Hui)
	double* getVertices() {return verticesList;}
    // return the number of vertices in the mesh
    int getNumVertices() const;
    // get the list of polygons; indices start count from 1
    // returns the number of polygons in mesh
    int getPolygons(int** &pList) const;
    // get the list of polygons; indices start count from 0
    // returns the number of polygons in mesh
    int getPolygons_idx0(int** &pList) const;
    // returns the list of polygons; indices start count from 0
    const int* const* getPolygons() const;
	//returns the list of polygons; indices start count from 0 (Added by Hui)
	int** getPolygons() {return polygonsList;} 
    // return the number of polygons in the mesh
    int getNumPolygons() const;
    // return the list of vertex normals
    const double* getVertexNormals() const;
    // return the list of face normals
    const double* getFaceNormals() const;
    // returns the neighbour list
    const vector<int>* getVertexNeighbourList() const;
    // returns the edge neighbour list
    const vector<EdgeNeighbours>* getEdgeNeighbourList() const;
    // returns the polygon neighbour list
    const vector<int>* getPolygonNeighbourList() const;
    // returns true if a mesh file is successfully loaded
    bool isLoaded() const;
    // update geometry of the mesh
    void updateGeometry(double *vList, int numPts, bool bColMajor = false);
    // perform edge flip on edge connecting vertexA and vertexB
    // return the indices for the two new triangles formed
    // return value: true if edge flip was successful
    bool edgeFlip(int vertexAIdx, int vertexBIdx, int &poly1Idx, int &poly2Idx);
    // check if an edge exists
    bool isEdgeExist(int vertexAIdx, int vertexBIdx) const;
    // return the edge e's neighbour
    // return true if neighbour found
    bool findEdgeNeighbour(int vertexAIdx, int vertexBIdx, int &neighbour1Idx, int &neighbour2Idx) const; 
    // move the vertex with index "vertexIdx" to the new location (x,y,z)
    void moveVertex(int vertexIdx, double x, double y, double z);
    // return the vertex indices that have the specified vertex degree
    void getVerticesByDegree(uint degree, vector<int> &vertexIndices) const;
    // return the one ring centroid of the specified vertex; oneRingCentroid must be an array of size 3
    void getOneRingCentroid(int vertexIdx, double *oneRingCentroid) const;
    // return the one ring median of the specified vertex calculated by taking the average
    // of min_x and max_x of the one ring neighbours and respectively for y and z.
    // oneRingMedian must be an array of size 3
    void getOneRingMedian(int vertexIdx, double *oneRingMedian) const;
    // return the degree of a vertex
    int getVertexDegree(int vertexIdx) const;
    // render scalarfield
    void renderScalarField(const float *sField, unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, float isoValue) const;
    // render all cell regions
    // bWiredCell: true - render cells in wireframe
    void renderCellInfo(unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, bool bWiredCell = false) const;
    // render one cell
    void renderCellInfo(unsigned int x, unsigned int y, unsigned int z, unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, bool bWiredCell = false) const;
    /*  
    // load scalarfield
    void loadScalarField(char *filename);
    // load scalarfield
    void loadScalarField(float *sField, int nScalars, int numCellX, int numCellY, int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ);
    */

protected:
    // ------- private methods -------

    // render in gouraud shading
    void gouraudRender() const;
    // render in flat shading
    void flatRender() const;
    // render in Vertex Selection Mode
    //void vertexSelectRender();
    // computes the vertex normal for the mesh
    void computeVertexNormal();
    // computes the face normal for the mesh
    void computeFaceNormal();
    // returns the weighted average ratio for all neighbour polygons of 'vertexindex'
    void getWeightedAvgRatio(int vertexIndex, double* &weightedAvgRatio) const;
    // returns the face normal of polygon with index 'polygonIndex'
    void getFaceNormal(int polygonIndex, double* normal) const;
    // returns the vertex normal of the vertex with index 'vertexIndex'
    void getVertexNormal(int vertexIndex, double* normal) const;
    // compute centroid of mesh
    void computeCentroid();
    // returns the index for edge neighbours vertexA and vertexB in the vertexA edge neighbour array
    bool findEdge(int vertexAIdx, int vertexBIdx, vector<EdgeNeighbours>::iterator &iter) const;
    // returns the list of boundary vertices in the mesh
    void getBoundaryVertices(vector<int> &boundaryVerticesList) const;
    // return the (UNSORTED) one ring vertices of a vertex
    void getOneRing(int vertexIdx, vector<int> &oneRingList) const;

    // ------- private variables -------

    // number of vertices
    int numVertices;
    // vertices list
    double* verticesList;
    // number of polygons
    int numPolygons;
    // polygon list
    int** polygonsList;

    // centroid of mesh
    double* meshCentroid;

    // list of vertex normals
    double* vertexNormalsList;
    // list of face normals;
    double* faceNormalsList;

    // determines if a model is loaded
    bool bFileLoaded;

    // stores the polygon index that a vertex is part of
    vector<int>* vertexNeighbourList;

    // stores the edge neighbour information
    vector<EdgeNeighbours>* edgeNeighbourList;

    // stores the polygon neighbour information
    vector<int>* polygonNeighbourList;
    /*
    // scalarField
    float *scalarField;

    // scalarField properties
    unsigned int numScalars;
    unsigned int nCellsX;
    unsigned int nCellsY;
    unsigned int nCellsZ;
    float fCellLengthX;
    float fCellLengthY;
    float fCellLengthZ;
    */

public:
    bool bDisplayModel;
};

#endif
