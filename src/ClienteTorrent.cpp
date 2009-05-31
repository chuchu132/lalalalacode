/*
 * ClienteTorrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "ClienteTorrent.h"

ClienteTorrent::ClienteTorrent() {
	// TODO Auto-generated constructor stub

}

ClienteTorrent::~ClienteTorrent() {
	// TODO Auto-generated destructor stub
}

std::string ClienteTorrent::getPeerId() {
	std::string temp = peer_id;
	return temp;
}

Torrent* ClienteTorrent::agregarTorrent(std::string ruta) {
	//implementar!
	//a cada nuevo torrent le tiene que pasar como parametro el controlador
	return NULL;
}

void ClienteTorrent::borrarTorrent(Torrent *t) {
	//implementar!
}

void ClienteTorrent::setControlador(Controlador *ctrl) {
	this->controlador = ctrl;
}
