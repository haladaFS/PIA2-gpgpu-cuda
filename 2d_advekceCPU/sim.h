#include "defs.h"

struct SIMULATION{

	//pole promennych
	real *u, *u_new;

	//pomocny pointer slouzici pro prohazovani poli
	real *u_temp;

	//alokace pameti - CPU, pokud je uzito i GPU
	void allocateMemory()
	{
		u = (real*)malloc(size_x*size_y*sizeof(real));
		u_new = (real*)malloc(size_x*size_y*sizeof(real));

	}

	//uvolneni pameti - CPU, pokud je uzito i GPU
	void deallocateMemory()
	{
		if (u) free(u);
		if (u_new) free(u_new);

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

	}

	//destruktor: uvolni pamet
	~SIMULATION()
	{
		deallocateMemory();
	}

};
