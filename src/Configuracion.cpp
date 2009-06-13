/*
 * Configuracion.cpp
 *
 *  Created on: 12/06/2009
 *      Author: teddy
 */

#include "Configuracion.h"

#define TAG_DESCARGAS "[descargas] "
#define TAG_PUERTO "[puerto] "

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
	std::cout<<"fin" <<std::endl;
}

void Configuracion::crearArchivo(){

	archivo.clear();//VER!

	archivo.open(RUTA_CONFIGURACION,std::ios_base::out | std::ios_base::trunc);

}

void Configuracion::cargarConfiguracionDefault() {
	std::cout<< "Configuracion por Default"<<std::endl;
	puerto = PUERTO_DEFAULT;
	rutaDescargas = RUTA_DESCARGAS;
}

void Configuracion::cargarConfiguracion() {

	std::cout<<"Cargando config desde archivo" <<std::endl;
	std::string linea;
	std::string opc;
	std::string::size_type pos;

	if (!archivo.eof()) {
		//leerLinea();
		archivo >>linea;
		archivo >>linea;
		archivo >>linea;
	}
	else {
		huboCambios = true;
	}

	while (!archivo.eof())
	{
		//linea = leerLinea();
		archivo >>linea;
		std::cout<<linea<<std::endl;
		pos = linea.find_first_of(' ');

		if (pos == std::string::npos) {
			continue;
			huboCambios = true;
		}

		opc = linea.substr(0, pos);

		if (opc == TAG_DESCARGAS) {
			rutaDescargas = linea.substr(pos + 1);
			std::cout<<"Descargas: "<< rutaDescargas<<std::endl;
		}
		else {
			if (opc == TAG_PUERTO){
				std::stringstream buffer;
				buffer << linea.substr(pos + 1);
				buffer >> puerto;
				std::cout<<"Puerto: "<<puerto <<std::endl;
			}
			else {
				huboCambios = true;
			}
		}
	}
}

void Configuracion::guardarConfiguracion() {
	std::cout<< "guardando config"<<std::endl;
	crearArchivo();
	archivo << ".: FiTorrent :."<<std::endl;
	archivo << TAG_DESCARGAS << rutaDescargas << std::endl;
	archivo << TAG_PUERTO << puerto << std::endl;
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

std::string Configuracion::leerLinea() {
	std::string linea;
	bool fin;
	char aux;
	archivo.get(aux);
	fin = (aux == '\n');

	while (!fin) {
		linea += aux;
		archivo.get(aux);
		fin = (aux == '\n');
	}
	//archivo.putback(aux);
	return linea;
}
