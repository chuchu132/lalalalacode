/*
 * File:   BencodeParser.h
 *
 * Created on 30 de mayo de 2009, 23:25
 */

#ifndef _BENCODEPARSER_H
#define	_BENCODEPARSER_H
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "DatosParser.h"
#include "Constantes.h"
#include "Sha1.h"
#include <sstream>
#include <string>

using namespace std;

class BencodeParser {
private:

	char buf[BUFSIZE]; //Almacena las cadenas temporarias
	int pos; //posicion dentro del buffer
	int buf_lim; //limite del buffer

	//FILE *fp; //File pointer al archivo .torrent que se esta parseando
	int ident; //Atributo para visualizacion por pantalla

	DatosParser datos;//objeto que almacena los datos obtenidos del parser

	int contador;
	int diccionario;

	int offsetInfoHash;//Marca el offset inicial del diccionario info
	int offsetFin;//Marca el final del diccionario info
	unsigned info_hash[5]; //Mensaje digerido del info_hash como lo devuelve el sha1

	unsigned int marcaFinHash;
    std::stringbuf is;

public:

	//Constructor
	BencodeParser(const char * cadena,unsigned longitud);

	//Destructor
	~BencodeParser();

	//Proceso del parser
	bool procesar();

	//Parser para el tipo diccionario
	void parserDiccionario();

	//Parser para el tipo lista
	void parserLista();

	//Parser para los datos numericos
	void parserNumerico();

	//Parser para las cadenas de caracteres
	void parserCadena();

	//Realiza la Carga del buffer
	void cargarBuffer();

	//Verifica si el buffer esta listo
	void verificarBuffer();

	//Verifica cual es el proximo caracter en el buffer
	char verCaracterSiguiente();

	//Obtiene el proximo caracter del buffer
	char obtenerCaracter();

	//Verifica que el caracter sea un match con el que se le pasa como parametro
	void compararCaracter(char c);

	//Retorna la salida del parser
	DatosParser* salidaParser();

	//Obtiene el info_hash del .torrent y lo guarda en mensajeInfoHash
	void procesarInfoHash();

};

#endif	/* _BENCODEPARSER_H */

