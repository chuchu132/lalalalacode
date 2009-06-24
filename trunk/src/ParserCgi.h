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

class ParserCgi {
public:
	ParserCgi();
	virtual ~ParserCgi();
	std::string codificar(const char* original,int tam);
	std::string decodificar(std::string codificado);
	void decodificar(std::string codificado,char** salida,int& tamanio);
	bool hayQueCodificarlo(char caracter);
	UCHAR intAhexaChar(UCHAR numero);
	char hexaCharAint(char hexa);
	std::string  intAhexaNN(const UCHAR numero);
	UCHAR hexaNNaChar(std::string NN);
};

#endif /* PARSERCGI_H_ */
