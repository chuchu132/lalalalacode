/*
 * TestParserCgi.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestParserCgi.h"
#include <iostream>

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
	Test::assert(codificado.compare(parser.codificar(original))==0,"El codificado con el ParserCgi debe ser igual al Harcodeado.");
}
void TestParserCgi::TestDecodificar(){
	Test::assert(original.compare(parser.decodificar(codificado))==0,"El decodificado con el ParserCgi debe ser igual al Harcodeado.");
}
