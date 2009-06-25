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
/*
 * La clase Archivo no tiene comportamiento, se usa
 * para almacenar la informacion de cada archivo
 * que se esta descargando, facilitando el acceso
 * a los diferentes campos.
 */
class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	ULINT getTamanio();
	std::list<std::string>* getPathList();
	std::string getPath();
	void setTamanio(ULINT tam);
	void addPath(std::string path);

private:
	std::list<std::string> path;
	ULINT tamanio;
};

#endif /* ARCHIVO_H_ */
