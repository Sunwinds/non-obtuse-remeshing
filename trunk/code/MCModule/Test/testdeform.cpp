#include "testdeform.h"



int testdeform(int argc, char* argv[])
{
	int   gShowObtuseAngles = 0;
	int   degree = 0;
	int   gShowDegree = 0;

	int   gShowNonManifoldVertices = 0;
	int   gShowNonManifoldEdges = 0;
	int   gShowOpenManifoldEdges = 0;

	float optn_alpha = 0.5;
	int   optn_numIter = 10;
	float optn_threshold = 0.0;
	int   optn_numOneRingSearch = 3;
	int   optn_numOneRingQuadric = 1;
	int   optn_numMovePerIter = 1000;
	int   gShowOriginalMesh = 0;
	float optn_opacity = 0.25;
	int   gOptnFirst = 1;
	int   optn_numOptn = 0;
	int   optn_numSmooth = 0;
	int   gOptnRecompute = 0;
	float anglePerBin = 10.0;
	int   gOptnRecomputeClosestFace = 1;
	int   optn_numSmoothingStep = 2;
	float optn_decimate_threshold = 0.0;

	float obj_scale = 1.0;

	float light0_intensity = 1.0;

	int   main_window;

	int gShowCellsRegion = 0;
	int gShowScalarField = 0;

	double* centroid = NULL;

	int optn_decimate_numRemaining = 5000;
	int optn_avoid_degree = 2;

	float optn_improvement_threshold = 0.0;

	float optn_angleBound = 30.0;
	int gOptnAngleBound = 1;

	int gCleanWhenStuck = 0;


	Renderer originalMesh,nonobtuseMesh,resultMesh;
	NonobtOptn optn;

	//Read meshes
	char  open_text_1[] = "./mesh/test_original.obj";
	char  open_text_2[] = "./mesh/test_nonobtuse.obj";
	originalMesh.loadFile(open_text_1);
	nonobtuseMesh.loadFile(open_text_2);

	//Processing
	optn.loadMesh(originalMesh.getVertices(), originalMesh.getNumVertices(), originalMesh.getPolygons(), originalMesh.getNumPolygons(), originalMesh.getVertexNeighbourList(), originalMesh.getEdgeNeighbourList(), originalMesh.getPolygonNeighbourList(), 
		nonobtuseMesh.getVertices(), nonobtuseMesh.getNumVertices(), nonobtuseMesh.getPolygons(), nonobtuseMesh.getNumPolygons());
	optn.removeBadValence(SIMPLE3PLANES_TOWARD_CENTER,30.0);
	optn.optimize_smoothing(3,0.0,30.0,false,SIMPLE3PLANES_TOWARD_CENTER);
	optn.optimize_afterMove(1000 * nonobtuseMesh.getNumVertices(), optn_threshold, optn_improvement_threshold, optn_alpha, optn_numOneRingSearch, optn_numOneRingQuadric, optn_angleBound, false, true, true, SIMPLE3PLANES_TOWARD_CENTER);

	//Save 
	char  save_text[] = "./mesh/test_result.obj";
	resultMesh.loadMeshInfo(optn.getNoVertices(), optn.getNumNoVertices(), optn.getNoPolygons(), optn.getNumNoPolygons());
	resultMesh.saveFile(save_text);

	return 0;
}