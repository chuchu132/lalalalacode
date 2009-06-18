/*
 * Archivo.h
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <string>
#include <list>

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	unsigned int getTamanio();
	std::list<std::string>* getPathList();
	std::string getPath();
	void setTamanio(unsigned int tam);
	void addPath(std::string path);

private:
	std::list<std::string> path;
	unsigned int tamanio;
};

#endif /* ARCHIVO_H_ */
