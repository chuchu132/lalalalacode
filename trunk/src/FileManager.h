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
#include "Archivo.h"
#include "BencodeParser.h"
#include "Bitmap.h"

class FileManager {

public:

	FileManager();

	virtual ~FileManager();

	int getTamanio();

	Bitmap& getBitmap();

	/* Devuelve un arreglo con el bloque pedido, quien lo solicita debe liberarlo*/
	char* readBlock(int index,int begin,int longitud);

	/*Recibe un Parser con la info del archivo .torrent y con el, inicializa el bitmap.*/
	void inicializar(BencodeParser* parser);

	/* Escribe en disco el bloque */
	void writeBlock(int index,int begin,int longitud,char* block);

	bool verificarHashPieza(int index);

	/* devuelve un iterador sobre los archivos. VER si poner const */
	std::list<Archivo*>::iterator getIteratorArchivos();

	/* devuelve un iterador al ultimo elemento */
	std::list<Archivo*>::iterator getEndArchivos();

private:
	/*TODO
	 * Los archivos se manejan como uno del tamaño total
	 * una vez que se completa la descarga se generan los
	 * archivos y carpetas correspondientes con la info
	 * obtenida del .torrent.
	 * */
	Bitmap bitmap;
	//TODO agregar una lista de mutex y bloquear escritura por archivo.
	std::list<Archivo*> archivos;
	int tamanioPieza;
	int bloquesXPieza;
	int bytesTotales; //tamnio total de los archivos a descargar VER no convendria que sea unsigned??????
	std::fstream descarga; // file del tamanio del total de la descarga.
	std::string hashPiezas;
};

#endif /* FILEMANAGER_H_ */
