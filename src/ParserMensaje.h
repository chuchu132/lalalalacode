/*
 * ParserMensaje.h
 *
 *  Created on: 04/06/2009
 *      Author: ale
 */

#ifndef PARSERMENSAJE_H_
#define PARSERMENSAJE_H_

class ParserMensaje {
public:
	ParserMensaje();
	virtual ~ParserMensaje();

	/*Formato mensaje: <pstrlen><pstr><reserved><info_hash><peer_id>*/
	void crearHandshake(const char* info_hash,const char* peer_id,char* buffer);

	/*Formato mensaje: <len=0001><Id=id>*/
	void crearMensajeId(const char id,char* buffer);

	/*Formato mensaje Have: <len=0005><Id=4><piece index> */
	void crearMensajeHave(const int index, char* buffer);

	/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
	void crearMensajeBitfield(const char* map,const int longitud,char* buffer);

	/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
	void crearMensajeRequest(int index, int block, int length,char* buffer);

	/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
	void crearMensajePiece(int index, int begin,int lenght,char* data,char* buffer);

	/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
	void crearMensajeCancel(int index, int block, int length,char* buffer);

	std::string crearGetBase(std::string info_hash,std::string peer_id,int port,int uploaded,int downloaded,int left);

	std::string crearGetConEvento(std::string info_hash,std::string peer_id,int port,int uploaded,int downloaded,int left,std::string event);

	std::string crearGetConNumwant(std::string info_hash,std::string peer_id,int port,int uploaded,int downloaded,int left,int numwant);

	std::string getHash(char* handshakeMsj);
};

#endif /* PARSERMENSAJE_H_ */
