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
#include "Torrent.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>

FileManager::FileManager(ClienteTorrent* cliente,Torrent* torrent) {
	this->clienteTorrent = cliente;
	tamanioPieza = 0;
	bytesTotales = 0;
	hashPiezas = NULL;
	this->torrent = torrent;
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
	guardarDatos();
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
			&& inicializarDatosYbitmap(datos));
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

bool FileManager::inicializarDatosYbitmap(DatosParser* datos) {
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
	url += ".data";
	urlDatos = url;
	this->inicializarBitmap();
	inicializarDatos();

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

void FileManager::inicializarBitmap(){
	std::ifstream archivoBitmap;
	archivoBitmap.open(urlDatos.c_str(), std::ios::in | std::ios::binary
			| std::ios::ate);

	if (archivoBitmap.is_open()) {
		int tam;
		archivoBitmap.seekg(OFFSET_BITMAP_SIZE, std::ios::beg);
		archivoBitmap.read((char*)&tam,4);
		archivoBitmap.seekg(OFFSET_BITMAP, std::ios::beg);
		char* bitmapRecuperado = new char[tam];
		archivoBitmap.read(bitmapRecuperado, tam);
		bitmap.inicializarBitmap(bitmapRecuperado, tam);
		archivoBitmap.close();
		delete[] bitmapRecuperado;
	} else {
		unsigned int cantPiezas = (unsigned int) ((bytesTotales / tamanioPieza)
				+ (((bytesTotales % tamanioPieza) == 0) ? 0 : 1));
		unsigned int cantBytes = (unsigned int) ((cantPiezas / 8)
				+ (((cantPiezas % 8) == 0) ? 0 : 1));
		bitmap.inicializarBitmap(cantBytes);
	}
	mapaPedidos.inicializarBitmap(bitmap.getTamanioEnBytes());
}


void FileManager::inicializarDatos(){
	std::ifstream archivoDatos;
	archivoDatos.open(urlDatos.c_str(), std::ios::in | std::ios::binary
			| std::ios::ate);

	if (archivoDatos.is_open()) {
		unsigned int temp;
		archivoDatos.seekg(OFFSET_DOWN, std::ios::beg);
		archivoDatos.read((char*)&temp,4);
		torrent->setDownloaded(temp);
		archivoDatos.seekg(OFFSET_UP, std::ios::beg);
		archivoDatos.read((char*)&temp,4);
		torrent->setUploaded(temp);
		char estado;
		archivoDatos.seekg(OFFSET_COMPLETED, std::ios::beg);
		archivoDatos.read(&estado,1);
		if(estado == 1){
			torrent->setEstado(T_COMPLETO);
		}
		archivoDatos.close();
	}
}


void FileManager::guardarDatos() {
	std::ofstream archivoDatos;
	archivoDatos.open(urlDatos.c_str(), std::ios::out | std::ios::binary
			| std::ios::trunc);
	if (archivoDatos.is_open()) {
		archivoDatos.seekp(OFFSET_DOWN,std::ios::beg);
		unsigned int temp =torrent->getTamanioDescargado();
		archivoDatos.write((char*) &temp,4);
		archivoDatos.seekp(OFFSET_UP,std::ios::beg);
		temp = torrent->getTamanioSubido();
		archivoDatos.write((char*) &temp,4);
		char estado = 0;

		if(torrent->estaCompleto()){
			estado = 1;
		}
		archivoDatos.seekp(OFFSET_COMPLETED,std::ios::beg);
		archivoDatos.write(&estado,1);

		archivoDatos.seekp(OFFSET_BITMAP_SIZE,std::ios::beg);
		temp = getBitmap()->getTamanioEnBytes();
		archivoDatos.write((char*) &temp,4);

		const char* bitmapAux = bitmap.getBitmap();
		archivoDatos.seekp(OFFSET_BITMAP,std::ios::beg);
		archivoDatos.write(bitmapAux, bitmap.getTamanioEnBytes());
		archivoDatos.close();
	}
}

unsigned int FileManager::getTamanio() {
	return bytesTotales;
}

Bitmap* FileManager::getBitmap() {
	return &bitmap;
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

unsigned int FileManager::writeBlock(int index, int begin, int longitud,
		const char* block) {
	unsigned int bytes = 0;
	if (!bitmap.estaMarcada(index)) {

		int offset = (index * tamanioPieza + begin);
		descarga.seekp(offset); // se para en el offset inicial
		descarga.write(block, longitud); // escribe
		bytes = longitud; //TODO muestra todo lo que entra.
		if (verificarHashPieza(index)) {
			bitmap.marcarBit(index);
			std::cout<<"                                                PIEZA"<<index<<" COMPLETA"<<std::endl;
			if (descargaCompleta()) {
				throw AvisoDescargaCompleta();
			}
		}
	}
	return bytes;
}

bool FileManager::verificarHashPieza(unsigned int index) {
	unsigned int tamTemp = getTamanioPieza(index);
	char* pieza = new char[tamTemp];
	memset(pieza, 0, tamTemp);
	unsigned int offset = (index * tamanioPieza);
	descarga.seekg(offset,std::ios::beg);
	descarga.read(pieza, tamTemp);
	Sha1 sha1Encoder;
	std::string hashObtenido = sha1Encoder.codificar(pieza, tamTemp);
	std::string hashOriginal = sha1Encoder.salidaAstring( &hashPiezas[index*5] );
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

	unsigned int i=0;
	char ultimo;
	if (resto != 0) {
		ultimo  =  0xFF << (8-resto);
	}else{
		ultimo = 0xFF;
	}

	const char* miBitmap = bitmap.getBitmap();
	bool completo = true;
	//comparo byte a byte mi bitmap con el completo hasta que haya una diferencia o se termine
	while (completo && (i < tamBitmap-1)) {
		completo = (0xFF == (0XFF & miBitmap[i]));
		i++;
	}
	return (completo)?(ultimo == (miBitmap[i] & ultimo)):false;
}

void FileManager::descargaAarchivos() {

	bool multifile = (archivos.size() != 1);
	std::string urlbase = clienteTorrent->getConfiguracion()->getRutaDescargas();
	urlbase += nombreCarpeta; //nombre de carpeta o nombre de archivo en caso de torrent single file

	unsigned int acumulado = 0;
	std::list<Archivo*>::iterator itA = archivos.begin();
	unsigned int tamanio;
	if(multifile){
		mkdir(urlbase.c_str(),PERMISOS);
		chdir(urlbase.c_str()); // el directorio ./ pasa a ser dentro de la carpeta donde copio losarchivos
		while(itA != archivos.end()){
			tamanio = (*itA)->getTamanio();
			std::string restoUrl = "./";
			std::list<std::string>::iterator itPath = (*itA)->getPathList()->begin();
			std::list<std::string>::iterator archivo = --((*itA)->getPathList()->end());
			while(itPath != archivo){
				restoUrl += (*itPath);
				restoUrl += "/";
				mkdir(restoUrl.c_str(),PERMISOS); //creo cada subdirectorio
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
	unsigned int cantPiezas = (unsigned int) ((bytesTotales / tamanioPieza)	+ (((bytesTotales % tamanioPieza) == 0) ? 0 : 1));
	int resto = cantPiezas % 8;

	if(resto != 0){
		char* bitmapAux = noPidioNadie->getBitmap();
		unsigned int tam = noPidioNadie->getTamanioEnBytes();
		bitmapAux[tam] = (bitmapAux[tam] & (0xFF<<(8-resto)));
	}
	if( (retorno = noPidioNadie->getPrimeroLibre(index)) ){
		mapaPedidos.marcarBit(index);
		std::cout<< "          SE RESERVO LA PIEZA: "<<index<<std::endl;
	}
	llavePedidos.unlock();
	delete faltanDescargar;
	delete noPidioNadie;
	return retorno;
}

void FileManager::cancelarPedido(unsigned int index){
	llavePedidos.lock();
	mapaPedidos.desmarcarBit(index);
	std::cout<<"                           PIEZA "<<index<<" DE NUEVO"<<std::endl;
	llavePedidos.unlock();
}

void FileManager::vaciarMapaPedidos(){
	llavePedidos.lock();
	int tam = mapaPedidos.getTamanioEnBytes();
	mapaPedidos.inicializarBitmap(tam);
	llavePedidos.unlock();
}
