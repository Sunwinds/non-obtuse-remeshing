#include "renderer.h"
#include "../MeshParser/smfparser.h"

Renderer::Renderer()
    :numVertices(0), verticesList(NULL), numPolygons(0),
    polygonsList(NULL), meshCentroid(NULL), vertexNormalsList(NULL),
    faceNormalsList(NULL), bFileLoaded(false), vertexNeighbourList(NULL), edgeNeighbourList(NULL),
    bDisplayModel(true)
{
}

Renderer::~Renderer()
{
    destroy();
}

// unload models and reset all members to default
void Renderer::destroy()
{
    for (int i = 0; i < numVertices; ++i)
    {
        vertexNeighbourList[i].clear();
        edgeNeighbourList[i].clear();
    }
    delete[] vertexNeighbourList;
    delete[] edgeNeighbourList;

    for (int i = 0; i < numPolygons; i++)
    {
        delete[] polygonsList[i];
        polygonNeighbourList[i].clear();
    }
    delete[] polygonNeighbourList;
    delete[] polygonsList;
    delete[] verticesList;
    delete[] vertexNormalsList;
    delete[] faceNormalsList;
    delete[] meshCentroid;

    /*  
    delete[] scalarField;
    */
    numVertices = 0;
    verticesList = NULL;
    numPolygons = 0;
    polygonsList = NULL;
    meshCentroid = NULL;
    vertexNormalsList = NULL;
    faceNormalsList = NULL;
    bFileLoaded = false;
    vertexNeighbourList = NULL;
    edgeNeighbourList = NULL;
    polygonNeighbourList = NULL;
    /*  
    scalarField = NULL;
    numScalars = 0;
    nCellsX = 0;
    nCellsY = 0;
    nCellsZ = 0;
    fCellLengthX = 0.0;
    fCellLengthY = 0.0;
    fCellLengthZ = 0.0;
    */
}

// Load Mesh Info
bool Renderer::loadMeshInfo(const float* vList, int numV,
    const int* const* fList, int numF,
    const float* vNormalsList,
    const float* fNormalsList)
{
    destroy();  // clear all members first

    // copy vertices and polygons info to member variables

    numVertices = numV;
    verticesList = new double[numVertices*3];
    for (int i = 0; i < 3*numVertices; ++i)
    {
        verticesList[i] = vList[i];
    }

    // setup vertex neighbour list
    vertexNeighbourList = new vector<int>[numVertices];
    // setup edge neighbour list
    edgeNeighbourList = new vector<EdgeNeighbours>[numVertices];
    // setup polygon neighbour list
    polygonNeighbourList = new vector<int>[numF];

    numPolygons = numF;
    polygonsList = new int*[numPolygons];
    for (int i = 0; i < numPolygons; ++i)
    {
        polygonsList[i] = new int[3];
        polygonsList[i][0] = fList[i][0];
        polygonsList[i][1] = fList[i][1];
        polygonsList[i][2] = fList[i][2];

        // stores vertex neighbour information
        vertexNeighbourList[polygonsList[i][0]].push_back(i);
        vertexNeighbourList[polygonsList[i][1]].push_back(i);
        vertexNeighbourList[polygonsList[i][2]].push_back(i);

        // stores edge neighbour information
        for (int j = 0; j < 3; ++j)
        {
            // setup the indices for the vertices
            int currentVertexIdx = polygonsList[i][j];
            int nextVertexIdx = polygonsList[i][(j+1)%3];
            int prevVertexIdx;
            if (j == 0)
                prevVertexIdx = polygonsList[i][2];
            else
                prevVertexIdx = polygonsList[i][j-1];


            bool bIsNextEdgeSetup(false);
            bool bIsPrevEdgeSetup(false);
            // search for already setup edge neighbour information
            for (vector<EdgeNeighbours>::iterator iter = edgeNeighbourList[currentVertexIdx].begin();
                iter != edgeNeighbourList[currentVertexIdx].end(); iter++)
            {
                // found "next" edge
                if (iter->e.vertex2Idx == nextVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsNextEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }
                // found "prev" edge
                else if (iter->e.vertex2Idx == prevVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsPrevEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }

                // stop searching
                if (bIsNextEdgeSetup && bIsPrevEdgeSetup)
                    break;
            }

            // next edge is not setup, so add the info now
            if (!bIsNextEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = nextVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsNextEdgeSetup = true;
            }

            // prev edge is not setup, so add the info now
            if (!bIsPrevEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = prevVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsPrevEdgeSetup = true;
            }
        }
    }

    // compute face normals
    if (fNormalsList == 0)
        computeFaceNormal();
    else
    {
        faceNormalsList = new double[numPolygons*3];
        for (int i = 0; i < 3*numPolygons; i++)
        {
            faceNormalsList[i] = fNormalsList[i];
        }
    }

    // compute vertex normals
    if (vNormalsList == 0)
        computeVertexNormal();
    else
    {
        vertexNormalsList = new double[numVertices*3];

        for (int i = 0; i < 3*numVertices; i++)
        {
            vertexNormalsList[i] = vNormalsList[i];
        }
    }

    // compute centroid of mesh
    computeCentroid();

    bFileLoaded = true;

    cout << "\nSuccessfully loaded mesh info" << endl;
    cout << "Total polygon count: " << numPolygons << endl;
    cout << "Total vertex count:  " << numVertices << endl;

    // setup vertex arrays

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, verticesList);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, vertexNormalsList);

    return true;
}

bool Renderer::loadMeshInfo(const double* vList, int numV,
    const int* const* fList, int numF,
    const float* vNormalsList,
    const float* fNormalsList)
{
    destroy();  // clear all members first

    // copy vertices and polygons info to member variables

    numVertices = numV;
    verticesList = new double[numVertices*3];
    for (int i = 0; i < 3*numVertices; ++i)
    {
        verticesList[i] = vList[i];
    }

    // setup vertex neighbour list
    vertexNeighbourList = new vector<int>[numVertices];
    // setup edge neighbour list
    edgeNeighbourList = new vector<EdgeNeighbours>[numVertices];
    // setup polygon neighbour list
    polygonNeighbourList = new vector<int>[numF];	

    numPolygons = numF;
    polygonsList = new int*[numPolygons];
    for (int i = 0; i < numPolygons; ++i)
    {
        polygonsList[i] = new int[3];
        polygonsList[i][0] = fList[i][0];
        polygonsList[i][1] = fList[i][1];
        polygonsList[i][2] = fList[i][2];

        // stores vertex neighbour information
        vertexNeighbourList[polygonsList[i][0]].push_back(i);
        vertexNeighbourList[polygonsList[i][1]].push_back(i);
        vertexNeighbourList[polygonsList[i][2]].push_back(i);

        // stores edge neighbour information
        for (int j = 0; j < 3; ++j)
        {
            // setup the indices for the vertices
            int currentVertexIdx = polygonsList[i][j];
            int nextVertexIdx = polygonsList[i][(j+1)%3];
            int prevVertexIdx;
            if (j == 0)
                prevVertexIdx = polygonsList[i][2];
            else
                prevVertexIdx = polygonsList[i][j-1];


            bool bIsNextEdgeSetup(false);
            bool bIsPrevEdgeSetup(false);
            // search for already setup edge neighbour information
            for (vector<EdgeNeighbours>::iterator iter = edgeNeighbourList[currentVertexIdx].begin();
                iter != edgeNeighbourList[currentVertexIdx].end(); iter++)
            {
                // found "next" edge
                if (iter->e.vertex2Idx == nextVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsNextEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }
                // found "prev" edge
                else if (iter->e.vertex2Idx == prevVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsPrevEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }

                // stop searching
                if (bIsNextEdgeSetup && bIsPrevEdgeSetup)
                    break;
            }

            // next edge is not setup, so add the info now
            if (!bIsNextEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = nextVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsNextEdgeSetup = true;
            }

            // prev edge is not setup, so add the info now
            if (!bIsPrevEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = prevVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsPrevEdgeSetup = true;
            }
        }
    }

    // compute face normals
    if (fNormalsList == 0)
        computeFaceNormal();
    else
    {
        faceNormalsList = new double[numPolygons*3];
        for (int i = 0; i < 3*numPolygons; i++)
        {
            faceNormalsList[i] = fNormalsList[i];
        }
    }

    // compute vertex normals
    if (vNormalsList == 0)
        computeVertexNormal();
    else
    {
        vertexNormalsList = new double[numVertices*3];

        for (int i = 0; i < 3*numVertices; i++)
        {
            vertexNormalsList[i] = vNormalsList[i];
        }
    }

    // compute centroid of mesh
    computeCentroid();

    bFileLoaded = true;

    cout << "\nSuccessfully loaded mesh info" << endl;
    cout << "Total polygon count: " << numPolygons << endl;
    cout << "Total vertex count:  " << numVertices << endl;

    // setup vertex arrays

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, verticesList);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, vertexNormalsList);

    return true;
}

