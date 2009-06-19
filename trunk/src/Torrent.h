/*
 * Torrent.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef TORRENT_H_
#define TORRENT_H_

#include <list>
#include <string>
#include "BencodeParser.h"
#include "ClienteTorrent.h"
#include "Controlador.h"
#include "FileManager.h"
#include "Mutex.h"
#include "Peer.h"
#include "Tracker.h"


/*******************************************************************
 * Torrent, contiene toda la informacion que viene en el archivo   *
 * .torrent, la cual es utilizada para comunicarse con el Tracker  *
 * y los Peers.                                                    *
 * *****************************************************************/

class Peer;
class Tracker;
class Controlador;
class ClienteTorrent;
class FileManager;

class Torrent {

public:

	Torrent(ClienteTorrent* clienteTorrent, std::string path);

	~Torrent();

	/*
	 * Inicializa los atributos de Torrent con la informacion que levanto el parser del
	 * archivo .torrent
	 * devuelve true en caso de exito
	 */
	bool inicializarTorrent(BencodeParser* parser);

	/*No confundir con el run de Threads, este metodo se llama
	 * una vez que el torrent esta inicializado, y pone a descargar/compartir el archivo
	 * TODO Implementar*/
	void run();

	bool conectarTracker(std::string url);

	/* Se envia un GET con la siguiente informacion:
	 * info_hash
	 * peer_id
	 * port
	 * uploaded
	 * downloaded
	 * left
	 * Si numwant < 0 envio Evento
	 *  event={EVENT_STARTED,EVENT_COMPLETED,EVENT_STOPPED}
	 * sino
	 *  envio numwant
	 */
	bool enviarEventoEstado(const char* event,int numwant);

	/*Cuando el Tracker recibe una lista de peers desde el Tracker remoto
	 * los agrega a la lista de peers del Torrent*/
	void agregarPeer(std::string ip,unsigned int puerto);

	void agregarPeer(Peer* peerNuevo);

	/* detiene el trafico del torrent */
	void detener();

	/* reanuda el trafico del torrent */
	void continuar();

	/* pausa el trafico del torrent */
	void pausar();

	/* refresca la lista de peers del torrent. Pide peers al Tracker */
	void refrescarPeers();

	/* Calcula cuantos bytes faltan descargar */
	unsigned int left();

	unsigned int* getInfoHash();

	std::string getHashString();

	std::string getPeerId();

	/* Devuelve el estado por ej: "detenido", "pausado", "conectando", "completo", etc */
	std::string getEstado();

	/* devuelve el tamaño del archivo en bytes */
	unsigned int getTamanio();

	/* devuelve el tamanio descargado en bytes*/
	unsigned int getTamanioDescargado();

	std::string getUrlTracker();

	int getVelocidadSubida();

	int getVelocidadBajada();

	/* devuelve el nombre del archivo .torrent */
	std::string getNombre();

	FileManager* getFileManager();

	std::list<Peer*>* getListaPeers();

	std::string getPath();

	unsigned int getCantPeers();

	void setControlador(Controlador* ctrl);

	bool estaActivo();

	/* transforma bytes en un string */
	std::string bytesToString(float bytes);

	std::list<Peer*>::iterator getIterPeers();
	std::list<Peer*>::iterator getEndIterPeers();
	int getCantidadMaximaPeers();

	void removerPeersInactivos();

	void detenerPeers();

private:

	ClienteTorrent* clienteTorrent;
	Tracker* tracker;
	Mutex llaveListaPeers;
	int cantidadMaximaPeers;
	std::list<Peer*> peers;
	FileManager fileManager;
	unsigned int info_hash[5];
	std::string nombre;
	std::string estado;
	std::string path; //ruta en la que esta guardado el archivo .torrent
	bool activo;// SE ESTA USANDO PARA ALGO???
	unsigned int port; // puerto donde esta escuchando el Cliente.
	unsigned int uploaded;
	unsigned int downloaded;

	Controlador *controlador;

	time_t horaInicial;

	time_t horaAnterior;
	unsigned int downAnterior;

};

#endif /* TORRENT_H_ */
