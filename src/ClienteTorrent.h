/*
 * ClienteTorrent.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef CLIENTETORRENT_H_
#define CLIENTETORRENT_H_

#include <list>

/* ********************************************************************
 * TODO Completar Comentario                                          *
 * El cliente torrent es el encargado de agregar y sacar torrents.    *
 * El usuario accede a él a traves de la vista.                       *
 **********************************************************************/
class Torrent;
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
};

#endif /* CLIENTETORRENT_H_ */