// Load file to render
bool Renderer::loadFile(char* filename)
{
    destroy();  // clear all members first

    SMFParser smfparser;
    if (!smfparser.loadFile(filename))
        return false;

    vector<vector<double> > vList;
    vector<vector<int> > fList;

    if (!smfparser.parse(vList, fList))
        return false;

    // copy vertices and polygons info to member variables

    numVertices = vList.size();
    verticesList = new double[numVertices*3];
    for (int i = 0; i < numVertices; ++i)
    {
        verticesList[3*i] = vList[i][0];
        verticesList[(3*i)+1] = vList[i][1];
        verticesList[(3*i)+2] = vList[i][2];
    }

    // setup vertex neighbour list
    vertexNeighbourList = new vector<int>[numVertices];
    // setup edge neighbour list
    edgeNeighbourList = new vector<EdgeNeighbours>[numVertices];
    // setup polygon neighbour list
    polygonNeighbourList = new vector<int>[fList.size()];

    numPolygons = fList.size();
    polygonsList = new int*[numPolygons];
    for (int i = 0; i < numPolygons; ++i)
    {
        polygonsList[i] = new int[3];
        polygonsList[i][0] = fList[i][0] - 1;  // SMF files start counting from 1
        polygonsList[i][1] = fList[i][1] - 1;
        polygonsList[i][2] = fList[i][2] - 1;

        // stores vertex neighbour information
        vertexNeighbourList[polygonsList[i][0]].push_back(i);
        vertexNeighbourList[polygonsList[i][1]].push_back(i);
        vertexNeighbourList[polygonsList[i][2]].push_back(i);

        // stores edge neighbour information
        for (int j = 0; j < 3; ++j)
        {
            // setup the indices for the vertices
            int currentVertexIdx = polygonsList[i][j];
            int nextVertexIdx = polygonsList[i][(j+1)%3];
            int prevVertexIdx;
            if (j == 0)
                prevVertexIdx = polygonsList[i][2];
            else
                prevVertexIdx = polygonsList[i][j-1];


            bool bIsNextEdgeSetup(false);
            bool bIsPrevEdgeSetup(false);
            // search for already setup edge neighbour information
            for (vector<EdgeNeighbours>::iterator iter = edgeNeighbourList[currentVertexIdx].begin();
                iter != edgeNeighbourList[currentVertexIdx].end(); iter++)
            {
                // found "next" edge 
                if (iter->e.vertex2Idx == nextVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsNextEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }
                // found "prev" edge
                else if (iter->e.vertex2Idx == prevVertexIdx)
                {
                    iter->polyNeighbour2Idx = i;
                    bIsPrevEdgeSetup = true;

                    // add polygon neighbour information
                    vector<int> &nbr1List = polygonNeighbourList[i];
                    if (find(nbr1List.begin(), nbr1List.end(), iter->polyNeighbour1Idx) == nbr1List.end())
                        nbr1List.push_back(iter->polyNeighbour1Idx);
                    vector<int> &nbr2List = polygonNeighbourList[iter->polyNeighbour1Idx];
                    if (find(nbr2List.begin(), nbr2List.end(), i) == nbr2List.end())
                        nbr2List.push_back(i);
                }

                // stop searching
                if (bIsNextEdgeSetup && bIsPrevEdgeSetup)
                    break;
            }

            // next edge is not setup, so add the info now
            if (!bIsNextEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = nextVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsNextEdgeSetup = true;
            }

            // prev edge is not setup, so add the info now
            if (!bIsPrevEdgeSetup)
            {
                EdgeNeighbours en;
                en.e.vertex1Idx = currentVertexIdx;
                en.e.vertex2Idx = prevVertexIdx;
                en.polyNeighbour1Idx = i;
                en.polyNeighbour2Idx = -1;

                edgeNeighbourList[currentVertexIdx].push_back(en);

                bIsPrevEdgeSetup = true;
            }
        }
    }

    // compute face normals
    computeFaceNormal();
    // compute vertex normals
    computeVertexNormal();

    // compute centroid of mesh
    computeCentroid();

    bFileLoaded = true;

    cout << "\nSuccessfully loaded " << filename << endl;
    cout << "Total polygon count: " << numPolygons << endl;
    cout << "Total vertex count:  " << numVertices << endl;

    // setup vertex arrays

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, verticesList);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, vertexNormalsList);

    return true;
}

// Save file
bool Renderer::saveFile(const char* filename)
{
    if (!bFileLoaded)
        return false;

    ofstream outFile;
    outFile.open(filename, ofstream::out);
    outFile.precision(10);

    if (!outFile.is_open())
        return false;

    // first line records the number of vertices and number of polygons
    //outFile << "# " << numVertices << " " << numPolygons << endl;
    outFile << "#$SMF 1.0" << endl;
    outFile << "#$vertices " << numVertices << endl;
    outFile << "#$faces " << numPolygons << endl;

    // record list of vertices
    for (int i = 0; i < numVertices; i++)
    {
        outFile << "v " << verticesList[3*i];
        outFile << " " << verticesList[(3*i)+1];
        outFile << " " << verticesList[(3*i)+2] << endl;
    }

    // record list of polygons
    for (int i = 0; i < numPolygons; i++)
    {
        outFile << "f " << polygonsList[i][0]+1;  // smf starts counting at 1 not 0
        outFile << " " << polygonsList[i][1]+1;
        outFile << " " << polygonsList[i][2]+1 << endl;
    }

    outFile.close();

    cout << "\nSuccessfully saved " << filename << endl;

    return true;
}

// render model
// renderMode = 0 <--> Gouraud Shading
// renderMode = 1 <--> Flat Shading
// renderMode = 2 <--> Select Vertices Mode
void Renderer::render(int renderMode) const
{
    if(!bDisplayModel)
        return;
    // sanity check
    if (!bFileLoaded)
        return;
    // render mesh
    if (renderMode == 0)
        gouraudRender();
    else if (renderMode == 1)
        flatRender();
    //  else
    //  	vertexSelectRender();
}

