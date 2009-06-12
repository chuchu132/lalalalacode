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
	void* run();

	//TODO falta un setTorrent();

	/*
	 * Cada vez que llega un mensaje desde el tracker remoto se lo
	 * procesa y luego continua recibiendo
	 */
	void procesar(std::string mensajeRecibido);

	/* Recibe una url por ejemplo: http://open.tracker.thepiratebay.org/announce
	 * y luego de extraer el nombre del host y el path para el GET
	 * intenta conectarse
	 */
	bool connect(std::string url,int port);
	bool send(const char* stream,unsigned int size);
	std::string getPath();
	void setPath(std::string path);

private:
	Torrent* torrent;
	Socket trackerRemoto; // Conexion con el tracker remoto.
	std::string path;
};

#endif /* TRACKER_H_ */
