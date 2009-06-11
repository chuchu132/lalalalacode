/*
 * ParserCgi.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef PARSERCGI_H_
#define PARSERCGI_H_

#include <string>

class ParserCgi {
public:
	ParserCgi();
	virtual ~ParserCgi();
	//TODO cambiar por codificar(char* original,int tam)
	std::string codificar(std::string original);
	std::string decodificar(std::string codificado);
	bool hayQueCodificarlo(char caracter);
	char intAhexaChar(int numero);
	int hexaCharAint(char hexa);
	std::string  intAhexaNN(int numero);
	char hexaNNaChar(std::string NN);
};

#endif /* PARSERCGI_H_ */
