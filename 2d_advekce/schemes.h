#include "defs.h"
#include "sim.h"

//schema pouzitelne na CPU i na GPU
struct upwind
{
	CUDA_HOSTDEV static void update(dreal* u_new, dreal* u, int i, int j)
	{
		double dx = dw/size_x;
		double dy = dh/size_y;

		u_new[pos(i,j)] = u[pos(i,j)] - dt/(dx*dy)*(a*(u[pos(i,j)] - u[pos(i-1,j)])*dx + b*(u[pos(i,j)] - u[pos(i,j-1)])*dy);
	}
};

//dalsi schemata by byla vytvorena obdobnym zpusobem
/*
struct central
{
	CUDA_HOSTDEV static void update(SIMULATIONDATA &sim, int x, int y)
	{

	}
};

struct laxFridrichs
{
	CUDA_HOSTDEV static void update(SIMULATIONDATA &sim, int x, int y)
	{

	}
};
*/
