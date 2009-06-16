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
}

void* PeerDown::run() {
	sendHandshake();
	sendBitfield();
	int index=-1;
	Bitmap bitmapTorrent;
	Bitmap* resultadoFusion;
	bool error = false; // error puede ser en la conexion, en lo recibido o al procesar
	while (getTorrent()->estaActivo() &&  conexionEstaOK() ) {
		int length;
		char* buffer;
		if(recvMsj(&buffer,length)){
			error=!procesar(buffer,length);
		}
		else error=true;
		if (!error) {

			if ( getAm_interested()==false && getPeer_choking()==false ){
				    bitmapTorrent = getTorrent()->getFileManager()->getBitmap();
				    Bitmap bitmapPeer= getBitmap();
			        resultadoFusion = bitmapTorrent.nuevoPorFusion (bitmapPeer);
			        index=resultadoFusion->indexPiezaRandom();
					if(index!=-1){
						sendMsg (2); //Interested
						setAm_interested(true);
					}
			}
			 if ( getPeer_choking()==false  && getAm_interested()==true) {
					sendRequest(index);
					setAm_interested(false);
			}
		}
	}
	return NULL;
}

