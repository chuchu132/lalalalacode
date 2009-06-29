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
#include <string>
#include "Tipos.h"

#include "Constantes.h"

class Configuracion {
	/* Guarda la configuracion del Cliente BitTorrent en un archivo */

private:
	std::fstream archivo; //archivo de configuracion

	std::string rutaDescargas; //directorio en el que se guardan las descargas
	UINT puerto; //puerto en el que el cliente escucha conexiones entrantes
	std::list<std::string>  torrents; // lista con los torrents abiertos en la ultima ejecucion
									 // y su estado
	std::string estado;
	std::string rutaConfiguracion;//ruta en la que se encuentra el arch de config

	void crearArchivo();

	/* guarda la configuracion actual en el archivo */
	void guardarConfiguracion();

	/* lee una ruta entre <> desde archivo */
	std::string leerRuta();

	/* carga la configuracion desde el archivo */
	void cargarConfiguracion();

public:

	/* obtiene los datos desde el archivo de configuracion */
	Configuracion();

	/* guarda los datos en el archivo de configuracion */
	~Configuracion();

	void guardarRutaDescargas(std::string);

	void guardarPuerto(UINT);

	void guardarTorrent(std::string estado, std::string ruta);

	/* devuelve la ruta en la que se deben guardar los archivos descargados */
	std::string getRutaDescargas();

	/* devuelve el puerto en el que se deben escuchar las conexiones de peers entrantes */
	UINT getPuerto();

	/* obtiene la ruta de un torrent abierto en la sesion anterior */
	std::string obtenerTorrent();

	/* devuelve el estado del torrent abierto en la sesion anterior */
	std::string getEstadoTorrent();

	/* indica si todavia quedan torrents por agregar al cliente */
	bool hayTorrents();

	/* carga la configuracion por default */
	void cargarConfiguracionDefault();

};

#endif /* CONFIGURACION_H_ */