// highlight the special points in red
void Renderer::renderPts(int numSpecialPts, int* indices) const
{
    // sanity checks
    if (indices == NULL)
        return;

    // highlight special vertices
    if (numSpecialPts > 0)
    {
        glDisable(GL_LIGHTING);
        glPointSize(5.0);
        glColor3f(1.0, 0.0, 0.0);

        glBegin(GL_POINTS);
        for (int i = 0; i < numSpecialPts; i++)
        {
            glVertex3d(verticesList[3*indices[i]],
                verticesList[3*indices[i]+1],
                verticesList[3*indices[i]+2]);
        }
        glEnd();

        glPointSize(1.0);
        glColor3f(0.0, 0.0, 0.0);
        glEnable(GL_LIGHTING);
    }
}

// highlight the special points in blue
void Renderer::renderPts(const vector<int> &specialPts) const
{
    // highlight special vertices
    if (!specialPts.empty())
    {
        glDisable(GL_LIGHTING);
        glPointSize(5.0);
        glColor3f(1.0, 0.0, 0.0);

        glBegin(GL_POINTS);
        for (vector<int>::const_iterator iter = specialPts.begin();
            iter != specialPts.end(); iter++)
        {
            glVertex3d(verticesList[3*(*iter)],
                verticesList[3*(*iter)+1],
                verticesList[3*(*iter)+2]);
        }
        glEnd();

        glPointSize(1.0);
        glColor3f(0.0, 0.0, 0.0);
        glEnable(GL_LIGHTING);
    }
}

// highlight one point in green
void Renderer::renderOnePt(int verySpecialPt) const
{
    // render one very special point
    if (verySpecialPt >= 0 && verySpecialPt < numVertices)
    {
        glDisable(GL_LIGHTING);
        glPointSize(5.0);
        glColor3f(0.0, 1.0, 0.0);

        glBegin(GL_POINTS);
        glVertex3d(verticesList[3*verySpecialPt],
            verticesList[3*verySpecialPt+1],
            verticesList[3*verySpecialPt+2]);
        glEnd();

        glPointSize(1.0);
        glColor3f(0.0, 0.0, 0.0);
        glEnable(GL_LIGHTING);
    }
}

// highlight one triangle in green
void Renderer::renderOneTriangle(int verySpecialTriangle) const
{
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_FILL);
    //glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][0]],     // vertex 1
        verticesList[3*polygonsList[verySpecialTriangle][0]+1],
        verticesList[3*polygonsList[verySpecialTriangle][0]+2]);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][1]],     // vertex 2
        verticesList[3*polygonsList[verySpecialTriangle][1]+1],
        verticesList[3*polygonsList[verySpecialTriangle][1]+2]);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][2]],     // vertex 3
        verticesList[3*polygonsList[verySpecialTriangle][2]+1],
        verticesList[3*polygonsList[verySpecialTriangle][2]+2]);
    glEnd();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPolygonOffset(-5.0f, -5.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][0]],     // vertex 1
        verticesList[3*polygonsList[verySpecialTriangle][0]+1],
        verticesList[3*polygonsList[verySpecialTriangle][0]+2]);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][1]],     // vertex 2
        verticesList[3*polygonsList[verySpecialTriangle][1]+1],
        verticesList[3*polygonsList[verySpecialTriangle][1]+2]);
    glNormal3d(faceNormalsList[3*verySpecialTriangle],
        faceNormalsList[3*verySpecialTriangle+1],
        faceNormalsList[3*verySpecialTriangle+2]);
    glVertex3d(verticesList[3*polygonsList[verySpecialTriangle][2]],     // vertex 3
        verticesList[3*polygonsList[verySpecialTriangle][2]+1],
        verticesList[3*polygonsList[verySpecialTriangle][2]+2]);
    glEnd();
    glPopAttrib();

    glColor3f(0.0, 0.0, 0.0);
    glEnable(GL_LIGHTING);
}

// highlight special edges
// percentage specifies the percentage of edge to be drawn
void Renderer::renderEdges(const vector<Edge> &edgeList, float percentage) const
{
    // highlight special edges
    if (!edgeList.empty())
    {
        glDisable(GL_LIGHTING);
        glLineWidth(5.0);
        glColor3f(1.0, 1.0, 1.0);

        double* vertex = new double[3];

        glBegin(GL_LINES);
        for (vector<Edge>::const_iterator iter = edgeList.begin();
            iter != edgeList.end(); iter++)
        {
            glVertex3d(verticesList[3*(iter->vertex1Idx)],
                verticesList[3*(iter->vertex1Idx)+1],
                verticesList[3*(iter->vertex1Idx)+2]);	 

            linearInterp(percentage, vertex, verticesList[3*(iter->vertex1Idx)], verticesList[3*(iter->vertex1Idx)+1], verticesList[3*(iter->vertex1Idx)+2],
                verticesList[3*(iter->vertex2Idx)], verticesList[3*(iter->vertex2Idx)+1], verticesList[3*(iter->vertex2Idx)+2]);	

            glVertex3d(vertex[0], vertex[1], vertex[2]);

        }
        glEnd();

        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(1.0);
        glEnable(GL_LIGHTING);

        // clean up
        delete[] vertex;
    }
}

// render in gouraud shading
void Renderer::gouraudRender() const
{
    // setup vertex arrays
    glVertexPointer(3, GL_DOUBLE, 0, verticesList);
    glNormalPointer(GL_DOUBLE, 0, vertexNormalsList);

    for (int i = 0; i < numPolygons; i++)
    {
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, polygonsList[i]);
    }
}

// render in flat shading
void Renderer::flatRender() const
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numPolygons; i++)
    {
        glNormal3d(faceNormalsList[3*i],
            faceNormalsList[3*i+1],
            faceNormalsList[3*i+2]);
        glVertex3d(verticesList[3*polygonsList[i][0]],     // vertex 1
            verticesList[3*polygonsList[i][0]+1],
            verticesList[3*polygonsList[i][0]+2]);
        glNormal3d(faceNormalsList[3*i],
            faceNormalsList[3*i+1],
            faceNormalsList[3*i+2]);
        glVertex3d(verticesList[3*polygonsList[i][1]],     // vertex 2
            verticesList[3*polygonsList[i][1]+1],
            verticesList[3*polygonsList[i][1]+2]);
        glNormal3d(faceNormalsList[3*i],
            faceNormalsList[3*i+1],
            faceNormalsList[3*i+2]);
        glVertex3d(verticesList[3*polygonsList[i][2]],     // vertex 3
            verticesList[3*polygonsList[i][2]+1],
            verticesList[3*polygonsList[i][2]+2]);
    }
    glEnd();
}

// render in Vertex Selection Mode
// void Renderer::vertexSelectRender()
// {
// 	for (int i = 0; i < numVertices; i++)
// 	{
// 		glLoadName(i);
// 		glBegin(GL_POINTS);
// 		glVertex3d(verticesList[3*i],
// 				verticesList[3*i+1],
// 				verticesList[3*i+2]);
// 		glEnd();
// 	}
// }

// computes the vertex normal for the mesh
void Renderer::computeVertexNormal()
{
    // sanity checks
    if (faceNormalsList == NULL)    // must compute face normals first
        return;
    if (vertexNormalsList != NULL)
        delete[] vertexNormalsList;

    // allocate space
    vertexNormalsList = new double[numVertices*3];

    double* vertexNormal = new double[3];
    for (int i = 0; i < numVertices; i++)
    {
        getVertexNormal(i, vertexNormal);

        for (int j = 0; j < 3; j++)
            vertexNormalsList[3*i+j] = vertexNormal[j];    
    }
    delete[] vertexNormal;
}

// computes the face normal for the mesh
void Renderer::computeFaceNormal()
{
    // sanity checks
    if (faceNormalsList != NULL)
        delete[] faceNormalsList;

    faceNormalsList = new double[numPolygons*3];

    double* faceNormal = new double[3];
    for (int i = 0; i < numPolygons; i++)
    {
        getFaceNormal(i, faceNormal);

        for (int j = 0; j < 3; j++)
            faceNormalsList[3*i+j] = faceNormal[j];
    }
    delete[] faceNormal;
}

