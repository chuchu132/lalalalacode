/*
 * Archivo.h
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <fstream>

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	std::fstream* getArchivo();
	int getTamanio();
	std::string getNombre();
	std::string getPath();

private:
	//FILE* o lo que corresponda en C++
	std::fstream file; // Esta abierto mientras viva Archivo.
	std::string path;
	std::string name;
	int tamanio;


};

#endif /* ARCHIVO_H_ */
