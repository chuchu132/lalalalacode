/*
 * PeerDown.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PEERDOWN_H_
#define PEERDOWN_H_

#include "Peer.h"

class PeerDown: public Peer {
public:
	PeerDown(Socket* peerRemoto,Torrent* torrent);
	virtual ~PeerDown();
	void* run();

	bool getPiezaPedida ();

	void setPiezaPedida (bool estado);

	int  getIndexPiezaPedida();

	void setIndexPiezaPedida(int indice);

private:

	bool piezaPedida;
	int  indexPiezaPedida;
};

#endif /* PEERDOWN_H_ */
