#include "DatosParser.h"
#include <cstring>


DatosParser::DatosParser() {}

DatosParser::~DatosParser() {

	for (it = datos.begin(); it != datos.end(); it++)
		delete[] *it;
}

void DatosParser::agregarDato(const char *cadena, int longitud) {
	char* copiaCadena = new char[longitud];
	memcpy(copiaCadena, cadena, longitud);
	datos.push_back(copiaCadena);
	len.push_back(longitud);
}

char* DatosParser::obtenerDato() {
	return *it;
}

int DatosParser::obtenerLongitudDato() {

	return (*itLongitudes);
}

void DatosParser::primero() {

	it = datos.begin();
	itLongitudes = len.begin();
}

void DatosParser::siguiente() {
	it++;
	itLongitudes++;
}

void DatosParser::anterior() {

	if (it != datos.begin()) {
		it--;
		itLongitudes--;
	}

}

bool DatosParser::final() {
	return (it == datos.end());
}

bool DatosParser::irAetiqueta(std::string nombre){

	int lonConsulta = nombre.length();
	while (it != datos.end()) {
		if ((lonConsulta + 1) == *itLongitudes) {
			if (nombre.compare((*it)) == 0) {
				return true;
			}
		}
		it++;
		itLongitudes++;
	}
	primero();
	return false;
}

bool DatosParser::obtenerDatoPorNombre(std::string nombre, char** dato,
		int& longitud) {

	if (irAetiqueta(nombre)) {
		it++;
		itLongitudes++;
		longitud = (*itLongitudes);
		*dato = new char[longitud];
		memcpy(*dato, (*it), longitud);
		return true;
	}
	return false;
}

