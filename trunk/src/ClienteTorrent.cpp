/*
 * ClienteTorrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */
#include <arpa/inet.h>
#include "ClienteTorrent.h"
#include "ParserMensaje.h"
#include "Peer.h"
#include "PeerUp.h"

ClienteTorrent::ClienteTorrent() {
	// TODO Auto-generated constructor stub

}

ClienteTorrent::~ClienteTorrent() {
	// TODO Auto-generated destructor stub
}

void* ClienteTorrent::run() {
	Socket* conexionPeerNuevo;
	int cantidad, length;
	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {
			cantidad = conexionPeerNuevo->receive((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
			if (cantidad > 0) {
				length = ntohl(length);// pasa a de big endian al endian local
				char* handshake = new char[length];
				cantidad = conexionPeerNuevo->receive(handshake, length);
				if (cantidad > 0) {
					ParserMensaje parser;
					std::string hash = parser.getHash(handshake);
					Torrent* torrent = buscarTorrent(hash);
					if (torrent != NULL) {
						Peer* peerNuevo =
								new PeerUp(conexionPeerNuevo, torrent);
						if (peerNuevo != NULL) {
							torrent->agregarPeer(peerNuevo);
							//TODO peerNuevo->run(); o algo asi
						}
					}
					delete[] handshake;
				}
			}
		}
	}
	return NULL;
}

Torrent* ClienteTorrent::buscarTorrent(std::string hashTorrent) {
	std::list<Torrent*>::iterator it;
	it = torrents.begin();
	while (it != torrents.end()) {
		if ((*it)->getInfoHash().compare(hashTorrent) == 0) {
			return (*it);
		}
	}
	return NULL;
}

bool ClienteTorrent::estaActivo() {
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
