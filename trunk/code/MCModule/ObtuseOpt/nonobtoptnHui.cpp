#include "nonobtoptnHui.h"


nonobtoptnHui::nonobtoptnHui(void)
{
}


nonobtoptnHui::~nonobtoptnHui(void)
{
}

clock_t nonobtoptnHui::constraint_smoothing()
{
	clock_t b4Time = clock();
	clock_t afterTime = clock();

	cout << "Smoothing vertices (" << numNoVertices << ")..." << flush << std::endl;
	for (int i = 0; i < numNoVertices; i++)
	{
		double newPosition[3];
	    constraint_smoothing_vertex(i, newPosition);
		updateMesh(i, newPosition);

		afterTime = clock();
		cout << i << ":" << afterTime - b4Time << std::endl;
	}
	
   return afterTime - b4Time;
}

void nonobtoptnHui::constraint_smoothing_vertex(unsigned int vertexIndex, double newPos[3])
{   
	
	unsigned int numNbrFaces = noVertexNeighbourList[vertexIndex].size(); //Number of neighbors facets

	nlopt_opt opt; //The solver
	opt = nlopt_create(NLOPT_LD_MMA, 3); // algorithm and dimensionality 

	//Add the optimization energy, i.e. Laplacian energy 
	double centroid[3];
	computeLaplacianSmoothness(vertexIndex, centroid);
	nlopt_set_min_objective(opt, myfunc, centroid);

	//Add the constraints of the 1-ring edges
	for(unsigned int i = 0; i < numNbrFaces; i++)
	{
		// find edge in one-ring from the neighbor face
		int polyIdx = noVertexNeighbourList[vertexIndex][i];
		Edge e;
		e.vertex1Idx = 0;
		e.vertex2Idx = 0;
		for (unsigned int j = 0; j < 3; ++j)
		   if (noPolygonsList[polyIdx][j] == vertexIndex)
			{
				e.vertex1Idx = noPolygonsList[polyIdx][(j + 1) % 3];	// v_i
				e.vertex2Idx = noPolygonsList[polyIdx][(j + 2) % 3];	// v_{i+1}
				break;
			}

		//The two points
		double p1[3] = {noVerticesList[3 * (e.vertex1Idx)], noVerticesList[3 * (e.vertex1Idx) + 1], noVerticesList[3 * (e.vertex1Idx) + 2]}; //vertex 1
		double p2[3] = {noVerticesList[3 * (e.vertex2Idx)], noVerticesList[3 * (e.vertex2Idx) + 1], noVerticesList[3 * (e.vertex2Idx) + 2]}; //vertex 2


		//Add sphere constraints
		double radius = 0.5 * length(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
		double sphere[4] = {(p1[0] + p2[0]) / 2.0, (p1[1] + p2[1]) / 2.0, (p1[2] + p2[2]) / 2.0, radius};
		nlopt_add_inequality_constraint(opt, sphereConstraint, sphere, 1e-18);

		//Add plane constraints
		double plane1[6] = {p1[0], p1[1], p1[2], p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
		nlopt_add_inequality_constraint(opt, planeConstraint, plane1, 1e-18);

		double plane2[6] = {p2[0], p2[1], p2[2], p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]};
		nlopt_add_inequality_constraint(opt, planeConstraint, plane2, 1e-18);
	  }

	 nlopt_set_xtol_rel(opt, 1e-6);
	 double newPosition[3] = {noVerticesList[3 * vertexIndex], noVerticesList[3 * vertexIndex + 1], noVerticesList[3 * vertexIndex + 2]};
	 double minf;
	 int status = nlopt_optimize(opt, newPosition, &minf);
	 std::cout << "Result:" << status << std::endl;
	 nlopt_destroy(opt);
   
	newPos[0] = newPosition[0];
	newPos[1] = newPosition[1];
	newPos[2] = newPosition[2];
}