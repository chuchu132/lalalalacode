#include <sstream>
#include "BencodeParser.h"

BencodeParser::BencodeParser(FILE*fp) {

    this->fp = fp;
    pos = 0;
    buf_lim = 0;
    ident = 0;

    diccionario = 0;
    contador = 1;   
}

BencodeParser::~BencodeParser() {

}

void BencodeParser::procesar() {

    char caracter = verCaracterSiguiente();

    switch (caracter) {

        case 'd': parserDiccionario(fp);
            break;

        case 'l': parserLista(fp);
            break;

        case 'i': parserNumerico(fp);
            break;

        default: parserCadena(fp);

    }
}

void BencodeParser::parserDiccionario(FILE *fp) {

    compararCaracter('d');
    ident = 0;

    if (contador == 0) {
        diccionario++;
    }

    while (verCaracterSiguiente() != 'e') {

        parserCadena(fp);

        procesar();
    }

    compararCaracter('e');

    if (contador == 0) {
        diccionario--;       
    }

    if (diccionario == 0) {
       
        datos.setOffsetFin(ftell(fp)-buf_lim+pos+1 );
        contador = 1;
        diccionario = -1;
    }
}

void BencodeParser::parserLista(FILE*fp) {

    compararCaracter('l');
    ident = 1;
    while (verCaracterSiguiente() != 'e')
        procesar();

    compararCaracter('e');

}

void BencodeParser::parserNumerico(FILE *fp) {

    compararCaracter('i');
    long val = 0;
    while (isdigit(verCaracterSiguiente()))
        val = val * 10 + (obtenerCaracter() - '0');          
    compararCaracter('e');
    std::stringstream cadena;
    cadena << val;
    datos.agregarDato(cadena.str().c_str(), cadena.str().length() + 1);

}

void BencodeParser::parserCadena(FILE *fp) {
    int len = 0;

    while (isdigit(verCaracterSiguiente()))
        len = len * 10 + (obtenerCaracter() - '0');
    
    compararCaracter(':');
    char *s = new char[len + 1];
    int i;

    for (i = 0; i < len; ++i) {
        s[i] = obtenerCaracter();        
    }
    s[len] = '\0';
    datos.agregarDato(s, len + 1);

    if (!strcmp(s, "info")) {
        datos.setOffsetInfoHash(pos);
        contador = 0;
    }
    delete[] s;
}

void BencodeParser::cargarBuffer() {
    buf_lim = fread(buf, 1, BUFSIZE, fp); 
    pos = 0;
}

void BencodeParser::verificarBuffer() {
    if (pos >= buf_lim)
        cargarBuffer();
}

char BencodeParser::verCaracterSiguiente() {
    verificarBuffer();

    return buf[pos];
}

char BencodeParser::obtenerCaracter() {
    verificarBuffer();
    return buf[pos++];
}

void BencodeParser::compararCaracter(char c) {
    if (obtenerCaracter() != c) {
        std::cerr << "ERROR AL PARSEAR .torrent " << std::endl;
        exit(1);
    }
}

DatosParser* BencodeParser::salidaParser() {
    DatosParser* salida = new DatosParser();
    datos.primero();
    while (!datos.final()) {
        salida->agregarDato(datos.obtenerDato(), datos.obtenerLongitudDato());
        datos.siguiente();
    }
    
    salida->setOffsetInfoHash(datos.getOffsetInfoHash());
    salida->setOffsetFin(datos.getOffsetFin());
    return salida;
}



