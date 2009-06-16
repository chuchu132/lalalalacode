/*
 * FileManager.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "AvisoDescargaCompleta.h"
#include "ClienteTorrent.h"
#include "Constantes.h"
#include "FileManager.h"
#include "Sha1.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>

FileManager::FileManager(ClienteTorrent* cliente) {
	this->clienteTorrent = cliente;
	tamanioPieza = 0;
	bytesTotales = 0;
	hashPiezas = NULL;
}

FileManager::~FileManager() {
	if (hashPiezas != NULL) {
		delete[] hashPiezas;
	}
	std::list<Archivo*>::iterator it;
	it = archivos.begin();
	while (it != archivos.end()) {
		delete (*it);
		it++;
	}
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
	return (       inicializarTamaniosYpiezas(datos)
			&& inicializarArchivosYdirectorios(datos)
			&& inicializarDatosBitmap(datos));
}

bool FileManager::inicializarTamaniosYpiezas(DatosParser* datos) {
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

	return true;
}

bool FileManager::inicializarArchivosYdirectorios(DatosParser* datos) {
	int tamanio;
	char* datoTemp;
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
		file->addPath(nombreCarpeta);
		archivos.push_back(file);
		delete[] datoTemp;
	} else {
		unsigned int tamTemp;
		while (datos->obtenerDatoPorNombre("length", &datoTemp, tamanio)) {
			tamTemp = (unsigned int) atol(datoTemp);
			bytesTotales += tamTemp;
			delete[] datoTemp;
			Archivo* file = new Archivo();
			file->setTamanio(tamTemp);
			std::string subdir;
			datos->obtenerDatoPorNombre("path", &datoTemp, tamanio);
			file->addPath(datoTemp);
			delete[] datoTemp;

			bool seguir = true;
			do {
				datos->siguiente();
				subdir = datos->obtenerDato();
				if ((subdir.compare("length") == 0) ||
					(subdir.compare("piece length") == 0) ||
					(subdir.compare("name") == 0)
					)
				{
					seguir = false;
				} else {
					file->addPath(subdir);
				}
			} while (seguir);

			archivos.push_back(file);
		}
	}

	return true;
}

bool FileManager::inicializarDatosBitmap(DatosParser* datos) {
	int tamanio;
	char* datoTemp;
	datos->primero(); //inicio la busqueda desde el principio
	if (!datos->obtenerDatoPorNombre("info_hash", &datoTemp, tamanio)) {
		return false;
	}

	Sha1 sha;
	std::string url = URL_CARPETA_TEMP;
	std::string hashString = sha.salidaAstring((unsigned int*) datoTemp);
	url += hashString;
	delete[] datoTemp;
	descarga.open(url.c_str(), ios::in | ios::out | ios::binary);
	if (!descarga.is_open()) {
		if (!crearArchivo(url, bytesTotales)) {
			return false;
		}
	}

	url += ".bitmap";
	inicializarBitmap(url);

	return true;
}


unsigned int FileManager::getTamanioPieza(unsigned int index){
	unsigned int cantidadPiezas = (unsigned int )(bytesTotales / tamanioPieza);
	if(index < cantidadPiezas){
		return tamanioPieza;
	}else{
		return (unsigned int )(bytesTotales % tamanioPieza);
	}
}



bool FileManager::crearArchivo(std::string path, unsigned int tamanio) {
	descarga.open(path.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
	if (descarga.is_open()) {
		descarga.seekp(tamanio, std::ios::beg);
		descarga.put(0);
		return true;
	}
	return false;
}

void FileManager::inicializarBitmap(std::string urlBitmap) {
	std::ifstream archivoBitmap;
	archivoBitmap.open(urlBitmap.c_str(), std::ios::in | std::ios::binary
			| std::ios::ate);

	if (archivoBitmap.is_open()) {
		int tam = archivoBitmap.tellg();
		archivoBitmap.seekg(0, std::ios::beg);
		char* bitmapRecuperado = new char[tam];
		archivoBitmap.read(bitmapRecuperado, tam);
		bitmap.inicializarBitmap(bitmapRecuperado, tam);
		delete[] bitmapRecuperado;
	} else {
		unsigned int cantPiezas = (unsigned int) ((bytesTotales / tamanioPieza)
				+ (((bytesTotales % tamanioPieza) == 0) ? 0 : 1));
		unsigned int cantBytes = (unsigned int) ((cantPiezas / 8)
				+ (((cantPiezas % 8) == 0) ? 0 : 1));
		bitmap.inicializarBitmap(cantBytes);
	}
}

void FileManager::guardarBitmap(std::string urlBitmap) {
	std::ofstream archivoBitmap;
	archivoBitmap.open(urlBitmap.c_str(), std::ios::out | std::ios::binary
			| std::ios::trunc);
	if (archivoBitmap.is_open()) {
		const char* bitmapAux = bitmap.getBitmap();
		archivoBitmap.seekp(std::ios::beg);
		archivoBitmap.write(bitmapAux, bitmap.getTamanioEnBytes());
		archivoBitmap.close();
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

void FileManager::writeBlock(int index, int begin, int longitud,
		const char* block) {
	if (!bitmap.estaMarcada(index)) {
		int offset = (index * tamanioPieza + begin);
		descarga.seekp(offset); // se para en el offset inicial
		descarga.write(block, longitud); // escribe
		if (verificarHashPieza(index)) {
			bitmap.marcarBit(index);
			if (descargaCompleta()) {
				throw AvisoDescargaCompleta();
			}
		}
	}
}
//TODO testear
bool FileManager::verificarHashPieza(int index) {
	char* pieza = new char[tamanioPieza];
	memset(pieza, 0, tamanioPieza);
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

bool FileManager::descargaCompleta() {
	unsigned int cantPiezas = (unsigned int) ((bytesTotales / tamanioPieza)
			+ (((bytesTotales % tamanioPieza) == 0) ? 0 : 1));
	unsigned int cantBytesCompletos = (unsigned int) (cantPiezas / 8);
	unsigned int resto = (unsigned int) (cantPiezas % 8);
	unsigned int tamBitmap = cantBytesCompletos + ((resto == 0) ? 0 : 1);
	char* bitmapAux = new char[tamBitmap];
	//creo un bitmap con 1 en los bits que corresponden a piezas
	unsigned int i;
	for (i = 0; i < cantBytesCompletos; i++) {
		bitmapAux[i] = 0xFF;
	}
	if (resto != 0) {
		bitmapAux[cantBytesCompletos] = (char) pow(2, 8 - resto);
	}

	const char* miBitmap = bitmap.getBitmap();
	bool completo = true;
	//comparo byte a byte mi bitmap con el completo hasta que haya una diferencia o se termine
	while (completo && (i < tamBitmap)) {
		completo = (bitmapAux[i] == (bitmapAux[i] & miBitmap[i]));
		i++;
	}
	delete[] bitmapAux;
	return completo;
}

void FileManager::descargaAarchivos() {

	bool multifile = (archivos.size() != 1);
	std::string urlbase = clienteTorrent->getConfiguracion()->getRutaDescargas();
	urlbase += nombreCarpeta; //nombre de carpeta o nombre de archivo en caso de torrent single file

	unsigned int acumulado = 0;
	std::list<Archivo*>::iterator itA = archivos.begin();
	unsigned int tamanio;
	if(multifile){
		mkdir(urlbase.c_str(),0755);
		chdir(urlbase.c_str()); // el directorio ./ pasa a ser dentro de la carpeta donde copio losarchivos
		while(itA != archivos.end()){
			tamanio = (*itA)->getTamanio();
			std::string restoUrl = "./";
			std::list<std::string>::iterator itPath = (*itA)->getPathList()->begin();
			std::list<std::string>::iterator archivo = --((*itA)->getPathList()->end());
			while(itPath != archivo){
				restoUrl += (*itPath);
				restoUrl += "/";
				mkdir(restoUrl.c_str(),0755); //creo cada subdirectorio
				itPath++;
			}
			restoUrl += (*itPath);
			copiar(acumulado,tamanio,restoUrl);
			acumulado += tamanio;
			itA++;
		}

	}
	else{
		unsigned int tamanio = (*itA)->getTamanio();
		copiar(0,tamanio,urlbase);
	}
}


void FileManager::copiar(unsigned int desde, unsigned int cantidad, std::string destino){
	std::ofstream salida;
	salida.open(destino.c_str(),std::ios::out | std::ios::trunc | std::ios::binary);
	salida.seekp(0,std::ios::beg);
	const unsigned int tamBuffer = UN_MEGA;
	char buffer[tamBuffer];
	unsigned int lecturasEnteras = (unsigned int )(cantidad / tamBuffer);
	unsigned int resto = (unsigned int)(cantidad % tamBuffer);
	descarga.seekg(desde,std::ios::beg);
	for(unsigned int i = 0; i< lecturasEnteras; i++){
		descarga.read(buffer,tamBuffer);
		salida.write(buffer,tamBuffer);
	}
	descarga.read(buffer,resto);
	salida.write(buffer,resto);

	salida.close();
}

unsigned int FileManager::getCantArchivos() {
	return archivos.size();
}

unsigned int FileManager::getTamanioPieza() {
	return tamanioPieza;
}

bool FileManager::getPiezaAdescargar(unsigned int &index,Bitmap& mapaPeerRemoto){
	bool retorno;
	llavePedidos.lock();
	Bitmap* faltanDescargar = bitmap.nuevoPorFusion(mapaPeerRemoto);
	Bitmap* noPidioNadie = mapaPedidos.nuevoPorFusion(*faltanDescargar);
	if( (retorno = noPidioNadie->getPrimeroLibre(index)) ){
		mapaPedidos.marcarBit(index);
	};
	llavePedidos.unlock();
	return retorno;
}
