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
	trackerRemoto.close();
	join();
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

bool Tracker::connect(std::string url,int port){
	return (trackerRemoto.connect(url,port) == OK);
}

bool Tracker::send(const char* stream,unsigned int size){
	return ((unsigned int)trackerRemoto.send(stream,size) == size);
}
