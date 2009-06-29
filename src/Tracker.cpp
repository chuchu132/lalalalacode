/*
 * Tracker.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Constantes.h"
#include "Tracker.h"
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <sstream>
#include "Tipos.h"

Tracker::Tracker() {
	puerto = PUERTO_DEFAULT_TRACKER;
	minInterval = 0;
	refresh = false;
}

Tracker::~Tracker() {
	refresh = false;
	this->cerrarConexion();
	//join????????????? todo
}

void* Tracker::run() {
	int cantidad;
	int caracteresProcesados=0;
	int longitud,posUltimoProcesado;
	bool seCerro = false;
	std::string buffer;
	char bufferTemp[BUFSIZE];
	memset(bufferTemp, 0, BUFSIZE);
	refresh=false;

	do{
		while (trackerRemoto.is_valid() && !seCerro && !refresh) {
			if ((cantidad = this->trackerRemoto.receive(bufferTemp, BUFSIZE-1)) > 0) {
				bufferTemp[cantidad] = '\0';
				std::cout<< bufferTemp <<std::endl;
				buffer.insert(caracteresProcesados,bufferTemp,cantidad);
				longitud=caracteresProcesados+cantidad;
				if (procesarResponse(buffer,longitud,posUltimoProcesado)) {
					buffer.erase(0,posUltimoProcesado);
				}
				else {
					//En caso de recibir un mensaje de falla del tracker lo notifico a la vistas
					if (buffer.find("failure",0)!=string::npos)
						 torrent->getControlador()->notificarVista(buffer);

					caracteresProcesados+=cantidad;
					buffer.resize(caracteresProcesados+cantidad);
				}

			} else {
				seCerro = true;
			}
		}
		if ( refresh){
			if (trackerRemoto.is_valid()){
				torrent->removerPeersInactivos(NULL);
				cerrarConexion();
			}
			if (connect()){
				torrent->enviarEventoEstado(NULL,400);
				refresh = false;
				seCerro = false;
				caracteresProcesados=0;
			}
		}
		sleep(3);
	}while (trackerRemoto.is_valid() || refresh);

	std::cout<<"fin run del tracker"<<std::endl;//todo.. sacar!!
	return NULL;
}

bool Tracker::connect() {

	if (this->url.length() > 0) {
		return (trackerRemoto.connect(this->url, puerto) == OK);
	} else {
		return false;
	}
}

bool Tracker::send(const char* stream, UINT size) {
	return ((UINT) trackerRemoto.send(stream, size) == size);
}

void Tracker::cerrarConexion() {
	trackerRemoto.close();
}

void Tracker::inicilizar(std::string url) {
	int ini = url.find('/', 0);
	int fin = url.find_last_of('/', url.length());
	UINT puerto = url.find(':', ini);
	path.assign(url, fin + 1, url.length() - fin - 1);
	if (puerto != std::string::npos) {
		std::string temp;
		temp.assign(url, puerto + 1, fin - puerto - 1);
		this->puerto = atoi(temp.c_str());
		this->url.assign(url, ini + 2, puerto - (ini + 2));
	} else {
		this->url.assign(url, ini + 2, fin - (ini + 2));
	}
}

bool Tracker::procesarResponse(std::string& buffer,int& longitud,int &posUltimoProcesado) {
	std::string salida;
	if (extraerBencode(buffer, longitud,salida,posUltimoProcesado)) {
		BencodeParser parser(salida.c_str(), longitud);
		if (parser.procesar()) {
			DatosParser* datos;
			char *aux;
			int tam;
			datos = parser.salidaParser();
			datos->primero();
			if (datos->obtenerDatoPorNombre("peers", &aux, tam)) {
				decodificarPeers(datos->obtenerDato(),
						datos->obtenerLongitudDato());
				delete[] aux;
			}
			delete datos;
		}
		else
			return false ;
	} else
		return false;

	return true;
}

bool Tracker::extraerBencode(std::string &buffer, int &longitud,std::string &salida,int &posUltimoProcesado) {

	unsigned i = 0, marca,longitudBencode;
	longitudBencode=obtenerLongitudBencode (buffer,marca);
	if(longitudBencode>0){
		char *aux = new char[longitudBencode+1];
		int longTemp = (longitudBencode+marca-1);
		if (longTemp <= longitud -1) {
			if (buffer[longTemp] == 'e') {
				i = 0;
				for (unsigned pos = marca; pos
				< (longitudBencode + marca); pos++) {
					aux[i] = buffer[pos];
					i++;
				}
				aux[i] = '\0';
				salida.insert(0, aux, longitudBencode);
				longitud = longitudBencode;
				posUltimoProcesado=longitudBencode+marca;
				delete[] aux;
			} else {
				delete[] aux;
				return false;
			}
		}
		else {
			delete[] aux;
			return false;
		}

	} else
		return false;
	return true;
}

int Tracker::obtenerLongitudBencode (std::string &buffer,UINT &marca){

	unsigned inicio = 0, i=0,marcaPeers,marcaFinLong,salida;

	marca       = buffer.find("d8:complete", 0);
	marcaPeers  = buffer.find("peers",0);
	marcaFinLong= buffer.find(":",marcaPeers);

	if (marca != string::npos && marcaPeers != string::npos && marcaFinLong != string::npos) {
		//Obtengo la longitud total de los peers
		std::string longitudPeers;
		for (inicio = marcaPeers + 5; inicio < marcaFinLong ; inicio++) {
			longitudPeers[i] = buffer[inicio];
			i++;
		}
		longitudPeers[i]='\0';
		salida= atoi(longitudPeers.c_str())+(marcaFinLong-marca)+2;
		return salida;
	}
	else return ERROR;

}

void Tracker::decodificarPeers(char * cadena, UINT longitudCadena) {

	int cantMax =  torrent->getCantidadMaximaPeers();
	int cantPeers = torrent->getCantPeers();
	int cantIps = (int) (longitudCadena / 6);
	unsigned short int puerto;
	int i = 0;
	int rondas = 0;
	refresh = false;
	time_t horaActual;
	UINT difInterval,difDownload;
	do{
		while ((cantMax > cantPeers) && (i < cantIps) && torrent->estaActivo() && !refresh) {
			std::stringstream ip;
			int index = (i * 6);
			UCHAR temp = (UCHAR) cadena[index];
			ip << (int) temp << ".";
			temp = (UCHAR) cadena[index + 1];
			ip << (int) temp << ".";
			temp = (UCHAR) cadena[index + 2];
			ip << (int) temp << ".";
			temp = (UCHAR) cadena[index + 3];
			ip << (int) temp;
			memcpy(&puerto, cadena + index + 4, sizeof(USINT));
			puerto = ntohs(puerto);
			std::string ip_string = ip.str();
			if (!torrent->existePeerIP(ip_string)){
				torrent->agregarPeer(ip_string, puerto);
			}
			i++;
			cantPeers = torrent->getCantPeers();
			while( torrent->estaActivo() && (cantPeers == cantMax) && !refresh){
				torrent->removerPeersInactivos(NULL);
				cantPeers = torrent->getCantPeers();
				sleep(5);
			}

			horaActual = time(NULL);
			difInterval = (UINT) difftime(horaActual, torrent->getTimeRefresh());
			difDownload = (UINT) difftime(horaActual, torrent->getTimeLastDown());
			if (difInterval >= minInterval && difDownload >=MIN_REFRESH_DOWNLOAD) {
				torrent->refrescarPeers();
				torrent->setTimeRefresh(horaActual);
				torrent->setTimeLastDown(horaActual);
			}

		}
		rondas ++;
		i = 0;
	}while(!refresh && torrent->estaActivo());
}

std::string Tracker::getPath() {
	return path;
}

std::string Tracker::getUrl() {
	return url;
}

UINT Tracker::getMinInterval() {
	return minInterval;
}

void Tracker::setTorrent(Torrent* unTorrent) {
	this->torrent = unTorrent;
}

void Tracker::setRefresh(bool refresh){
	this->refresh = refresh;
}
