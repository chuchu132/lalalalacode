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


class Peer: public Thread {
public:
	Peer();
	virtual ~Peer();
	void* run(){return NULL;};
};

#endif /* PEER_H_ */
