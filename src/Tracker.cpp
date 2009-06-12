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
			buffer = bufferTemp;
			std::cout <<"\nReciev: \n" << buffer << std::endl;
			procesarResponse(buffer);
		}
		seCerro = true;
	}
	return NULL;
}


std::string Tracker::getPath() {
	return path;
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

//TODO falta testeo
void Tracker::procesarResponse (std::string buffer){
	archivoTracker(buffer);
	BencodeParser parser("responseTracker.bin");
    DatosParser* datos;
    char *aux;
    int tam;
    if (parser.procesar()) {
    		datos = parser.salidaParser();
    		datos->primero();
    		datos->obtenerDatoPorNombre("peers",&aux,tam);
    		//while (!datos->final()){
				decodificarPeers(datos->obtenerDato(),datos->obtenerLongitudDato());
			//	datos->siguiente();
			//}
	   }
       delete[]aux;
}

void Tracker::archivoTracker(std::string buffer){

    int inicio=0,lonInicial,lonTotal,i=0,marca;
    inicio=buffer.find("Content-Length: ",inicio);
    lonInicial=buffer.find(" ",inicio);
    lonTotal=buffer.find("\n",lonInicial);
    marca=buffer.find("d",lonTotal);

    //Obtengo la longitud total del archivo bencode
    char longitudBencode[lonTotal-lonInicial+1];
    for (inicio=lonInicial+1;inicio<lonTotal-1;inicio++){
    	longitudBencode[i]=buffer[inicio];
    	i++;
    }longitudBencode[i]='\0';
    char aux[atoi(longitudBencode)+1];

    FILE*fp=fopen("responseTracker.bin","w");i=0;
	for (int pos=marca;pos<(atoi(longitudBencode)+marca);pos++){
		aux[i]=buffer[pos]; i++;
	}	aux[i]='\0';

	for (int pos=0;pos<atoi(longitudBencode);pos++){
			fputc(aux[pos],fp);
		}
	fclose(fp);

}

void Tracker::decodificarPeers( char * cadena,unsigned int longitudCadena ){
	unsigned ip=0;
	unsigned port=0;
	unsigned int i,j,pos=0;
	//Tomar cadenas de cada 6 bytes
	do{
		for ( i=pos;i<pos+6;i++){
			j=0;
			if (i<pos+4){
				ip= cadena[i];
				std::cout<<ntohl(ip)<<".";
				if(i==pos+3) std::cout<<std::endl;
			}
			else{
		        port=cadena[i];
		        std::cout<<ntohl(port);
			}
		}
		pos=i;
		std::cout<<std::endl;
		//Crear peer
	}while (i<longitudCadena-1);

}

