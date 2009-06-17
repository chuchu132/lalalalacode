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
#include "Constantes.h"
#include <cstring>
#include <cstdio>
#include <iomanip>
#include <cstdio>
#include <ctime>

Torrent::Torrent(ClienteTorrent* clienteTorrent, std::string path):fileManager(clienteTorrent) {
	tracker = new Tracker();
	tracker->setTorrent(this);
	uploaded = 0;
	downloaded = 0;
	this->path = path;
	estado = T_DETENIDO;
	activo = true;
	this->clienteTorrent = clienteTorrent;
	controlador = NULL;
	port = clienteTorrent->getPuerto();
	cantidadMaximaPeers = PEERS_MAX;
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

	datos->primero();
	if(!datos->obtenerDatoPorNombre("announce",&datoTemp,tamTemp)){
		delete datos;
		return false;
	}
	tracker->inicilizar(datoTemp);
	delete[] datoTemp;

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

	horaInicial = time (NULL);//Obtiene los segundos que pasaron desde 1970
	horaAnterior = time(NULL);
	downAnterior = downloaded;
	if (controlador != NULL) {
		std::string notif = "Conectando con ";
		notif += tracker->getPath();
		controlador->notificarVista(notif);
	}
	if (tracker->connect()) {
		std::cout<<"conecto"<<std::endl;
		std::cout<<((enviarEventoEstado(NULL,50))?"envio":"noenvio")<<std::endl;
		tracker->execute();
		if (controlador != NULL) {
			std::string notif = "Conectado con el tracker ";
			notif += tracker->getPath();
			controlador->notificarVista(notif);
		}
		sleep(5);

	} else {
		std::cout<<"noconecto"<<std::endl;

		if (controlador != NULL) {
			std::string notif = "Fallo la conexion con el tracker ";
			notif += tracker->getPath();
			controlador->notificarVista(notif);
		}
	}
}

bool Torrent::conectarTracker(std::string url) {
	tracker->inicilizar(url);
	return tracker->connect();
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
	conexion->setNonblocking(); /*TODO aca se traba con algunos peers q no responden el connect
	invente los no bloqueantes y bloqueantes pero no andan hay q ver de poner algun timeout
	xq como estan ahora practicamente no esperan respuesta y pasan de largo
	*/
	if(conexion->connect(ip,puerto)==OK){
		conexion->setBlocking();
		conexion->setIp(ip);
		Peer* nuevoPeer = new PeerDown(conexion,this);
		peers.push_back(nuevoPeer);
		nuevoPeer->execute();
	}else{
		delete conexion;
	}

}
void Torrent::agregarPeer(Peer* peerNuevo){
	peers.push_back(peerNuevo);
	peerNuevo->execute();
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

	// no esta probado ;) lo mismo para vel de subida
	//	time_t horaAct = time(NULL);
	//	double tiempo = difftime(horaAct,horaAnterior);
	//	horaAnterior = horaAct;
	//
	//	unsigned int diferencia = downloaded - downAnterior;
	//
	//	return  ((diferencia/1024)/tiempo);
}

void Torrent::setControlador(Controlador* ctrl) {
	this->controlador = ctrl;
}

void Torrent::refrescarPeers() {
	time_t horaActual = time(NULL);
	unsigned int dif = (unsigned int) difftime(horaActual,horaInicial);
	if(dif >= tracker->getMinInterval() ){
		enviarEventoEstado(NULL,0);
		horaInicial = horaActual;
	}
	controlador->actualizarEstado(this);
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
	return peers.size();
}

std::string Torrent::bytesToString(float bytes) {
	std::stringstream buffer;
	buffer << std::setprecision(4);
	if (bytes < 1024)
	{
		buffer<< bytes<<" bytes";
	}
	else
	{
		bytes /= 1024;
		if (bytes < 1024)
		{
			buffer <<bytes<<" kb";
		}
		else
		{
			bytes /= 1024;
			buffer<<bytes<<" Mb";
		}
	}
	return buffer.str();
}

std::list<Peer*>::iterator Torrent::getIterPeers() {
	return peers.begin();
}

std::list<Peer*>::iterator Torrent::getEndIterPeers() {
	return peers.end();
}

int Torrent::getCantidadMaximaPeers(){
	return cantidadMaximaPeers;
}
