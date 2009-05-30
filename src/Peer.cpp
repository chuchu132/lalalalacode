/*
 * Peer.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */
#include "Constantes.h"
#include "Peer.h"

Peer::Peer(Socket* peerRemoto,Torrent* torrent){
	this->peerRemoto = peerRemoto;
	this->torrent = torrent;
	run();
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
}

void* Peer::run(){
	char buffer[TAM_BUFFER];
	int cantidad;
	while(torrent->estaActivo()){
		cantidad = peerRemoto->receive(buffer, TAM_BUFFER);
		if(cantidad > 0){
			/*TODO ver como leer
			  tratar de procesar el mensaje
			*/
		}
		else{
			//TODO error marcar peer como desactivado para eliminarlo de la lista
		}
	}
}
