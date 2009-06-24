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

	int ident; //Atributo para visualizacion por pantalla

	DatosParser datos;//objeto que almacena los datos obtenidos del parser

	int contador;
	int diccionario;

	int offsetInfoHash;//Marca el offset inicial del diccionario info
	int offsetFin;//Marca el final del diccionario info
	unsigned info_hash[5]; //Mensaje digerido del info_hash como lo devuelve el sha1

	unsigned int marcaFinHash;
    std::stringbuf is;
    bool estado;
public:

	//Constructor
	BencodeParser(const char * url);

	//Constructor: parametros , Cadena a parsear y su longitud
	BencodeParser(const char * cadena,ULINT longitud);

	//Destructor
	~BencodeParser();

	//Inicializa los atributos del bencode
	void inicializar();

	//Proceso del parser
	bool procesar();

	//Parser para el tipo diccionario
	void parserDiccionario() throw (ExcepcionCaracterInvalido);

	//Parser para el tipo lista
	void parserLista() throw (ExcepcionCaracterInvalido);

	//Parser para los datos numericos
	void parserNumerico() throw (ExcepcionCaracterInvalido);

	//Parser para las cadenas de caracteres
	void parserCadena() throw (ExcepcionCaracterInvalido);

	//Realiza la Carga del buffer
	void cargarBuffer();

	//Verifica si el buffer esta listo
	void verificarBuffer();

	//Verifica cual es el proximo caracter en el buffer
	char verCaracterSiguiente();

	//Obtiene el proximo caracter del buffer
	char obtenerCaracter();

	//Verifica que el caracter sea un match con el que se le pasa como parametro
	void compararCaracter(char c) throw (ExcepcionCaracterInvalido);

	//Retorna la salida del parser
	DatosParser* salidaParser();

	//Obtiene el info_hash del .torrent y lo guarda en mensajeInfoHash
	void procesarInfoHash();

	char* archivoAString (const char *url,ULINT *longitud);

};

#endif	/* _BENCODEPARSER_H */

