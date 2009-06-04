/*
 * ParserMensaje.cpp
 *
 *  Created on: 04/06/2009
 *      Author: ale
 */
#include <arpa/inet.h>
#include <sstream>
#include "Constantes.h"
#include "ParserMensaje.h"
#include "ParserCgi.h"
#include "Sha1.h"

ParserMensaje::ParserMensaje() {
	// TODO Auto-generated constructor stub

}

ParserMensaje::~ParserMensaje() {
	// TODO Auto-generated destructor stub
}

void ParserMensaje::crearHandshake(const char* info_hash, const char* peer_id,
		char* buffer) {
	buffer[OFFSET_PSTRLEN] = PSTRLEN;
	memcpy((buffer + OFFSET_PROTOCOL), PROTOCOL, PSTRLEN);
	memset((buffer + OFFSET_RESERVED), 0, LEN_RESERVED);
	memcpy((buffer + OFFSET_INFO_HASH), info_hash, LEN_SHA1);
	memcpy((buffer + OFFSET_PEER_ID), peer_id, LEN_SHA1);
}

void ParserMensaje::crearMensajeId(const char id, char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_COMMON); //seteo la longitud del resto del msj
	buffer[OFFSET_ID] = id; //seteo id del msj
}

void ParserMensaje::crearMensajeHave(const int index, char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_HAVE); //seteo len
	buffer[OFFSET_LEN] = ID_MSJ_HAVE; //seteo el Id
	(*((int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo piece index
}

void ParserMensaje::crearMensajeBitfield(const char* map, const int longitud,
		char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_BITFIELD + longitud); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo el Id
	memcpy(buffer + OFFSET_ARG_1, map, longitud);
}

void ParserMensaje::crearMensajeRequest(int index, int block, int length,
		char* buffer) {
	(*((int*) buffer + OFFSET_LEN)) = htonl(LEN_MSJ_REQUEST); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_REQUEST; //seteo el Id
	(*((int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((int*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

void ParserMensaje::crearMensajePiece(int index, int begin, int lenght,
		char* data, char* buffer) {

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_PIECE + lenght); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo  Id
	(*((int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = htonl(begin); //seteo block
	memcpy(buffer + OFFSET_ARG_3, data, lenght); //seteo block
}

void ParserMensaje::crearMensajeCancel(int index, int block, int length,
		char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_CANCEL); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_CANCEL; //seteo  Id
	(*((int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((int*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((int*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

std::string ParserMensaje::crearGetBase(std::string info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left) {
	ParserCgi parserCgi;
	std::stringstream buffer;
	buffer << "GET /?info_hash=" << parserCgi.codificar(info_hash)
			<< "& peer_id=" << parserCgi.codificar(peer_id) << "&port=" << port
			<< "&uploaded=" << uploaded << "&downloaded=" << downloaded
			<< "&left=" << left;
	return buffer.str();
}

std::string ParserMensaje::crearGetConEvento(std::string info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left,
		std::string event) {
	std::string base = crearGetBase(info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&event=" << event;
	return buffer.str();
}

std::string ParserMensaje::crearGetConNumwant(std::string info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left,
		int numwant) {
	std::string base = crearGetBase(info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&numwant=" << numwant;
	return buffer.str();
}

std::string ParserMensaje::getHash(char* handshakeMsj) {
	int saltoHastaHash;
	char hashBinario[20];
	memcpy(&saltoHastaHash, handshakeMsj, sizeof(int));
	memcpy(hashBinario, (handshakeMsj + saltoHastaHash), LEN_SHA1);
	return Sha1::hashAstring(hashBinario);
}
