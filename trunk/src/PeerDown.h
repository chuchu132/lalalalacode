/*
 * PeerDown.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PEERDOWN_H_
#define PEERDOWN_H_

#include "Peer.h"

/*
 * El peer down es un peer cuya funcion principal es
 * pedir descargar. Si un peer del cual esta descargando
 * le pide una pieza que tiene, se la envia.
 */

class PeerDown: public Peer {
public:
	PeerDown(Socket* peerRemoto,Torrent* torrent);
	virtual ~PeerDown();
	/* En el run se implementa un bucle que se ocupa de recibir de a uno los mensajes
	 * que envia el peer remoto e ir procesandolos.
	 */
	void* run();
	/* En el procesar KeepAlive se ocupa de cerrar una conexion
	 * si se recibe X cantidad de keepalive seguidos, y ningun mensaje
	 * de utilidad para el peer local.
	 */
	virtual void procesarKeepAlive();
private:
	int cantidadKeepAlive;//lleva la cuenta de los keepalive que envia un peerdown sin recibir cambios del peer
};

#endif /* PEERDOWN_H_ */
