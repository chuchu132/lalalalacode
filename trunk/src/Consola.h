/*
 * Consola.h
 *
 *  Created on: 24/06/2009
 *      Author: ale
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "Vista.h"

class Consola: public Vista {

public:
	Consola();
	virtual ~Consola();

	/* setea el controlador */
	void setControlador(Controlador *c);

	/* actualiza el estado del torrent */
	void actualizarEstado(Torrent*);

	/* agrega el torrent a la vista */
	void addTorrent(Torrent*);

	void mostrarNotificacion(std::string);

	/* abre la ventana principal */
	/* devuelve 0 en caso de exito y 1 en caso de error */
	int correr();

	/* devuelve cero si no se especifico ningun puerto */
	unsigned int getPuerto();

	std::string getRutaDescargas();

	void detener();


};

#endif /* CONSOLA_H_ */
