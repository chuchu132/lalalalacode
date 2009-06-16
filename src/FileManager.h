/*
 * FileManager.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

/*
 * Este objeto es el encargado de llevar el control
 * de las partes descargadas y de las que falten descargar.
 * Tambien coordina el acceso al archivo que se
 * esta descargando.
 */

#include <list>
#include <fstream>
#include <string>
#include "Archivo.h"
#include "Bitmap.h"
#include "DatosParser.h"
#include "Mutex.h"

class ClienteTorrent;
class FileManager {

public:

	FileManager(ClienteTorrent* cliente);

	virtual ~FileManager();

	unsigned int getTamanio();

	unsigned int getTamanioPieza();

	Bitmap& getBitmap();

	/* devuelve la cantidad de archivos */
	unsigned int getCantArchivos();

	/* Devuelve un arreglo con el bloque pedido, quien lo solicita debe liberarlo*/
	char* readBlock(int index,int begin,int longitud);

	/*Recibe los Datos del Parser con la info del archivo .torrent y con el, inicializa el bitmap.*/
	bool inicializar(DatosParser* datos);

	/* Escribe en disco el bloque */
	void writeBlock(int index,int begin,int longitud,const char* block);

	bool verificarHashPieza(int index);

	/* devuelve un iterador sobre los archivos.  */
	std::list<Archivo*>::iterator getIteratorArchivos();

	/* devuelve un iterador al ultimo elemento */
	std::list<Archivo*>::iterator getEndArchivos();

	void guardarBitmap(std::string urlBitmap);
	/*Hace el split del archivo descargas*/
	void descargaAarchivos();

	unsigned int getTamanioPieza(unsigned int  index);

	bool getPiezaAdescargar(unsigned int &index,Bitmap& mapaPeerRemoto);

private:
	/*
	 * Los archivos se manejan como uno del tamaño total
	 * una vez que se completa la descarga se generan los
	 * archivos y carpetas correspondientes con la info
	 * obtenida del .torrent.
	 * */
	ClienteTorrent* clienteTorrent;
	Bitmap bitmap;
	std::string nombreCarpeta;
	std::list<Archivo*> archivos; //informacion sobre los distintos archivos, sirve para partir la descarga al final.
	unsigned int tamanioPieza;
	unsigned int bytesTotales; //tamnio total de los archivos a descargar
	std::fstream descarga; // file del tamanio del total de la descarga.
	unsigned int*  hashPiezas;

	bool crearArchivo(std::string path,unsigned int tamanio);

	bool inicializarTamaniosYpiezas(DatosParser* datos);
	bool inicializarArchivosYdirectorios(DatosParser* datos);
	bool inicializarDatosBitmap(DatosParser* datos);
	void inicializarBitmap(std::string urlBitmap);

	bool descargaCompleta();

	void copiar(unsigned int desde, unsigned int cantidad, std::string destino);

};

#endif /* FILEMANAGER_H_ */
