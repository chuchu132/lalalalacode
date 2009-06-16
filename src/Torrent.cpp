/*
 * Torrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "AvisoDescargaCompleta.h"
#include "ParserCgi.h"
#include "ParserMensaje.h"
#include "Torrent.h"
#include "PeerDown.h"
#include <cstring>
#include <cstdio>

Torrent::Torrent(ClienteTorrent* clienteTorrent, std::string path):fileManager(clienteTorrent) {
	tracker = new Tracker();
	uploaded = 0;
	downloaded = 0;
	this->path = path;
	estado = T_DETENIDO;
	activo = true;
	this->clienteTorrent = clienteTorrent;
	controlador = NULL;
	port = clienteTorrent->getPuerto();
}

Torrent::~Torrent() {
	if (activo)
		detener();
	delete tracker;
}

bool Torrent::inicializarTorrent(BencodeParser* parser){
	DatosParser* datos = parser->salidaParser();
	char* datoTemp;
	int tamTemp;

	if(!datos->obtenerDatoPorNombre("info_hash",&datoTemp,tamTemp)){
		delete datos;
		return false;
	}
	memcpy(info_hash,datoTemp,LEN_SHA1);
	delete[] datoTemp;

	datos->primero();
	if(!datos->obtenerDatoPorNombre("name",&datoTemp,tamTemp)){
		delete datos;
		return false;
	}

	bool result = fileManager.inicializar(datos);

	nombre = datoTemp;
	delete[] datoTemp;
	delete datos;

	return result;
}

void Torrent::run(){
try {

	 horaSistema = time (NULL);//Obtiene los segundos que pasaron desde 1970
     tracker->run();
} catch ( AvisoDescargaCompleta aviso) {
	// TODO parar todo el aviso lo lanza el filemanager al verificar q se descargaron todas las piezas
}

}

bool Torrent::conectarTracker(std::string url) {
	return tracker->connect(url);
}

bool Torrent::enviarEventoEstado(const char* event = NULL, int numwant = 0) {
	ParserMensaje parser;
	std::string envio;
	if (numwant < 0) {
		envio += parser.crearGetConEvento(tracker->getUrl(),tracker->getPath(),info_hash,clienteTorrent->getPeerId(),port,uploaded,downloaded,left(),event);
	} else {
		envio = parser.crearGetConNumwant(tracker->getUrl(),tracker->getPath(),info_hash,clienteTorrent->getPeerId(),port,uploaded,downloaded,left(),numwant);
	}
	return (tracker->send(envio.c_str(), envio.length()));
}
void Torrent::agregarPeer(std::string ip,unsigned int puerto){
	Socket* conexion = new Socket();
	if(conexion->connect(ip,puerto)==OK){
		conexion->setIp(ip);
		Peer* nuevoPeer = new PeerDown(conexion,this);
		peers.push_back(nuevoPeer);
		nuevoPeer->execute();
	}else{
		delete conexion;
		}

}

std::string Torrent::getNombre() {
	return nombre;
}

unsigned int  Torrent::left(){
	return (fileManager.getTamanio() - downloaded);
}


unsigned int* Torrent::getInfoHash(){
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

//implementar
void Torrent::continuar() {
	estado = T_ACTIVO;
	activo = true;
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

void Torrent::detener() {
	estado = T_DETENIDO;
	activo = false;
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

void Torrent::pausar() {
	estado = T_PAUSADO;
	activo = false;
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

std::string Torrent::getEstado() {
	return estado;
}

bool Torrent::estaActivo(){
	return activo;
}

unsigned int Torrent::getTamanio() {
	return fileManager.getTamanio();
}

unsigned int Torrent::getTamanioDescargado() {
	return downloaded;
}

int Torrent::getVelocidadSubida() {
	return 2;//todo.. ver ocmo calcular la velocidad
}

int Torrent::getVelocidadBajada() {
	return 30;
}

void Torrent::setControlador(Controlador* ctrl) {
	this->controlador = ctrl;
}

void Torrent::refrescarPeers() {
	//todo implementar

	controlador->actualizarEstado(this);
}

void Torrent::setCarpetaDescarga(std::string url) {
	//todo.. se la tiene qu epasar al file manager??
}

std::string Torrent::getPath() {
	return path;
}

std::string Torrent::getUrlTracker() {
	return tracker->getUrl();
}

std::string Torrent::getHashString() {
	Sha1 decodif;
	return decodif.salidaAstring(info_hash);
}

unsigned int Torrent::getCantPeers() {
	std::list<Peer*>::iterator it = peers.begin();
	unsigned int cant = 0;

	while (it != peers.end()) {
		it++;
		cant++;
	}
	return cant;
}
