/*
 * ClienteTorrent.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef CLIENTETORRENT_H_
#define CLIENTETORRENT_H_

#include <list>
#include <iostream>

#include "Controlador.h"

/* ********************************************************************
 * TODO Completar Comentario                                          *
 * El cliente torrent es el encargado de agregar y sacar torrents.    *
 * El usuario accede a él a traves de la vista.                       *
 **********************************************************************/
class Torrent;
class Controlador;
class ClienteTorrent {
public:
	ClienteTorrent();
	virtual ~ClienteTorrent();

	//TODO Hay que implementar la interfaz que necesite Lucia.

private:
	/*Hay un Torrent por cada archivo .torrent que ingresa el usuario. */
	std::list<Torrent*> torrents;

	/*
	 * peer_id (string de 20 bytes) es un codigo que genera la aplicacion
	 * al iniciar y que debe identificar al ClienteTorrent univocamente
	 * por lo menos en la maquina local, es decir, no puede haber 2 ClienteTorrent
	 * corriendo en una pc con los mismos peers ids.
	 * TODO definir que algoritmo usar para generar el codigo.
	 * Opciones: usar Pocess ID, time stamp.
	 */
	char peer_id[20];

	Controlador *controlador;

public:

	/* recibe la ruta del archivo .torrent
	 * devuelve un puntero al objeto torrent o NULL en caso de error */
	Torrent* agregarTorrent(std::string ruta);

	/* borra el torrent indicado en el parametro
	 * ver si devuelve algo en caso de error */
	void borrarTorrent(Torrent*);

	std::string getPeerId();

	void setControlador(Controlador *ctrl);
};

#endif /* CLIENTETORRENT_H_ */
