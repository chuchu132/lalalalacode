/*
 * Torrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Torrent.h"

Torrent::Torrent() {
	// TODO Auto-generated constructor stub

}

Torrent::~Torrent() {
	// TODO Auto-generated destructor stub
}

void* Torrent::run(){
	Socket* conexionPeerNuevo;
	while(activo){
		conexionPeerNuevo = peerListener.accept();
		if(peerNuevo != NULL){
			Peer* peerNuevo = new Peer(conexionPeerNuevo);
			if(peerNuevo != NULL){
			//TODO peerNuevo->run(); o algo asi
			llaveListaPeers.lock();
			peers.insert(peers.end(),peerNuevo);
			llaveListaPeers.unlock();
			}
		}
	}
}
