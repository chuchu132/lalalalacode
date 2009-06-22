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
#include <errno.h>

Torrent::Torrent(ClienteTorrent* clienteTorrent, std::string path) :
	fileManager(clienteTorrent) {
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
	detener();
	delete tracker;
}

bool Torrent::inicializarTorrent(BencodeParser* parser) {
	DatosParser* datos = parser->salidaParser();
	char* datoTemp;
	int tamTemp;

	datos->primero();
	if (!datos->obtenerDatoPorNombre("announce", &datoTemp, tamTemp)) {
		delete datos;
		return false;
	}
	tracker->inicilizar(datoTemp);
	delete[] datoTemp;

	if (!datos->obtenerDatoPorNombre("info_hash", &datoTemp, tamTemp)) {
		delete datos;
		return false;
	}
	memcpy(info_hash, datoTemp, LEN_SHA1);
	delete[] datoTemp;

	datos->primero();
	if (!datos->obtenerDatoPorNombre("name", &datoTemp, tamTemp)) {
		delete datos;
		return false;
	}

	bool result = fileManager.inicializar(datos);

	nombre = datoTemp;
	delete[] datoTemp;
	delete datos;

	return result;
}

void Torrent::run() {

	horaInicial = time(NULL);//Obtiene los segundos que pasaron desde 1970
	horaAnterior = time(NULL);
	downAnterior = downloaded;
	if (controlador != NULL) {
		std::string notif = "Conectando con ";
		notif += tracker->getUrl();
		controlador->notificarVista(notif);
	}

	if (tracker->connect()) {
		std::cout << "conecto" << std::endl;
		std::cout << ((enviarEventoEstado(NULL, 200)) ? "envio" : "noenvio")
				<< std::endl;
		tracker->execute();
		if (controlador != NULL) {
			std::string notif = "Conectado con el tracker ";
			notif += tracker->getUrl();
			controlador->notificarVista(notif);
		}
	} else {
		std::cout << "noconecto" << std::endl;

		estado = T_DETENIDO;//si no se conecto el estado es detenido
		activo = false;
		if (controlador != NULL) {
			std::string notif = "Fallo la conexion con el tracker ";
			notif += tracker->getUrl();
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
		envio += parser.crearGetConEvento(tracker->getUrl(),
				tracker->getPath(), info_hash, clienteTorrent->getPeerId(),
				port, uploaded, downloaded, left(), event);
	} else {
		envio = parser.crearGetConNumwant(tracker->getUrl(),
				tracker->getPath(), info_hash, clienteTorrent->getPeerId(),
				port, uploaded, downloaded, left(), numwant);
	}
	return (tracker->send(envio.c_str(), envio.length()));
}

void Torrent::agregarPeer(std::string ip, unsigned int puerto) {
	Socket* conexion = new Socket();
	if (conexion->connectWithTimeout(ip.c_str(), puerto, 5) == OK) {
		conexion->setIp(ip);
		conexion->setPuerto(puerto);
		Peer* nuevoPeer = new PeerDown(conexion, this);
		llaveListaPeers.lock();
		peers.push_back(nuevoPeer);
		llaveListaPeers.unlock();
		nuevoPeer->execute();

//		if (controlador != NULL)
//			controlador->actualizarEstado(this);
	} else {
		delete conexion;
	}
}

bool Torrent::agregarPeer(Peer* peerNuevo) {
	bool resultado;

	llaveListaPeers.lock();
	if (getCantPeers() < getCantidadMaximaPeers()) {
		peers.push_back(peerNuevo);
		peerNuevo->execute();
		resultado = true;
	}
	else {
		resultado = false;
	}
	llaveListaPeers.unlock();
	return resultado;
}

void Torrent::removerPeersInactivos() {
	llaveListaPeers.lock();
	std::list<Peer*>::iterator it = peers.begin();
	Peer* temp;
	while (it != peers.end()) {
		if (!(*it)->conexionEstaOK()) {
			(*it)->join();
			temp = (*it);
			it = peers.erase(it);
			delete temp;
		}
		it++;
	}
	llaveListaPeers.unlock();
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

void Torrent::detenerPeers() {
	std::list<Peer*>::iterator it = peers.begin();
	Peer* temp; //todo.. ver que no haya problemas con otros threads
	while (it != peers.end()) {
		temp = (*it);
		if(temp->conexionEstaOK()){
		temp->cerrarConexion();
		temp->join();
		}
		it = peers.erase(it);
		delete temp;
		it++;
	}
}

void Torrent::refrescarPeers() {
	time_t horaActual = time(NULL);
	unsigned int dif = (unsigned int) difftime(horaActual, horaInicial);
	if (dif >= tracker->getMinInterval()) {
		removerPeersInactivos();
		enviarEventoEstado(NULL, 200);
		horaInicial = horaActual;
	}
	//controlador->actualizarEstado(this);
}

void Torrent::continuar() {

	if (estado != T_ACTIVO) {

		activo = true;
		estado = T_ACTIVO;
		run();
		if (controlador != NULL)
			controlador->actualizarEstado(this);
	}
}

void Torrent::detener() {

	if (estado != T_DETENIDO && estado != T_COMPLETO ) {
		activo = false;
		tracker->cerrarConexion();
		tracker->join();
		std::cout << "tracker detenido" << std::endl;
		detenerPeers();
		estado = T_DETENIDO;
		std::cout << "torrent detenido" << std::endl;
		if (controlador != NULL)
			controlador->actualizarEstado(this);
	}
}

void Torrent::pausar() {
	//todo.. ver que hace el pausar
	if (estado != T_PAUSADO) {

		estado = T_PAUSADO;
		activo = false;
		if (controlador != NULL)
			controlador->actualizarEstado(this);
	}
}

std::string Torrent::getNombre() {
	return nombre;
}

unsigned int Torrent::left() {
	return (fileManager.getTamanio() - downloaded);
}

unsigned int* Torrent::getInfoHash() {
	return info_hash;
}

std::string Torrent::getPeerId() {
	return clienteTorrent->getPeerId();
}

FileManager* Torrent::getFileManager() {
	return &fileManager;
}

std::list<Peer*>* Torrent::getListaPeers() {
	return &peers;
}

std::string Torrent::getEstado() {
	return estado;
}

bool Torrent::estaActivo() {
	return activo;
}

unsigned int Torrent::getTamanio() {
	return fileManager.getTamanio();
}

unsigned int Torrent::getTamanioDescargado() {
	return downloaded;
}

int Torrent::getVelocidadSubida() {
	return 0;//todo.. ver como calcular la velocidad
}

int Torrent::getVelocidadBajada() {


	time_t horaAct = time(NULL);
	double tiempo = difftime(horaAct,horaAnterior);
	horaAnterior = horaAct;
	unsigned int diferencia = downloaded - downAnterior;

	downAnterior=downloaded;
	return((diferencia/1024)/tiempo);

}

void Torrent::setControlador(Controlador* ctrl) {
	this->controlador = ctrl;
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
	if (bytes < 1024) {
		buffer << bytes << " bytes";
	} else {
		bytes /= 1024;
		if (bytes < 1024) {
			buffer << bytes << " kb";
		} else {
			bytes /= 1024;
			buffer << bytes << " Mb";
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

unsigned int Torrent::getCantidadMaximaPeers() {
	return cantidadMaximaPeers;
}

std::string Torrent::getTiempoRestante() {
	return "N/A";
	//calcular el tiempo restante usando la velocidad
}

void Torrent::setDownloaded(unsigned int bytes) {
	downloaded += bytes;
	std::cout << "_________Bytes descargados: " << downloaded << std::endl;
	if (controlador != NULL)//ver
		controlador->actualizarEstado(this);

}

void Torrent::setUploaded(unsigned int bytes) {
	uploaded += bytes;
	if (controlador != NULL)//ver
		controlador->actualizarEstado(this);
}

void Torrent::descargaCompleta() {
	activo = false;
	tracker->cerrarConexion(); //esto hace que no se puedan bajar de nosotros :S
	tracker->join();
	detenerPeers(); //habria que cerrar solo los peerdown
	estado = T_COMPLETO;
	std::cout<<"<---------------SE COMPLETO LA DESCARGA!!!------------------>"<<std::endl;

	fileManager.descargaAarchivos();

	if (controlador != NULL){
		std::string notif = "Se completo la descada del Torrent ";
		notif += getNombre();
		notif += " =)";
		controlador->notificarVista(notif);
		controlador->actualizarEstado(this);
	}

}
