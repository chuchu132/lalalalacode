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
	FileManager* fileManager;
	bool error = false; // error puede ser en la conexion, en lo recibido o al procesar
	while (getTorrent()->estaActivo() &&  conexionEstaOK() ) {
		int length;
		char* buffer;
		if(recvMsj(&buffer,length)){
		procesar(buffer,length);
		}
		if (!error) {

			if ( getAm_interested()==false && getPeer_choking()==false ){
					fileManager = getTorrent()->getFileManager();
				    bitmapTorrent = fileManager->getBitmap();
				    Bitmap bitmapPeer= getBitmap();
			        resultadoFusion = bitmapTorrent.nuevoPorFusion (bitmapPeer);
					for(int i=0;i<resultadoFusion->getTamanioEnBytes();i++){
						if (resultadoFusion->estaMarcada(i)){
							index=i;
							break;
						}
					}
					if(index!=-1){
						sendMsg (2); //Interested
						setAm_interested(true);
					}
			}
			 if ( getPeer_choking()==false  && getAm_interested()==true) {
					for (int i=0; i<TAM_BLOQUES;i+= FIXED_LENGTH_REQUEST)
						sendRequest(index, i, FIXED_LENGTH_REQUEST);
					setAm_interested(false);
			}
		}
	}
	return NULL;
}
