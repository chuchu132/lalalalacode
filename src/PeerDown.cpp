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

void* PeerDown::run() {
	sendHandshake();
	sendBitfield();
	bool error = true; // error puede ser en la conexion, en lo recibido o al procesar
	while (getTorrent()->estaActivo() &&  conexionEstaOK() ) {
		int length;
		char* buffer;
		if(recvMsj(&buffer,length)){
		procesar(buffer,length);

		}
		if (!error) {

		}
	}
	return NULL;
}
