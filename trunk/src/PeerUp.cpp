/*
 * PeerUp.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "PeerUp.h"

PeerUp::PeerUp(Socket* peerRemoto,Torrent* torrent):Peer(peerRemoto,torrent) {
	run();

}

PeerUp::~PeerUp() {
	// TODO Auto-generated destructor stub
}

void* PeerUp::run(){

		return NULL;

}
