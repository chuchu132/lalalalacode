/*
 * Configuracion.cpp
 *
 *  Created on: 12/06/2009
 *      Author: teddy
 */

#include "Configuracion.h"

#define TAG_DESCARGAS "[descargas]"
#define TAG_PUERTO "[puerto]"

Configuracion::Configuracion() {

	huboCambios = false;
	cargarConfiguracionDefault();

	archivo.open(RUTA_CONFIGURACION,std::fstream::in);

	if (archivo.is_open()) {
		cargarConfiguracion();
	}

	archivo.close();
}

Configuracion::~Configuracion() {
	if (huboCambios)
		guardarConfiguracion();
}

void Configuracion::crearArchivo(){

	archivo.clear();

	archivo.open(RUTA_CONFIGURACION,std::ios_base::out | std::ios_base::trunc);

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
	else {
		huboCambios = true;
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
				huboCambios = true;
			}
		}
	}
}

void Configuracion::guardarConfiguracion() {
	crearArchivo();
	archivo << "[FiTorrent]"<<std::endl;
	archivo << TAG_DESCARGAS<<" <"<< rutaDescargas <<'>'<< std::endl;
	archivo << TAG_PUERTO<<' '<< puerto << std::endl;
	archivo.close();
}

void Configuracion::guardarPuerto(unsigned int puerto) {
	this->puerto = puerto;
	huboCambios = true;
}

void Configuracion::guardarRutaDescargas(std::string ruta) {
	rutaDescargas = ruta;
	huboCambios = true;
}

std::string Configuracion::getRutaDescargas() {
	return rutaDescargas;
}

unsigned int Configuracion::getPuerto() {
	return puerto;
}

std::string Configuracion::leerRuta() {
	std::string ruta;
	char aux;
	archivo.get(aux);//\n
	archivo.get(aux); //'<'
	archivo.get(aux);//primer caracter

	while (aux != '>') {
		ruta += aux;
		archivo.get(aux);
	}
	return ruta;
}
