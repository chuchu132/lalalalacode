/*
 * TestTracker.cpp
 *
 *  Created on: 11/06/2009
 *      Author: ale
 */

#include "../src/BencodeParser.h"
#include "../src/ParserMensaje.h"
#include "../src/Tracker.h"
#include "TestTracker.h"


TestTracker::TestTracker() {
	datos = NULL;
}

TestTracker::~TestTracker() {
	if (datos != NULL) {
		delete datos;
	}
}

void TestTracker::setup() {
	if (datos != NULL) {
		delete datos;
	}
	BencodeParser parser("../Tests/AngelsAndDemons.torrent");
	if (parser.procesar()) {
		datos = parser.salidaParser();
	} else {
		assert(false, "No se pudo parsear el archivo");
	}
}

void TestTracker::run() {
	Tracker tracker;
	std::string url;
	char* datoTemp;
	int tam;
	datos->obtenerDatoPorNombre("announce",&datoTemp,tam);
	url = datoTemp;
	delete[] datoTemp;

	assert(tracker.connect(url.c_str(),PUERTO_DEFAULT_TRACKER),"Se conecto al tracker");

	ParserMensaje parser;
	unsigned int info_hash[5];
	int puerto = 9999; // Levantar un servidor en ese puerto antes de correr el test para ver si llega algo

	datos->obtenerDatoPorNombre("length",&datoTemp,tam);
	unsigned int left = atol(datoTemp);
	delete[] datoTemp;
	std::string peer_id = "ALEBLABLAAACacharii";
	std::string get = parser.crearGetConEvento(info_hash,peer_id.c_str(),puerto,0,0,left,EVENT_STARTED);

	tracker.send(get.c_str(),get.length());

	assert(true,"Paso del send");

}

