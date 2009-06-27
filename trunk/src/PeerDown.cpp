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
	cantidadKeepAlive = 0;
}

PeerDown::~PeerDown() {
	join();
}

void* PeerDown::run() {
	if (sendHandshake() && sendBitfield()) {
		int contadorCiclos = 0;
		bool error = !recvHandshake(); // error puede ser en la conexion, en lo recibido o al procesar
		while (getTorrent()->estaActivo() && !error && conexionEstaOK()) {
			int length;
			char* buffer=NULL;
			if (recvMsj(&buffer, length)) {
				error = (!procesar(buffer, length));
				if (buffer!=NULL)
					delete[] buffer;
				if(getHuboCambios()){
					cantidadKeepAlive = 0;
				}
			} else {
				error = true;
			}

			if (!error) {
				if (getPeer_choking() == true && getAm_interested() == false ) {
					if (actualizarImInterested()){
						sendMsg(ID_MSJ_INTERESTED); //Interested
					}
				}
				if (getPeer_choking() == false && getAm_interested() == false) {

					actualizarImInterested();
				}

				contadorCiclos++;
				if (contadorCiclos == 3) {
					contadorCiclos = 0;
					sendKeepAlive();
				}
			}
		}
	}
	cerrarConexion(); // si la conexion esta cerrada el peer puede ser eliminado de la lista
	/*Si al peer que muere se le habia pedido una pieza y no la completo, se marca para volver a pedir*/
	if(tienePiezaPendiente()){
		getTorrent()->getFileManager()->cancelarPedido(getIdxPiezaPendiente());
	}
	return NULL;
}

void PeerDown::procesarKeepAlive(){
	cantidadKeepAlive++;
	if(cantidadKeepAlive > TIEMPO_LIMITE){
		cerrarConexion();
	}
}
