/*
 * TestBencodeParser.cpp
 *
 *  Created on: 09/06/2009
 *      Author: ale
 */

#include "TestBencodeParser.h"
#include "../src/BencodeParser.h"
#include "../src/DatosParser.h"
#include "../src/Sha1.h"
#include <cstring>
#include <cstdio>

TestBencodeParser::TestBencodeParser() {

}

TestBencodeParser::~TestBencodeParser() {

}

void TestBencodeParser::test(const char* url) {

	FILE*fp = fopen(url, "r");
	fseek(fp,0,SEEK_END);
	int tam=ftell(fp);
	char * aux=new char[tam+1];
	fseek(fp,0,SEEK_SET);
	fread(aux,1,tam,fp);
    fclose (fp);

	BencodeParser parser(aux,tam);
	if (parser.procesar()) {
		DatosParser* datos = parser.salidaParser();
		datos->primero();
		while (!datos->final()) {
			if (memcmp("pieces", datos->obtenerDato(),
					datos->obtenerLongitudDato()) == 0) {
				datos->siguiente();
				datos->siguiente();
			}
			if (memcmp("info_hash", datos->obtenerDato(),
					datos->obtenerLongitudDato()) == 0) {
				std::cout << "info_hash" << std::endl;
				datos->siguiente();
				Sha1 sha;
				std::cout
						<< sha.salidaAstring((unsigned*) datos->obtenerDato())
						<< std::endl;
				if (!datos->final()) {
					datos->siguiente();
				}
			} else {
				std::cout << datos->obtenerDato() << std::endl;
				datos->siguiente();
			}
		}
		delete datos;
	}
}

void TestBencodeParser::run() {
	test("./Tests/AngelsAndDemons.torrent");
//	test("./Tests/PSP.torrent");
//  test("./Tests/PSP2.torrent");
//	test("./Tests/PSP3.torrent");
//	test("./Tests/Sec.torrent");

}
