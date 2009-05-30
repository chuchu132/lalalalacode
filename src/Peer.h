/*
 * Peer.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef PEER_H_
#define PEER_H_


/*
 * Cada Peer se ejecuta en un hilo aparte y se va a encargar de
 * pedir y enviar partes a otros Peers remotos.
 *
 * TODO tengo que leer mas aca no llegue.
 */
#include "Socket.h"
#include "Thread.h"
#include "Torrent.h"

class Torrent;
class Peer: public Thread {
public:
	Peer(Socket* peerRemoto,Torrent* torrent);
	virtual ~Peer();
	void* run();
private:
	Socket* peerRemoto;
	Torrent* torrent;
};

#endif /* PEER_H_ */
