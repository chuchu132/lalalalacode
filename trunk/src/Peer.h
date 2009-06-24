/*
 * Peer.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef PEER_H_
#define PEER_H_


/*
 * CLASE ABSTRACTA
 *
 * Cada Peer se ejecuta en un hilo aparte y se va a encargar de
 * pedir y enviar partes a otros Peers remotos.
 */
#include "Mutex.h"
#include "Socket.h"
#include "Thread.h"
#include "Torrent.h"
#include "Tipos.h"
#include "AvisoDescargaCompleta.h"

class Torrent;
class Peer: public Thread {
public:
	Peer(Socket* peerRemoto,Torrent* torrent);
	virtual ~Peer();

	/*
	 * Handshake, es el primer mensaje que se envia al Peer remoto.
	 * Envia info_hash y peer_id
	 */
	bool sendHandshake();

	/********* Metodos que envian diferentes mensajes al Peer remoto *********
	 *                                                                       *
	 * Todos los mensajes entre peers empiezan con un entero de 4 bytes que  *
	 * indica la cantidad de bytes a leer a continuacion.                    *
	 * El siguiente campo es un numero de 1 byte que representa el tipo de   *
	 * mensaje que se va a procesar, del cual va a depender la cantidad de   *
	 * parametros a leer. El resto de los campos es variable.                *
	 * 																		 *
	 * ***********************************************************************/

	/*
	 * El mensaje keep-alive, contiene todos 0, y sirve para
	 * indicarle al peer remoto que tiene que mantener la conexion.
	 * Este mensaje debe ser enviado periodicamente en un intervalo
	 * no mayor a 2 minutos.
	 */
	bool sendKeepAlive();

	/*
	 * Formato mensaje: <len=0001><Id=id>
	 * El campo len se envia en formato Big Endian.
	 * Posibles ids: 0 - choke: Indica a un cliente que sus peticiones serán ignoradas
	 *               1 - unchoke: Derogación de lo dispuesto por el mensaje choke
	 *               2 - interested: Declaración del interés por alguna parte ofrecida por el cliente remoto
	 *               3 - not interested: Derogación de lo dispuesto por el mensaje interested
	 */
	bool sendMsg(const char id);

	/*
	 * Formato mensaje have: <len=0005><Id=4><piece index>
	 * have: Información a un Peer de que una parte ha sido completada
	 * El mensaje se envia solo si el peer remoto no tiene la pieza.
	 */
	bool sendHave(int index);
	/*
	 * Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield
	 * bitfield: Información de partes ya obtenidas. Cada bit del bitmap representa una pieza, si
	 * el bit esta en 0 indican piezas buscadas y si esta en 1 es una pieza completa.
	 */
	bool sendBitfield();
	/*
	 * Formato mensaje Request: <len=0013><Id=6><index><begin><length>
	 * request: Solicitud de un bloque
	 *   index: numero de pieza
	 *   begin: offset en bytes dentro de la pieza
	 *   length: longitud del bloque solicitado
	 */
	bool sendRequest(UINT  index);

	/*
	 * Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block
	 * piece: Transmisión de un bloque
	 *   index: numero de pieza
	 *   begin:offset en bytes dentro de la pieza
	 *   block: el bloque con los datos, que es una porcion de la pieza solicitada.
	 */
	bool sendPiece(int index,int begin,int length);
	/*
	 * Formato mensaje Piece: <len=0013><Id=8><index><begin><length>
	 * cancel: Petición de cancelación de transmisión de un bloque
	 *   index: numero de pieza
	 *   begin: offset en bytes dentro de la pieza
	 *   length: longitud del bloque solicitado y que quiero cancelar
	 */
	bool sendCancel(int index,int block,int length);


	/*
	 * Cuando llega un mensaje desde el peer remoto, se lo guarda en un
	 * buffer que luego se manda a procesar.
	 * */
	bool procesar(char* buffer,int length);

	/* Recibe un mensaje desde el peer remoto.
	 * en el buffer guarda el mensaje
	 * y en length su longitud.
	 * En caso de devolver true,el buffer debe ser liberado.
	 * */
	bool recvMsj(char** buffer,int& length);

	/*Recibe el handshake del peer remoto y verifica los hash*/
	bool recvHandshake() ;

	bool conexionEstaOK();

	Torrent* getTorrent();

	Bitmap  getBitmap();

	bool getAm_interested();

	bool getAm_choking();

	void setAm_interested(bool estado);

	bool getPeer_choking();

	void setPeer_choking(bool estado);

	bool getPeer_interested();

	void setPeer_interested(bool estado);

	char getTipo();

	void setTipo(char estado);

	std::string getIp();

	UINT getPuerto();

	void cerrarConexion();

	bool actualizarImInterested();

	bool tienePiezaPendiente();

	void setEstadoPiezaPendiente(bool estado);

	void setIdxPiezaPendiente(UINT index);

	UINT getIdxPiezaPendiente();

	bool getHuboCambios();

	virtual void procesarKeepAlive(){};

private:
	/*
	 * La llave de envio se bloquea antes de cualquier envio al peer remoto
	 * y se desbloquea inmediatamente despues de completarse el envio.
	 */
	Mutex llaveEnvio;
	Socket* peerRemoto;
	bool conexionOK;
	Torrent* torrent;

	/*
	 * Esta en true cuando se le solicito una pieza al peer remoto y todavia
	 * no completo la entrega.
	 */
	bool piezaPendiente;
	UINT idxPiezaPendiente;

	char tipo;
	bool am_choking;
	bool am_interested;
	bool peer_choking;
	bool peer_interested;
	bool huboCambios;
	Bitmap bitmap;

	/*Metodos utilizados para procesar los mensajes que llegan desde el peer remoto*/
	/*Marca en el Bitmap la pieza indicada por index*/
	virtual void procesarHave(int index);

	/*Guarda en el Bitmap el Bitfield recibido*/
	void procesarBitfield(const char* bitfield, int length );

	/*Busca y envia el bloque indicado por index y begin*/
	void procesarRequest(int index,int begin,int length);

	/*Guarda en disco el bloque de datos recibidos en la posicion correspondiente*/
	void procesarPiece(int index,int begin,int longitud,char* data );

	/*TODO no se q hace*/
	void procesarCancel(int index,int begin,int length);

	/*Envia un mensaje de Have a los peers que no tienen la pieza indicada por index*/
	void repartirHave(int index);


};

#endif /* PEER_H_ */
