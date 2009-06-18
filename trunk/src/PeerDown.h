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
};

#endif /* PEERDOWN_H_ */
