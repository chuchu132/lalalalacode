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
#include "Tipos.h"
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

	bool send(const char* stream,UINT size);

	void cerrarConexion();

	std::string getPath();

	std::string getUrl();

	/*
	 * Procesa la lista de peer que envia el tracker luego de un request
	 * los decodifica y los agrega en la lista del torrent.
	 */
	bool procesarResponse(std::string &buffer,int &longitud,int& posUltimoProcesado);

	UINT getMinInterval();

	void setTorrent(Torrent* unTorrent);

	void setRefresh(bool refresh);

private:
	Torrent* torrent;
	UINT minInterval;//Intervalo minimo para realizar pedidos al Tracker
	Socket trackerRemoto; // Conexion con el tracker remoto.
	std::string path;
	std::string url;
	UINT puerto; //puerto al que se conecta el tracker
	bool refresh;//flag determinando si se realiza un refresh de los peers

	/* Obtiene el bencode dentro del string buffer
	 * Parametros:
	 * buffer: string que posee el bencode correspondiente al response del tracker
	 * longitud: cantidad de caracteres leidos en el recive del tracker
	 * salida: string que se devuelve con el bencode
	 * posUltimoProcesado: indica la posicion dentro del buffer , del ultimo caracter procesado del bencode
	 * Retorna: true en caso de haber obtenido el bencode en su totalidad , false en caso contrario
	 */
	bool extraerBencode(std::string &buffer, int &longitud,std::string &salida,int &posUltimoProcesado);

	/*
	 * Retorna la longitud total del bencode dentro del "buffer"
	 * Retorna en marca la posicion dentro del buffer del comienzo del bencode
	 */
	int obtenerLongitudBencode (std::string &buffer,UINT &marca);

	/*
	 * Realiza la decodificacion de la cadena binaria de peers y los agrega a los PeerDown
	 */
	void decodificarPeers(char* cadena,UINT longitudCadena);
};

#endif /* TRACKER_H_ */
