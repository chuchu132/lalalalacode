#include "datosParser.h"

datosParser::datosParser() {


}

datosParser::~datosParser() {

    for (it = datos.begin(); it != datos.end(); it++)
        delete [] * it;
}

void datosParser::agregarDato(char *cadena) {

    datos.push_front(cadena);

}

char* datosParser::obtenerDato() {
    return *it;
}

unsigned datosParser::obtenerLongitudDato() {

    return strlen(*it);
}

void datosParser::primero() {

    it = datos.begin();
}

void datosParser::siguiente() {
    it++;
}

void datosParser::anterior() {

    if (it != datos.begin())
        it--;

}

bool datosParser::final() {

    if (it != datos.end())
        return FALSE;
    else
        return TRUE;

}

void datosParser::ordenar() {

    datos.reverse();

}




