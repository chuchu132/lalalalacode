#include <sstream>
#include <cstring>
#include <cstdio>
#include "BencodeParser.h"

BencodeParser::BencodeParser(const char * url) {

	char *cadena;
    unsigned longitud;
    cadena=archivoAString (url,&longitud);

	is.sputn(cadena,longitud);
	inicializar();
	if (cadena!=NULL)
	delete []cadena;

}

BencodeParser::BencodeParser(const char * cadena,int longitud) {

	is.sputn(cadena,longitud);
	inicializar();

}


BencodeParser::~BencodeParser() {

}

void BencodeParser::inicializar (){

	is.pubseekpos(0);
	pos = 0;
	buf_lim = 0;
	ident = 0;
	diccionario = 0;
	contador = 1;

	offsetInfoHash=0;
	offsetFin=0;
	marcaFinHash=0;

}

bool BencodeParser::procesar() {

		char caracter = verCaracterSiguiente();

		switch (caracter) {

		case 'd':
			parserDiccionario();
			break;

		case 'l':
			parserLista();
			break;

		case 'i':
			parserNumerico();
			break;

		default:
			parserCadena();

		}

	return true;
}
void BencodeParser::parserDiccionario() {

	compararCaracter('d');
	ident = 0;

	if (contador == 0) {
		diccionario++;
	}

	while (verCaracterSiguiente() != 'e') {

		parserCadena();

		procesar();
	}

	compararCaracter('e');

	if (contador == 0) {
		diccionario--;
	}

	if (diccionario == 0) {

		offsetFin=  marcaFinHash-buf_lim+pos+1;
		procesarInfoHash();
		contador = 1;
		diccionario = -1;
	}
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
	unsigned int val = 0;
	while (isdigit(verCaracterSiguiente()))
		val = val * 10 + (obtenerCaracter() - '0');
	compararCaracter('e');
	std::stringstream cadena;
	cadena << val;
	datos.agregarDato(cadena.str().c_str(), cadena.str().length() + 1);

}

void BencodeParser::parserCadena() {
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
		offsetInfoHash = pos;
		contador = 0;
	}
	delete[] s;
}

void BencodeParser::cargarBuffer() {
	buf_lim = is.sgetn(buf,BUFSIZE);
	marcaFinHash+=buf_lim;
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
	salida->primero();
	return salida;
}

void BencodeParser::procesarInfoHash() {

	Sha1 sha;
    int i, pos = 0;
	int vuelta=marcaFinHash-buf_lim+pos+1;

	is.pubseekoff(offsetInfoHash-1,ios_base::beg, ios_base::in | ios_base::out);

	i=offsetInfoHash;

	char* buffer = new char[offsetFin - offsetInfoHash
			+ 1];

	do {
		buffer[pos] = is.snextc();
		i++;
		pos++;
	} while (i < offsetFin - 1);
	//Inicializo el sha1
	sha.inicializacion();

	//Ingreso la cadena a calcularle el sha1
	sha.entrada(buffer,offsetFin - offsetInfoHash - 1);

	//Obtengo la salida del sha1 en el mensajeDigerido
	sha.salida(info_hash);

	datos.agregarDato("info_hash",10);
	datos.agregarDato((char*)info_hash,LEN_SHA1);

    is.pubseekoff(vuelta,ios_base::beg, ios_base::in | ios_base::out);

	delete[] buffer;
}

char* BencodeParser::archivoAString(const char *url,  unsigned *tam){

    char *salida=NULL;

    FILE *fp = fopen(url, "r");
	if (fp!=NULL){

		fseek(fp,0,SEEK_END);
		*tam=ftell(fp);
		salida=new char[*tam+1];
		fseek(fp,0,SEEK_SET);
		fread(salida,1,*tam,fp);
		fclose(fp);
	}
    return salida;
}

