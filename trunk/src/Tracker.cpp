/*
 * Tracker.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Constantes.h"
#include "Tracker.h"
#include <cstring>

Tracker::Tracker() {

}

Tracker::~Tracker() {

}
//TODO re implementar
void* Tracker::run(){
	int cantidad;
	std::string buffer;
	char bufferTemp[1000];
	memset(bufferTemp,0,1000);
	while(trackerRemoto.is_valid()){
	if( (cantidad = this->trackerRemoto.receive(bufferTemp, 999)) > 0){
				bufferTemp[cantidad] = '\0';
					buffer = bufferTemp;
					std::cout<<"\nReciev: \n"<<buffer<<std::endl;
				}
			}
			return NULL;
}

void Tracker::setPath(std::string path){
	this->path = path;
}

std::string Tracker::getPath(){
	return path;
}

bool Tracker::connect(std::string url,int port){
	//TODO parsear la url e inicializar campo path
	return (trackerRemoto.connect(url,port) == OK);
}

bool Tracker::send(const char* stream,unsigned int size){
	return ((unsigned int)trackerRemoto.send(stream,size) == size);
}
