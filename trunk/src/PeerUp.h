/*
 * PeerUp.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PEERUP_H_
#define PEERUP_H_

#include "Peer.h"

/* El peer up es un peer que se ocupa de atender a los
 * pedidos del peer remoto.
 */

class PeerUp: public Peer {
public:
	PeerUp(Socket* peerRemoto,Torrent* torrent);
	virtual ~PeerUp();
	/* En el run se implementa un bucle que se ocupa de recibir de a uno los mensajes
	 * que envia el peer remoto e ir procesandolos.
	 */
	void* run();
};

#endif /* PEERUP_H_ */
