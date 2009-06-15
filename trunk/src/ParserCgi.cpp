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

ParserCgi::ParserCgi() {}

ParserCgi::~ParserCgi() {}

/*Codifica un string en URL Encoding */
std::string ParserCgi::codificar(const char* original,int tam){
	std::string codificado = "";
	for (int i = 0; i < tam; ++i) {
		if (hayQueCodificarlo(original[i])){ //TODO ver si es lo mismo q codifique todos los caracteres
			codificado += intAhexaNN(original[i]);
		}else{
			codificado += original[i];
		}
	}

	return codificado;
}

/*Deodifica un string en URL Encoding y lo pasa a ASCII*/
void ParserCgi::decodificar(std::string codificado,char** salida, int& tamanio){
	*salida = new char[codificado.length()];
	memset(*salida,0,codificado.length());
		int index = 0;
		char aux[3];
		memset(aux,0,3);
		for (unsigned int i = 0; i < codificado.length(); i++) {
			if(codificado.c_str()[i] == '%'){
				codificado.copy(aux,2,i+1);
				i+=2;
				(*salida)[index] = hexaNNaChar(aux);
			}
			else{
				(*salida)[index] = codificado.c_str()[i];
			}
			index++;
		}

		tamanio = index;
}


bool ParserCgi::hayQueCodificarlo(const char caracter){
	return !(('0'<=caracter && caracter<='9')||
			('a'<=caracter && caracter<='z')||
			('A'<=caracter && caracter<='Z'));
}
/*El numero ingresado debe estar en el rango [0,15]*/
unsigned char ParserCgi::intAhexaChar(const unsigned char numero){
	if( numero<=9 ){
		return (numero + 48);
	}
	if(10<=numero && numero<=15){
		return (numero + 55);
	}
	return 0;
}
/*Convierte un numero [0,255] a %nn */
std::string ParserCgi::intAhexaNN(const unsigned char numero){
	unsigned char primero = (unsigned char)(numero / 16);
	unsigned char segundo = (numero % 16);
	std::string resultado = "%";
	resultado += intAhexaChar(primero);
	resultado += intAhexaChar(segundo);

	return resultado;
}

char ParserCgi::hexaCharAint(char hexa){
	if('0'<=hexa && hexa<='9'){
		return (hexa - 48);
	}
	if('A'<=hexa && hexa<='F') {
		return (hexa - 55);
	}
	if('a'<=hexa && hexa<='f') {
		return (hexa - 87);
	}
	return 0;
}

unsigned char ParserCgi::hexaNNaChar(std::string nn){
	unsigned char resultado = 16 * hexaCharAint(nn[0]);
	resultado += hexaCharAint(nn[1]);
	return resultado;
}
