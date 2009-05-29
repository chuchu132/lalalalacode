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
#include "Bitmap.h"
#include "ClienteTorrent.h"
#include "Peer.h"
#include "Tracker.h"
/******************************************************************
 * Torrent, contiene toda la informacion que viene en el archivo  *
 * .torrent, la cual es utilizada para comunicarse con el Tracker *
 * y los Peers.                                                    *
 * ****************************************************************/

/*ACA hay que poner un server que actue como peer para el resto del mundo*/

class Tracker;
class Torrent {
public:
	Torrent();
	/*
	 * TODO ver si se parsea el torrent y despues se crea con parametros o
	 * se pasa la ruta del archivo .torrent y se parsea adentro.(Podria haber un
	 * error al construir :S ).
	 */
	Torrent(std::string url);
	virtual ~Torrent();

	bool conectarTracker(std::string url,int port);
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
	bool enviarEventoEstado(const char* event=NULL,int numwant = 0);

	/*Calcula cuantos bytes faltan descargar*/
	int  left();

	/*Cuando el Tracker recibe una lista de peers desde el Tracker remoto
	 * los agrega a la lista de peers del Torrent*/
	void agregarPeer(std::string ip,int puerto);

private:

	ClienteTorrent* clienteTorrent;
	Tracker* tracker;
	std::list<Peer*> peers;
	Bitmap partes;
	std::string info_hash;
	int port; // puerto donde esta escuchando el Cliente.
	int uploaded;
	int downloaded;

};

#endif /* TORRENT_H_ */
