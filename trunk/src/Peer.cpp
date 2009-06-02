/*
 * Peer.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */
#include <arpa/inet.h>
#include <ctime>
#include <cstring>
#include "Constantes.h"
#include "Peer.h"

Peer::Peer(Socket* peerRemoto, Torrent* torrent) {
	this->peerRemoto = peerRemoto;
	this->torrent = torrent;
	run();
}

Peer::~Peer() {
	// TODO Auto-generated destructor stub
}

void* Peer::run() {
	int length;
	int cantidad;
	bool error = true; // error puede ser en la conexion, en lo recibido o al procesar
	while (torrent->estaActivo()) {
		cantidad = peerRemoto->receive((char*) &length, sizeof(int)); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
		if (cantidad > 0) {
			char buffer[length];
			cantidad = peerRemoto->receive(buffer, length);
			if (cantidad > 0) {
				error = !procesar(buffer, length);
			}
		}
		if (error) {
			//TODO error marcar peer como desactivado para eliminarlo de la lista
		}
	}
	return NULL;
}

bool Peer::procesar(const char* buffer, int length) {
	return true;
	//TODO implementar
}

bool Peer::sendHandshake(){
	char buffer[LEN_HANDSHAKE];
	buffer[OFFSET_PSTRLEN]= PSTRLEN;
	memcpy((buffer + OFFSET_PROTOCOL),PROTOCOL,PSTRLEN);
	memset((buffer + OFFSET_RESERVED),0,LEN_RESERVED);
	memcpy((buffer + OFFSET_INFO_HASH),torrent->getInfoHash().c_str(),LEN_SHA1);
	memcpy((buffer + OFFSET_PEER_ID),torrent->getPeerId().c_str(),LEN_SHA1);
	return (peerRemoto->send(buffer,LEN_HANDSHAKE) != ERROR);
}

bool Peer::sendKeepAlive() {
	return (peerRemoto->send(LEN_MSJ_KEEPALIVE, sizeof(int)) != ERROR);
}

/*Formato mensaje: <len=0001><Id=id>*/
bool Peer::sendMsg(const char id) {
	char buffer[FIXED_LENGTH_COMMON];

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_COMMON); //seteo la longitud del resto del msj
	buffer[OFFSET_ID] = id; //seteo id del msj

	return (peerRemoto->send(buffer, FIXED_LENGTH_COMMON) != ERROR);
}
;

/*Formato mensaje Have: <len=0005><Id=4><piece index> */
bool Peer::sendHave(int index) {
	char buffer[FIXED_LENGTH_HAVE];

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_HAVE); //seteo len
	buffer[OFFSET_LEN] = ID_MSJ_HAVE; //seteo el Id
	(*((int*) (buffer + OFFSET_ARG_1))) = index; //seteo piece index

	return (peerRemoto->send(buffer, FIXED_LENGTH_HAVE) != ERROR);
}
/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
bool Peer::sendBitfield() {
	FileManager* fileManager = this->torrent->getFileManager();
	char* map = NULL;
	int longitud;
	fileManager->getMap(map, &longitud);
	int tamBuffer = (LEN_BASE_MSJ_BITFIELD + longitud + 1);
	char* buffer = new char[tamBuffer];

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_BITFIELD + longitud); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo el Id
	memcpy(buffer + OFFSET_ARG_1, map, longitud);
	delete[] map;
	return (peerRemoto->send(buffer, tamBuffer) != ERROR);
}

/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
bool Peer::sendRequest(int index, int block, int length) {
	char buffer[FIXED_LENGTH_REQUEST];

	(*((int*) buffer + OFFSET_LEN)) = htonl(LEN_MSJ_REQUEST); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_REQUEST; //seteo el Id
	(*((int*) (buffer + OFFSET_ARG_1))) = index; //seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = block; //seteo block
	(*((int*) (buffer + OFFSET_ARG_3))) = length; //seteo length

	return (peerRemoto->send(buffer, FIXED_LENGTH_REQUEST) != ERROR);
}

/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
bool Peer::sendPiece(int index, int begin,int lenght) {
	FileManager* fileManager = this->torrent->getFileManager();
	char* data = NULL;

	if((data = fileManager->getBlock(index, begin, lenght)) != NULL){

		int tamBuffer = (LEN_BASE_MSJ_PIECE + lenght + 1);
		char* buffer = new char[tamBuffer];

		(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_PIECE + lenght); //seteo len
		buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo  Id
		(*((int*) (buffer + OFFSET_ARG_1))) = index; // seteo index
		(*((int*) (buffer + OFFSET_ARG_2))) = begin; // seteo begin
		memcpy(buffer + OFFSET_ARG_3, data, lenght); //seteo block

		delete[] data;
		delete[] buffer;
		return (peerRemoto->send(buffer, tamBuffer) != ERROR);
	}
	else{
		//TODO error cerrar conexion/dar de baja

	}
	return false;
}
/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
bool Peer::sendCancel(int index, int block, int length) {
	char buffer[FIXED_LENGTH_CANCEL];

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_CANCEL); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_CANCEL; //seteo  Id
	(*((int*) (buffer + OFFSET_ARG_1))) = index; // seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = block; // seteo block
	(*((int*) (buffer + OFFSET_ARG_3))) = length; // seteo length

	return (peerRemoto->send(buffer, FIXED_LENGTH_CANCEL) != ERROR);
}

