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
	codificadoEntero = "%48%6F%6C%61%20%63%6F%6D%6F%20%65%73%74%61%73%3F%2E%20%54%6F%64%6F%20%42%69%65%6E%21%21%21%20%49%67%75%61%6C%20%3D%20%52%61%6E%67%6F%20%5B%33%3B%36%5D";
}

TestParserCgi::~TestParserCgi() {}

void TestParserCgi::run(){
	TestCodificar();
	TestDecodificar();
}
void TestParserCgi::TestCodificar(){
	std::string aux = original;
	assert(codificadoEntero.compare(parser.codificar(original.c_str(),original.length()))==0,"El codificado con el ParserCgi debe ser igual al Harcodeado.");
}
void TestParserCgi::TestDecodificar(){
	char* salida = NULL;
	int tamanio = 0;
	parser.decodificar(codificado,&salida,tamanio);
	assert(memcmp(original.c_str(),salida,tamanio)==0,"El decodificado con el ParserCgi debe ser igual al Harcodeado.");
	delete[] salida;

}
