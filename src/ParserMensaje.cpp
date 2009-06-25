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

void ParserMensaje::crearHandshake(UINT* info_hash, const char* peer_id,
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

void ParserMensaje::crearMensajeHave(const UINT index, char* buffer) {
	(*((UINT*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_HAVE); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_HAVE; //seteo el Id
	(*((UINT*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo piece index
}

void ParserMensaje::crearMensajeBitfield(const char* map, const UINT longitud,
		char* buffer) {
	(*((UINT*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_BITFIELD + longitud); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo el Id
	memcpy(buffer + OFFSET_ARG_1, map, longitud);
}

void ParserMensaje::crearMensajeRequest(UINT index, UINT block, UINT length,
		char* buffer) {
	(*((int*) buffer + OFFSET_LEN)) = htonl(LEN_MSJ_REQUEST); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_REQUEST; //seteo el Id
	(*((UINT*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((UINT*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((UINT*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

void ParserMensaje::crearMensajePiece(UINT index, UINT begin, UINT lenght,
		char* data, char* buffer) {

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_PIECE + lenght); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo  Id
	(*((UINT*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((UINT*) (buffer + OFFSET_ARG_2))) = htonl(begin); //seteo block
	memcpy(buffer + OFFSET_ARG_3, data, lenght); //seteo block
}

void ParserMensaje::crearMensajeCancel(UINT index, UINT block, UINT length,
		char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_CANCEL); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_CANCEL; //seteo  Id
	(*((UINT*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((UINT*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((UINT*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

std::string ParserMensaje::crearGetBase(std::string path,UINT* info_hash,
		std::string peer_id, UINT port, UINT uploaded, UINT downloaded, UINT left) {
	ParserCgi parserCgi;
	Sha1 sha;
	std::stringstream buffer;
	buffer << "GET /"<<path<<"?info_hash=" << sha.binAUrlEncode(info_hash)
	<< "&peer_id=" << parserCgi.codificar(peer_id.c_str(),LEN_SHA1) << "&port=" << port
	<< "&uploaded=" << uploaded << "&downloaded=" << downloaded
	<< "&left=" << left<<"&compact=1";
	return buffer.str();
}

std::string ParserMensaje::crearGetConEvento(std::string host,std::string path,UINT* info_hash,
		std::string peer_id, UINT port, UINT uploaded, UINT downloaded, UINT left,
		std::string event) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&event=" << event<<" HTTP/1.0\r\n"<<"Host: "<<host<<"\r\n\r\n";

	return buffer.str();
}

std::string ParserMensaje::crearGetConNumwant(std::string host,std::string path,UINT* info_hash,
		std::string peer_id, UINT port, UINT uploaded, UINT downloaded, UINT left,
		int numwant) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&numwant="<< numwant<<"&event="<<EVENT_STARTED<<" HTTP/1.0\r\n"<<"Host: "<<host<<"\r\n\r\n";
	return buffer.str();
}

std::string ParserMensaje::crearGetReAnnounce(std::string host,std::string path,UINT* info_hash,
		std::string peer_id, UINT port, UINT uploaded, UINT downloaded, UINT left,
		int numwant) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&numwant="<< numwant<<" HTTP/1.0\r\n"<<"Host: "<<host<<"\r\n\r\n";
	return buffer.str();
}

std::string ParserMensaje::getHash(char* handshakeMsj,int longProto) {
	Sha1 sha;
	int saltoHastaHash;
	char hashBinario[20];
	saltoHastaHash = (longProto + LEN_RESERVED);
	memcpy(hashBinario, (handshakeMsj + saltoHastaHash), LEN_SHA1);
	return sha.salidaAstring((UINT *)hashBinario);
}

char ParserMensaje::decodificarId(char* buffer){
	return buffer[OFFSET_ID_SIN_LEN];
}

void ParserMensaje::decodificarHave(char* buffer,UINT& index){
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(UINT));
	index = ntohl(temp);
}
void ParserMensaje::decodificarBitfield(char* buffer,UINT longitudbuffer, UINT& longitud,char** bitfield){
	*bitfield = (buffer + OFFSET_ARG_2_SIN_LEN - 1);
	longitud = longitudbuffer - LEN_BASE_MSJ_BITFIELD;
}

void ParserMensaje::decodificarRequest(char* buffer,UINT& index,UINT& begin,UINT& length){
	UINT temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(UINT));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(UINT));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(UINT));
	length = ntohl(temp);
}

void ParserMensaje::decodificarPiece(char* buffer,UINT longitudBuffer,UINT& index,UINT& begin,UINT& longitud,char** data){
	longitud = longitudBuffer - LEN_BASE_MSJ_PIECE;
	UINT temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(UINT));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(UINT));
	begin = ntohl(temp);
	*data = (buffer + OFFSET_ARG_3_SIN_LEN);
}

void ParserMensaje::decodificarCancel(char* buffer,UINT& index,UINT& begin,UINT& length){
	UINT temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(UINT));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(UINT));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(UINT));
	length = ntohl(temp);
}
