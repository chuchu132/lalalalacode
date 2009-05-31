
#include "BencodeParser.h"

BencodeParser::BencodeParser(FILE*fp) {

    this->fp = fp;
    pos = 0;
    buf_lim = 0;
    ident = 0;
   
}

BencodeParser::~BencodeParser() {

    fclose(fp);
}

void BencodeParser::procesar() {

    char caracter = verCaracterSiguiente();

    switch (caracter) {

        case 'd': parserDiccionario();
            break;

        case 'l': parserLista();
            break;

        case 'i': parserNumerico();
            break;

        default: parserCadena();

    }
}

void BencodeParser::parserDiccionario() {

    compararCaracter('d');
    ident = 0;
    while (verCaracterSiguiente() != 'e') {

        parserCadena();
       
        procesar();
    }
   
    compararCaracter('e');

}

void BencodeParser::parserLista() {

    compararCaracter('l');
    ident = 1;
    while (verCaracterSiguiente() != 'e') 
        procesar();
    
    compararCaracter('e');

}

void BencodeParser::parserNumerico() {

    compararCaracter('i');
    long val = 0;
    while (isdigit(verCaracterSiguiente()))
        val = val * 10 + (obtenerCaracter() - '0');
    compararCaracter('e');
    identacion(ident);
    std::cout << val << std::endl << std::endl;


}

void BencodeParser::parserCadena() {


    int len = 0;
    
    while (isdigit(verCaracterSiguiente()))
        len = len * 10 + (obtenerCaracter() - '0');

    
    compararCaracter(':');
    char *s = new char[len + 1];
    int i;

    for (i = 0; i < len; ++i)
           s[i] = obtenerCaracter();
    s[len] = 0;
    identacion(ident);
    std::cout << s << std::endl << std::endl;

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
        std::cout << "ERROR " << std::endl;
        exit(1);
    }
}

void BencodeParser::identacion(int indent) {
    int i;
    for (i = 0; i < indent; ++i)
        std::cout << "   ";
}
