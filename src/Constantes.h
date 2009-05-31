/*
 * Constantes.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#define EVENT_STARTED   "started"
#define EVENT_COMPLETED "completed"
#define EVENT_STOPPED   "stopped"

#define ID_MSJ_CHOKE '\0'
#define ID_MSJ_UNCHOKE '\1'
#define ID_MSJ_INTERESTED '\2'
#define ID_MSJ_NOT_INTERESTED '\3'
#define ID_MSJ_HAVE '\4'
#define ID_MSJ_BITFIELD '\5'
#define ID_MSJ_REQUEST '\6'
#define ID_MSJ_PIECE '\7'
#define ID_MSJ_CANCEL '\8'
#define ID_MSJ_PORT '\9'

#define LEN_MSJ_KEEPALIVE  0
#define LEN_MSJ_CHOKE  1
#define LEN_MSJ_UNCHOKE  1
#define LEN_MSJ_INTERESTED 1
#define LEN_MSJ_NOT_INTERESTED 1
#define LEN_MSJ_HAVE  5
#define LEN_BASE_MSJ_BITFIELD 1
#define LEN_MSJ_REQUEST 13
#define LEN_BASE_MSJ_PIECE 9
#define LEN_MSJ_CANCEL 13
#define LEN_MSJ_PORT 3

#define ERROR -1
#define OK 0

#define TAM_BLOQUES 16384 //ver http://wiki.theory.org/BitTorrentSpecification#Messages
#define TAM_BUFFER 100 //TODO redefinir

#define TIMEOUT 2 // segundos

#endif /* CONSTANTES_H_ */
