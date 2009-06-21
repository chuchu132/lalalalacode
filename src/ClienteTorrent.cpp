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
#include <cmath>
#include <cstring>

#define CANT_CLIENTES 5

ClienteTorrent::ClienteTorrent() {
	std::string temp = "-FITORRENT-FIUBA-";
	temp += (rand()%1000);
	memcpy(peer_id,temp.c_str(),LEN_SHA1);
	inicializarDirectorios();
	controlador = NULL;
	activo = false;

	std::string estado;
	Torrent *t;
	while (config.hayTorrents()) {
		t = agregarTorrent(config.obtenerTorrent());
		if (t != NULL) {
			estado = config.getEstadoTorrent();
			if (estado == T_ACTIVO) {
				//t-run();
			}
			else {
				if (estado == T_PAUSADO) {
					//t->run();
					//t->pausar();
				}
			}
		}
	}
}

ClienteTorrent::~ClienteTorrent() {
	if (activo)
		finalizar();
}

void* ClienteTorrent::run() {

	peerListener.listen(config.getPuerto(), CANT_CLIENTES);

	Socket* conexionPeerNuevo;
	int cantidad, length;
	char* handshake;
	activo = true;

	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		std::cout<<"conexion entrante de "<<conexionPeerNuevo->getIp()<<std::endl;
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
							if (!torrent->agregarPeer(peerNuevo)) {
								delete peerNuevo;
							}
							else {
								std::cout<<"agregar PeerUp "
									<<conexionPeerNuevo->getIp()<<std::endl;
							}
						}
					}
				}
				delete[] handshake;
			}
		}
		std::cout<<"*** sleep 10: clientetorrent run ***"<<std::endl;
		sleep(10);
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

unsigned int ClienteTorrent::getPuerto(){
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
	}
	else {
		t = new Torrent(this, ruta);
		t->setControlador(controlador);
		if ( t->inicializarTorrent(&parserTorrent)){
			torrents.push_back(t); //agrego el torrent a la lista de torrents
			notif = "Se ha agregado el Torrent ";
			notif += t->getNombre();
		}
		else {
			std::cout<<"error al inicializar el torrent "<<std::endl;
			delete t;
			t = NULL;
			notif = "Error al decodificar el archivo ";
			notif += ruta;
		}
	}

	if (controlador != NULL){
		controlador->notificarVista(notif);}
	else{
		std::cout<<notif<<std::endl;
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

void ClienteTorrent::inicializarDirectorios(){
		mkdir(config.getRutaDescargas().c_str(),0755);
		mkdir(URL_CARPETA_TEMP,0755);
	return;
}

std::list<Torrent*>* ClienteTorrent::getListaTorrents() {
	return &torrents;
}
