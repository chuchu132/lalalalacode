/*
 * ParserMensaje.cpp
 *
 *  Created on: 04/06/2009
 *      Author: ale
 */
#include <arpa/inet.h>
#include <sstream>
#include <string.h>
#include "Constantes.h"
#include "ParserMensaje.h"
#include "ParserCgi.h"
#include "Sha1.h"

ParserMensaje::ParserMensaje() {
}

ParserMensaje::~ParserMensaje() {
}

void ParserMensaje::crearHandshake(unsigned int* info_hash, const char* peer_id,
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

std::string ParserMensaje::crearGetBase(std::string path,unsigned int* info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left) {
	ParserCgi parserCgi;
	std::stringstream buffer;
	buffer << "GET /"<<path<<"?info_hash=" << parserCgi.codificar((char*)info_hash,LEN_SHA1)
	<< "&peer_id=" << parserCgi.codificar(peer_id.c_str(),LEN_SHA1) << "&port=" << port
	<< "&uploaded=" << uploaded << "&downloaded=" << downloaded
	<< "&left=" << left<<"&compact=1";
	return buffer.str();
}

std::string ParserMensaje::crearGetConEvento(std::string path,unsigned int* info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left,
		std::string event) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&event=" << event<<"\r\n\r\n";

	return buffer.str();
}

std::string ParserMensaje::crearGetConNumwant(std::string path,unsigned int* info_hash,
		std::string peer_id, int port, int uploaded, int downloaded, int left,
		int numwant) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&numwant=" << numwant<<"\r\n\r\n";
	return buffer.str();
}

std::string ParserMensaje::getHash(char* handshakeMsj) {
	Sha1 sha;
	int saltoHastaHash;
	char hashBinario[20];
	memcpy(&saltoHastaHash, handshakeMsj, sizeof(int));
	memcpy(hashBinario, (handshakeMsj + saltoHastaHash), LEN_SHA1);
	return sha.salidaAstring((unsigned int *)hashBinario);
}

char ParserMensaje::decodificarId(char* buffer){
	return buffer[OFFSET_ID_SIN_LEN];
}

void ParserMensaje::decodificarHave(char* buffer,int& index){
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(int));
	index = ntohl(temp);
}
void ParserMensaje::decodificarBitfield(char* buffer,int longitudbuffer, int& longitud,char** bitfield){
	*bitfield = (buffer + OFFSET_ARG_2_SIN_LEN);
	longitud = longitudbuffer - LEN_BASE_MSJ_BITFIELD;
}

void ParserMensaje::decodificarRequest(char* buffer,int& index,int& begin,int& length){
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(int));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(int));
	length = ntohl(temp);
}

void ParserMensaje::decodificarPiece(char* buffer,int longitudBuffer,int& index,int& begin,int& longitud,char** data){
	longitud = longitudBuffer - LEN_BASE_MSJ_PIECE;
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(int));
	begin = ntohl(temp);
	*data = (buffer + OFFSET_ARG_3_SIN_LEN);
}

void ParserMensaje::decodificarCancel(char* buffer,int& index,int& begin,int& length){
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(int));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(int));
	length = ntohl(temp);
}