// returns the weighted average ratio for all neighbour polygons of 'vertexindex'
void Renderer::getWeightedAvgRatio(int vertexIndex, double* &weightedAvgRatio) const
{
    if (weightedAvgRatio != NULL) // weightedAvgRatio needs to be NULL
        return;

    // setup output allocation
    int numNeighbours = vertexNeighbourList[vertexIndex].size();
    weightedAvgRatio = new double[numNeighbours];

    // setup allocation to store the polygon's area
    double* neighbourArea = new double[numNeighbours];
    double sumNeighbourArea(0.0);

    // compute each polygon's area
    int i(0);
    double *pt1 = new double[3];
    double *pt2 = new double[3];
    double *pt3 = new double[3];
    for (vector<int>::iterator iter = vertexNeighbourList[vertexIndex].begin();
        iter != vertexNeighbourList[vertexIndex].end();
        iter++)
    {
        pt1[0] = verticesList[3*polygonsList[*iter][0]];
        pt1[1] = verticesList[(3*polygonsList[*iter][0])+1];
        pt1[2] = verticesList[(3*polygonsList[*iter][0])+2];
        pt2[0] = verticesList[3*polygonsList[*iter][1]];
        pt2[1] = verticesList[(3*polygonsList[*iter][1])+1];
        pt2[2] = verticesList[(3*polygonsList[*iter][1])+2];
        pt3[0] = verticesList[3*polygonsList[*iter][2]];
        pt3[1] = verticesList[(3*polygonsList[*iter][2])+1];
        pt3[2] = verticesList[(3*polygonsList[*iter][2])+2];

        neighbourArea[i] = areaTriangle(pt1, pt2, pt3);
        sumNeighbourArea += neighbourArea[i];
        i++;
    }

    if (sumNeighbourArea == 0.0)   // something wrong
        return;

    // calculate the weighted average
    for (int i = 0; i < numNeighbours; i++)
        weightedAvgRatio[i] = neighbourArea[i] / sumNeighbourArea;

    delete[] pt1;
    delete[] pt2;
    delete[] pt3;
    delete[] neighbourArea;
}

// returns the face normal of polygon with index 'polygonIndex'
void Renderer::getFaceNormal(int polygonIndex, double* normal) const
{
    double edge1[3];
    double edge2[3];

    vectorSub(edge1,
        verticesList[3*polygonsList[polygonIndex][1]],
        verticesList[3*polygonsList[polygonIndex][1]+1],
        verticesList[3*polygonsList[polygonIndex][1]+2],
        verticesList[3*polygonsList[polygonIndex][0]],
        verticesList[3*polygonsList[polygonIndex][0]+1],
        verticesList[3*polygonsList[polygonIndex][0]+2]);
    vectorSub(edge2,
        verticesList[3*polygonsList[polygonIndex][2]],
        verticesList[3*polygonsList[polygonIndex][2]+1],
        verticesList[3*polygonsList[polygonIndex][2]+2],
        verticesList[3*polygonsList[polygonIndex][0]],
        verticesList[3*polygonsList[polygonIndex][0]+1],
        verticesList[3*polygonsList[polygonIndex][0]+2]);
    crossProduct(normal, edge1, edge2);
    normalize(normal);
}

// returns the vertex normal of the vertex with index 'vertexIndex'
void Renderer::getVertexNormal(int vertexIndex, double* normal) const
{
    // sanity checks
    if (faceNormalsList == NULL)
        return;

    normal[0] = 0.0;
    normal[1] = 0.0;
    normal[2] = 0.0;

    double* weightedAvgRatio = NULL;
    getWeightedAvgRatio(vertexIndex, weightedAvgRatio);

    // weighted avg of the nearby face normals
    int numNeighbours = vertexNeighbourList[vertexIndex].size();
    for (int i = 0; i < numNeighbours; i++)
    {
        normal[0] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]];
        normal[1] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]+1];
        normal[2] += weightedAvgRatio[i] * faceNormalsList[3*vertexNeighbourList[vertexIndex][i]+2];
    }

    normalize(normal);

    // clean up
    delete[] weightedAvgRatio;
}

// compute centroid of mesh
void Renderer::computeCentroid()
{
    // sanity checks
    if (meshCentroid == NULL)
        meshCentroid = new double[3];

    meshCentroid[0] = 0.0;
    meshCentroid[1] = 0.0;
    meshCentroid[2] = 0.0;
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < 3; j++)
            meshCentroid[j] += verticesList[3*i+j];
    }

    for (int i = 0; i < 3; i++)
        meshCentroid[i] /= numVertices;
}

// get centroid of mesh
// for better rotation
bool Renderer::getCentroid(double* centroid) const
{
    if (meshCentroid == NULL)
        return false;

    for (int j = 0; j < 3; j++)
        centroid[j] = meshCentroid[j];

    return true;
}

// get the list of vertices coordinates; returns the number of vertices in mesh
int Renderer::getVertices(double* &vList) const
{
    // sanity checks
    if (vList != NULL)
        return -1;

    vList = new double[numVertices*3];
    for (int i = 0; i < numVertices; i++)
        for (int j = 0; j < 3; j++)
            vList[3*i+j] = verticesList[3*i+j];

    return numVertices;
}

// returns the const list of vertices coordinates
const double* Renderer::getVertices() const
{
    return verticesList;
}

// get the list of polygons; indices start count from 1
// returns the number of polygons in mesh
int Renderer::getPolygons(int** &pList) const
{
    // sanity checks
    if (pList != NULL)
        return -1;

    pList = new int*[numPolygons];
    for (int i = 0; i < numPolygons; i++)
    {
        pList[i] = new int[3];
        for (int j = 0; j < 3; j++)
            pList[i][j] = polygonsList[i][j] + 1;   // indices start counting from 1
    }

    return numPolygons;
}

// get the list of polygons; indices start count from 0
// returns the number of polygons in mesh
int Renderer::getPolygons_idx0(int** &pList) const
{
    // sanity checks
    if (pList != NULL)
        return -1;

    pList = new int*[numPolygons];
    for (int i = 0; i < numPolygons; i++)
    {
        pList[i] = new int[3];
        for (int j = 0; j < 3; j++)
            pList[i][j] = polygonsList[i][j];   // indices start counting from 0
    }

    return numPolygons;
}

// returns the list of polygons; indices start count from 0
const int* const* Renderer::getPolygons() const
{
    return polygonsList;
}

// return the number of polygons in the mesh
int Renderer::getNumPolygons() const
{
    return numPolygons;
}

// returns true if a mesh file is successfully loaded
bool Renderer::isLoaded() const
{
    return bFileLoaded;
}

// update geometry of the mesh
void Renderer::updateGeometry(double *vList, int numPts, bool bColMajor)
{
    // sanity checks
    if (!bFileLoaded)        // only can update if a mesh is loaded
        return;
    if (numPts != numVertices)  // only allow updates if number of vertices are equal
        return;

    // update geometry
    for (int i = 0; i < numVertices; i++)
    {
        if (bColMajor == false)
        {
            verticesList[3*i] = vList[3*i];
            verticesList[(3*i)+1] = vList[3*i+1];
            verticesList[(3*i)+2] = vList[3*i+2];
        }
        else
        {
            verticesList[3*i] = vList[i];
            verticesList[(3*i)+1] = vList[numVertices+i];
            verticesList[(3*i)+2] = vList[2*numVertices+i];
        }
    }

    // update face normals
    computeFaceNormal();
    // update vertex normals
    computeVertexNormal();
    // update centroid of mesh
    computeCentroid();

    cout << "\nSuccessfully updated " << numPts << " of vertices" << endl;
}

