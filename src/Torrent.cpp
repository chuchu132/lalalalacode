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
	uploaded = 0;
	downloaded = 0;
	estado = T_DETENIDO;
}

Torrent::~Torrent() {
	// TODO Auto-generated destructor stub
}

bool Torrent::inicializarTorrent(BencodeParser* parser){
	// TODO implementar despues de tener el bencodeparser


	return true;
}

void Torrent::run(){


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
	//controlador->actualizarEstado(this);
}

std::string Torrent::getNombre() {
	return nombre;
}

unsigned int  Torrent::left(){
	return (fileManager.getTamanio() - downloaded);
}


std::string Torrent::getInfoHash(){
	return info_hash;
}

std::string Torrent::getPeerId(){
	return clienteTorrent->getPeerId();
}


FileManager* Torrent::getFileManager(){
	return &fileManager;
}

std::list<Peer*>* Torrent::getListaPeers(){
	return &peers;
}

//implementar.. los valores estan para probar
void Torrent::continuar() {
	estado = T_ACTIVO;
	activo = true;
	controlador->actualizarEstado(this);
}

void Torrent::detener() {
	estado = T_DETENIDO;
	activo = false;
	controlador->actualizarEstado(this);
}

void Torrent::pausar() {
	estado = T_PAUSADO;
	activo = false;
	controlador->actualizarEstado(this);
}

std::string Torrent::getEstado() {
	return estado;
}

bool Torrent::estaActivo(){
	return activo;
}

unsigned int Torrent::getTamanio() {
	return 40000;
	//return fileManager.getTamanio();
}

unsigned int Torrent::getTamanioDescargado() {
	return 1000;
	//return downloaded;
}

int Torrent::getVelocidadSubida() {
	return 2;//todo.. ver ocmo calcular la velocidad
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
	//controlador->actualizarEstado(this);
}


