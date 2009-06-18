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

std::string Archivo::getPath(){
	std::list<std::string>::iterator itPath = path.begin();
	std::list<std::string>::iterator archivo = --path.end();
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

std::list<std::string>* Archivo::getPathList()
{
	return &path;
}

void Archivo::setTamanio(unsigned int tam){
	this->tamanio = tam;
}

void Archivo::addPath(std::string path){
	this->path.push_back(path);
}
