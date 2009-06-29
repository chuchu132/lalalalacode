/*
 * Configuracion.cpp
 *
 *  Created on: 12/06/2009
 *      Author: Lucia
 */

#include "Configuracion.h"

#define TAG_DESCARGAS "[descargas]"
#define TAG_PUERTO "[puerto]"
#define TAG_TORRENTS "[torrents]"
#define TAG_ENDTORRENTS "[/torrents]"

Configuracion::Configuracion() {

	cargarConfiguracionDefault();
	char pathActual[BUFSIZE];

    rutaConfiguracion=getcwd(pathActual,BUFSIZE);
    rutaConfiguracion.append("/FiTorrent.config");

	archivo.open(rutaConfiguracion.c_str(),std::fstream::in);

	if (archivo.is_open()) {
		cargarConfiguracion();
	}

	archivo.close();
}

Configuracion::~Configuracion() {
	guardarConfiguracion();
}

void Configuracion::crearArchivo(){

	archivo.clear();
	archivo.open(rutaConfiguracion.c_str(),std::ios_base::out | std::ios_base::trunc);
}

void Configuracion::cargarConfiguracionDefault() {
	puerto = PUERTO_DEFAULT;
	rutaDescargas = RUTA_DESCARGAS;
}

void Configuracion::cargarConfiguracion() {

	std::string linea;

	if (!archivo.eof()) {
		archivo >>linea;
	}

	while (!archivo.eof())
	{
		archivo >>linea;

		if (linea == TAG_DESCARGAS) {
			rutaDescargas = leerRuta();
		}
		else {
			if (linea == TAG_PUERTO){
				std::stringstream buffer;
				archivo >>linea;
				buffer << linea;
				buffer >> puerto;
			}
			else {
				if (linea == TAG_TORRENTS) {
					std::string temp;
					archivo >>linea;
					while (linea != TAG_ENDTORRENTS) {
						temp = linea + ' ';
						temp += leerRuta();
						torrents.push_back(temp);
						archivo >> linea;
					}
				}
			}
		}
	}
}

void Configuracion::guardarConfiguracion() {
	crearArchivo();
	archivo << "[FiTorrent]"<<std::endl;
	archivo << TAG_DESCARGAS <<" <"<< rutaDescargas <<'>'<< std::endl;
	archivo << TAG_PUERTO <<' '<< puerto << std::endl;
	archivo << TAG_TORRENTS << std::endl;

	while (!torrents.empty()) {
		archivo << torrents.front() <<std::endl;
		torrents.pop_front();
	}

	archivo << TAG_ENDTORRENTS << std::endl;
	archivo.close();
}

std::string Configuracion::leerRuta() {
	std::string ruta;
	char aux;
	archivo.get(aux);//' '
	archivo.get(aux); //'<'
	archivo.get(aux);//primer caracter

	while (aux != '>') {
		ruta += aux;
		archivo.get(aux);
	}
	return ruta;
}

void Configuracion::guardarTorrent(std::string estado, std::string ruta) {
	std::string info;

	info = estado + " <";
	info += ruta + '>';
	torrents.push_back(info);

}

std::string Configuracion::obtenerTorrent() {

	std::string info = torrents.front();
	std::string::size_type pos;

	pos = info.find_first_of(' ');
	estado = info.substr(0,pos);

	torrents.pop_front();

	return info.substr(pos+1);
}

bool Configuracion::hayTorrents() {
	return (!torrents.empty());
}

std::string Configuracion::getEstadoTorrent() {
	return estado;
}


void Configuracion::guardarPuerto(UINT puerto) {
	this->puerto = puerto;
}

void Configuracion::guardarRutaDescargas(std::string ruta) {
	rutaDescargas = ruta;
}

std::string Configuracion::getRutaDescargas() {
	return rutaDescargas;
}

UINT Configuracion::getPuerto() {
	return puerto;
}
