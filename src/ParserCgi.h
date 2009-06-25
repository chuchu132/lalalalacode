/*
 * ParserCgi.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PARSERCGI_H_
#define PARSERCGI_H_

#include <string>
#include "Tipos.h"
/*
 * Se encarga de codificar una tira de bytes en URL Encode.
 */
class ParserCgi {
public:
	ParserCgi();
	virtual ~ParserCgi();

	/*Codifica un string en URL Encoding */
	std::string codificar(const char* original, int tam);

	/*Deodifica un string en URL Encoding y lo pasa a ASCII*/
	void decodificar(std::string codificado, char** salida, int& tamanio);

	bool hayQueCodificarlo(char caracter);

	/*  Dado un numero decimal  lo convierte en una
	 * letra que representa el numero en hexa
	 */
	UCHAR intAhexaChar(UCHAR numero);

	/*Dado un numero en hexa representado en un char, devuelve el valor decimal*/
	char hexaCharAint(char hexa);

	/*Convierte un numero [0,255] a %nn (url encode)*/
	std::string intAhexaNN(const UCHAR numero);

	/*Convierte un numero representado en %nn a un decimal*/
	UCHAR hexaNNaChar(std::string NN);
};

#endif /* PARSERCGI_H_ */
