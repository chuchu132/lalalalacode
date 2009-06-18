/*
 * PeerDown.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "PeerDown.h"

PeerDown::PeerDown(Socket* peerRemoto, Torrent* torrent) :
	Peer(peerRemoto, torrent) {
	setTipo('D');
	std::cout << "new PeerDown" << std::endl;
}

PeerDown::~PeerDown() {
	std::cout << "~PeerDown" << std::endl;
}

void* PeerDown::run() {
	std::cout << "run PeerDown" << std::endl;
	if (sendHandshake() && sendBitfield()) {
		unsigned int index = -1;
		int contadorCiclos = 0;
		bool error = recvHandshake(); // error puede ser en la conexion, en lo recibido o al procesar
		while (getTorrent()->estaActivo() && !error && conexionEstaOK()) {
			int length;
			char* buffer;
			if (recvMsj(&buffer, length)) {
				error = (!procesar(buffer, length));
			} else {
				error = true;
			}
			if (!error) {
				if (getAm_interested() == false && getPeer_choking() == false) {
					Bitmap mapaPeerRemoto = getBitmap();
					if (getTorrent()->getFileManager()->getPiezaAdescargar(
							index, mapaPeerRemoto)) {
						sendMsg(2); //Interested
						setAm_interested(true);
					}
				}
				if (getPeer_choking() == false && getAm_interested() == true) {
					sendRequest(index);
					setAm_interested(false);
				}

				contadorCiclos++;
				sleep(2);
				if (contadorCiclos == 30) {
					contadorCiclos = 0;
					sendKeepAlive();
				}
			}
		}
	}
	return NULL;
}
