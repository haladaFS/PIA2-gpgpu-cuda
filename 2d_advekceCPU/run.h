#include "defs.h"
#include "dataToParaviewField.h"
#include "schemes.h"

//vypis vysledku
void get_results(SIMULATION& sim, int steps)
{
	std::string name = "output_";
	name += std::to_string(steps) + ".vtk";
	printDataToParaview(sim.u, size_x, size_y, name);
}


template <typename SCHEME>
void Step(real* u_new, real* u, int x, int y)
{
	if (x != 0 && y != 0)
		SCHEME::update(u_new, u, x, y);

}

//funkce zajistujici casovy cyklus simulace
void execute(SIMULATION &sim){

	int steps = 1;
	int steps_max = 100;
	bool stop = false;


	while (!stop){

		#pragma omp parallel for schedule(static) collapse(2)
		for (int y=0; y<size_y;y++)
		for (int x=0; x<size_x;x++)
			Step< upwind >(sim.u_new, sim.u, x, y);

		// prohozeni pointeru
		sim.u_temp = sim.u_new;
		sim.u_new = sim.u;
		sim.u = sim.u_temp;

		if(steps%100==0) get_results(sim, steps);
		if(steps%20==0) printf("Step: %d\n", steps);

		steps++;
		if(steps>steps_max) stop = true;
	}
}

