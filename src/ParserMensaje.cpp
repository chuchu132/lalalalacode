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

void ParserMensaje::crearMensajeHave(const unsigned int index, char* buffer) {
	(*((unsigned int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_HAVE); //seteo len
	buffer[OFFSET_LEN] = ID_MSJ_HAVE; //seteo el Id
	(*((unsigned int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo piece index
}

void ParserMensaje::crearMensajeBitfield(const char* map, const unsigned int longitud,
		char* buffer) {
	(*((unsigned int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_BITFIELD + longitud); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo el Id
	memcpy(buffer + OFFSET_ARG_1, map, longitud);
}

void ParserMensaje::crearMensajeRequest(unsigned int index, unsigned int block, unsigned int length,
		char* buffer) {
	(*((int*) buffer + OFFSET_LEN)) = htonl(LEN_MSJ_REQUEST); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_REQUEST; //seteo el Id
	(*((unsigned int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((unsigned int*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((unsigned int*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

void ParserMensaje::crearMensajePiece(unsigned int index, unsigned int begin, unsigned int lenght,
		char* data, char* buffer) {

	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_BASE_MSJ_PIECE + lenght); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_BITFIELD; //seteo  Id
	(*((unsigned int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((unsigned int*) (buffer + OFFSET_ARG_2))) = htonl(begin); //seteo block
	memcpy(buffer + OFFSET_ARG_3, data, lenght); //seteo block
}

void ParserMensaje::crearMensajeCancel(unsigned int index, unsigned int block, unsigned int length,
		char* buffer) {
	(*((int*) (buffer + OFFSET_LEN))) = htonl(LEN_MSJ_CANCEL); //seteo len
	buffer[OFFSET_ID] = ID_MSJ_CANCEL; //seteo  Id
	(*((unsigned int*) (buffer + OFFSET_ARG_1))) = htonl(index); //seteo index
	(*((unsigned int*) (buffer + OFFSET_ARG_2))) = htonl(block); //seteo block
	(*((unsigned int*) (buffer + OFFSET_ARG_3))) = htonl(length); //seteo length
}

std::string ParserMensaje::crearGetBase(std::string path,unsigned int* info_hash,
		std::string peer_id, unsigned int port, unsigned int uploaded, unsigned int downloaded, unsigned int left) {
	ParserCgi parserCgi;
	Sha1 sha;
	std::stringstream buffer;
	buffer << "GET /"<<path<<"?info_hash=" << sha.binAUrlEncode(info_hash)
	<< "&peer_id=" << parserCgi.codificar(peer_id.c_str(),LEN_SHA1) << "&port=" << port
	<< "&uploaded=" << uploaded << "&downloaded=" << downloaded
	<< "&left=" << left<<"&compact=1";
	return buffer.str();
}

std::string ParserMensaje::crearGetConEvento(std::string host,std::string path,unsigned int* info_hash,
		std::string peer_id, unsigned int port, unsigned int uploaded, unsigned int downloaded, unsigned int left,
		std::string event) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&event=" << event<<" HTTP/1.0\r\n"<<"Host: "<<host<<"\r\n\r\n";

	return buffer.str();
}

std::string ParserMensaje::crearGetConNumwant(std::string host,std::string path,unsigned int* info_hash,
		std::string peer_id, unsigned int port, unsigned int uploaded, unsigned int downloaded, unsigned int left,
		int numwant) {
	std::string base = crearGetBase(path,info_hash, peer_id, port, uploaded,
			downloaded, left);
	std::stringstream buffer;
	buffer << base << "&numwant="<< numwant<<"&event="<<EVENT_STARTED<<" HTTP/1.0\r\n"<<"Host: "<<host<<"\r\n\r\n";
	std::cout<<buffer.str()<<std::endl;
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

void ParserMensaje::decodificarHave(char* buffer,unsigned int& index){
	int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(unsigned int));
	index = ntohl(temp);
}
void ParserMensaje::decodificarBitfield(char* buffer,unsigned int longitudbuffer, unsigned int& longitud,char** bitfield){
	*bitfield = (buffer + OFFSET_ARG_2_SIN_LEN);
	longitud = longitudbuffer - LEN_BASE_MSJ_BITFIELD;
}

void ParserMensaje::decodificarRequest(char* buffer,unsigned int& index,unsigned int& begin,unsigned int& length){
	unsigned int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(unsigned int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(unsigned int));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(unsigned int));
	length = ntohl(temp);
}

void ParserMensaje::decodificarPiece(char* buffer,unsigned int longitudBuffer,unsigned int& index,unsigned int& begin,unsigned int& longitud,char** data){
	longitud = longitudBuffer - LEN_BASE_MSJ_PIECE;
	unsigned int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(unsigned int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(unsigned int));
	begin = ntohl(temp);
	*data = (buffer + OFFSET_ARG_3_SIN_LEN);
}

void ParserMensaje::decodificarCancel(char* buffer,unsigned int& index,unsigned int& begin,unsigned int& length){
	unsigned int temp;
	memcpy(&temp,buffer + OFFSET_ARG_1_SIN_LEN, sizeof(unsigned int));
	index = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_2_SIN_LEN, sizeof(unsigned int));
	begin = ntohl(temp);
	memcpy(&temp,buffer + OFFSET_ARG_3_SIN_LEN, sizeof(unsigned int));
	length = ntohl(temp);
}
