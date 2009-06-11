/*
 * TestParserCgi.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestParserCgi.h"
#include <iostream>
#include <cstring>

TestParserCgi::TestParserCgi() {
	original = "Hola como estas?. Todo Bien!!! Igual = Rango [3;6]";
	codificado = "Hola%20como%20estas%3F.%20Todo%20Bien%21%21%21%20Igual%20%3D%20Rango%20%5B3%3B6%5D";
}

TestParserCgi::~TestParserCgi() {}

void TestParserCgi::run(){
	TestCodificar();
	TestDecodificar();
}
void TestParserCgi::TestCodificar(){
	std::string aux = original;
	assert(codificado.compare(parser.codificar(original.c_str(),original.length()))==0,"El codificado con el ParserCgi debe ser igual al Harcodeado.");
}
void TestParserCgi::TestDecodificar(){
	char* salida = NULL;
	int tamanio = 0;
	parser.decodificar(codificado,&salida,tamanio);
	assert(memcmp(original.c_str(),salida,tamanio)==0,"El decodificado con el ParserCgi debe ser igual al Harcodeado.");
	delete[] salida;

}
