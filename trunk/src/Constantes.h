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

/*Constantes utilizadas en el Handshake entre Peers*/
#define PROTOCOL  "BitTorrent protocol"
const char  PSTRLEN = 19;
const int	LEN_BASE_HANDSHAKE = 49;
const int   LEN_HANDSHAKE = PSTRLEN + LEN_BASE_HANDSHAKE;
const int   OFFSET_PSTRLEN = 0;
const int   OFFSET_PROTOCOL = 1;
const int   OFFSET_RESERVED = (OFFSET_PROTOCOL + PSTRLEN);
const int   LEN_RESERVED = 8;
const int   OFFSET_INFO_HASH = (OFFSET_RESERVED + LEN_RESERVED);
const int   OFFSET_PEER_ID = (OFFSET_INFO_HASH + 20);

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
const char ID_MSJ_KEEPALIVE = 9;

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

/*
 * Formato generico de un mensaje sin campo len:   <id><arg1><arg2><arg3>
 * Offsets al comienzo de cada campo:              0   1     5    9
 */
const int OFFSET_ID_SIN_LEN = 0;
const int OFFSET_ARG_1_SIN_LEN = 1;
const int OFFSET_ARG_2_SIN_LEN = 5;
const int OFFSET_ARG_3_SIN_LEN = 9;

/*Otras Constantes*/
const int LEN_SHA1 = 20;
const int LEN_SHA1_ASCII = 40;
const int TIME_OUT_CONNECT = 2;
const int BUFSIZE = 1000;
const int UN_MEGA = 1048576;
const int PEERS_MAX = 30; //TODO ojo lo agrande!!
const float PRIMER_END_GAME = 0.5;
const float SEGUNDO_END_GAME = 0.9;
const int PERMISOS = 0777;
const int TIEMPO_LIMITE = 3;

const int ERROR = -1;
const int OK = 0;

const int TAM_BLOQUES = 16384; //ver http://wiki.theory.org/BitTorrentSpecification#Messages
const int TAM_BUFFER = 100; //TODO redefinir

const int PUERTO_DEFAULT_TRACKER = 80;

/* Estados del Torrent */
#define T_ACTIVO "Activo"
#define T_PAUSADO "Pausado"
#define T_DETENIDO "Detenido"
#define T_COMPLETO "Completo"
#define T_CONECTANDO "Conectando"

/* Configuracion Default */
#define URL_CARPETA_TEMP "./incompletos/"
#define RUTA_CONFIGURACION "FiTorrent.config"
#define RUTA_DESCARGAS "./Descargas/"
const unsigned int PUERTO_DEFAULT = 36277;


/*Constantes de offsets del archivo *.data*/
const int OFFSET_DOWN = 0;
const int OFFSET_UP = 4;
const int OFFSET_COMPLETED = 8;
const int OFFSET_BITMAP_SIZE = 9;
const int OFFSET_BITMAP = 13;

#endif /* CONSTANTES_H_ */
