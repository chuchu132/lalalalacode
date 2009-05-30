/*
 * Tracker.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef TRACKER_H_
#define TRACKER_H_

#include <string>
#include "Socket.h"
#include "Thread.h"
#include "Torrent.h"

class Torrent;
class Tracker: public Thread{
public:
	Tracker();
	virtual ~Tracker();

	/*En el run el tracker va a estar escuchando al tracker remoto*/
	void* run(){return NULL;}; //TODO implementar

	/*
	 * Cada vez que llega un mensaje desde el tracker remoto se lo
	 * procesa y luego continua recibiendo
	 */
	void procesar(std::string mensajeRecibido);
	bool connect(std::string url,int port);
	bool send(const char* stream,unsigned int size);
private:
	Torrent* torrent;
	Socket trackerRemoto; // Conexion con el tracker remoto.
};

#endif /* TRACKER_H_ */
