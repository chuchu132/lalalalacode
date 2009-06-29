/*
 * Controlador.h
 *
 *  Created on: 29/05/2009
 *      Author: Lucia
 */

#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include "Vista.h"
#include "ClienteTorrent.h"
#include "Torrent.h"
#include "Mutex.h"
#include "Tipos.h"
#include <list>

class Vista;
class ClienteTorrent;
class Torrent;
class Configuracion;

class Controlador {
	/* El controlador establece el lazo entre la vista y el modelo */

private:
	Vista *vista;
	ClienteTorrent &cliente;
	Mutex mutex_lista;
	std::list<Torrent*> actualizaciones; //lista de torrents a actualizar
public:

	Controlador(ClienteTorrent&);
	~Controlador();

	void setVentana(Vista *ventana);

	/* cierra el cliente
	 * es llamado al salir de la aplicacion */
	void cerrarCliente();

	/* borra un torrent del cliente */
	void borrarTorrent(Torrent *t);

	/* agrega un torrent al cliente */
	Torrent* agregarTorrent(std::string ruta);

	/* detiene el torrent */
	void detenerTorrent(Torrent*);


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

	/* guarda la configuracion establecida en la vista en el modelo */
	void guardarConfiguracion();

	/* indica si hay elementos en la lista de actualizaciones */
	bool hayCambios();

	/* obtiene el torrent a actualizar en la vista */
	Torrent* getCambio();

};

#endif /* CONTROLADOR_H_ */
