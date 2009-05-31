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
	Bitmap* bitmap = this->torrent->getBitmap();
	char* map = NULL;
	int longitud;
	bitmap->getMap(map, &longitud);
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
bool Peer::sendPiece(int index, int begin) {
	Bitmap* bitmap = this->torrent->getBitmap();
	char* data = NULL;
	int longitud;
	bitmap->getBlock(index, begin, data, &longitud);
	int tamBuffer = (LEN_BASE_MSJ_PIECE + longitud + 1);
	char* buffer = new char[tamBuffer];

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_PIECE + longitud); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo  Id
	(*((int*) (buffer + OFFSET_ARG_1))) = index; // seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = begin; // seteo begin
	memcpy(buffer + OFFSET_ARG_3, data, longitud); //seteo block

	delete[] data;
	return (peerRemoto->send(buffer, tamBuffer) != ERROR);
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

