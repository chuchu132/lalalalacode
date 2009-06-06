/*
 * Archivo.cpp
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#include "Archivo.h"

Archivo::Archivo() {
	// TODO Auto-generated constructor stub

}

Archivo::~Archivo() {
	// TODO Auto-generated destructor stub
}
int Archivo::getTamanio(){
	return tamanio;
}
std::fstream* Archivo::getArchivo(){
	return &file;
}

std::string Archivo::getNombre()
{
	return name;
}

std::string Archivo::getPath()
{
	return path;
}
