#ifndef DATATOPARAVIEW_H
#define DATATOPARAVIEW_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void printDataToParaview(double *data, //pole dat, které chceš zobrazit
															//std::vector<double> rychlsot_x, //druhé pole dat, pro demonstraci
															//std::vector<double> rychlost_y, //třetí pole dat, pro demonstraci
															int pocetBoduSiteX, //velikost sítě ve směru Y, ve kterých jsou data
															int pocetBoduSiteY, //velikost sítě ve směru X, ve který jsou data
															std::string nazevSouboru)
{

	const int pocPoli = 1; //<------------------ TADY

	int sitParaviewX = pocetBoduSiteX + 1;
	int sitParaviewY = pocetBoduSiteY + 1;
	int pocBunek = (pocetBoduSiteX)*(pocetBoduSiteY);

	std::ofstream file;
	file.open(nazevSouboru);

	file << "# vtk DataFile Version 3.0" << std::endl;
	file << "vtk output" << std::endl;
	file << "ASCII" << std::endl;
	file << "DATASET STRUCTURED_GRID" << std::endl;
	file << "DIMENSIONS "<< sitParaviewX << " " << sitParaviewY << " 1" << std::endl;
	file << "POINTS " << sitParaviewX*sitParaviewY << " float" << std::endl;

	for(float i=0;i<sitParaviewX;i++)
		for(float j=0;j<sitParaviewY;j++)
			file << i/sitParaviewX << " " << j/sitParaviewY << " 0" << std::endl;

	file << "CELL_DATA " << pocBunek << std::endl;
	file << "FIELD FieldData " << pocPoli << std::endl;

	file << "value 1 " << pocBunek << " float" << std::endl;
	for(int i=0;i<pocetBoduSiteX;i++)
		for(int j=0;j<pocetBoduSiteY;j++)
				file << data[i*pocetBoduSiteY +j] << std::endl; //<------------------ TADY, pozor na indexování


	file.close();
}

#endif