// perform edge flip on edge connecting vertexA and vertexB
// return the indices for the two new triangles formed
// return value: true if edge flip was successful
bool Renderer::edgeFlip(int vertexAIdx, int vertexBIdx, int &poly1Idx, int &poly2Idx)
{
    // sanity checks
    if (vertexAIdx >= numVertices || vertexBIdx >= numVertices)
        return false;

    // flipping edge that connects to a degree-3 vertex
    if (edgeNeighbourList[vertexAIdx].size() <= 3 || edgeNeighbourList[vertexBIdx].size() <= 3)
        return false;

    // ----- 1. do initial setups -----

    // setup indices to locate the edge neighbour informations
    vector<EdgeNeighbours>::iterator vertexANeighbourIter;
    vector<EdgeNeighbours>::iterator vertexBNeighbourIter;
    if (!findEdge(vertexAIdx, vertexBIdx, vertexANeighbourIter) || !findEdge(vertexBIdx, vertexAIdx, vertexBNeighbourIter))
        return false;

    // indices for the two polygons that will be removed after the edge flip
    int oldPoly1Idx = vertexANeighbourIter->polyNeighbour1Idx;
    int oldPoly2Idx = vertexANeighbourIter->polyNeighbour2Idx;

    // can't do edge flip
    if (oldPoly2Idx == -1)
        return false;

    // indices for the two vertices that will connect to make up the new edge
    int newEdgeEndPt1Idx(-1);
    int newEdgeEndPt2Idx(-1);
    for (int i = 0; i < 3; ++i)
    {
        if (polygonsList[oldPoly1Idx][i] != vertexAIdx && polygonsList[oldPoly1Idx][i] != vertexBIdx)
            newEdgeEndPt1Idx = polygonsList[oldPoly1Idx][i];
        if (polygonsList[oldPoly2Idx][i] != vertexAIdx && polygonsList[oldPoly2Idx][i] != vertexBIdx)
            newEdgeEndPt2Idx = polygonsList[oldPoly2Idx][i];
    }

    // can't find end pt1 or end pt2
    if (newEdgeEndPt1Idx == -1 || newEdgeEndPt2Idx == -1)
        return false;


    // ----- 2. remove all the vertexNeighbour information for the old polys -----

    // newEdgeEndPt1Idx's neighbour
    for(vector<int>::iterator iter = vertexNeighbourList[newEdgeEndPt1Idx].begin();
        iter != vertexNeighbourList[newEdgeEndPt1Idx].end(); iter++)
    {
        if (*iter == oldPoly1Idx || *iter == oldPoly2Idx)
        {
            vertexNeighbourList[newEdgeEndPt1Idx].erase(iter);
            break;
        }
    }

    // newEdgeEndPt2Idx's neighbour
    for(vector<int>::iterator iter = vertexNeighbourList[newEdgeEndPt2Idx].begin();
        iter != vertexNeighbourList[newEdgeEndPt2Idx].end(); iter++)
    {
        if (*iter == oldPoly1Idx || *iter == oldPoly2Idx)
        {
            vertexNeighbourList[newEdgeEndPt2Idx].erase(iter);
            break;
        }
    }

    // vertexAidx's neighbour
    int eraseCount(0);
    vector<int>::iterator iter = vertexNeighbourList[vertexAIdx].begin();
    while (iter != vertexNeighbourList[vertexAIdx].end())
    {
        if (*iter == oldPoly1Idx || *iter == oldPoly2Idx)
        {
            iter = vertexNeighbourList[vertexAIdx].erase(iter);
            if (++eraseCount > 1)
                break;

            continue;
        }

        iter++;
    }

    // vertexBidx's neighbour
    eraseCount = 0;
    iter = vertexNeighbourList[vertexBIdx].begin();
    while (iter != vertexNeighbourList[vertexBIdx].end())
    {
        if (*iter == oldPoly1Idx || *iter == oldPoly2Idx)
        {
            vertexNeighbourList[vertexBIdx].erase(iter);
            if (++eraseCount > 1)
                break;

            continue;
        }

        iter++;
    }

    // ----- 3. update the old polys to the new polys -----

    // oldpoly1
    for (int i = 0; i < 3; ++i)
    {
        if (polygonsList[oldPoly1Idx][i] == vertexAIdx)
        {
            polygonsList[oldPoly1Idx][i] = newEdgeEndPt2Idx;
            break;
        }
    }

    // oldpoly2
    for (int i = 0; i < 3; ++i)
    {
        if (polygonsList[oldPoly2Idx][i] == vertexBIdx)
        {
            polygonsList[oldPoly2Idx][i] = newEdgeEndPt1Idx;
            break;
        }
    }

    // ----- 4. update edge neighbour information -----

    // erase the old edge first
    edgeNeighbourList[vertexAIdx].erase(vertexANeighbourIter);
    edgeNeighbourList[vertexBIdx].erase(vertexBNeighbourIter);

    // add the new edge information
    EdgeNeighbours en;
    en.e.vertex1Idx = newEdgeEndPt1Idx;
    en.e.vertex2Idx = newEdgeEndPt2Idx;
    en.polyNeighbour1Idx = oldPoly1Idx;
    en.polyNeighbour2Idx = oldPoly2Idx;
    edgeNeighbourList[newEdgeEndPt1Idx].push_back(en);

    en.e.vertex1Idx = newEdgeEndPt2Idx;
    en.e.vertex2Idx = newEdgeEndPt1Idx;
    edgeNeighbourList[newEdgeEndPt2Idx].push_back(en);

    // update other edges
    // indices for the surrounding edges
    vector<EdgeNeighbours>::iterator vAnew1Iter, new1vAIter, vBnew2Iter, new2vBIter;
    findEdge(vertexAIdx, newEdgeEndPt1Idx, vAnew1Iter);
    findEdge(newEdgeEndPt1Idx, vertexAIdx, new1vAIter);
    findEdge(vertexBIdx, newEdgeEndPt2Idx, vBnew2Iter);
    findEdge(newEdgeEndPt2Idx, vertexBIdx, new2vBIter);

    if (vAnew1Iter->polyNeighbour1Idx == oldPoly1Idx)
        vAnew1Iter->polyNeighbour1Idx = oldPoly2Idx;
    else
        vAnew1Iter->polyNeighbour2Idx = oldPoly2Idx;

    if (new1vAIter->polyNeighbour1Idx == oldPoly1Idx)
        new1vAIter->polyNeighbour1Idx = oldPoly2Idx;
    else
        new1vAIter->polyNeighbour2Idx = oldPoly2Idx;

    if (vBnew2Iter->polyNeighbour1Idx == oldPoly2Idx)
        vBnew2Iter->polyNeighbour1Idx = oldPoly1Idx;
    else
        vBnew2Iter->polyNeighbour2Idx = oldPoly1Idx;

    if (new2vBIter->polyNeighbour1Idx == oldPoly2Idx)
        new2vBIter->polyNeighbour1Idx = oldPoly1Idx;
    else
        new2vBIter->polyNeighbour2Idx = oldPoly1Idx;


    // ----- 5. Update vertex neighbour information -----

    // newEdgeEndPt1Idx's neighbours
    vertexNeighbourList[newEdgeEndPt1Idx].push_back(oldPoly1Idx);
    vertexNeighbourList[newEdgeEndPt1Idx].push_back(oldPoly2Idx);

    // newEdgeEndPt2Idx's neighbours
    vertexNeighbourList[newEdgeEndPt2Idx].push_back(oldPoly1Idx);
    vertexNeighbourList[newEdgeEndPt2Idx].push_back(oldPoly2Idx);

    // vertexAIdx's neighbours
    vertexNeighbourList[vertexAIdx].push_back(oldPoly2Idx);

    // vertexBIdx's neighbours
    vertexNeighbourList[vertexBIdx].push_back(oldPoly1Idx);

    // ----- 6. return the new polygon indices
    poly1Idx = oldPoly1Idx;
    poly2Idx = oldPoly2Idx;

    return true;
}

