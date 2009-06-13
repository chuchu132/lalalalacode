/*
 * Configuracion.h
 *
 *  Created on: 12/06/2009
 *      Author: teddy
 */

#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include <iostream>
#include <fstream>
#include <sstream>

#include "Constantes.h"

class Configuracion {
	/* Guarda la configuracion del Cliente BitTorrent */

private:
	std::fstream archivo; //archivo de configuracion

	std::string rutaDescargas; //directorio en el que se guardan las descargas
	unsigned int puerto; //puerto en el que el cliente escucha conexiones entrantes

	bool huboCambios; //indica si hubo cambios en la configuracion

	void crearArchivo();

	/* guarda la configuracion actual en el archivo */
	void guardarConfiguracion();

	std::string leerLinea();
	std::string leerRuta();

public:

	Configuracion();
	~Configuracion();

	void guardarRutaDescargas(std::string);
	void guardarPuerto(unsigned int);

	std::string getRutaDescargas();

	unsigned int getPuerto();

	/* carga la configuracion desde el archivo */
	void cargarConfiguracion();

	/* carga la configuracion por default */
	void cargarConfiguracionDefault();
};

#endif /* CONFIGURACION_H_ */
