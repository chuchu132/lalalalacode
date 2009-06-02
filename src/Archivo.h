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

private:
	//FILE* o lo que corresponda en C++
	std::fstream file;
	std::string path;
	std::string name;
	int tamPieza;
	//sha1

};

#endif /* ARCHIVO_H_ */
