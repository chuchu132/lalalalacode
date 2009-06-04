/*
 * ParserCgi.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */
#include "ParserCgi.h"
#include <string>

ParserCgi::ParserCgi() {}

ParserCgi::~ParserCgi() {}

/*Codifica un string en URL Encoding */
std::string ParserCgi::codificar(const std::string original){
	std::string codificado = "";
	for (unsigned int i = 0; i < original.length(); ++i) {
		if (hayQueCodificarlo(original.c_str()[i])){
			codificado += intAhexaNN(original.c_str()[i]);
		}else{
			codificado += original.c_str()[i];
		}
	}
	return codificado;
}

/*Codifica un string en URL Encoding */
std::string ParserCgi::decodificar(const std::string codificado){
	std::string decodificado = "";
	char aux[3];
	memset(aux,0,3);
	for (unsigned int i = 0; i < codificado.length(); i++) {
		if(codificado.c_str()[i] == '%'){
			codificado.copy(aux,2,i+1);
			i+=2;
			decodificado += hexaNNaChar(aux);
		}
		else{
			decodificado += codificado.c_str()[i];
		}
	}
	return decodificado;
}

bool ParserCgi::hayQueCodificarlo(const char caracter){
	return !(('0'<=caracter && caracter<='9')||
			('a'<=caracter && caracter<='z')||
			('A'<=caracter && caracter<='Z')||
			caracter==',' || caracter=='-' ||
			caracter=='.' || caracter=='~' ||
			caracter=='_');
}
/*El numero ingresado debe estar en el rango [0,15]*/
char ParserCgi::intAhexaChar(const int numero){
	if(0 <= numero && numero<=9){
		return (numero + 48);
	}
	if(10<=numero && numero<=15){
		return (numero + 55);
	}
	return 0;
}
/*Convierte un numero [0,255] a %nn */
std::string ParserCgi::intAhexaNN(const int numero){
	int primero = (int)(numero / 16);
	int segundo = (numero % 16);
	std::string resultado = "%";
	resultado += intAhexaChar(primero);
	resultado += intAhexaChar(segundo);
	return resultado;
}

int ParserCgi::hexaCharAint(char hexa){
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

char ParserCgi::hexaNNaChar(std::string nn){
	int resultado = 16 * hexaCharAint(nn[0]);
	resultado += hexaCharAint(nn[1]);
	return resultado;
}

//std::string ParserCgi::getHash(char* handshakeMsj,int longitud){
//	handshakeMsg.find()
//TODO implementar}
