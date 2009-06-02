/*
 * Torrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include <cstdio>
#include "Torrent.h"

Torrent::Torrent() {
	// TODO Auto-generated constructor stub

}

Torrent::~Torrent() {
	// TODO Auto-generated destructor stub
}
bool Torrent::conectarTracker(std::string url, int port) {
	return tracker->connect(url, port);
}

bool Torrent::enviarEventoEstado(const char* event = NULL, int numwant = 0) {
	//TODO ver si el mensaje no lo arma un "Parser" o va en otro metodo
	std::string envio;
	envio = "GET /?info_hash=";
	envio += info_hash;
	envio += "& peer_id=";
	envio += clienteTorrent->getPeerId();
	envio += "&port=";
	char temp[20];
	sprintf(temp, "%d", port);
	envio += temp;
	envio += "&uploaded=";
	sprintf(temp, "%d", uploaded);
	envio += temp;
	envio += "&downloaded=";
	sprintf(temp, "%d", downloaded);
	envio += temp;
	envio += "&left=";
	sprintf(temp, "%d", left());
	envio += temp;
	if (numwant < 0) {
		envio += "&event=";
		envio += event;
	} else {
		envio += "&numwant=";
		sprintf(temp, "%d", numwant);
		envio += temp;
	}
	return (tracker->send(envio.c_str(), envio.length()));
}

std::string Torrent::getNombre() {
	return nombre;
}

unsigned int  Torrent::left(){
	return (partes.getTamanio() - downloaded);
}

bool Torrent::estaActivo(){
	return activo;
}

std::string Torrent::getInfoHash(){
	return info_hash;
}

std::string Torrent::getPeerId(){
	return clienteTorrent->getPeerId();
}

void* Torrent::run() {
	Socket* conexionPeerNuevo;
	while (activo) {
		conexionPeerNuevo = peerListener.accept();
		if (conexionPeerNuevo != NULL) {
			Peer* peerNuevo = new Peer(conexionPeerNuevo, this);
			if (peerNuevo != NULL) {
				//TODO peerNuevo->run(); o algo asi
				llaveListaPeers.lock();
				peers.insert(peers.end(), peerNuevo);
				llaveListaPeers.unlock();
			}
		}
	}
	return NULL;
}

Bitmap* Torrent::getBitmap(){
	return &partes;
}


//implementar
void Torrent::continuar() {

}

void Torrent::detener() {

}

void Torrent::pausar() {

}

std::string Torrent::getEstado() {
	return T_DETENIDO;
}

unsigned int Torrent::getTamanio() {
	return 0;
}

unsigned int Torrent::getTamanioDescargado() {
	return 0;
}

int Torrent::getVelocidadSubida() {
	return 0;
}

int Torrent::getVelocidadBajada() {
	return 0;
}

void Torrent::setControlador(Controlador* ctrl)
{
	this->controlador = ctrl;
}

