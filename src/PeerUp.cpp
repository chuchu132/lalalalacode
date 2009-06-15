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
}
/*
 * El ClienteTorrent recibe el handshake del peer remoto, de donde saca la informacion
 * necesaria para linkearlo con un Torrent determinado.
 */
void* PeerUp::run(){
//	sendHandshake();
//	sendBitfield();
//	while(conexionOK){
//		if(am_choking){
//			sendMsg(ID_MSJ_CHOKE);
//			esperarUnchoke.lock();
//			esperarUnchoke.unlock();
//			//TODO cuanto am_chiking est en true CPU se va a ir a 100%
//		}else{
//			sendMsg(ID_MSJ_UNCHOKE);
//
//
//			}
//
//		}
		return NULL;

//	}
}
