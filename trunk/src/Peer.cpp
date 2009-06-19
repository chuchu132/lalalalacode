/*
 * Peer.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */
#include <arpa/inet.h>
#include <ctime>
#include <cstring>
#include <list>
#include "Constantes.h"
#include "Peer.h"
#include "ParserMensaje.h"

Peer::Peer(Socket* peerRemoto, Torrent* torrent) {
	this->peerRemoto = peerRemoto;
	conexionOK = peerRemoto->is_valid();
	this->torrent = torrent;
	am_choking = true;
	am_interested = false;
	peer_choking = true;
	peer_interested = false;
	tipo = 'P';
}

Peer::~Peer() {
}

bool Peer::procesar(char* buffer, int length) {
	ParserMensaje parser;
	char id = parser.decodificarId(buffer);
	switch (id) {
	case ID_MSJ_CHOKE: {
		peer_choking = true;
	}
		break;
	case ID_MSJ_UNCHOKE: {
		peer_choking = false;
	}
		break;
	case ID_MSJ_INTERESTED: {
		peer_interested = true;
	}
		break;
	case ID_MSJ_NOT_INTERESTED: {
		peer_interested = false;
	}
		break;
	case ID_MSJ_HAVE: {
		unsigned int index;
		parser.decodificarHave(buffer, index);
		procesarHave(index);
	}
		break;
	case ID_MSJ_BITFIELD: {
		char* bitfield;
		unsigned int longitud;
		parser.decodificarBitfield(buffer, length, longitud, &bitfield);
		procesarBitfield(bitfield, longitud);
	}
		break;
	case ID_MSJ_REQUEST: {
		unsigned int index, begin, length2;
		parser.decodificarRequest(buffer, index, begin, length2);
		procesarRequest(index, begin, length2);
	}
		break;
	case ID_MSJ_PIECE: {
		char* data;
		unsigned int index, begin, length2;
		parser.decodificarPiece(buffer, length, index, begin, length2, &data);
		procesarPiece(index, begin, length2, data);
	}
		break;
	case ID_MSJ_CANCEL: {
		unsigned int index, begin, length2;
		parser.decodificarCancel(buffer, index, begin, length2);
		procesarCancel(index, begin, length2);
	}
		break;
	default:
		return false;
	}
	return true;

}

bool Peer::sendHandshake() {
	ParserMensaje parser;
	char buffer[LEN_HANDSHAKE];
	bool retorno;
	parser.crearHandshake(torrent->getInfoHash(), torrent->getPeerId().c_str(),
			buffer);
	llaveEnvio.lock();
	retorno = (peerRemoto->send(buffer, LEN_HANDSHAKE) != ERROR);
	llaveEnvio.unlock();
	return retorno;
}

bool Peer::sendKeepAlive() {
	bool retorno;
	llaveEnvio.lock();
	retorno = (peerRemoto->send(LEN_MSJ_KEEPALIVE, sizeof(int)) != ERROR);
	llaveEnvio.unlock();
	return retorno;
}

/*Formato mensaje: <len=0001><Id=id>*/
bool Peer::sendMsg(const char id) {
	bool retorno;
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_COMMON];
	parser.crearMensajeId(id, buffer);
	llaveEnvio.lock();
	retorno = (peerRemoto->send(buffer, FIXED_LENGTH_COMMON) != ERROR);
	llaveEnvio.unlock();
	return retorno;
}
;

/*Formato mensaje Have: <len=0005><Id=4><piece index> */
bool Peer::sendHave(int index) {
	bool retorno = true;
	if (!bitmap.estaMarcada(index)) {
		ParserMensaje parser;
		char buffer[FIXED_LENGTH_HAVE];
		parser.crearMensajeHave(index, buffer);
		llaveEnvio.lock();
		retorno = (peerRemoto->send(buffer, FIXED_LENGTH_HAVE) != ERROR);
		llaveEnvio.unlock();
	}
	return retorno;
}
/*Formato mensaje Bitfiel: <len=0001 + X><Id=5><bitfield>  X = longitud en bytes de bitfield*/
bool Peer::sendBitfield() {
	ParserMensaje parser;
	FileManager* fileManager = this->torrent->getFileManager();
	int longitud;
	bool retorno;

	const char* map = fileManager->getBitmap().getBitmap();
	longitud = fileManager->getBitmap().getTamanioEnBytes();
	int tamBuffer = (LEN_BASE_MSJ_BITFIELD + longitud + 1);
	char* buffer = new char[tamBuffer];

	parser.crearMensajeBitfield(map, longitud, buffer);
	llaveEnvio.lock();
	retorno = (peerRemoto->send(buffer, tamBuffer) != ERROR);
	llaveEnvio.unlock();
	delete[] buffer;
	return retorno;
}

/*Formato mensaje Request: <len=0013><Id=6><index><begin><length> */
bool Peer::sendRequest(unsigned int index) {
	int block, cantBlockEnteros, resto;
	unsigned int tamPieza;
	ParserMensaje parser;
	bool error = false;
	char buffer[FIXED_LENGTH_REQUEST];

	tamPieza = torrent->getFileManager()->getTamanioPieza(index);
	cantBlockEnteros = (unsigned int) (tamPieza / TAM_BLOQUES);
	resto = (unsigned int) (tamPieza % TAM_BLOQUES);
	block = 0;
	while ((block < cantBlockEnteros) && !error) {
		parser.crearMensajeRequest(index, (block * TAM_BLOQUES), TAM_BLOQUES,
				buffer);
		llaveEnvio.lock();
		error = (peerRemoto->send(buffer, FIXED_LENGTH_REQUEST) == ERROR);
		llaveEnvio.unlock();
		block++;
	}
	if (resto != 0) {
		parser.crearMensajeRequest(index, (block * TAM_BLOQUES), resto, buffer);
		llaveEnvio.lock();
		error = (peerRemoto->send(buffer, FIXED_LENGTH_REQUEST) == ERROR);
		llaveEnvio.unlock();
	}
	return !error;
}

