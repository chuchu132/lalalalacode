/*
 * Peer.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include <ctime>
#include <cstring>
#include "Constantes.h"
#include "Peer.h"
#include "ParserMensaje.h"

//void* Peer::run() {
//	int length;
//	int cantidad;
//	bool error = true; // error puede ser en la conexion, en lo recibido o al procesar
//	while (torrent->estaActivo()) {
//		cantidad = peerRemoto->receive((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
//		if (cantidad > 0) {
//			char buffer[length];
//			cantidad = peerRemoto->receive(buffer, length);
//			if (cantidad > 0) {
//				error = !procesar(buffer, length);
//			}
//		}
//		if (error) {
//			//TODO error marcar peer como desactivado para eliminarlo de la lista
//		}
//	}
//	return NULL;
//}





Peer::Peer(Socket* peerRemoto, Torrent* torrent) {
	this->peerRemoto = peerRemoto;
	this->torrent = torrent;
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
}


bool Peer::procesar(const char* buffer, int length) {
	return true;
	//TODO implementar
}

bool Peer::sendHandshake(){
	ParserMensaje parser;
	char buffer[LEN_HANDSHAKE];
	parser.crearHandshake(torrent->getInfoHash().c_str(),torrent->getPeerId().c_str(),buffer);
	return (peerRemoto->send(buffer,LEN_HANDSHAKE) != ERROR);
}

bool Peer::sendKeepAlive() {
	return (peerRemoto->send(LEN_MSJ_KEEPALIVE, sizeof(int)) != ERROR);
}

/*Formato mensaje: <len=0001><Id=id>*/
bool Peer::sendMsg(const char id) {
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_COMMON];
	parser.crearMensajeId(id,buffer);
	return (peerRemoto->send(buffer, FIXED_LENGTH_COMMON) != ERROR);
}
;

/*Formato mensaje Have: <len=0005><Id=4><piece index> */
bool Peer::sendHave(int index) {
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_HAVE];
	parser.crearMensajeHave(index,buffer);
	return (peerRemoto->send(buffer, FIXED_LENGTH_HAVE) != ERROR);
}
/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
bool Peer::sendBitfield() {
	ParserMensaje parser;
	FileManager* fileManager = this->torrent->getFileManager();
	char* map = NULL;
	int longitud;
	bool retorno;

	fileManager->getMap(map, &longitud);
	int tamBuffer = (LEN_BASE_MSJ_BITFIELD + longitud + 1);
	char* buffer = new char[tamBuffer];

	parser.crearMensajeBitfield(map,longitud,buffer);

	retorno = (peerRemoto->send(buffer, tamBuffer) != ERROR);
	delete[] map;
	delete[] buffer;
	return retorno;
}

/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
bool Peer::sendRequest(int index, int block, int length) {
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_REQUEST];
	parser.crearMensajeRequest(index,block,length,buffer);
	return (peerRemoto->send(buffer, FIXED_LENGTH_REQUEST) != ERROR);
}

/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
bool Peer::sendPiece(int index, int begin,int lenght) {
	FileManager* fileManager = this->torrent->getFileManager();
	char* data = NULL;

	if((data = fileManager->getBlock(index, begin, lenght)) != NULL){
		ParserMensaje parser;
		bool retorno;
		int tamBuffer = (LEN_BASE_MSJ_PIECE + lenght + 1);
		char* buffer = new char[tamBuffer];
		parser.crearMensajePiece(index,begin,lenght,data,buffer);
		retorno =  (peerRemoto->send(buffer, tamBuffer) != ERROR);
		delete[] data;
		delete[] buffer;
		return retorno;
	}
	else{
		//TODO error cerrar conexion/dar de baja
	}
	return false;
}
/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
bool Peer::sendCancel(int index, int block, int length) {
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_CANCEL];
	parser.crearMensajeCancel(index,block,length,buffer);
	return (peerRemoto->send(buffer, FIXED_LENGTH_CANCEL) != ERROR);
}

