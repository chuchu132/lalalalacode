
#include "BencodeParser.h"

BencodeParser::BencodeParser(FILE*fp) {

    this->fp = fp;
    pos = 0;
    buf_lim = 0;
    ident = 0;
    datos=new datosParser();
}

BencodeParser::~BencodeParser() {

    fclose(fp);
    delete datos;
   
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
       
        procesar();
    }
   
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
   
    char *cadena=new char[BUFSIZE];
    sprintf (cadena,"%ld",val);
    datos->agregarDato(cadena);

}

void BencodeParser::parserCadena(FILE *fp) {


    int len = 0;
    
    while (isdigit(verCaracterSiguiente()))
        len = len * 10 + (obtenerCaracter() - '0');

    
    compararCaracter(':');
    char *s = new char[len + 1];
    int i;

    for (i = 0; i < len; ++i)
           s[i] = obtenerCaracter();
    s[len] = 0;
   
    datos->agregarDato(s);
  
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


datosParser* BencodeParser::salidaParser(){

   datosParser* salida=new datosParser();
   datos->primero();
   
   while (datos->final()){  
	
   	char *cadena= new char [datos->obtenerLongitudDato()+1];
        strcpy(cadena,datos->obtenerDato());
	salida->agregarDato(cadena);
        datos->siguiente();
   
   }
   return salida; 
}



