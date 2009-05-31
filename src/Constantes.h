/*
 * Constantes.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_


/*Tipos de eventos informados al tracker remoto*/
#define EVENT_STARTED   "started"
#define EVENT_COMPLETED  "completed"
#define EVENT_STOPPED   "stopped"

/*IDs de los diferentes tipos de mensajes entre Peers*/
const char ID_MSJ_CHOKE = 0;
const char ID_MSJ_UNCHOKE = 1;
const char ID_MSJ_INTERESTED = 2;
const char ID_MSJ_NOT_INTERESTED = 3;
const char ID_MSJ_HAVE = 4;
const char ID_MSJ_BITFIELD = 5;
const char ID_MSJ_REQUEST = 6;
const char ID_MSJ_PIECE = 7;
const char ID_MSJ_CANCEL = 8;
const char ID_MSJ_PORT = 9;//TODO borrar si no se implementa msj port

/*Longitudes constantes del campo Len de los mensajes entre peers*/
const int  LEN_MSJ_KEEPALIVE = 0;
const int  LEN_MSJ_COMMON = 1;

/*Hay 4 mensajes que son manejados por el mismo metodo ya que su longitud es LEN_MSJ_1 , y
 *  solo difieren en el ID*/
const int  LEN_MSJ_CHOKE = LEN_MSJ_COMMON;
const int  LEN_MSJ_UNCHOKE =  LEN_MSJ_COMMON;
const int  LEN_MSJ_INTERESTED = LEN_MSJ_COMMON;
const int  LEN_MSJ_NOT_INTERESTED = LEN_MSJ_COMMON;

const int  LEN_MSJ_HAVE = 5;
const int  LEN_BASE_MSJ_BITFIELD = 1;
const int  LEN_MSJ_REQUEST = 13;
const int  LEN_BASE_MSJ_PIECE = 9;
const int  LEN_MSJ_CANCEL = 13;
const int  LEN_MSJ_PORT = 3; //TODO borrar si no se implementa msj port

/*Longitudes de los mensajes constantes*/
const int FIXED_LENGTH_COMMON = 5;
const int FIXED_LENGTH_HAVE = 9;
const int FIXED_LENGTH_REQUEST = 17;
const int FIXED_LENGTH_CANCEL = 17;

/*
 * Formato generico de un mensaje:     <len><id><arg1><arg2><arg3>
 * Offsets al comienzo de cada campo:  0    4  5      9    13
 */
const int OFFSET_LEN = 0;
const int OFFSET_ID = 4;
const int OFFSET_ARG_1 = 5;
const int OFFSET_ARG_2 = 9;
const int OFFSET_ARG_3 = 13;

const int ERROR = -1;
const int OK = 0;

const int TAM_BLOQUES = 16384; //ver http://wiki.theory.org/BitTorrentSpecification#Messages
const int TAM_BUFFER = 100; //TODO redefinir

#endif /* CONSTANTES_H_ */
