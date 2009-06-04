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
/*
 * El ClienteTorrent recibe el handshake del peer remoto, de donde saca la informacion
 * necesaria para linkearlo con un Torrent determinado.
 */
void* PeerUp::run(){
	sendHandshake();
	//TODO Poner a recibir / procesar los mensajes entrantes
		return NULL;

}
