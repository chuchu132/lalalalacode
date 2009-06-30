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
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>

#define CANT_CLIENTES 5

ClienteTorrent::ClienteTorrent() {
	std::cout << "*** Iniciando FiTorrent ***" << std::endl;
	std::string temp = "-FITORRENT-FIUBA----";
	temp += (rand() % 10);
	memcpy(peer_id, temp.c_str(), LEN_SHA1);
	inicializarDirectorios();
	controlador = NULL;
	activo = false;

	//agrega los torrents que estaban agregados en la ultima sesion
	std::string estado;
	Torrent *t;
	while (config.hayTorrents()) {
		t = agregarTorrent(config.obtenerTorrent());
		if (t != NULL) {
			estado = config.getEstadoTorrent();
			t->setEstado(estado);
			if (estado != T_DETENIDO)
				t->continuar();
		}
	}
	std::cout << "*** FiTorrent ha iniciado correctamente ***" << std::endl;
}

ClienteTorrent::~ClienteTorrent() {
	if (activo)
		finalizar();
	std::cout << "*** FiTorrent se ha cerrado correctamente ***" << std::endl;
}

void* ClienteTorrent::run() {

	if (peerListener.listen(config.getPuerto(), CANT_CLIENTES) == ERROR) {
		peerListener.close();
	}
	Socket* conexionPeerNuevo;
	int cantidad;

	activo = true;

	while (activo && peerListener.is_valid()) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {

			char longProto;
			cantidad = conexionPeerNuevo->receiveExact(&longProto, 1);

			if (cantidad > 0) {
				int tamHsk = (unsigned char) longProto + LEN_BASE_HANDSHAKE - 1;
				char* handshake = new char[tamHsk];
				if (conexionPeerNuevo->receiveExact(handshake, tamHsk) != ERROR) {
					ParserMensaje parser;
					std::string hash = parser.getHash(handshake, longProto);
					sleep(5);
					Torrent* torrent = buscarTorrent(hash);
					if (torrent != NULL) {
						Peer* peerNuevo =
							new PeerUp(conexionPeerNuevo, torrent);
						if (peerNuevo != NULL) {
							if (!torrent->agregarPeer(peerNuevo)) {
								delete peerNuevo;
							}
						}
					}
				}
				delete[] handshake;
			} else { //Si no envia el handshake luego de conectarse se cierra la coneccion con ese peer
				conexionPeerNuevo->close();
			}
		}

		sleep(5);//tiene un sleep alto para darle prioridad a los peerdown
	}

	peerListener.close();
	return NULL;
}

Torrent* ClienteTorrent::buscarTorrent(std::string hashTorrent) {
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		if ((*it)->getHashString().compare(hashTorrent) == 0) {
			return (*it);
		}
	}
	return NULL;
}

void ClienteTorrent::finalizar() {
	if (activo) {
		std::cout << "Cerrando FiTorrent" << std::endl;
		std::cout.flush();
		activo = false;
		peerListener.close();
		this->join();
	}
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		//guarda info sobre el torrent
		config.guardarTorrent((*it)->getEstado(), (*it)->getPath());
		(*it)->detener();
		delete (*it);
		it++;
	}
}

bool ClienteTorrent::estaActivo() {
	return activo;
}

std::string ClienteTorrent::getPeerId() {
	std::string temp = peer_id;
	return temp;
}

UINT ClienteTorrent::getPuerto() {
	return config.getPuerto();
}

Torrent* ClienteTorrent::agregarTorrent(std::string ruta) {

	BencodeParser parserTorrent(ruta.c_str());
	std::string notif;
	Torrent *t = NULL;
	if (!parserTorrent.procesar()) {
		notif = "Error al examinar el archivo ";
		notif += ruta;
		t = NULL;
	} else {
		t = new Torrent(this, ruta);
		t->setControlador(controlador);
		if (t->inicializarTorrent(&parserTorrent)) {
			//busca si el torrent no fue agregado anteriormente
			if (this->buscarTorrent(t->getHashString()) == NULL) {
				torrents.push_back(t); //agrego el torrent a la lista de torrents
				notif = "Se ha agregado el Torrent ";
				notif += t->getNombre();
			} else {
				notif = "Ya existe el Torrent ";
				notif += t->getNombre();
				delete t;
				t = NULL;
			}
		} else {
			delete t;
			t = NULL;
			notif = "Error al decodificar el archivo ";
			notif += ruta;
		}
	}

	if (controlador != NULL) {
		controlador->notificarVista(notif);
	}
	return t;
}

void ClienteTorrent::borrarTorrent(Torrent *t) {
	std::list<Torrent*>::iterator it = torrents.begin();
	std::string notif = "Se ha borrado el Torrent ";
	while (it != torrents.end()) {
		if ((*it) == t) {
			t->detener();
			notif += t->getNombre();
			delete t;
			torrents.erase(it);
			controlador->notificarVista(notif);
			break;
		}
		it++;
	}
}

void ClienteTorrent::setControlador(Controlador *ctrl) {
	this->controlador = ctrl;
	//seteo el controlador a todos los torrents existentes
	std::list<Torrent*>::iterator it = torrents.begin();
	while (it != torrents.end()) {
		(*it)->setControlador(ctrl);
		it++;
	}
}

Configuracion* ClienteTorrent::getConfiguracion() {
	return &config;
}

void ClienteTorrent::inicializarDirectorios() {
	mkdir(config.getRutaDescargas().c_str(), PERMISOS);
	mkdir(URL_CARPETA_TEMP, PERMISOS);
	return;
}

std::list<Torrent*>* ClienteTorrent::getListaTorrents() {
	return &torrents;
}
