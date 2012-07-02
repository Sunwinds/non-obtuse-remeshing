#include "testdeform.h"



int testdeform(int argc, char* argv[])
{
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
	//optn.optimize_smoothing(nonobtuseMesh.getNumVertices(),0.0,30.0,false,SIMPLE3PLANES_TOWARD_CENTER);
	optn.optimize_afterMove(2 * nonobtuseMesh.getNumVertices(), 0.0, 0.0, 0.5, 3, 1, 30.0, false, true, true, SIMPLE3PLANES_TOWARD_CENTER);
	//optn.optimize_smoothing(2 * nonobtuseMesh.getNumVertices(),0.0,30.0,false,SIMPLE3PLANES_TOWARD_CENTER);

	//Save 
	char  save_text[] = "./mesh/test_result.obj";
	resultMesh.loadMeshInfo(optn.getNoVertices(), optn.getNumNoVertices(), optn.getNoPolygons(), optn.getNumNoPolygons());
	resultMesh.saveFile(save_text);

	return 0;
}