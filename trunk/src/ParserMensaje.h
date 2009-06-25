/*
 * ParserMensaje.h
 *
 *  Created on: 04/06/2009
 *      Author: ale
 */

#ifndef PARSERMENSAJE_H_
#define PARSERMENSAJE_H_

#include "Tipos.h"
/*
 * Esta clase se encarga de codificar y decodificar los mensajes que
 * se mandan el peer local y el peer remoto.
 * Tambien codifica los mensajes entre el Tracker local y el remoto
 */

class ParserMensaje {
public:
	ParserMensaje();
	virtual ~ParserMensaje();

	/*Decofica el Id del mensaje*/
	char decodificarId(char* buffer);

	/*Formato mensaje: <pstrlen><pstr><reserved><info_hash><peer_id>*/
	void crearHandshake(UINT* info_hash,const char* peer_id,char* buffer);
	std::string getHash(char* handshakeMsj,int longProto);

	/*Formato mensaje: <len=0001><Id=id>*/
	void crearMensajeId(const char id,char* buffer);

	/*Formato mensaje Have: <len=0005><Id=4><piece index> */
	void crearMensajeHave(const UINT index, char* buffer);
	void decodificarHave(char* buffer,UINT& index);

	/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
	void crearMensajeBitfield(const char* map,const UINT longitud,char* buffer);
	void decodificarBitfield(char* buffer,UINT longitudBuffer, UINT& longitud,char** bitfield);

	/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
	void crearMensajeRequest(UINT index, UINT block, UINT length,char* buffer);
	void decodificarRequest(char* buffer,UINT& index,UINT& begin,UINT& length);

	/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
	void crearMensajePiece(UINT index, UINT begin,UINT lenght,char* data,char* buffer);
	void decodificarPiece(char* buffer,UINT longitudBuffer,UINT& index,UINT& begin,UINT& longitud,char** data);

	/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
	void crearMensajeCancel(UINT index, UINT block, UINT length,char* buffer);
	void decodificarCancel(char* buffer,UINT& index,UINT& begin,UINT& length);


	/*Mensajes en formato Http */
	std::string crearGetBase(std::string path,UINT* info_hash,std::string peer_id,UINT port,UINT uploaded,UINT downloaded,UINT left);

	std::string crearGetConEvento(std::string host,std::string path,UINT* info_hash,std::string peer_id,UINT port,UINT uploaded,UINT downloaded,UINT left,std::string event);

	std::string crearGetConNumwant(std::string host,std::string path,UINT* info_hash,std::string peer_id,UINT port,UINT uploaded,UINT downloaded,UINT left,int numwant);

	std::string crearGetReAnnounce(std::string host,std::string path,UINT* info_hash,std::string peer_id, UINT port, UINT uploaded, UINT downloaded, UINT left,int numwant);

};

#endif /* PARSERMENSAJE_H_ */
