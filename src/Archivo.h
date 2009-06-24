/*
 * Archivo.h
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <string>
#include "Tipos.h"

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	ULINT getTamanio();
	LString* getPathList();
	std::string getPath();
	void setTamanio(ULINT tam);
	void addPath(std::string path);

private:
	LString path;
	ULINT tamanio;
};

#endif /* ARCHIVO_H_ */
