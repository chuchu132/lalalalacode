/*
 * Archivo.cpp
 *
 *  Created on: 01/06/2009
 *      Author: ale
 */

#include "Archivo.h"


Archivo::Archivo() {}

Archivo::~Archivo() {}

ULINT Archivo::getTamanio(){
	return tamanio;
}

std::string Archivo::getPath(){
	LStringIter itPath = path.begin();
	LStringIter archivo = --path.end();
	std::string url= "/";
	while(itPath != path.end()){
		url+=(*itPath);
		if(itPath != archivo){
			url += "/";
		}
		itPath++;
	}
	return url;
}

LString* Archivo::getPathList()
{
	return &path;
}

void Archivo::setTamanio(ULINT tam){
	this->tamanio = tam;
}

void Archivo::addPath(std::string path){
	this->path.push_back(path);
}
