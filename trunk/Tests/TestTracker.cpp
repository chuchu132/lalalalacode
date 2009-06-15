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

#include <cstring>

TestTracker::TestTracker() {
	datos = NULL;
}

TestTracker::~TestTracker() {
	if (datos != NULL) {
		delete datos;
	}
}

void TestTracker::run() {

	BencodeParser parser("./Tests/AngelsAndDemons.torrent");
	if (parser.procesar()) {
		datos = parser.salidaParser();
		Tracker tracker;
		std::string url;
		char* datoTemp;
		int tam;
		datos->primero();
		/*Obtengo la url del tracker */
		datos->obtenerDatoPorNombre("announce",&datoTemp,tam);
		url = datoTemp;
		delete[] datoTemp;
		/*Intento conectarme*/
		if( tracker.connect(url) ){
		assert(true,"Se conecto al tracker");
		/*Lanzo un hilo a escuchar lo que manda el tracker*/
		tracker.execute();

		ParserMensaje parser;
		unsigned int info_hash[5];
		int puerto = 9999; // Levantar un servidor en ese puerto antes de correr el test para ver si llega algo

		datos->obtenerDatoPorNombre("length",&datoTemp,tam);
		unsigned int left = atol(datoTemp);
		delete[] datoTemp;
		std::string peer_id = "ALEBLABLAAACacharii";

		if(datos->obtenerDatoPorNombre("info_hash",&datoTemp,tam)){
		memcpy(info_hash,datoTemp,LEN_SHA1);
		}
		delete[] datoTemp;
		/*Creo un mensaje para enviar el GET al tracker*/
		std::string get = parser.crearGetConEvento("annonunce",info_hash,peer_id.c_str(),puerto,0,0,left,EVENT_STARTED);

		tracker.send(get.c_str(),get.length());

		get =parser.crearGetConNumwant("annonunce",info_hash,peer_id.c_str(),puerto,0,0,left,100);

		tracker.send(get.c_str(),get.length());

		assert(true,"Paso del send");
		sleep(3);
		tracker.cerrarConexion();
		tracker.join();
		}else{
			assert(false,"Se conecto al tracker");
		}

	} else {
		assert(false, "No se pudo parsear el archivo");
	}

}

