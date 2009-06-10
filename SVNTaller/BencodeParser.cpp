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

        datos.setOffsetFin(ftell(fp) - buf_lim + pos + 1);
        procesarInfoHash();
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

void BencodeParser::procesarInfoHash() {

    SHA1 sha;
    unsigned int i, pos = 0;
    unsigned int vuelta=ftell(fp);
    fseek(fp, datos.getOffsetInfoHash(), SEEK_SET);
    i = ftell(fp);
    char* buffer = new char[datos.getOffsetFin() - datos.getOffsetInfoHash() + 1];

    do {
        buffer[pos] = fgetc(fp);
        i++;
        pos++;
    } while (i < datos.getOffsetFin() - 1);

    //Inicializo el sha1
    sha.inicializacion();

    //Ingreso la cadena a calcularle el sha1
    sha.entrada(buffer, datos.getOffsetFin() - datos.getOffsetInfoHash() - 1);

    //Obtengo la salida del sha1 en el mensajeDigerido
    sha.salida(mensajeInfoHash);

    fseek(fp, vuelta , SEEK_SET);
    std::cout<<ftell(fp)<<std::endl;
    strcpy(info_hash, sha.sha1Abinario(sha.salidaAstring(mensajeInfoHash)));
  
    delete []buffer;
}

void BencodeParser::obtenerInfoHash(unsigned *mensaje) {

    for (int i = 0; i < 5; i++)
        mensaje[i] = mensajeInfoHash[i];
}

char* BencodeParser::getInfoHashBinario(){
    
    char *hash=new char[20*8];
    strcpy(hash,info_hash);
    
    return hash;  
}
