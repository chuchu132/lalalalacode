/*
 * Tracker.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Constantes.h"
#include "Tracker.h"

Tracker::Tracker() {
	// TODO Auto-generated constructor stub

}

Tracker::~Tracker() {
	// TODO Auto-generated destructor stub
}

bool Tracker::connect(std::string url,int port){
	return (trackerRemoto.connect(url,port) == OK);
}

bool Tracker::send(const char* stream,unsigned int size){
	return ((unsigned int)trackerRemoto.send(stream,size) == size);
}
