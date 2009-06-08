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

#define PORT 33333
#define CANT_CLIENTES 5

ClienteTorrent::ClienteTorrent() {
	// TODO
	//crear el peer_id
	//sacar los datos del archivo de configuracion
	puerto = PORT;
	peerListener.listen(puerto, CANT_CLIENTES);

}

ClienteTorrent::~ClienteTorrent() {
	// TODO
	if (activo)
		finalizar();
	peerListener.close();
}

void* ClienteTorrent::run() {

	Socket* conexionPeerNuevo;
	int cantidad, length;
	char* handshake;
	activo = true;

	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {
			cantidad = conexionPeerNuevo->receive((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
			if (cantidad > 0) {
				length = ntohl(length);// pasa a de big endian al endian local
				handshake = new char[length];
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
							peerNuevo->run();
						}
					}
					//delete[] handshake;
				}
				delete[] handshake;
			}
		}
	}
	return NULL;
}

Torrent* ClienteTorrent::buscarTorrent(std::string hashTorrent) {
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		if ((*it)->getInfoHash().compare(hashTorrent) == 0) {
			return (*it);
		}
	}
	return NULL;
}

void ClienteTorrent::finalizar() {
	//implementar: yo hago esto ;) LU

	activo = false;
	//this->join();

	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		(*it)->detener();//join??
		//guardar info sobre el torrent
		delete (*it);
	}
	//guardar info del cliente
}

bool ClienteTorrent::estaActivo() {
	return activo;
}

std::string ClienteTorrent::getPeerId() {
	std::string temp = peer_id;
	return temp;
}

Torrent* ClienteTorrent::agregarTorrent(std::string ruta) {
	FILE* fpTorrent;

	if ((fpTorrent = fopen(ruta.c_str(), "r")) == NULL) {
		return NULL;
	} else {
		BencodeParser parserTorrent(fpTorrent);
		Torrent *t = new Torrent();
		if ( t->inicializarTorrent(&parserTorrent)){
			t->setControlador(controlador);
			torrents.push_back(t); //agrego el torrent a la lista de torrents
			t->run();
		} else {
			delete t;
			t = NULL;
		}
		return t;
	}
}

void ClienteTorrent::borrarTorrent(Torrent *t) {
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		if ((*it) == t) {
			t->detener();//join??
			delete t;
			return;
		}
	}
}

void ClienteTorrent::setControlador(Controlador *ctrl) {
	this->controlador = ctrl;
}
