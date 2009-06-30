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
#include "Tipos.h"


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

	bool enviarEventoReAnnounce(int numwant);

	/* Cuando el Tracker recibe una lista de peers desde el Tracker remoto
	 * los agrega a la lista de peers del Torrent
	 * Estos peers son Peers Down.
	 */
	void agregarPeer(std::string ip,UINT puerto);

	/*
	 * Se usa para agregar los peers up, que son los
	 * que se conectan para descargar de nosotros
	 */
	bool agregarPeer(Peer* peerNuevo);

	/* detiene el trafico del torrent */
	void detener();

	/* reanuda el trafico del torrent */
	/* debe llamarse para activar al torrent */
	void continuar();

	/* refresca la lista de peers del torrent. Pide peers al Tracker */
	void refrescarPeers();

	/* Calcula cuantos bytes faltan descargar */
	ULINT left();

	/* Devuelve un puntero al hash del torrent*/
	UINT* getInfoHash();

	std::string getHashString();

	std::string getPeerId();

	/* Devuelve el estado por ej: "detenido", "pausado", "conectando", "completo", etc */
	std::string getEstado();

	/* devuelve el tamaño del archivo en bytes */
	ULINT getTamanio();

	/* devuelve el tamanio descargado en bytes*/
	ULINT getTamanioDescargado();

	ULINT getTamanioSubido();

	std::string getUrlTracker();

	int getVelocidadSubida();

	int getVelocidadBajada();

	/* devuelve el nombre del archivo .torrent */
	std::string getNombre();

	FileManager* getFileManager();

	std::list<Peer*>* getListaPeers();

	std::string getPath();

	UINT getCantPeers();

	time_t getTimeRefresh();

	time_t getTimeLastDown();

	Controlador* getControlador();

	UINT getPeersUnchoked ();

	UINT getCantidadMaximaPeers();

	std::string getTiempoRestante();

	std::list<Peer*>::iterator getIterPeers();
	std::list<Peer*>::iterator getEndIterPeers();


	void setControlador(Controlador* ctrl);

	void setDownloaded(ULINT bytes);

	void setBytesRecived(ULINT bytes);

	void setUploaded(ULINT bytes);

	void setEstado(std::string estado);

	void setTimeRefresh(time_t Trefresh);

	void setTimeLastDown(time_t horaActual);

	void setPeersUnchoked (UINT cantidad);


	bool estaCompleto();

	bool estaActivo();


	/* transforma bytes en un string con formato */
	std::string bytesToString(ULINT bytes);

	bool existePeerIP(std::string ip);

	void removerPeersInactivos(Peer* peerQueQueda);

	/* cierra las conexiones con todos los peers */
	void detenerPeers();

	/* se ejecuta al completarse la descarga */
	void descargaCompleta();

	/*Vacia el mapa de pedidos, y borra los peers cuyas conexiones estan caidas*/
	bool reiniciarPedidos(Peer* peerQueQueda);

private:

	/*
	 *  No confundir con el run de Threads, este metodo se llama
	 * una vez que el torrent esta inicializado, y pone a descargar/compartir el archivo
	 */
	/*para descargar hay que llamar al metodo continuar*/
	void run();


	ClienteTorrent* clienteTorrent;
	Tracker* tracker;
	Mutex llaveListaPeers;
	UINT cantidadMaximaPeers;
	std::list<Peer*> peers;
	FileManager fileManager;
	UINT info_hash[5];
	std::string nombre;
	std::string estado;
	std::string path; //ruta en la que esta guardado el archivo .torrent
	bool activo;
	UINT port; // puerto donde esta escuchando el Cliente.

	ULINT uploaded;//cantidad de bytes actuales subidos a los peer.
	ULINT uploadedAnt;//cantidad de bytes actuales subidos a los peer en un ciclo anterior.
	ULINT downloaded;//cantidad de bytes actuales grabados en disco sin problemas
	ULINT downAnterior;// cantidad de bytes grabados en un ciclo anterior

	ULINT bytesReceived;// bytes descargados por la red. Cuenta todo lo que llega sin verificar que haya sido grabado
	ULINT bytesReceivedPrev;// bytes descargados por la red, en un ciclo anterior al actual
	Mutex llaveCambiosDownloaded;

	float endGame; // porcentaje a partir del cual se vuelven a pedir todas las piezas restantes.

	Controlador *controlador;

	time_t horaInicial;//hora en que se realiza el refresh
	time_t horaAnterior;//hora anterior para el calculo de la velocidad de bajada.
	time_t horaAnteriorSubida;//hora anterior para el calculo de la velocidad de subida.

	int velocidadAnt;//Velocidad de bajada
	int velocidadAntUp;//Velocidad de subida
	time_t timeLastRefresh;//Tiempo del ultimo Refresh que se realizo
	time_t timeLastDownload;//Tiempo de la ultima vez q se descargo algun byte desde cualquier peer

	UINT peersUnchoked;//cantidad de peers que se encuentran en unchoked


};

#endif /* TORRENT_H_ */
