#include <sstream>
#include <cstring>
#include "BencodeParser.h"

BencodeParser::BencodeParser(const char* url) {

	this->fp = fopen(url, "r");
	pos = 0;
	buf_lim = 0;
	ident = 0;
	diccionario = 0;
	contador = 1;
}

BencodeParser::~BencodeParser() {
	fclose(fp);
}

bool BencodeParser::procesar() {
	if (fp != NULL) {
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
	} else {
		return false;
	}
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

		offsetFin = (ftell(fp) - buf_lim + pos + 1);
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
	salida->primero();
	return salida;
}

void BencodeParser::procesarInfoHash() {

	Sha1 sha;
	unsigned int i, pos = 0;
	unsigned int vuelta = ftell(fp);
	fseek(fp, offsetInfoHash, SEEK_SET);
	i = ftell(fp);
	char* buffer = new char[offsetFin - offsetInfoHash
			+ 1];

	do {
		buffer[pos] = fgetc(fp);
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


	fseek(fp, vuelta, SEEK_SET);


	delete[] buffer;
}
