#include "defs.h"
#include "dataToParaviewField.h"
#include "schemes.h"

//kontrolni funkce pro GPU volana po kazde vetsi operaci
#ifdef USE_CUDA
#define checkCudaDevice checkDevice( __FILE__, __LINE__, cudaGetLastError() )
bool checkDevice( const char* file_name, int line, cudaError error )
{
	if( error == cudaSuccess )
		return true;
	std::cerr << "CUDA ERROR(" << error << ") at line " << line << " in " << file_name << ":" << std::endl;
	std::cerr << cudaGetErrorString( error )  << std::endl;
	abort();
	return true;
}
#endif

//vypis vysledku
void get_results(SIMULATION& sim, int steps)
{
	#ifdef USE_CUDA
	sim.copyDataToHost();
	#endif
	std::string name = "output_";
	name += std::to_string(steps) + ".vtk";
	printDataToParaview(sim.u, size_x, size_y, name);
}


//CPU/GPU hybridni funkce casoveho kroku
template <typename SCHEME>
#ifdef USE_CUDA
__global__ void cudaStep(real* u_new, real* u)
#else
void Step(real* u_new, real* u, int x, int y)
#endif
{
	#ifdef USE_CUDA
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	#endif

	if (x != 0 && y != 0)
		SCHEME::update(u_new, u, x, y);

}

//funkce zajistujici casovy cyklus simulace
void execute(SIMULATION &sim){

	int steps = 1;
	int steps_max = 100;
	bool stop = false;

	#ifdef USE_CUDA
	sim.copyDataToDevice();
	checkCudaDevice;
	#endif

	while (!stop){
		#ifdef USE_CUDA
		checkCudaDevice;
		dim3 blockSize(1, sim.block_size, 1);
		dim3 gridSize(size_x, size_y/sim.block_size, 1);

		if (gridSize.y * sim.block_size != size_y)
		{
			printf("Spatne nastavene parametry!");
			return;
		}
    		cudaStep< upwind ><<<gridSize, blockSize>>>(sim.du_new, sim.du);


		checkCudaDevice;

		// prohozeni pointeru
		sim.du_temp = sim.du_new;
		sim.du_new = sim.du;
		sim.du = sim.du_temp;

		checkCudaDevice;

		#else
		#pragma omp parallel for schedule(static) collapse(2)
		for (int y=0; y<size_y;y++)
		for (int x=0; x<size_x;x++)
			Step< upwind >(sim.u_new, sim.u, x, y);

		// prohozeni pointeru
		sim.u_temp = sim.u_new;
		sim.u_new = sim.u;
		sim.u = sim.u_temp;
		#endif

		if(steps%100==0) get_results(sim, steps);
		if(steps%20==0) printf("Step: %d\n", steps);

		steps++;
		if(steps>steps_max) stop = true;
	}
}