/*Formato mensaje Piece: <len=0009 + X><Id=7><index><begin><block>  X = longitud en bytes de block */
bool Peer::sendPiece(int index, int begin, int lenght) {
	FileManager* fileManager = this->torrent->getFileManager();
	char* data = NULL;

	if ((data = fileManager->readBlock(index, begin, lenght)) != NULL) {
		ParserMensaje parser;
		bool retorno;
		int tamBuffer = (LEN_BASE_MSJ_PIECE + lenght + 1);
		char* buffer = new char[tamBuffer];
		parser.crearMensajePiece(index, begin, lenght, data, buffer);
		llaveEnvio.lock();
		retorno = (peerRemoto->send(buffer, tamBuffer) != ERROR);
		llaveEnvio.unlock();
		delete[] data;
		delete[] buffer;
		return retorno;
	} else {
		peerRemoto->close();
	}
	return false;
}
/* Formato mensaje Piece: <len=0013><Id=8><index><begin><length>  */
bool Peer::sendCancel(int index, int block, int length) {
	bool retorno;
	ParserMensaje parser;
	char buffer[FIXED_LENGTH_CANCEL];
	parser.crearMensajeCancel(index, block, length, buffer);
	llaveEnvio.lock();
	retorno = (peerRemoto->send(buffer, FIXED_LENGTH_CANCEL) != ERROR);
	llaveEnvio.unlock();
	return retorno;
}

void Peer::procesarHave(int index) {
	if (bitmap.estaOk()) {
		bitmap.marcarBit(index);
	}
}

void Peer::procesarBitfield(const char* bitfield, int length) {
	bitmap.inicializarBitmap(bitfield, length);
}

void Peer::procesarRequest(int index, int begin, int length) {
	if (am_choking) {
		sendMsg(ID_MSJ_CHOKE);
	} else {
		sendPiece(index, begin, length);
	}
}

void Peer::procesarPiece(int index, int begin, int longitud, char* data) {
	try {
		torrent->getFileManager()->writeBlock(index, begin, longitud, data);
		if (torrent->getFileManager()->getBitmap().estaMarcada(index)) {
			repartirHave(index);
			bitmap.desmarcarBit(index);// desmarca el bit que representa la pieza obtenida del bitmap del peer remoto
		}
	} catch (AvisoDescargaCompleta aviso) {
		//TODO el archivo esta completo.
	}
}

void Peer::procesarCancel(int index, int begin, int length) {
	//TODO de momento se ignora.
}

void Peer::repartirHave(int index) {
	std::list<Peer*>* listaPeers = torrent->getListaPeers();
	std::list<Peer*>::iterator it;
	it = listaPeers->begin();
	while (it != listaPeers->end()) {
		if ((*it) != this) {
			(*it)->sendHave(index);
		}
	}
}

bool Peer::recvHandshake() {

	char longProto;
	int cantidad = peerRemoto->receiveExact(&longProto, 1);
	if (cantidad > 0) {
		int tamHsk= (unsigned char)longProto + LEN_BASE_HANDSHAKE - 1;
		char buffer[tamHsk];
		if (peerRemoto->receiveExact(buffer, tamHsk) != ERROR) {
				if (memcmp(torrent->getInfoHash(), buffer + longProto + LEN_RESERVED,
					LEN_SHA1) != 0) {
				peerRemoto->close();
			} else {
				return true;
			}
		}
	}
	return false;
}

bool Peer::recvMsj(char** buffer, int& length) {
	int temp = 0;
	int cantidad = peerRemoto->receiveExact((char*) &temp, 4); //TODO RE IMPORTNATE!!ver que el receive llene el buffer socket->receive no testeado!!
	if (cantidad > 0) {
		length = ntohl(temp);
		*buffer = new char[length];
		cantidad = peerRemoto->receiveExact(*buffer, length);
		if (cantidad > 0) {
			return true;
		}
		delete[] (*buffer);
		buffer = NULL;
	}
	return false;
}

bool Peer::conexionEstaOK() {
	return conexionOK;
}

Torrent* Peer::getTorrent() {
	return torrent;
}

Bitmap Peer::getBitmap() {
	return bitmap;
}

bool Peer::getAm_interested() {
	return am_interested;
}

void Peer::setAm_interested(bool estado) {
	am_interested = estado;
}

bool Peer::getPeer_choking() {
	return peer_choking;
}

void Peer::setPeer_choking(bool estado) {
	peer_choking = estado;
}

char Peer::getTipo() {
	return tipo;
}

void Peer::setTipo(char estado) {
	tipo = estado;
}

std::string Peer::getIp() {
	return peerRemoto->getIp();
}

void Peer::cerrarConexion(){
	conexionOK = false;
	peerRemoto->close();
}

bool Peer::actualizarImInterested(){
	Bitmap* nuevo = torrent->getFileManager()->bitmap.nuevoPorFusion(bitmap);
	bool retorno = !nuevo->estaVacio();
	delete nuevo;
	am_interested = retorno;
	return retorno;
}

