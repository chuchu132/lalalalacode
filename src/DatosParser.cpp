#include "DatosParser.h"
#include <cstring>

DatosParser::DatosParser() {

}

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

bool DatosParser::obtenerDatoPorNombre(std::string nombre, char** dato,
		int& longitud) {
	list<char*>::iterator unit;
	list<int>::iterator unitLongitudes;
	unit = datos.begin();
	unitLongitudes = len.begin();
	bool encontro = false;

	int lonConsulta = nombre.length();

	while (!encontro && (unit != datos.end())) {
		std::cout << "longitud del elemento de la lista " << *unitLongitudes
				<< std::endl;
		if ((lonConsulta + 1) == *unitLongitudes) {
			if (nombre.compare((*unit)) == 0) {
				unit++;
				unitLongitudes++;
				longitud = (*unitLongitudes);
				*dato = new char[longitud];
				memcpy(*dato, (*unit), longitud);
				encontro = true;
			}
		}
		unit++;
		unitLongitudes++;
	}
	return encontro;
}

