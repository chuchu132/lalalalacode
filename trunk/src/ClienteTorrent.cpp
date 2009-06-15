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

#define CANT_CLIENTES 5

ClienteTorrent::ClienteTorrent() {
	// TODO crear el peer_id
	puerto = config.getPuerto();
	rutaDescargas = config.getRutaDescargas();

	std::string estado;
	Torrent *t;

	while (config.hayTorrents()) {
		t = agregarTorrent(config.obtenerTorrent());
		estado = config.getEstadoTorrent();

		if (estado == T_DETENIDO) {
			t->detener();
		}
		else {
			if (estado == T_PAUSADO) {
				t->pausar();
			}
		}
	}

	activo = false;
}

ClienteTorrent::~ClienteTorrent() {
	if (activo)
		finalizar();
}

void* ClienteTorrent::run() {

	peerListener.listen(puerto, CANT_CLIENTES);

	Socket* conexionPeerNuevo;
	int cantidad, length;
	char* handshake;
	activo = true;

	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {
			cantidad = conexionPeerNuevo->receiveExact((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
			if (cantidad > 0) {
				length = ntohl(length);// pasa a de big endian al endian local
				handshake = new char[length];
				cantidad = conexionPeerNuevo->receiveExact(handshake, length);
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

	peerListener.close();
	return NULL;
}

Torrent* ClienteTorrent::buscarTorrent(std::string hashTorrent) {
	std::list<Torrent*>::iterator it = torrents.begin();
	Sha1 sha;
	while (it != torrents.end()) {
		if (sha.salidaAstring((*it)->getInfoHash()).compare(hashTorrent) == 0) {
			return (*it);
		}
	}
	return NULL;
}

void ClienteTorrent::finalizar() {
	//implementar
	if (activo)
	{
		activo = false;
		this->join();
	}
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		//guarda info sobre el torrent
		config.guardarTorrent((*it)->getEstado(), (*it)->getPath());
		(*it)->detener();
		delete (*it);
	}
}

bool ClienteTorrent::estaActivo() {
	return activo;
}

std::string ClienteTorrent::getPeerId() {
	std::string temp = peer_id;
	return temp;
}

unsigned int ClienteTorrent::getPuerto(){
	return puerto;
}

Torrent* ClienteTorrent::agregarTorrent(std::string ruta) {

	BencodeParser parserTorrent(ruta.c_str());
	std::string notif;
	if (!parserTorrent.procesar()) {
		notif = "Error al examinar el archivo ";
		notif += ruta;
		controlador->notificarVista(notif);
		return NULL;
	} else {
		Torrent *t = new Torrent(this, ruta);
		if ( t->inicializarTorrent(&parserTorrent)){
			t->setControlador(controlador);
			t->setCarpetaDescarga(rutaDescargas);
			torrents.push_back(t); //agrego el torrent a la lista de torrents
			t->run();
			notif = "Se ha agregado el Torrent ";
			notif += t->getNombre();
		} else {
			delete t;
			t = NULL;
			notif = "Error al decodificar el archivo ";
			notif += ruta;
		}
		controlador->notificarVista(notif);
		return t;
	}
}

void ClienteTorrent::borrarTorrent(Torrent *t) {
	std::list<Torrent*>::iterator it = torrents.begin();
	std::string notif = "Se ha borrado el Torrent ";
	while (it != torrents.end()) {
		if ((*it) == t) {
			t->detener();
			notif += t->getNombre();
			delete t;
			controlador->notificarVista(notif);
			break;
		}
	}
}

void ClienteTorrent::setControlador(Controlador *ctrl) {
	this->controlador = ctrl;
}

Configuracion* ClienteTorrent::getConfiguracion() {
	return &config;
}
