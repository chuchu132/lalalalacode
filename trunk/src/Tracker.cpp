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
	puerto = PUERTO_DEFAULT_TRACKER;
}

Tracker::~Tracker() {

}
//TODO re implementar
void* Tracker::run() {
	int cantidad;
	bool seCerro = false;
	std::string buffer;
	char bufferTemp[1000];
	memset(bufferTemp, 0, 1000);
	while (trackerRemoto.is_valid() && !seCerro) {
		if ((cantidad = this->trackerRemoto.receive(bufferTemp, 999)) > 0) {
			bufferTemp[cantidad] = '\0';
			buffer = bufferTemp;
			std::cout << "\nReciev: \n" << buffer << std::endl;
		}
		seCerro = true;
	}
	return NULL;
}


std::string Tracker::getPath() {
	return path;
}

bool Tracker::connect(std::string url) {
	inicilizar(url);
	if (this->url.length() > 0) {
		return (trackerRemoto.connect(this->url, puerto) == OK);
	} else {
		return false;
	}
}

bool Tracker::send(const char* stream, unsigned int size) {
	return ((unsigned int) trackerRemoto.send(stream, size) == size);
}

void Tracker::cerrarConexion() {
	trackerRemoto.close();
}

void Tracker::inicilizar(std::string url) {
	int ini = url.find('/', 0);
	int fin = url.find_last_of('/', url.length());
	unsigned int puerto = url.find(':', ini);
	path.assign(url, fin + 1, url.length() - fin - 1);
	if (puerto != std::string::npos) {
		std::string temp;
		temp.assign(url, puerto + 1, fin - puerto - 1);
		this->puerto = atoi(temp.c_str());
		this->url.assign(url, ini + 2, puerto - (ini + 2));
	} else {
		this->url.assign(url, ini + 2, fin - (ini + 2));
	}
}
