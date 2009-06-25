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
	fileManager(clienteTorrent, this) {
	tracker = new Tracker();
	tracker->setTorrent(this);
	uploaded = 0;
	downloaded = 0;
	downAnterior = 0;
	velocidadAnt = 0;
	this->path = path;
	estado = T_DETENIDO;
	activo = false;
	this->clienteTorrent = clienteTorrent;
	controlador = NULL;
	port = clienteTorrent->getPuerto();
	cantidadMaximaPeers = PEERS_MAX;
	endGame = PRIMER_END_GAME;
	timeLastRefresh = time(NULL);
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
		enviarEventoEstado(NULL, 200);
		tracker->execute();
		if (controlador != NULL) {
			std::string notif = "Conectado con el tracker ";
			notif += tracker->getUrl();
			controlador->notificarVista(notif);
		}
	} else {
		activo = false;
		if (estado != T_COMPLETO)
			estado = T_DETENIDO;
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

bool Torrent::enviarEventoReAnnounce(int numwant = 0) {
	ParserMensaje parser;
	std::string envio;
	envio = parser.crearGetReAnnounce(tracker->getUrl(), tracker->getPath(),
			info_hash, clienteTorrent->getPeerId(), port, uploaded, downloaded,
			left(), numwant);

	return (tracker->send(envio.c_str(), envio.length()));
}

void Torrent::agregarPeer(std::string ip, UINT puerto) {
	Socket* conexion = new Socket();
	if (conexion->connectWithTimeout(ip.c_str(), puerto, TIME_OUT_CONNECT)
			== OK) {
		conexion->setIp(ip);
		conexion->setPuerto(puerto);
		Peer* nuevoPeer = new PeerDown(conexion, this);
		llaveListaPeers.lock();
		peers.push_back(nuevoPeer);
		llaveListaPeers.unlock();
		nuevoPeer->execute();
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
	} else {
		resultado = false;
	}
	llaveListaPeers.unlock();
	return resultado;
}

bool Torrent::existePeerIP(std::string ip) {
	llaveListaPeers.lock();
	std::list<Peer*>::iterator it = peers.begin();
	bool existe = false;
	while (it != peers.end()) {
		if (ip == (*it)->getIp()) {
			existe = true;
			break;
		}
		it++;
	}
	llaveListaPeers.unlock();
	return existe;
}

void Torrent::removerPeersInactivos(Peer* peerQueQueda) {
	llaveListaPeers.lock();
	std::list<Peer*>::iterator it = peers.begin();
	Peer* temp;
	while (it != peers.end()) {
		if (!(*it)->conexionEstaOK() && ((*it) != peerQueQueda)) {
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
	while (it != peers.end()) {
		if ((*it)->conexionEstaOK()) {
			(*it)->cerrarConexion();
		}
		it++;
	}
}

void Torrent::refrescarPeers() {
	time_t horaActual = time(NULL);
	if (enviarEventoReAnnounce(400)) {
		tracker->setRefresh(true);
		removerPeersInactivos(NULL);
		horaInicial = horaActual;
		fileManager.vaciarMapaPedidos();
		// reinicia el mapa de pedidos para aprovechar los nuevos peers
	}
}

void Torrent::continuar() {
	if (!activo) {
		activo = true;
		if (downloaded >= getTamanio()) {
			estado = T_COMPLETO;
		} else
			estado = T_ACTIVO;

		run();
		if (controlador != NULL)
			controlador->actualizarEstado(this);
	}
}

void Torrent::detener() {

	if (activo) {
		activo = false;
		enviarEventoEstado(EVENT_STOPPED, -1);
		tracker->cerrarConexion();
		tracker->join();
		std::cout << "tracker detenido" << std::endl;
		detenerPeers();
		removerPeersInactivos(NULL);
		estado = T_DETENIDO;
		std::cout << "torrent detenido" << std::endl;
		if (controlador != NULL)
			controlador->actualizarEstado(this);
	}
}

std::string Torrent::getNombre() {
	return nombre;
}

ULINT Torrent::left() {
	return (fileManager.getTamanio() - downloaded);
}

UINT* Torrent::getInfoHash() {
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

bool Torrent::estaCompleto() {
	if (estado == T_COMPLETO)
		return true;
	else
		return false;
}

bool Torrent::estaActivo() {
	return activo;
}

ULINT Torrent::getTamanio() {
	return fileManager.getTamanio();
}

ULINT Torrent::getTamanioDescargado() {
	return downloaded;
}

ULINT Torrent::getTamanioSubido() {
	return uploaded;
}

int Torrent::getVelocidadSubida() {
	return 0;//TODO.. ver como calcular la velocidad
}

int Torrent::getVelocidadBajada() {
	time_t horaAct = time(NULL);
	int tiempo = (int) difftime(horaAct, horaAnterior);
	if (tiempo <= 0)
		return velocidadAnt;
	horaAnterior = horaAct;
	ULINT diferencia = downloaded - downAnterior;

	downAnterior = downloaded;
	velocidadAnt = (int) ((diferencia / 1024) / tiempo);
	return velocidadAnt;
}

void Torrent::setControlador(Controlador* ctrl) {
	this->controlador = ctrl;
}

void Torrent::setEstado(std::string estado) {
	this->estado = estado;
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

UINT Torrent::getCantPeers() {
	return peers.size();
}

std::string Torrent::bytesToString(ULINT bytes) {
	float temp = bytes;
	std::stringstream buffer;
	buffer << std::setprecision(4);
	if (bytes < 1024) {
		buffer << bytes << " bytes";
	} else {
		temp = (bytes / 1024);
		if (temp < 1024) {
			buffer << temp << " kb";
		} else {
			temp /= 1024;
			buffer << temp << " Mb";
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

UINT Torrent::getCantidadMaximaPeers() {
	return cantidadMaximaPeers;
}

std::string Torrent::getTiempoRestante() {
	if (estado == T_ACTIVO) {
		std::stringstream buffer;
		int velocidad = getVelocidadBajada();
		if (velocidad == 0)
			buffer << "N/A";
		else {
			velocidad *= 1024;
			float tiempo = (left() / velocidad);
			int temp, auxTemp;
			temp = (UINT) (tiempo / 86400);
			if (temp > 0)
				buffer << temp << " d ";
			temp = (UINT) tiempo % 86400;
			auxTemp = (UINT) (temp / 3600);
			if (auxTemp > 0)
				buffer << auxTemp << " hs ";
			temp = temp % 3600;
			auxTemp = (UINT) (temp / 60);
			if (auxTemp > 0)
				buffer << auxTemp << " m ";
			temp = temp % 60;
			if (temp > 0)
				buffer << temp << " s";
		}
		return buffer.str();
	} else
		return "-";
}

void Torrent::setDownloaded(ULINT bytes) {
	timeLastDownload = time(NULL);
	llaveCambiosDownloaded.lock();

	if ((downloaded + bytes) >= fileManager.getTamanio())
		downloaded = fileManager.getTamanio();
	else
		downloaded += bytes;
	llaveCambiosDownloaded.unlock();
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

void Torrent::setUploaded(ULINT bytes) {
	uploaded += bytes;
	if (controlador != NULL)
		controlador->actualizarEstado(this);
}

time_t Torrent::getTimeRefresh() {
	return timeLastRefresh;
}

time_t Torrent::getTimeLastDown() {
	return timeLastDownload;
}

void Torrent::setTimeRefresh(time_t Trefresh) {
	timeLastRefresh = Trefresh;
}

void Torrent::setTimeLastDown(time_t horaActual) {
	timeLastDownload = horaActual;
}

bool Torrent::reiniciarPedidos(Peer* peerQueQueda) {
	ULINT total = fileManager.getTamanio();
	float porcent = (downloaded / total);
	if (porcent > endGame) {
		fileManager.vaciarMapaPedidos();
		removerPeersInactivos(peerQueQueda);
		if (endGame == SEGUNDO_END_GAME) {
			endGame = 1.0;
		} else {
			endGame = SEGUNDO_END_GAME;
		}
		return true;
	}
	return false;
}

void Torrent::descargaCompleta() {
	activo = false;
	downloaded = fileManager.getTamanio();
	enviarEventoEstado(EVENT_COMPLETED, -1);
	tracker->cerrarConexion(); //todo ver esto hace que no se puedan bajar de nosotros :S
	tracker->join();
	detenerPeers();
	estado = T_COMPLETO;
	fileManager.descargaAarchivos();
	fileManager.guardarDatos();

	if (controlador != NULL) {
		std::string notif = "Se completo la descada del Torrent ";
		notif += getNombre();
		controlador->notificarVista(notif);
		controlador->actualizarEstado(this);
	}
}
