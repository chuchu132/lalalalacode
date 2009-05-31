/*
 * Peer.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */
#include <ctime>
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
	int length;
	int cantidad;
	bool error = true; // error puede ser en la conexion, en lo recibido o al procesar
	while(torrent->estaActivo()){
		cantidad = peerRemoto->receive((char*)&length,sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
		if(cantidad > 0){
			char buffer[length];
			cantidad =  peerRemoto->receive(buffer,length);
			if(cantidad > 0){
				error = !procesar(buffer,length);
			}
		}
		if(error){
			//TODO error marcar peer como desactivado para eliminarlo de la lista
		}
	}
	return NULL;
}

bool Peer::procesar(const char* buffer,int length){
	return true;
	//TODO implementar
}
