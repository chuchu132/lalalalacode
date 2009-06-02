/*
 * FileManager.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "FileManager.h"
#include <fstream>

FileManager::FileManager() {

}

FileManager::~FileManager() {
	// TODO Auto-generated destructor stub
}

void FileManager::inicializar(BencodeParser* parser) {
	archivos = parser->getListaArchivos(); //TODO ojo quedan apuntando los mismos Archivo*
	std::list<Archivo*>::iterator it;
	int acum = 0;
	for (it = archivos.begin(); it != archivos.end(); ++it) {
		acum += (*it)->getTamanio();
	}
	bytesTotales = acum;
	tamanioPieza = parser->getTamanioPiezas();
	bloquesXPieza = (tamanioPieza > TAM_BLOQUES) ? (tamanioPieza / TAM_BLOQUES)
			: 1; // division entre potencias de 2

	tamBitmapBytes = (bytesTotales / 8) + (((bytesTotales % 8) == 0) ? 0 : 1);
	bitmap = new char[tamBitmapBytes];
	memset(bitmap, 0, tamBitmapBytes);

}

int FileManager::getTamanio() {
	return bytesTotales;
}

void FileManager::getMap(char* map, int* length) {
	map = new char[tamBitmapBytes];
	mempcpy(map, bitmap, tamBitmapBytes);
	(*length) = tamBitmapBytes;
}


char* FileManager::getBlock(int index, int begin, int longitud) {
	int sumaAnterior = 0;
	int sumaAcumulada = 0;
	int inicioBloque = (index * tamanioPieza) + begin;
	std::list<Archivo*>::iterator it;
	it = archivos.begin();
	do {
		sumaAnterior = sumaAcumulada;
		sumaAcumulada += (*it)->getTamanio();
		it++;
	} while (sumaAcumulada < inicioBloque && (it != archivos.end()));
	if (it == archivos.end()) {
		return NULL;
	}

	char* data = new char[longitud + 1];
	int offset = (index * tamanioPieza - sumaAnterior);
	int resto = longitud;
	int leidos = 0;

	while ((resto != 0) && it != archivos.end()) { //TODO ¡¡¡¡¡¡¡¡¡ ver si funciona !!!!!!
		std::fstream* file = (*it)->getArchivo(); // pide el archivo
		file->seekg(offset);       // se para en el offset inicial, en caso de tener q leer de otro archivo, empieza desde el inicio
		file->get(data, longitud); // lee
		leidos = strlen(data);    // compruebo cuantos leyo
		resto = longitud - leidos;
		it++;					  // salto al siguiente archivo
		offset = 0;
	}
	if (resto == 0) {
		return data;
	} else {
		delete[] data;
		return NULL;
	}
}
