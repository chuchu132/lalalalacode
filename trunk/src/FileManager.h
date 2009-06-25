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
#include <fstream>
#include <list>
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
	/*Retorna el tamanio en bytes del total de la descarga*/
	ULINT getTamanio();

	UINT getTamanioPieza();

	UINT getTamanioPieza(UINT  index);

	Bitmap* getBitmap();

	/* devuelve la cantidad de archivos */
	UINT getCantArchivos();

	/* Devuelve un arreglo con el bloque pedido, quien lo solicita debe liberarlo*/
	char* readBlock(UINT index,UINT begin,UINT longitud);

	/*
	 * Recibe los Datos del Parser con la info del archivo .torrent y con el, inicializa el bitmap.
	 *
	 * Se pide el info_hash del .torrent y se trata de abrir un archivo
	 * con ese info_hash como nombre dentro de la carpeta donde se guardan
	 * las descargas incompletas, sino existe se crea del tamaño TOTAL
	 * que indica el torrent, de existir el archivo, se trata de leer el
	 * bitmap, bytes subidos y bajados desde un archivo con info_hash por
	 * nombre y extension .data .
	 * Si no existe el archivo .data pero se encuentra una descarga
	 * incompleta se verificar cada pieza del archivo incompleto y se genera
	 * el bitmap.
	 * Inicializar el resto de los campos segun corresponda con la
	 * info obtenida del .torrent y del archivo descargado parcialmente.
	 * */
	bool inicializar(DatosParser* datos);

	/* Escribe en disco el bloque */
	/* Devuelve la cantidad de bytes escritos */
	UINT writeBlock(UINT index,UINT begin,UINT longitud,const char* block) throw(AvisoDescargaCompleta);

	/*Calcula el hash a la pieza index y luego lo compara con el hash recuperado del .torrent*/
	bool verificarHashPieza(UINT index);

	/* devuelve un iterador sobre los archivos.  */
	std::list<Archivo*>::iterator getIteratorArchivos();

	/* devuelve un iterador al ultimo elemento */
	std::list<Archivo*>::iterator getEndArchivos();

	/*Se guarda el estado de una descarga.*/
	void guardarDatos();

	/*Hace el split del archivo descargas*/
	void descargaAarchivos();

	/* De las piezas que me ofrece el peer remoto, devuelve el index
	 * a la primera que me falta descargar y que no esta descargando
	 * ningun peer local.
	 */
	bool getPiezaAdescargar(UINT &index,Bitmap& mapaPeerRemoto);

	/*Reinicia el bitmap donde se "reservan" las piezas a descargar.*/
	void vaciarMapaPedidos();

	void cancelarPedido(UINT index);


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

	/*Crea un archivo en el path dado y reserva el espacio en disco.*/
	bool crearArchivo(std::string path,ULINT tamanio);

	/* ------- Metodos que forman parte del inicializar publico --------*/

	/*Setea tamanio de archivo y piezas*/
	bool inicializarTamaniosYpiezas(DatosParser* datos);

	/*Inicializa la lista de Archivos con tamanios, nombres de carpeta, etc*/
	bool inicializarArchivosYdirectorios(DatosParser* datos);

	/*Recuperan los datos almacenados en disco, que permiten continuar
	 * con una descarga pendiente.*/
	bool inicializarDatosYbitmap(DatosParser* datos);

	void inicializarBitmap();

	/*Inicializa cantidad de bytes descargados y subidos*/
	void inicializarDatos();

	/*Inicializa el bitmap en cero*/
	void inicializarBitmapVacio();

	/*Genera un bitmap verificando el hash de cada una de las piezas*/
	void recuperarBitmap();

	/*Retorna si la descarga esta completa.*/
	bool descargaCompleta();

	/*Copia una parte de un archivo en el path de destino */
	void copiar(ULINT desde, UINT cantidad, std::string destino);

};

#endif /* FILEMANAGER_H_ */
