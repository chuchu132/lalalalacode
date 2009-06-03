/*
 * PeerDown.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "PeerDown.h"

PeerDown::PeerDown(Socket* peerRemoto,Torrent* torrent):Peer(peerRemoto,torrent) {
	run();
}

PeerDown::~PeerDown() {
	// TODO Auto-generated destructor stub
}

void* PeerDown::run(){
	return NULL;
}
