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
#include "AvisoDescargaCompleta.h"
#include "Bitmap.h"
#include "DatosParser.h"
#include "Mutex.h"
#include "Tipos.h"

class Torrent;
class ClienteTorrent;
class FileManager {

public:

	FileManager(ClienteTorrent* cliente,Torrent* torrent);

	virtual ~FileManager();

	ULINT getTamanio();

	UINT getTamanioPieza();

	Bitmap* getBitmap();

	/* devuelve la cantidad de archivos */
	UINT getCantArchivos();

	/* Devuelve un arreglo con el bloque pedido, quien lo solicita debe liberarlo*/
	char* readBlock(UINT index,UINT begin,UINT longitud);

	/*Recibe los Datos del Parser con la info del archivo .torrent y con el, inicializa el bitmap.*/
	bool inicializar(DatosParser* datos);

	/* Escribe en disco el bloque */
	/* Devuelve la cantidad de bytes escritos */
	UINT writeBlock(UINT index,UINT begin,UINT longitud,const char* block) throw(AvisoDescargaCompleta);

	bool verificarHashPieza(UINT index);

	/* devuelve un iterador sobre los archivos.  */
	std::list<Archivo*>::iterator getIteratorArchivos();

	/* devuelve un iterador al ultimo elemento */
	std::list<Archivo*>::iterator getEndArchivos();

	void guardarDatos();

	/*Hace el split del archivo descargas*/
	void descargaAarchivos();

	UINT getTamanioPieza(UINT  index);

	bool getPiezaAdescargar(UINT &index,Bitmap& mapaPeerRemoto);


	void cancelarPedido(UINT index);

	void vaciarMapaPedidos();

private:
	/*
	 * Los archivos se manejan como uno del tamaño total
	 * una vez que se completa la descarga se generan los
	 * archivos y carpetas correspondientes con la info
	 * obtenida del .torrent.
	 * */
	ClienteTorrent* clienteTorrent;
	Torrent* torrent;
	Bitmap bitmap;
	std::string  urlDatos;
	Bitmap mapaPedidos;
	Mutex llavePedidos;
	std::string nombreCarpeta;
	std::list<Archivo*> archivos; //informacion sobre los distintos archivos, sirve para partir la descarga al final.
	UINT tamanioPieza;
	ULINT bytesTotales; //tamnio total de los archivos a descargar
	std::fstream descarga; // file del tamanio del total de la descarga.
	UINT*  hashPiezas;

	bool crearArchivo(std::string path,ULINT tamanio);

	bool inicializarTamaniosYpiezas(DatosParser* datos);
	bool inicializarArchivosYdirectorios(DatosParser* datos);
	bool inicializarDatosYbitmap(DatosParser* datos);
	void inicializarBitmap();
	void inicializarDatos();
	void inicializarBitmapVacio();
	void recuperarBitmap();

	bool descargaCompleta();

	void copiar(ULINT desde, UINT cantidad, std::string destino);

};

#endif /* FILEMANAGER_H_ */
