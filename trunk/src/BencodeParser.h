/*
 * File:   BencodeParser.h
 *
 * Created on 30 de mayo de 2009, 23:25
 */

#ifndef _BENCODEPARSER_H
#define	_BENCODEPARSER_H

#include "ExcepcionCaracterInvalido.h"
#include "DatosParser.h"
#include "Constantes.h"
#include "Sha1.h"
#include <sstream>
#include "Tipos.h"


class BencodeParser {
private:

	char buf[BUFSIZE]; //Almacena las cadenas temporarias
	int pos; //posicion dentro del buffer
	int buf_lim; //limite del buffer

	DatosParser datos;//objeto que almacena los datos obtenidos del parser

	int contador;// habilita al atributo diccionario para que indique cuando se abre y cierra un diccionario
	int diccionario;//flag que indica cuando un diccionario se cierra y se abre

	int offsetInfoHash;//Marca el offset inicial del diccionario info
	int offsetFin;//Marca el final del diccionario info
	unsigned info_hash[5]; //Mensaje digerido del info_hash como lo devuelve el sha1

	unsigned int marcaFinHash;
    std::stringbuf is;
    bool estado;
public:

	/*Constructor*/
	BencodeParser(const char * url);

	/*Constructor: parametros , Cadena a parsear y su longitud*/
	BencodeParser(const char * cadena,ULINT longitud);

	/*Destructor*/
	~BencodeParser();

	/*Inicializa los atributos del bencode*/
	void inicializar();

	/*Proceso del parser*/
	bool procesar();

	/*Parser para los diccionarios*/
	void parserDiccionario() throw (ExcepcionCaracterInvalido);

	/*Parser para las lista*/
	void parserLista() throw (ExcepcionCaracterInvalido);

	/*Parser para los datos numericos*/
	void parserNumerico() throw (ExcepcionCaracterInvalido);

	/*Parser para las cadenas de caracteres*/
	void parserCadena() throw (ExcepcionCaracterInvalido);

	/*Realiza la Carga del buffer*/
	void cargarBuffer();

	/*
	 * Verifica si el buffer no esta en el limite.
	 * En caso que este en el limite, lo carga con los proximos caracteres a procesar
	 */
	void verificarBuffer();

	/*
	 * Verifica cual es el proximo caracter en el buffer y lo devuelve	 *
	 * No el atributo posicion asociado al buffer.
	 */
	char verCaracterSiguiente();

	/*
	 * Obtiene el proximo caracter dentro del buffer.
	 * Modifica el atributo posicion asociado al buffer
	 */
	char obtenerCaracter();

	/*
	 * Verifica que el caracter sea un match con el que se le pasa como parametro
	 * Lanza una excepcion en caso de que sea invalido el caracter
	 */
	void compararCaracter(char c) throw (ExcepcionCaracterInvalido);

	/* Retorna la salida del parser*/
	DatosParser* salidaParser();

	/*
	 * Obtiene el info_hash del .torrent.
	 * Lo agrega al atributo "datos" anteponiendo el campo clave "info_hash"
	 */
	void procesarInfoHash();

	/*
	 * Pasa el contenido de un archivo .torrent a una cadena de caracteres
	 * Recibe como parametros la direccion del archivo
	 * Retorna la longitud de la cadena de salida en el parametro "longitud"
	 */
	char* archivoAString (const char *url,ULINT *longitud);

};

#endif	/* _BENCODEPARSER_H */