// returns the index for edge neighbours vertexA and vertexB in the vertexA edge neighbour array
bool Renderer::findEdge(int vertexAIdx, int vertexBIdx, vector<EdgeNeighbours>::iterator &iter) const
{
    // sanity checks
    if (vertexAIdx >= numVertices || vertexBIdx >= numVertices)
        return false;

    for (iter = edgeNeighbourList[vertexAIdx].begin();
        iter != edgeNeighbourList[vertexAIdx].end(); iter++)
    {
        if (iter->e.vertex2Idx == vertexBIdx)
            return true;
    }

    return false;
}

// return the number of vertices in the mesh
int Renderer::getNumVertices() const
{
    return numVertices;
}

// check if an edge exists
bool Renderer::isEdgeExist(int vertexAIdx, int vertexBIdx) const
{
    vector<EdgeNeighbours>::iterator iter;
    return (findEdge(vertexAIdx, vertexBIdx, iter) && findEdge(vertexBIdx, vertexAIdx, iter));
}

// return the edge e's neighbour
// return true if neighbour found
bool Renderer::findEdgeNeighbour(int vertexAIdx, int vertexBIdx, int &neighbour1Idx, int &neighbour2Idx) const
{
    vector<EdgeNeighbours>::iterator iter;
    if (findEdge(vertexAIdx, vertexBIdx, iter))
    {
        neighbour1Idx = iter->polyNeighbour1Idx;
        neighbour2Idx = iter->polyNeighbour2Idx;
        return true;
    }

    return false;
}

// move the vertex with index "vertexIdx" to the new location (x,y,z)
void Renderer::moveVertex(int vertexIdx, double x, double y, double z)
{
    // sanity checks
    if (vertexIdx >= numVertices)
        return;

    // update geometry
    verticesList[3*vertexIdx] = x;
    verticesList[3*vertexIdx+1] = y;
    verticesList[3*vertexIdx+2] = z;

    // update face normals for neighbour polygons
    double* normal = new double[3];
    for (vector<int>::iterator iter = vertexNeighbourList[vertexIdx].begin();
        iter != vertexNeighbourList[vertexIdx].end(); iter++)
    {
        getFaceNormal(*iter, normal);
        for (int j = 0; j < 3; ++j)
            faceNormalsList[3*(*iter)+j] = normal[j];
    }

    // update vertex normal
    getVertexNormal(vertexIdx, normal);
    for (int j = 0; j < 3; j++)
        vertexNormalsList[3*vertexIdx+j] = normal[j];

    // update centroid
    computeCentroid();

    // clean up
    delete[] normal;
}

// return the list of vertex normals
const double* Renderer::getVertexNormals() const
{
    return vertexNormalsList;
}

// return the list of face normals
const double* Renderer::getFaceNormals() const
{
    return faceNormalsList;
}

// returns the neighbour list
const vector<int>* Renderer::getVertexNeighbourList() const
{
    return vertexNeighbourList;
}

// returns the edge neighbour list
const vector<EdgeNeighbours>* Renderer::getEdgeNeighbourList() const
{
    return edgeNeighbourList;
}

// returns the polygon neighbour list
const vector<int>* Renderer::getPolygonNeighbourList() const
{
    return polygonNeighbourList;
}

// return the vertex indices that have the specified vertex degree
void Renderer::getVerticesByDegree(uint degree, vector<int> &vertexIndices) const
{
    for (int i = 0; i < numVertices; ++i)
    {
        if (edgeNeighbourList[i].size() == degree)
            vertexIndices.push_back(i);
    }
}

// return the one ring centroid of the specified vertex; oneRingCentroid must be an array of size 3
void Renderer::getOneRingCentroid(int vertexIdx, double *oneRingCentroid) const
{
    oneRingCentroid[0] = 0.0;
    oneRingCentroid[1] = 0.0;
    oneRingCentroid[2] = 0.0;

    vector<int> oneRingVerticesIdx;
    bool isInList(false);

    // find all the one ring vertices
    for (vector<int>::const_iterator iter = getVertexNeighbourList()[vertexIdx].begin();
        iter != getVertexNeighbourList()[vertexIdx].end(); iter++)
    {
        // find the other two neighbour vertices
        for (int i = 0; i < 3; ++i)
        {
            if (polygonsList[*iter][i] != vertexIdx)
            {
                // check if the vertex index is in the list already
                isInList = false;
                for (vector<int>::iterator iter2 = oneRingVerticesIdx.begin();
                    iter2 != oneRingVerticesIdx.end(); iter2++)
                {
                    if (*iter2 == polygonsList[*iter][i])
                    {
                        isInList = true;
                        break;
                    }
                }

                // insert the index if it's not in the list
                if (!isInList)
                    oneRingVerticesIdx.push_back(polygonsList[*iter][i]);
            }
        }
    }

    // calculate the average of the one ring vertices
    for (vector<int>::iterator iter2 = oneRingVerticesIdx.begin();
        iter2 != oneRingVerticesIdx.end(); iter2++)
    {
        oneRingCentroid[0] += verticesList[3*(*iter2)];
        oneRingCentroid[1] += verticesList[3*(*iter2)+1];
        oneRingCentroid[2] += verticesList[3*(*iter2)+2];
    }

    oneRingCentroid[0] /= oneRingVerticesIdx.size();
    oneRingCentroid[1] /= oneRingVerticesIdx.size();
    oneRingCentroid[2] /= oneRingVerticesIdx.size();
}

// return the one ring median of the specified vertex calculated by taking the average
// of min_x and max_x of the one ring neighbours and respectively for y and z.
// oneRingMedian must be an array of size 3
void Renderer::getOneRingMedian(int vertexIdx, double *oneRingMedian) const
{
    oneRingMedian[0] = 0.0;
    oneRingMedian[1] = 0.0;
    oneRingMedian[2] = 0.0;

    vector<int> oneRingVerticesIdx;
    bool isInList(false);

    // find all the one ring vertices
    for (vector<int>::const_iterator iter = getVertexNeighbourList()[vertexIdx].begin();
        iter != getVertexNeighbourList()[vertexIdx].end(); iter++)
    {
        // find the other two neighbour vertices
        for (int i = 0; i < 3; ++i)
        {
            if (polygonsList[*iter][i] != vertexIdx)
            {
                // check if the vertex index is in the list already
                isInList = false;
                for (vector<int>::iterator iter2 = oneRingVerticesIdx.begin();
                    iter2 != oneRingVerticesIdx.end(); iter2++)
                {
                    if (*iter2 == polygonsList[*iter][i])
                    {
                        isInList = true;
                        break;
                    }
                }

                // insert the index if it's not in the list
                if (!isInList)
                    oneRingVerticesIdx.push_back(polygonsList[*iter][i]);
            }
        }
    }

    // find the min and max of the one ring vertices
    double minx(0.0);
    double maxx(0.0);
    double miny(0.0);
    double maxy(0.0);
    double minz(0.0);
    double maxz(0.0);

    bool initMinx(false);
    bool initMiny(false);
    bool initMinz(false);

    for (vector<int>::iterator iter2 = oneRingVerticesIdx.begin();
        iter2 != oneRingVerticesIdx.end(); iter2++)
    {
        if (verticesList[3*(*iter2)] > maxx)
            maxx = verticesList[3*(*iter2)];
        if (verticesList[3*(*iter2)] < minx || !initMinx)
        {
            minx = verticesList[3*(*iter2)];
            initMinx = true;
        }

        if (verticesList[3*(*iter2)+1] > maxy)
            maxy = verticesList[3*(*iter2)+1];
        if (verticesList[3*(*iter2)+1] < miny || !initMiny)
        {
            miny = verticesList[3*(*iter2)+1];
            initMiny = true;
        }

        if (verticesList[3*(*iter2)+2] > maxz)
            maxz = verticesList[3*(*iter2)+2];
        if (verticesList[3*(*iter2)+2] < minz || !initMinz)
        {
            minz = verticesList[3*(*iter2)+2];
            initMinz = true;
        }
    }

    oneRingMedian[0] = (minx + maxx)/2.0;
    oneRingMedian[1] = (miny + maxy)/2.0;
    oneRingMedian[2] = (minz + maxz)/2.0;
}

