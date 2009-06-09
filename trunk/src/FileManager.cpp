/*
 * FileManager.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "FileManager.h"
#include "Constantes.h"
#include <fstream>
#include "Sha1.h"


FileManager::FileManager() {

}

FileManager::~FileManager() {
	// TODO Auto-generated destructor stub
}

/*TODO Pedir info_hash tratar de abrir un archivo con ese nombre
 * de la carpeta donde se guardan las descargas, sino existe
 * crearlo del tamaño TOTAL que indica e torrent y rellenar de
 * 0, si existe verificar cada pieza y crear un bitmap con
 * el estado del archivo.
 * Inicializar el resto de los campos segun corresponda con la
 * info obtenida del .torrent y del archivo descargado parcialmente.
 * */
void FileManager::inicializar(BencodeParser* parser) {
//	archivos = parser->getListaArchivos(); //TODO ojo quedan apuntando los mismos Archivo*
//	std::list<Archivo*>::iterator it;
//	int acum = 0;
//	for (it = archivos.begin(); it != archivos.end(); ++it) {
//		acum += (*it)->getTamanio();
//	}
//	bytesTotales = acum;
//	tamanioPieza = parser->getTamanioPiezas();
//	bloquesXPieza = (tamanioPieza > TAM_BLOQUES) ? (tamanioPieza / TAM_BLOQUES)
//			: 1; // division entre potencias de 2
//
//	int tamBitmapBytes = (bytesTotales / 8) + (((bytesTotales % 8) == 0) ? 0
//			: 1);
//	bitmap.inicializarBitmap(tamBitmapBytes);

}

int FileManager::getTamanio() {
	return bytesTotales;
}

Bitmap& FileManager::getBitmap() {
	return bitmap;
}

char* FileManager::readBlock(int index, int begin, int longitud) {
	if (bitmap.estaMarcada(index)) {
		char* data = new char[longitud + 1];
		int offset = (index * tamanioPieza + begin );
		descarga.seekg(offset); // se para en el offset inicial
		descarga.get(data, longitud); // lee
		return data;
	} else {
		return NULL;
	}
}

void FileManager::writeBlock(int index,int begin,int longitud,char* block){
	if (!bitmap.estaMarcada(index)) {
		int offset = (index * tamanioPieza + begin );
		descarga.seekp(offset); // se para en el offset inicial
		descarga.write(block, longitud); // escribe
		if(verificarHashPieza(index)){
			bitmap.marcarBit(index);
		}
	}
}

bool FileManager::verificarHashPieza(int index){
	char pieza[tamanioPieza];
	int offset = (index * tamanioPieza);
	descarga.seekg(offset);
	descarga.get(pieza,tamanioPieza);
	Sha1 sha1Encoder;
	std::string hashObtenido = sha1Encoder.codificar(pieza,tamanioPieza);
	std::string hashOriginal;
	//hashOriginal.assign(hashPiezas,index*LEN_SHA1_ASCII,LEN_SHA1_ASCII);//TODO probar que el 2do arg sea correcto y no vaya un blbla-1
	return (hashOriginal.compare(hashObtenido) == 0);
}

std::list<Archivo*>::iterator FileManager::getIteratorArchivos()
{
	return archivos.begin();
}

std::list<Archivo*>::iterator FileManager::getEndArchivos()
{
	return archivos.end();
}
