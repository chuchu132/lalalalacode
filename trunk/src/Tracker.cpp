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
	bool respuestaCompleta = false;
	std::string buffer;
	char bufferTemp[1000];
	memset(bufferTemp, 0, 1000);
	while (trackerRemoto.is_valid() && !seCerro) {
		if ((cantidad = this->trackerRemoto.receive(bufferTemp, 999)) > 0) {
			bufferTemp[cantidad] = '\0';
			buffer += bufferTemp;
			std::cout << "\nReciev: \n" /*<< buffer */<< std::endl;
			/*creo que,entre otras cosas, el cout hacia q todo explote antes*/
			respuestaCompleta = procesarResponse(buffer);
		}
		if (respuestaCompleta == true)
			seCerro = true;
		else {
			cantidad=0;
		}
	}

	return NULL;
}

std::string Tracker::getPath() {
	return path;
}

std::string Tracker::getUrl(){
	return url;
}

bool Tracker::connect(std::string url) {
	inicilizar(url);
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

unsigned int Tracker::getMinInterval(){
	return minInterval;
}


//TODO falta testeo
bool Tracker::procesarResponse(std::string buffer) {
	int longitud;
	buffer = archivoTracker(buffer,&longitud);
	if (buffer != "Error") {
		BencodeParser parser(buffer.c_str(), longitud /*buffer.size()*/);
		DatosParser* datos;
		char *aux;
		int tam;
		if (parser.procesar()) {
			datos = parser.salidaParser();
			datos->primero();
			if (datos->obtenerDatoPorNombre("peers", &aux, tam)) {
				//while (!datos->final()){
				decodificarPeers(datos->obtenerDato(),
						datos->obtenerLongitudDato());
				//	datos->siguiente();
				//}
				delete datos;
				delete[] aux;
			}
		}

		return true;
	} else
		return false;
}

std::string Tracker::archivoTracker(std::string buffer,int *longitud) {

	unsigned inicio = 0, lonInicial, lonTotal, i = 0, marca;
	std::string salida;

	inicio = buffer.find("Content-Length: ", inicio);
	lonInicial = buffer.find(" ", inicio);
	lonTotal = buffer.find("\n", lonInicial);
	marca = buffer.find("d", lonTotal);

	if (inicio != string::npos && lonInicial != string::npos && lonTotal
			!= string::npos && marca != string::npos) {
		//Obtengo la longitud total del archivo bencode
		char* longitudBencode = new char[lonTotal - lonInicial + 1]; /*OJO al crear arrays de tamaño desconocido no crearlos estaticos*/
		for (inicio = lonInicial + 1; inicio < lonTotal - 1; inicio++) {
			longitudBencode[i] = buffer[inicio];
			i++;
		}
		longitudBencode[i] = '\0';
		char *aux = new char[atoi(longitudBencode) + 1];
		int longTemp = ( atoi(longitudBencode) + marca - 1); /*ACA hay lio se vas de rango*/
		if (buffer[longTemp] == 'e') {
			i = 0;
			for (unsigned pos = marca; pos < (atoi(longitudBencode) + marca); pos++) {
				aux[i] = buffer[pos];
				i++;
			}
			aux[i] = '\0';
			buffer.erase(0);
			salida.insert(0, aux, atoi(longitudBencode));
			*longitud=atoi(longitudBencode);
		}
		else
			salida= "Error";
		delete[] aux;
		delete[] longitudBencode;
	} else
		salida="Error";
	return salida;
}

void Tracker::decodificarPeers(char * cadena, unsigned int longitudCadena) {
	int cantMax = torrent->getCantidadMaximaPeers();
	int cantPeers= torrent->getCantPeers();
	int cantIps = (int) (longitudCadena / 6);
	unsigned short int puerto;
	int i = 0;
	while((cantMax > cantPeers) && (i < cantIps) ) {
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
		torrent->agregarPeer(ip.str(),puerto);
		i++;
		cantPeers = torrent->getCantPeers();
	}
}

