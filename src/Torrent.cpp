/*
 * Torrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include <cstdio>
#include "Torrent.h"

Torrent::Torrent() {
	// TODO Auto-generated constructor stub

}

Torrent::~Torrent() {
	// TODO Auto-generated destructor stub
}
bool Torrent::conectarTracker(std::string url,int port){
	if(tracker->connect(url,port)){
		//TODO ver si el mensaje no lo arma un "Parser" o va en otro metodo
		std::string envio;
		envio = "GET /?info_hash=";
		envio += info_hash;
		envio += "& peer_id=";
		envio += clienteTorrent->getPeerId();
		envio += "&port=";
		char temp[20];
		sprintf(temp,"%d",port);
		envio += temp;
		envio +="&uploaded=";
		sprintf(temp,"%d",uploaded);
		envio += temp;
		envio += "&downloaded=";
		sprintf(temp,"%d",downloaded);
		envio += temp;
		envio += "&left=";
		sprintf(temp,"%d",left());
		envio += temp;
		envio += "&event=started";
		return (tracker->send(envio.c_str(),envio.length()));
	}
	return false;
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
