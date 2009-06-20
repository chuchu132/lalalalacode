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

	/*
	 * Cada vez que llega un mensaje desde el tracker remoto se lo
	 * procesa y luego continua recibiendo
	 */
	void procesar(std::string mensajeRecibido);

	/* Recibe una url por ejemplo: http://open.tracker.thepiratebay.org:80/announce
	 * De la url extrae la direccion del host, el puerto (sino usa el default) y
	 * el path.
	 */
	void inicilizar(std::string url);

	/*se conecta a la url seteada con inicializar*/
	bool connect();

	bool send(const char* stream,unsigned int size);

	void cerrarConexion();

	std::string getPath();

	std::string getUrl();

	//TODO tal vez va privado
	bool procesarResponse(std::string &buffer,int &longitud,int& posUltimoProcesado);

	bool extraerBencode(std::string &buffer, int &longitud,std::string &salida,int &posUltimoProcesado);

	int obtenerLongitudBencode (std::string &buffer,unsigned int &marca);

	void decodificarPeers(char* cadena,unsigned int longitudCadena);

	unsigned int getMinInterval();

	void setTorrent(Torrent* unTorrent);

private:
	Torrent* torrent;
	unsigned int minInterval;
	Socket trackerRemoto; // Conexion con el tracker remoto.
	std::string path;
	std::string url;
	unsigned int puerto;
};

#endif /* TRACKER_H_ */
