
#include "BencodeParser.h"

BencodeParser::BencodeParser(FILE*fp) {

    this->fp = fp;
    pos = 0;
    buf_lim = 0;
    ident = 0;
    salida = 1;
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
    while (verCaracterSiguiente() != 'e') {

        parserCadena(fp);
        if (salida == 0)break;
        procesar();
    }
    if (salida == 1)
        compararCaracter('e');

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
    identacion(ident);
    std::cout << val << std::endl << std::endl;


}

void BencodeParser::parserCadena(FILE *fp) {


    int len = 0;


    while (isdigit(verCaracterSiguiente()))
        len = len * 10 + (obtenerCaracter() - '0');

    if (salida == 1) {
        compararCaracter(':');
        char *s = new char[len + 1];
        int i;

        for (i = 0; i < len; ++i)
            s[i] = obtenerCaracter();
        s[len] = 0;
        identacion(ident);
        std::cout << s << std::endl << std::endl;


        if (!strcmp(s, "pieces")) {
            int len = 0;
            while (isdigit(verCaracterSiguiente()))
                len = len * 10 + (obtenerCaracter() - '0');
            compararCaracter(':');
            identacion(ident);
            std::cout << len << std::endl;
            salida = 0;


	    // Imprime caracteres que se encuentran al final del archivo .torrent
            // En teoria son los caracteres del sha1 , pero no son legibles para la impresion almenos en las pruebas
	    // Si se descomenta esta seccion del codigo los muestra por pantalla
            /*
	      int posActual,posFinal;
	      posActual=ftell(fp);
              fseek(fp,0,SEEK_END);
              posFinal=ftell(fp);
              fseek(fp,posActual,SEEK_SET);

              char* cadenaSha1= new char[len+1];
              fread(cadenaSha1,1,len,fp);

              std::cout<< " Caracteres supuestos de 'SHA1' : "<<std::endl;
              std::cout<<cadenaSha1<<std::endl;
              delete []cadenaSha1;*/
        }
        delete []s;
    }

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

std::list<Archivo*> BencodeParser::getListaArchivos(){
	return listaArchivos;
}

int BencodeParser::getTamanioPiezas(){
	return tamanioPiezas;
}
