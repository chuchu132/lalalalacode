/*
 * PeerUp.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "PeerUp.h"

PeerUp::PeerUp(Socket* peerRemoto,Torrent* torrent):Peer(peerRemoto,torrent) {
	setTipo('U');
}

PeerUp::~PeerUp() {	}
/*
 * El ClienteTorrent recibe el handshake del peer remoto, de donde saca la informacion
 * necesaria para linkearlo con un Torrent determinado.
 */
void* PeerUp::run(){

	int contadorCiclos=0;
	bool error = !sendHandshake();
	if (!error)
		error = !sendBitfield();

	while(conexionEstaOK() && getTorrent()->estaActivo() && (!error)){
		int length;
		char* buffer;
		if (recvMsj(&buffer, length)) {
			error = (!procesar(buffer, length));
			if (getPeer_interested() && getAm_choking() ) {
					//ver en que caso lo unchokeo por ej tener un max de 3 peers up unchoked solamente
					sendMsg(ID_MSJ_UNCHOKE);
			}
			contadorCiclos++;
			if (contadorCiclos == 10) {
				contadorCiclos = 0;
				sendKeepAlive();
			}
		} else {
			error = true;
		}
		sleep(3);
	}
	return NULL;
}


