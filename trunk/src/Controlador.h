/*
 * Controlador.h
 *
 *  Created on: 29/05/2009
 *      Author: teddy
 */

#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include "Ventana.h"
#include "ClienteTorrent.h"
#include "Torrent.h"

class Ventana;
class ClienteTorrent;
class Torrent;

class Controlador {


private:
	Ventana *ventana;
	ClienteTorrent &cliente;
public:

	Controlador(ClienteTorrent&);
	~Controlador();

	void setVentana(Ventana *ventana);

	//TODO ver que devuelven las funciones en caso de error

	/* borra un torrent del cliente */
	void borrarTorrent(Torrent *t);

	/* agrega un torrent al cliente */
	Torrent* agregarTorrent(std::string ruta);

	void detenerTorrent(Torrent*);

	void pausarTorrent(Torrent*);

	void continuarTorrent(Torrent*);

	/* actualiza el estado de todos los torrents en la vista */
	void actualizarTodos();

	/* actualiza el estado de un torrent en la vista */
	void actualizarEstado(Torrent*);

	/* muestra todos los torrents que tiene el cliente */
	void mostrarTorrents();

	/* muestra los torrents completos */
	void mostrarCompletos();

	/* muestra los torrents que se estan descargado o subiendo */
	void mostrarActivos();

	/* agrega el torrent en la vista */
	void agregarTorrentEnVista(Torrent*);

};

#endif /* CONTROLADOR_H_ */