// return the degree of a vertex
int Renderer::getVertexDegree(int vertexIdx) const
{
    if (vertexIdx >= numVertices)
        return -1;

    return edgeNeighbourList[vertexIdx].size();
}

// return the (UNSORTED) one ring vertices of a vertex
void Renderer::getOneRing(int vertexIdx, vector<int> &oneRingList) const
{
    if (!oneRingList.empty())
        oneRingList.clear();

    // find all the one ring vertices
    for (vector<int>::const_iterator iter = getVertexNeighbourList()[vertexIdx].begin();
        iter != getVertexNeighbourList()[vertexIdx].end(); iter++)
    {
        // find the other two neighbour vertices
        for (int i = 0; i < 3; ++i)
        {
            if (polygonsList[*iter][i] != vertexIdx)
            {
                // check if the vertex index is in the list already
                bool isInList = false;
                for (vector<int>::iterator iter2 = oneRingList.begin();
                    iter2 != oneRingList.end(); iter2++)
                {
                    if (*iter2 == polygonsList[*iter][i])
                    {
                        isInList = true;
                        break;
                    }
                }

                // insert the index if it's not in the list
                if (!isInList)
                    oneRingList.push_back(polygonsList[*iter][i]);
            }
        }
    }
}

// returns the list of boundary vertices in the mesh
void Renderer::getBoundaryVertices(vector<int> &boundaryVerticesList) const
{
    if (!boundaryVerticesList.empty())
        boundaryVerticesList.clear();

    for (int i = 0; i < numVertices; ++i)
    {
        vector<int> oneRingList;
        getOneRing(i, oneRingList);

        bool isBoundaryVertex(false);

        // check if all vertices in one ring has an edge to two other vertices in the one ring
        int numOneRingVertices = oneRingList.size();
        for (int j = 0; j < numOneRingVertices; ++j)
        {
            int numEdgeFound(0);
            for (int k = 0; k < numOneRingVertices; ++k)
            {
                if (k == j)
                    continue;

                if (isEdgeExist(oneRingList[j], oneRingList[k]))
                    ++numEdgeFound;

                if (numEdgeFound >= 2)
                    break;
            }

            // one of the one ring vertex doesn't have edge to 2 other one ring vertices
            if (numEdgeFound < 2)
            {
                isBoundaryVertex = true;
                break;
            }
        }

        // add boundary vertex to list
        if (isBoundaryVertex)
            boundaryVerticesList.push_back(i);
    }
}

// render scalarfield
void Renderer::renderScalarField(const float *sField, unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, float isoValue) const
{
    if (sField == NULL)
        return;

    unsigned int nPointsInXDirection = (numCellX + 1);
    unsigned int nPointsInSlice = nPointsInXDirection*(numCellY + 1);

    glDisable(GL_LIGHTING);
    glPointSize(5.0);

    // draw axis
    glLineWidth(3.0);
    glBegin(GL_LINES);
        // x axis
        glColor3f(1.0, 0.0, 0.0);
        glVertex3d(xoffset, yoffset, zoffset);
        glVertex3d(xoffset + 1.5, yoffset, zoffset);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3d(xoffset, yoffset, zoffset);
        glVertex3d(xoffset, yoffset + 1.5, zoffset);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3d(xoffset, yoffset, zoffset);
        glVertex3d(xoffset, yoffset, zoffset + 1.5);
    glEnd();
    glLineWidth(1.0);

    // Generate isosurface.
    for(unsigned int z = 0; z <= numCellZ; z++)
    {
        for(unsigned int y = 0; y <= numCellY; y++)
        {
            for(unsigned int x = 0; x <= numCellX; x++)
            {
                double xcoord = x*fCLengthX + xoffset;
                double ycoord = y*fCLengthY + yoffset;
                double zcoord = z*fCLengthZ + zoffset;

                if (sField[z*nPointsInSlice + y*nPointsInXDirection + x] < isoValue)
                {
                    glColor3f(1.0, 0.0, 0.0);
                }
                else
                {
                    if(bDisplayModel)
                    {
                        continue;
                    }
                    glColor3f(0.0, 0.0, 1.0);
                }

                glBegin(GL_POINTS);
                glVertex3d(xcoord, ycoord, zcoord);
                glEnd();
            }
        }
    }

    glPointSize(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glEnable(GL_LIGHTING);
}

// render cell regions
// bWiredCell: true - render cells in wireframe
void Renderer::renderCellInfo(unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, bool bWiredCell) const
{
    glDisable(GL_LIGHTING);

    if (bWiredCell)
        glLineWidth(2.0);
    else
        glPointSize(5.0);

    // Generate isosurface.
    for (unsigned int z = 0; z <= numCellZ; z++)
        for (unsigned int y = 0; y <= numCellY; y++)
            for (unsigned int x = 0; x <= numCellX; x++) {
                double xcoord = x*fCLengthX + xoffset;
                double ycoord = y*fCLengthY + yoffset;
                double zcoord = z*fCLengthZ + zoffset;

                glColor3f(1.0, 0.0, 0.0);

                if (bWiredCell)
                {
                    glBegin(GL_LINES);
                    glVertex3d(xcoord, ycoord, zcoord);
                    double xcoord2 = x*fCLengthX + xoffset;
                    double ycoord2 = (y+1)*fCLengthY + yoffset;
                    double zcoord2 = z*fCLengthZ + zoffset;
                    glVertex3d(xcoord2, ycoord2, zcoord2);

                    glVertex3d(xcoord, ycoord, zcoord);
                    xcoord2 = (x+1)*fCLengthX + xoffset;
                    ycoord2 = y*fCLengthY + yoffset;
                    zcoord2= z*fCLengthZ + zoffset;
                    glVertex3d(xcoord2, ycoord2, zcoord2);

                    glVertex3d(xcoord, ycoord, zcoord);
                    xcoord2 = x*fCLengthX + xoffset;
                    ycoord2 = y*fCLengthY + yoffset;
                    zcoord2 = (z+1)*fCLengthZ + zoffset;
                    glVertex3d(xcoord2, ycoord2, zcoord2);

                    if (x == numCellX)
                    {
                        xcoord = (x+1)*fCLengthX + xoffset;
                        ycoord = y*fCLengthY + yoffset;
                        zcoord = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = (x+1)*fCLengthX + xoffset;
                        ycoord2 = y*fCLengthY + yoffset;
                        zcoord2 = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);

                        xcoord = (x+1)*fCLengthX + xoffset;
                        ycoord = y*fCLengthY + yoffset;
                        zcoord = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = (x+1)*fCLengthX + xoffset;
                        ycoord2 = (y+1)*fCLengthY + yoffset;
                        zcoord2 = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);
                    }
                    if (y == numCellY)
                    {
                        xcoord = x*fCLengthX + xoffset;
                        ycoord = (y+1)*fCLengthY + yoffset;
                        zcoord = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = (x+1)*fCLengthX + xoffset;
                        ycoord2 = (y+1)*fCLengthY + yoffset;
                        zcoord2 = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);

                        xcoord = x*fCLengthX + xoffset;
                        ycoord = (y+1)*fCLengthY + yoffset;
                        zcoord = z*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = x*fCLengthX + xoffset;
                        ycoord2 = (y+1)*fCLengthY + yoffset;
                        zcoord2 = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);
                    }
                    if (z == numCellZ)
                    {
                        xcoord = x*fCLengthX + xoffset;
                        ycoord = y*fCLengthY + yoffset;
                        zcoord = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = (x+1)*fCLengthX + xoffset;
                        ycoord2 = y*fCLengthY + yoffset;
                        zcoord2 = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);

                        xcoord = x*fCLengthX + xoffset;
                        ycoord = y*fCLengthY + yoffset;
                        zcoord = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord, ycoord, zcoord);
                        xcoord2 = x*fCLengthX + xoffset;
                        ycoord2 = (y+1)*fCLengthY + yoffset;
                        zcoord2 = (z+1)*fCLengthZ + zoffset;
                        glVertex3d(xcoord2, ycoord2, zcoord2);
                    }

                    glEnd();
                }
                else
                {
                    glBegin(GL_POINTS);
                    glVertex3d(xcoord, ycoord, zcoord);
                    glEnd();
                }
            }

            if (bWiredCell)
                glLineWidth(1.0);
            else
                glPointSize(1.0);
            glColor3f(0.0, 0.0, 0.0);
            glEnable(GL_LIGHTING);
}

