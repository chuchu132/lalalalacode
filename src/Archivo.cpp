/*
 * Archivo.cpp
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#include "Archivo.h"

Archivo::Archivo() {}

Archivo::~Archivo() {}

unsigned int Archivo::getTamanio(){
	return tamanio;
}

std::string Archivo::getPath()
{
	return path;
}

void Archivo::setTamanio(unsigned int tam){
	this->tamanio = tam;
}

void Archivo::setPath(std::string path){
	this->path = path;
}
