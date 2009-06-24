/*
 * Tipos.h
 *
 *  Created on: 23/06/2009
 *      Author: ale
 */

#ifndef TIPOS_H_
#define TIPOS_H_

#include <list>
#include <string>
#include "Torrent.h"
#include "Archivo.h"
#include "Peer.h"

typedef unsigned long int ULINT;
typedef unsigned int UINT;
typedef unsigned char UCHAR;

typedef std::list<std::string> LString;
typedef std::list<std::string>::iterator LStringIter;
typedef std::list<Torrent*> LTorrentp;
typedef std::list<Torrent*>::iterator LTorrentpIter;
typedef std::list<char*> LCharp;
typedef std::list<char*>::iterator LCharpIter;
typedef std::list<int>  ListInt;
typedef std::list<int>::iterator  ListIntIter;
typedef std::list<Archivo*>::iterator LArchivopIter;
typedef std::list<Peer*>::iterator LPeerpIter;

#endif /* TIPOS_H_ */
