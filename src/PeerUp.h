/*
 * PeerUp.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PEERUP_H_
#define PEERUP_H_

#include "Peer.h"

class PeerUp: public Peer {
public:
	PeerUp(Socket* peerRemoto,Torrent* torrent);
	virtual ~PeerUp();
	void* run();
};

#endif /* PEERUP_H_ */
