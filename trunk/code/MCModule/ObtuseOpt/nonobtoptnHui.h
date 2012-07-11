#pragma once
#include <nlopt.h>

#include "nonobtoptn.h"



class nonobtoptnHui : public NonobtOptn
{
public:
	nonobtoptnHui(void);
	~nonobtoptnHui(void);

	clock_t constraint_smoothing();
private:
	void constraint_smoothing_vertex(unsigned int vertexIndex, double newPos[3]);
};

