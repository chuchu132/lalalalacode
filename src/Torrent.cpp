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


std::string Torrent::getNombre()
{
	return nombre;
}

void* Torrent::run(){
	Socket* conexionPeerNuevo;
	while(activo){
		conexionPeerNuevo = peerListener.accept();
		if(conexionPeerNuevo != NULL){
			Peer* peerNuevo = new Peer(conexionPeerNuevo,this);
			if(peerNuevo != NULL){
			//TODO peerNuevo->run(); o algo asi
			llaveListaPeers.lock();
			peers.insert(peers.end(),peerNuevo);
			llaveListaPeers.unlock();
			}
		}
	}
	return NULL;
}
