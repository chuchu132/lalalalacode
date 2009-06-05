/*
 * FileManager.h
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

/*
 * Este objeto va a ser el encargado de llevar el control
 * de las partes descargadas y de las que falten descargar.
 * Tambien va a coordinar, el acceso al archivo que se
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
	char* getBlock(int index,int begin,int longitud);

	/*Recibe un Parser con la info del archivo .torrent y con el, inicializa el bitmap.*/
	void inicializar(BencodeParser* parser);

	//TODO agregar metodo para escribir un bloque en una pieza de un archivo

private:

	Bitmap bitmap;
	//TODO agregar una lista de mutex y bloquear escritura por archivo.
	std::list<Archivo*> archivos;
	int tamanioPieza;
	int bloquesXPieza;
	int bytesTotales; //tamnio total de los archivos a descargar
};

#endif /* FILEMANAGER_H_ */
