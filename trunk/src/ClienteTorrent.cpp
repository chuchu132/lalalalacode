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

void* ClienteTorrent::run() {
	Socket* conexionPeerNuevo;
	int cantidad,length;
	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {
			cantidad = conexionPeerNuevo->receive((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
			if (cantidad > 0) {
				char* buffer = new char[length];
				cantidad = conexionPeerNuevo->receive(buffer, length);
				if (cantidad > 0) {
					//					ParserCgi parserCgi;
					//					std::string hash = parserCgi.getHash(buffer);
					//					Torrent* torrent = buscarTorrent(hash);
					//					if(torrent != NULL){
					//						Peer* peerNuevo = new PeerUp(conexionPeerNuevo, torrent);
					//						if (peerNuevo != NULL) {
					//							torrent->agregarPeer(peerNuevo);
					//							//TODO peerNuevo->run(); o algo asi
					//						}
				}
				delete[] buffer;
			}
		}
	}
	return NULL;
}


bool ClienteTorrent::estaActivo(){
	return activo;
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
