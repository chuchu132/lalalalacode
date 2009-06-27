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
	bool unchoked=false;
	UINT cantUnchoked=0;
	if (!error)
		error = !sendBitfield();
	while(conexionEstaOK() && getTorrent()->estaActivo() && (!error)){
		int length;
		char* buffer;
		if (recvMsj(&buffer, length)) {
			error = (!procesar(buffer, length));
			cantUnchoked=this->getTorrent()->getPeersUnchoked();
			if (getPeer_interested() && getAm_choking()){
				if (cantUnchoked<3){
					sendMsg(ID_MSJ_UNCHOKE);
					this->getTorrent()->setPeersUnchoked(cantUnchoked+1);
					unchoked=true;
				}
				else
					sendMsg(ID_MSJ_CHOKE);
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
	if (unchoked)
		this->getTorrent()->setPeersUnchoked(this->getTorrent()->getPeersUnchoked()-1);
	return NULL;
}


