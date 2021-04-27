#include "defs.h"

struct SIMULATION{

	//pole promennych
	real *u, *u_new; //CPU
	dreal *du, *du_new; //GPU

	//pomocny pointer slouzici pro prohazovani poli
	real *u_temp; //CPU
	dreal *du_temp; //GPU

	int block_size = 128; //GPU parametr - velikost bloku, max 1024

	//alokace pameti - CPU, pokud je uzito i GPU
	void allocateMemory()
	{
		size_t local_size_dreal = size_x*size_y*sizeof(dreal);
		#ifdef USE_CUDA
		cudaMalloc((void**)&du,  size_x*size_y*(sizeof(dreal)));
		cudaMalloc((void**)&du_new, size_x*size_y*(sizeof(dreal)));
        		cudaError err = cudaGetLastError();
        		if( err != cudaSuccess )
        		{
        		    std::cerr << "allocateMemory: Computation on the device failed with error: " << cudaGetErrorString( err ) << "." << std::endl;
        		}
		#endif
		u = (real*)malloc(size_x*size_y*sizeof(real));
		u_new = (real*)malloc(size_x*size_y*sizeof(real));
		std::cout << "ALOCATED MEM: size = " << size_x*size_y*(sizeof(dreal)) << std::endl;

	}

	//kopirovani dat na GPU
	void copyDataToDevice()
	{
		#ifdef USE_CUDA
		//if (!u) return;
		cudaMemcpy((void*)du, (void*)u, size_x*size_y*(sizeof(dreal)), cudaMemcpyHostToDevice);
		cudaMemcpy(du_new, u_new, size_x*size_y*sizeof(real), cudaMemcpyHostToDevice);
        		cudaError err = cudaGetLastError();
        		if( err != cudaSuccess )
        		{
        		    std::cerr << "copyDataToDevice: Computation on the device failed with error: " << cudaGetErrorString( err ) << "." << std::endl;
        		}
		#endif
		std::cout << "DATA TO DEVICE: size = " << size_x*size_y*(sizeof(dreal)) << std::endl;
	}

	//kopirovani dat z GPU
	void copyDataToHost()
	{
		#ifdef USE_CUDA
		cudaMemcpy((void*)u, (void*)du, size_x*size_y*sizeof(dreal), cudaMemcpyDeviceToHost);
		cudaMemcpy((void*)u_new, (void*)du_new, size_x*size_y*sizeof(dreal), cudaMemcpyDeviceToHost);
        		cudaError err = cudaGetLastError();
        		if( err != cudaSuccess )
        		{
        		    std::cerr << "copyDataToHost: Computation on the device failed with error: " << cudaGetErrorString( err ) << "." << std::endl;
        		}
		#endif

	}

	//uvolneni pameti - CPU, pokud je uzito i GPU
	void deallocateMemory()
	{
		if (u) free(u);
		if (u_new) free(u_new);

		#ifdef USE_CUDA
		if (du) cudaFree(du);
		if (du_new) cudaFree(du_new);
		#endif
	}

	//pocatecni podminky - zadavame pouze na CPU, na GPU nasledne kopirujeme
	void initalCond_const()
	{
		for(int y=1;y<size_y;y++)
		for(int x=1;x<size_x;x++)
		{u[pos(x,y)] = initvalue; u_new[pos(x,y)] = initvalue;}

		for(int x=0;x<size_x;x++)
		{u[pos(x,0)] = initvalueX; u_new[pos(x,0)] = initvalueX;}

		for(int y=0;y<size_y;y++)
		{u[pos(0,y)] = initvalueY; u_new[pos(0,y)] = initvalueY;}
	}


	//konstruktor: alokuje pamet, nastavi poc. podminky
	SIMULATION()
	{
		allocateMemory();
		initalCond_const();

			#ifdef USE_CUDA
        		cudaError err = cudaGetLastError();
        		if( err != cudaSuccess )
        		{
        		    std::cerr << "SIMULATION: Computation on the device failed with error: " << cudaGetErrorString( err ) << "." << std::endl;
        		}
			#endif

	}

	//destruktor: uvolni pamet
	~SIMULATION()
	{
		deallocateMemory();
	}

};
