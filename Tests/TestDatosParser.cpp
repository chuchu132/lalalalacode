/*
 * TestDatosParser.cpp
 *
 *  Created on: 09/06/2009
 *      Author: ale
 */

#include "TestDatosParser.h"
#include <cstring>
#include <string>

TestDatosParser::TestDatosParser() {

}

TestDatosParser::~TestDatosParser() {

}

void TestDatosParser::setup(){

	datos.agregarDato("campo1",7);
	datos.agregarDato("datosDelCampo1",15);
	datos.agregarDato("info_hash",10);
	int numero = 1234;
	datos.agregarDato((char*)&numero,4);
}

void TestDatosParser::run(){

	datos.primero();
	char* dato = datos.obtenerDato();
	assert(memcmp(dato,"campo1",6)==0,"Debe dar OK si el dato recuperado es \"campo1\"");
	assert(datos.obtenerLongitudDato()==7,"Debe dar OK si la longitud del dato recuperado es 7");

	char* datoChar = NULL;
	int longitud = 0;
	bool result = datos.obtenerDatoPorNombre("campo1",datoChar,longitud);
	assert(result,"Debe encontrar el dato");
	assert(longitud==15,"El dato encotrado debe medir 15 bytes");
	assert(memcmp(datoChar,"datosDelCampo1",longitud)==0,"Debe dar OK si el dato recuperado es el de campo1");


}

