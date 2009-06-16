/*
 * Controlador.h
 *
 *  Created on: 29/05/2009
 *      Author: Lucia
 */

#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include "Ventana.h"
#include "ClienteTorrent.h"
#include "Torrent.h"

class Ventana;
class ClienteTorrent;
class Torrent;
class Configuracion;

class Controlador {


private:
	Ventana *ventana;
	ClienteTorrent &cliente;
public:

	Controlador(ClienteTorrent&);
	~Controlador();

	void setVentana(Ventana *ventana);

	/* cierra el cliente
	 * es llamado al salir de la aplicacion */
	void cerrarCliente();

	/* borra un torrent del cliente */
	void borrarTorrent(Torrent *t);

	/* agrega un torrent al cliente */
	Torrent* agregarTorrent(std::string ruta);

	void detenerTorrent(Torrent*);

	void pausarTorrent(Torrent*);

	void continuarTorrent(Torrent*);

	/* refresca la lista de peers del torrent */
	void refrescarPeers(Torrent*);

	/* actualiza el estado de un torrent en la vista */
	void actualizarEstado(Torrent*);

	/* agrega todos los torrents del cliente en la vista */
	void agregarTorrentsEnVista();

	/* muestra un mensaje en la pestaña de notificaciones */
	void notificarVista(std::string);

	/* inicia la vista */
	int correrVista();

	void guardarConfiguracion();

};

#endif /* CONTROLADOR_H_ */
