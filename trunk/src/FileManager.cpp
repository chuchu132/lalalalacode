/*
 * FileManager.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "ClienteTorrent.h"
#include "Constantes.h"
#include "FileManager.h"
#include "Sha1.h"
#include <fstream>
#include <cstring>

FileManager::FileManager(ClienteTorrent* cliente) {
	this->clienteTorrent = cliente; //TODO no se esta usando q ondaaa?
	tamanioPieza = 0;
	bytesTotales = 0;
	hashPiezas = NULL;
}

FileManager::~FileManager() {
	if(hashPiezas != NULL){delete[] hashPiezas;}
	std::list<Archivo*>::iterator it;
	it = archivos.begin();
	while(it != archivos.end() ){
		delete (*it);
		it++;
	}
	//TODO limpiar lista archivos guardar datos admin
}

/*TODO Pedir info_hash tratar de abrir un archivo con ese nombre
 * de la carpeta donde se guardan las descargas, sino existe
 * crearlo del tamaño TOTAL que indica e torrent y rellenar de
 * 0, si existe verificar cada pieza y crear un bitmap con
 * el estado del archivo.
 * Inicializar el resto de los campos segun corresponda con la
 * info obtenida del .torrent y del archivo descargado parcialmente.
 * */
bool FileManager::inicializar(DatosParser* datos) {
	int tamanio;
	char* datoTemp;
	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->obtenerDatoPorNombre("piece length", &datoTemp, tamanio)) {
		return false;
	}
	tamanioPieza = (unsigned int) atol(datoTemp);
	delete[] datoTemp;
	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->obtenerDatoPorNombre("pieces", &datoTemp, tamanio)) {
		return false;
	}
	hashPiezas = (unsigned int*) new char[tamanio];
	memcpy(hashPiezas, datoTemp, tamanio);
	delete[] datoTemp;
	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->obtenerDatoPorNombre("name", &datoTemp, tamanio)) {
		return false;
	}
	nombreCarpeta = datoTemp;
	delete[] datoTemp;

	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->irAetiqueta("files")) {
		if (!datos->obtenerDatoPorNombre("length", &datoTemp, tamanio)) {
			return false;
		}
		bytesTotales = (unsigned int) atol(datoTemp);
		Archivo* file = new Archivo();
		file->setTamanio(bytesTotales);
		file->setPath(nombreCarpeta);
		archivos.push_back(file);
		delete[] datoTemp;
	} else {
		unsigned int tamTemp;
		while (datos->obtenerDatoPorNombre("length", &datoTemp, tamanio)) {
			tamTemp = (unsigned int) atol(datoTemp);
			bytesTotales += tamTemp;
			delete[] datoTemp;
			datos->obtenerDatoPorNombre("path", &datoTemp, tamanio);
			Archivo* file = new Archivo();
			file->setTamanio(tamTemp);
			file->setPath(datoTemp);
			archivos.push_back(file);
			delete[] datoTemp;
		}
	}

	unsigned int cantidadPiezasMod8 =((unsigned int) (bytesTotales / 8) + ((bytesTotales % 8 == 0)? 0 : 1));
	bitmap.inicializarBitmap(cantidadPiezasMod8);

	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->obtenerDatoPorNombre("info_hash", &datoTemp, tamanio)) {
		return false;
	}

	Sha1 sha;
	std::string hash = URL_CARPETA_TEMP;
	hash += sha.salidaAstring((unsigned int*) datoTemp);
	delete[] datoTemp;
	descarga.open(hash.c_str(), ios::in | ios::out | ios::binary);
	if (!descarga.is_open()) {
		if(!crearArchivo(hash, bytesTotales)){return false;}
	}
	//ver comentario en la implementacion del inicializar
	//inicializarBitmap();


	return true;
}

//TODO ver de optimizar el llenado del archivo
bool FileManager::crearArchivo(std::string path, unsigned int tamanio) {
	descarga.open(path.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
	if (descarga.is_open()) {
		char cero = '\0';
		for (unsigned int i = 0; i < tamanio; ++i) {
			descarga.put(cero);
		}
		return true;
	}
	return false;
}
/* TODO Muuuuuuuuuuuuuuuuuuy lento
 * Solucion, guardar el bitmap antes de cerrar la aplicacion.
 * Aunque al completar la descarga deberiamos verificar que este todo bien.
 * */
void FileManager::inicializarBitmap() {
	unsigned int cantidadPiezas = (unsigned int) ((bytesTotales / tamanioPieza)
			+( ((bytesTotales % tamanioPieza) == 0) ? 0 : 1));
	for (unsigned int i = 0; i < cantidadPiezas; ++i) {
		if (verificarHashPieza(i)) {
			bitmap.marcarBit(i);
		}
		std::cout<<i<<std::endl;
	}
}

unsigned int FileManager::getTamanio() {
	return bytesTotales;
}

Bitmap& FileManager::getBitmap() {
	return bitmap;
}
/*Solo se comparten piezas que estan enteras */
char* FileManager::readBlock(int index, int begin, int longitud) {
	if (bitmap.estaMarcada(index)) {
		char* data = new char[longitud + 1];
		int offset = (index * tamanioPieza + begin);
		descarga.seekg(offset); // se para en el offset inicial
		descarga.get(data, longitud); // lee
		return data;
	} else {
		return NULL;
	}
}

void FileManager::writeBlock(int index, int begin, int longitud, const char* block) {
	if (!bitmap.estaMarcada(index)) {
		int offset = (index * tamanioPieza + begin);
		descarga.seekp(offset); // se para en el offset inicial
		descarga.write(block, longitud); // escribe
		if (verificarHashPieza(index)) {
			bitmap.marcarBit(index);
		}
	}
}
//TODO testear
bool FileManager::verificarHashPieza(int index) {
	char* pieza = new char[tamanioPieza];
	int offset = (index * tamanioPieza);
	descarga.seekg(offset);
	descarga.get(pieza, tamanioPieza);
	Sha1 sha1Encoder;
	std::string hashObtenido = sha1Encoder.codificar(pieza, tamanioPieza);
	std::string hashOriginal = sha1Encoder.codificar(((char*) hashPiezas)
			+ (index * LEN_SHA1), LEN_SHA1);
	delete[] pieza;
	return (hashOriginal.compare(hashObtenido) == 0);
}

std::list<Archivo*>::iterator FileManager::getIteratorArchivos() {
	return archivos.begin();
}

std::list<Archivo*>::iterator FileManager::getEndArchivos() {
	return archivos.end();
}
