/*
 * Tracker.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Constantes.h"
#include "Tracker.h"
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
Tracker::Tracker() {
	puerto = PUERTO_DEFAULT_TRACKER;
	minInterval = 0;
}

Tracker::~Tracker() {

}
//TODO re implementar
void* Tracker::run() {
	int cantidad;
	bool seCerro = false;
	std::string buffer;
	char bufferTemp[1000];
	memset(bufferTemp, 0, 1000);

	while (trackerRemoto.is_valid() && !seCerro) {
		if ((cantidad = this->trackerRemoto.receive(bufferTemp, 999)) > 0) {
			bufferTemp[cantidad] = '\0';
			buffer.insert(0,bufferTemp,cantidad);
			std::cout.write(bufferTemp,cantidad)<<std::endl;
			if (procesarResponse(buffer,cantidad)) {
				//borrar buffr procesado
			}
		} else {
			seCerro = true;
		}

	}

	return NULL;
}

std::string Tracker::getPath() {
	return path;
}

std::string Tracker::getUrl() {
	return url;
}

bool Tracker::connect() {

	if (this->url.length() > 0) {
		return (trackerRemoto.connect(this->url, puerto) == OK);
	} else {
		return false;
	}
}

bool Tracker::send(const char* stream, unsigned int size) {
	return ((unsigned int) trackerRemoto.send(stream, size) == size);
}

void Tracker::cerrarConexion() {
	trackerRemoto.close();
}

void Tracker::inicilizar(std::string url) {
	int ini = url.find('/', 0);
	int fin = url.find_last_of('/', url.length());
	unsigned int puerto = url.find(':', ini);
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

unsigned int Tracker::getMinInterval() {
	return minInterval;
}

void Tracker::setTorrent(Torrent* unTorrent) {
	this->torrent = unTorrent;
}

//TODO falta testeo
bool Tracker::procesarResponse(std::string& buffer,int& longitud) {

	buffer = extraerBencode(buffer, longitud);
	if (buffer != "Error") {
		BencodeParser parser(buffer.c_str(), longitud);
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
		return true;
	} else {
		return false;
	}
}

std::string Tracker::extraerBencode(std::string &buffer, int &longitud) {

	unsigned i = 0, marca,longitudBencode;
	std::string salida;

	longitudBencode=obtenerLongitudBencode (buffer,marca);
	if(longitudBencode>0){
		char *aux = new char[longitudBencode+1];
		int longTemp = (longitudBencode+marca-1);
		if (longTemp == longitud -1) {
			if (buffer[longTemp] == 'e') {
				i = 0;
				for (unsigned pos = marca; pos
				< (longitudBencode + marca); pos++) {
					aux[i] = buffer[pos];
					i++;
				}
				aux[i] = '\0';
				buffer.erase(0);
				salida.insert(0, aux, longitudBencode);
				longitud = longitudBencode; // sumar el offset hasta el inicio del bencode creo q esta en "marca"
			} else {
				salida = "Error";
			}
		}
		else {
			salida="Error";
		}
		delete[] aux;
	} else
		salida = "Error";
	return salida;
}

int Tracker::obtenerLongitudBencode (std::string &buffer,unsigned int &marca){

	unsigned inicio = 0, i=0,lonInicial, lonTotal,salida;

	inicio    = buffer.find("Content-Length: ", inicio);
	lonInicial= buffer.find(" ", inicio);
	lonTotal  = buffer.find("\n", lonInicial);
	marca     = buffer.find("d", lonTotal);

	if (inicio != string::npos && lonInicial != string::npos && lonTotal
			!= string::npos && marca != string::npos) {
		//Obtengo la longitud total del archivo bencode
		char* longitudBencode = new char[lonTotal - lonInicial + 1];
		for (inicio = lonInicial + 1; inicio < lonTotal - 1; inicio++) {
			longitudBencode[i] = buffer[inicio];
			i++;
		}
		longitudBencode[i] = '\0';
		salida= atoi(longitudBencode);
		delete[] longitudBencode;
		return salida;
	}
	else return -1;

}


void Tracker::decodificarPeers(char * cadena, unsigned int longitudCadena) {
	// No inicializado el torrent y no pasa estas dos lineas
	int cantMax =  torrent->getCantidadMaximaPeers();
	int cantPeers = torrent->getCantPeers();
	int cantIps = (int) (longitudCadena / 6);
	unsigned short int puerto;
	int i = 0;
	while ((cantMax > cantPeers) && (i < cantIps)) {
		std::stringstream ip;
		int index = (i * 6);
		unsigned char temp = (unsigned char) cadena[index];
		ip << (int) temp << ".";
		temp = (unsigned char) cadena[index + 1];
		ip << (int) temp << ".";
		temp = (unsigned char) cadena[index + 2];
		ip << (int) temp << ".";
		temp = (unsigned char) cadena[index + 3];
		ip << (int) temp;
		memcpy(&puerto, cadena + index + 4, sizeof(unsigned short int));
		puerto = ntohs(puerto);
		std::cout<<"IP: "<<ip.str()<<" Puerto: "<< puerto<<std::endl;
		//torrent->agregarPeer(ip.str(), puerto);
		i++;
		cantPeers ++;//= torrent->getCantPeers();
	}
}

