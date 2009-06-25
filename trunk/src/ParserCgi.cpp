/*
 * ParserCgi.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */
#include "ParserCgi.h"
#include <string>
#include <string.h>
#include <iostream>

ParserCgi::ParserCgi() {
}

ParserCgi::~ParserCgi() {
}

std::string ParserCgi::codificar(const char* original, int tam) {
	std::string codificado = "";
	for (int i = 0; i < tam; i++) {
		if (hayQueCodificarlo(original[i])) {
			codificado += intAhexaNN(original[i]);
		} else {
			codificado += original[i];
		}
	}

	return codificado;
}

void ParserCgi::decodificar(std::string codificado, char** salida, int& tamanio) {
	*salida = new char[codificado.length()];
	memset(*salida, 0, codificado.length());
	int index = 0;
	char aux[3];
	memset(aux, 0, 3);
	for (unsigned int i = 0; i < codificado.length(); i++) {
		if (codificado.c_str()[i] == '%') {
			codificado.copy(aux, 2, i + 1);
			i += 2;
			(*salida)[index] = hexaNNaChar(aux);
		} else {
			(*salida)[index] = codificado.c_str()[i];
		}
		index++;
	}

	tamanio = index;
}

bool ParserCgi::hayQueCodificarlo(const char caracter) {
	return !(('0' <= caracter && caracter <= '9') || ('a' <= caracter
			&& caracter <= 'z') || ('A' <= caracter && caracter <= 'Z'));
}

/*El numero ingresado debe estar en el rango [0,15]*/
UCHAR ParserCgi::intAhexaChar(const UCHAR numero) {
	if (numero <= 9) {
		return (numero + 48);
	}
	if (10 <= numero && numero <= 15) {
		return (numero + 55);
	}
	return 0;
}

std::string ParserCgi::intAhexaNN(const UCHAR numero) {
	UCHAR primero = (UCHAR) (numero / 16);
	UCHAR segundo = (numero % 16);
	std::string resultado = "%";
	resultado += intAhexaChar(primero);
	resultado += intAhexaChar(segundo);

	return resultado;
}

char ParserCgi::hexaCharAint(char hexa) {
	if ('0' <= hexa && hexa <= '9') {
		return (hexa - 48);
	}
	if ('A' <= hexa && hexa <= 'F') {
		return (hexa - 55);
	}
	if ('a' <= hexa && hexa <= 'f') {
		return (hexa - 87);
	}
	return 0;
}

UCHAR ParserCgi::hexaNNaChar(std::string nn) {
	UCHAR resultado = 16 * hexaCharAint(nn[0]);
	resultado += hexaCharAint(nn[1]);
	return resultado;
}
