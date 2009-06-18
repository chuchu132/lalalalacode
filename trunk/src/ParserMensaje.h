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
	void crearHandshake(unsigned int* info_hash,const char* peer_id,char* buffer);

	/*Formato mensaje: <len=0001><Id=id>*/
	void crearMensajeId(const char id,char* buffer);

	/*Formato mensaje Have: <len=0005><Id=4><piece index> */
	void crearMensajeHave(const unsigned int index, char* buffer);

	/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
	void crearMensajeBitfield(const char* map,const unsigned int longitud,char* buffer);

	/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
	void crearMensajeRequest(unsigned int index, unsigned int block, unsigned int length,char* buffer);

	/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
	void crearMensajePiece(unsigned int index, unsigned int begin,unsigned int lenght,char* data,char* buffer);

	/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
	void crearMensajeCancel(unsigned int index, unsigned int block, unsigned int length,char* buffer);

	std::string crearGetBase(std::string path,unsigned int* info_hash,std::string peer_id,unsigned int port,unsigned int uploaded,unsigned int downloaded,unsigned int left);

	std::string crearGetConEvento(std::string host,std::string path,unsigned int* info_hash,std::string peer_id,unsigned int port,unsigned int uploaded,unsigned int downloaded,unsigned int left,std::string event);

	std::string crearGetConNumwant(std::string host,std::string path,unsigned int* info_hash,std::string peer_id,unsigned int port,unsigned int uploaded,unsigned int downloaded,unsigned int left,int numwant);

	std::string getHash(char* handshakeMsj);

	char decodificarId(char* buffer);
	void decodificarHave(char* buffer,unsigned int& index);
	void decodificarBitfield(char* buffer,unsigned int longitudBuffer, unsigned int& longitud,char** bitfield);
	void decodificarRequest(char* buffer,unsigned int& index,unsigned int& begin,unsigned int& length);
	void decodificarPiece(char* buffer,unsigned int longitudBuffer,unsigned int& index,unsigned int& begin,unsigned int& longitud,char** data);
	void decodificarCancel(char* buffer,unsigned int& index,unsigned int& begin,unsigned int& length);
};

#endif /* PARSERMENSAJE_H_ */
