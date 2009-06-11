/*
 * Archivo.h
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <string>

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	unsigned int getTamanio();
	std::string getPath();
	void setTamanio(unsigned int tam);
	void setPath(std::string path);

private:
	std::string path;
	unsigned int tamanio;
};

#endif /* ARCHIVO_H_ */