// render one cell
void Renderer::renderCellInfo(unsigned int x, unsigned int y, unsigned int z, unsigned int numCellX, unsigned int numCellY, unsigned int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ, double xoffset, double yoffset, double zoffset, bool bWiredCell) const
{
    glDisable(GL_LIGHTING);

    if (bWiredCell)
        glLineWidth(5.0);
    else
        glPointSize(5.0);

    // Generate isosurface.
    double xcoord = x*fCLengthX + xoffset;
    double ycoord = y*fCLengthY + yoffset;
    double zcoord = z*fCLengthZ + zoffset;

    glColor3f(1.0, 1.0, 1.0);

    if (bWiredCell)
    {
        glBegin(GL_LINES);

        // edge 0
        glVertex3d(xcoord, ycoord, zcoord);
        double xcoord2 = x*fCLengthX + xoffset;
        double ycoord2 = (y+1)*fCLengthY + yoffset;
        double zcoord2 = z*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 3
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = (x+1)*fCLengthX + xoffset;
        ycoord2 = y*fCLengthY + yoffset;
        zcoord2= z*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 8
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = x*fCLengthX + xoffset;
        ycoord2 = y*fCLengthY + yoffset;
        zcoord2 = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 1
        xcoord = x*fCLengthX + xoffset;
        ycoord = (y+1)*fCLengthY + yoffset;
        zcoord = z*fCLengthZ + zoffset;
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = (x+1)*fCLengthX + xoffset;
        ycoord2 = (y+1)*fCLengthY + yoffset;
        zcoord2 = z*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 9
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = x*fCLengthX + xoffset;
        ycoord2 = (y+1)*fCLengthY + yoffset;
        zcoord2 = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 4
        xcoord = x*fCLengthX + xoffset;
        ycoord = y*fCLengthY + yoffset;
        zcoord = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord, ycoord, zcoord);
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 5
        xcoord = (x+1)*fCLengthX + xoffset;
        ycoord = (y+1)*fCLengthY + yoffset;
        zcoord = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord, ycoord, zcoord);
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 10
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = (x+1)*fCLengthX + xoffset;
        ycoord2 = (y+1)*fCLengthY + yoffset;
        zcoord2 = z*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 6
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = (x+1)*fCLengthX + xoffset;
        ycoord2 = y*fCLengthY + yoffset;
        zcoord2 = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 7
        xcoord = x*fCLengthX + xoffset;
        ycoord = y*fCLengthY + yoffset;
        zcoord = (z+1)*fCLengthZ + zoffset;
        glVertex3d(xcoord, ycoord, zcoord);
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 11
        xcoord = (x+1)*fCLengthX + xoffset;
        ycoord = y*fCLengthY + yoffset;
        zcoord = z*fCLengthZ + zoffset;
        glVertex3d(xcoord, ycoord, zcoord);
        glVertex3d(xcoord2, ycoord2, zcoord2);

        // edge 2
        glVertex3d(xcoord, ycoord, zcoord);
        xcoord2 = (x+1)*fCLengthX + xoffset;
        ycoord2 = (y+1)*fCLengthY + yoffset;
        zcoord2 = z*fCLengthZ + zoffset;
        glVertex3d(xcoord2, ycoord2, zcoord2);

        glEnd();
    }
    else
    {
        glBegin(GL_POINTS);
        glVertex3d(xcoord, ycoord, zcoord);
        glEnd();
    }


    if (bWiredCell)
        glLineWidth(1.0);
    else
        glPointSize(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glEnable(GL_LIGHTING);
}

/*
// render scalarfield
// ***OBSOLETE***  DO NOT USE!
void Renderer::loadScalarField(char *filename)
{
	ifstream inFile;
	inFile.open(filename, ifstream::in);
	
	if (!inFile.is_open())
		return;
	
	// get scalar field properties
	unsigned int nScalars;
	unsigned int numCellsX;
	unsigned int numCellsY;
	unsigned int numCellsZ;
	float fCLengthX;
	float fCLengthY;
	float fCLengthZ;
	if (inFile.good())
	{
		char tmp[16];
		inFile >> tmp;
		inFile >> nScalars;
		
		inFile >> tmp;
		inFile >> numCellsX;
		inFile >> tmp;
		inFile >> numCellsY;
		inFile >> tmp;
		inFile >> numCellsZ;
		
		inFile >> tmp;
		inFile >> fCLengthX;
		inFile >> tmp;
		inFile >> fCLengthY;
		inFile >> tmp;
		inFile >> fCLengthZ;

		numScalars = nScalars;
		nCellsX = numCellsX;
		nCellsY = numCellsY;
		nCellsZ = numCellsZ;
		fCellLengthX = fCLengthX;
		fCellLengthY = fCLengthY;
		fCellLengthZ = fCLengthZ;
	}
	
	if (scalarField != NULL)
	{
		delete[] scalarField;
		scalarField = new float[numScalars];
	}
	unsigned int i = 0;
	while (inFile.good())
	{	
		inFile >> scalarField[i];
		++i;
	}

	inFile.close();
}

  // load scalarfield
  // ***OBSOLETE***  DO NOT USE!
void Renderer::loadScalarField(float *sField, int nScalars, int numCellX, int numCellY, int numCellZ, float fCLengthX, float fCLengthY, float fCLengthZ)
{
	numScalars = nScalars;
	nCellsX = numCellX;
	nCellsY = numCellY;
	nCellsZ = numCellZ;
	fCellLengthX = fCLengthX;
	fCellLengthY = fCLengthY;
	fCellLengthZ = fCLengthZ;

	if (scalarField != NULL)
	{
		delete[] scalarField;
		scalarField = NULL;
	}

	scalarField = new float[numScalars];
	for (unsigned int i = 0; i < numScalars; ++i)
		scalarField[i] = sField[i];
}
*/

