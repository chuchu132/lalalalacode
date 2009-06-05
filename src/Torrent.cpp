/*
 * Torrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include <cstdio>
#include "ParserCgi.h"
#include "ParserMensaje.h"
#include "Torrent.h"

Torrent::Torrent() {
	// TODO Auto-generated constructor stub

}

Torrent::~Torrent() {
	// TODO Auto-generated destructor stub
}

bool Torrent::inicializarTorrent(BencodeParser* parser){
	// TODO implementar despues de tener el bencodeparser


	return true;
}

bool Torrent::conectarTracker(std::string url, int port) {
	return tracker->connect(url, port);
}

bool Torrent::enviarEventoEstado(const char* event = NULL, int numwant = 0) {
	ParserMensaje parser;
	std::string envio;
	if (numwant < 0) {
		envio += parser.crearGetConEvento(info_hash,clienteTorrent->getPeerId(),port,uploaded,downloaded,left(),event);
	} else {
		envio = parser.crearGetConNumwant(info_hash,clienteTorrent->getPeerId(),port,uploaded,downloaded,left(),numwant);
	}
	return (tracker->send(envio.c_str(), envio.length()));
}

void Torrent::agregarPeer(Peer* peerNuevo){
	llaveListaPeers.lock();
	peers.insert(peers.end(), peerNuevo);
	llaveListaPeers.unlock();
}

std::string Torrent::getNombre() {
	return nombre;
}

unsigned int  Torrent::left(){
	return (archivos.getTamanio() - downloaded);
}


std::string Torrent::getInfoHash(){
	return info_hash;
}

std::string Torrent::getPeerId(){
	return clienteTorrent->getPeerId();
}


FileManager* Torrent::getFileManager(){
	return &archivos;
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
	return 40000;
}

unsigned int Torrent::getTamanioDescargado() {
	return 1000;
}

int Torrent::getVelocidadSubida() {
	return 2;
}

int Torrent::getVelocidadBajada() {
	return 30;
}

void Torrent::setControlador(Controlador* ctrl)
{
	this->controlador = ctrl;
}

void Torrent::refrescarPeers()
{
	//todo implementar
}
