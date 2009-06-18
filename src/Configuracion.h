/*
 * Configuracion.h
 *
 *  Created on: 12/06/2009
 *      Author: Lucia
 */

#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "Constantes.h"

class Configuracion {
	/* Guarda la configuracion del Cliente BitTorrent */

private:
	std::fstream archivo; //archivo de configuracion

	std::string rutaDescargas; //directorio en el que se guardan las descargas
	unsigned int puerto; //puerto en el que el cliente escucha conexiones entrantes
	std::list<std::string> torrents; // lista con los torrents abiertos en la ultima ejecucion
									 // y su estado
	std::string estado;

	void crearArchivo();

	/* guarda la configuracion actual en el archivo */
	void guardarConfiguracion();

	std::string leerRuta();

	/* carga la configuracion desde el archivo */
	void cargarConfiguracion();



public:

	/* obtiene los datos desde el archivo de configuracion */
	Configuracion();

	/* guarda los datos en el archivo de configuracion */
	~Configuracion();

	void guardarRutaDescargas(std::string);

	void guardarPuerto(unsigned int);

	void guardarTorrent(std::string estado, std::string ruta);

	/* devuelve la ruta en la que se deben guardar los archivos descargados */
	std::string getRutaDescargas();

	unsigned int getPuerto();

	std::string obtenerTorrent();

	std::string getEstadoTorrent();

	bool hayTorrents();

	/* carga la configuracion por default */
	void cargarConfiguracionDefault();

};

#endif /* CONFIGURACION_H_ */
