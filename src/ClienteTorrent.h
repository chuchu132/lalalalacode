/*
 * ClienteTorrent.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef CLIENTETORRENT_H_
#define CLIENTETORRENT_H_

#include <iostream>
#include <list>
#include "Configuracion.h"
#include "Controlador.h"
#include "Socket.h"
#include "Thread.h"


/* ********************************************************************
 * El cliente torrent es el encargado de agregar y sacar torrents.    *
 * El usuario accede a él a traves de la vista.                       *
 **********************************************************************/
class Torrent;
class Controlador;
class FileManager;

class ClienteTorrent : public Thread {

private:
	/*Hay un Torrent por cada archivo .torrent que ingresa el usuario. */
	std::list<Torrent*> torrents;

	/*
	 * peer_id (string de 20 bytes) es un codigo que genera la aplicacion
	 * al iniciar y que debe identificar al ClienteTorrent univocamente
	 * por lo menos en la maquina local, es decir, no puede haber 2 ClienteTorrent
	 * corriendo en una pc con los mismos peers ids.
	 */
	char peer_id[20];
	Socket peerListener; //socket para escuchar conexiones de peers
	unsigned int puerto; //puerto en el que escucha conexiones
	bool activo; //sirve para dejar de escuchar conexiones
	Controlador *controlador;
	Configuracion config;

public:

	ClienteTorrent();

	~ClienteTorrent();

	/* recibe la ruta del archivo .torrent
	 * devuelve un puntero al objeto torrent o NULL en caso de error */
	Torrent* agregarTorrent(std::string ruta);

	/* borra el torrent indicado en el parametro
	 * ver si devuelve algo en caso de error */
	void borrarTorrent(Torrent*);

	/*
	 * El ClienteTorrent recibe conexiones de Peers remotos y los
	 * linkea con el Torrent correspondiente.
	 */
	void* run();

	/* Realiza todo lo necesario para cerrar el programa
	 * por ej: cerrar todas las conexiones y guardar info sobre los torrents
	 * se llama al cerrar la ventana*/
	void finalizar();

	/*
	 *  Devuelve el Torrent asociado al hash pasado por parametro o NULL si no lo
	 * encuentra
	 */
	Torrent* buscarTorrent(std::string  hashTorrent);

	bool estaActivo();

	std::string getPeerId();

	UINT getPuerto();

	void setControlador(Controlador *ctrl);

	Configuracion* getConfiguracion();

	std::list<Torrent*> * getListaTorrents();

	/*Crea las carpetas que necesita el programa.*/
	void inicializarDirectorios();

};

#endif /* CLIENTETORRENT_H_ */
