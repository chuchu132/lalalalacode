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
#include "../src/Torrent.h"
#include <cstdlib>

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

	BencodeParser parser("./Tests/PSP.torrent");
	if (parser.procesar()) {
		datos = parser.salidaParser();
		Tracker tracker;
		Torrent unTorrent(NULL,"frutaaa");
		tracker.setTorrent(&unTorrent);
		std::string url;
		char* datoTemp;
		int tam;
		datos->primero();
		/*Obtengo la url del tracker */
		datos->obtenerDatoPorNombre("announce",&datoTemp,tam);
		url = datoTemp;
		delete[] datoTemp;
		/*Intento conectarme*/
		tracker.inicilizar(url);
		if( tracker.connect() ){
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
		std::string get = parser.crearGetConEvento(tracker.getUrl(),"annonunce",info_hash,peer_id.c_str(),puerto,0,0,left,EVENT_STARTED);

		std::cout<<get<<std::endl;

		tracker.send(get.c_str(),get.length());

		assert(true,"Paso del send");

		sleep(100);
		assert(true,"Paso el sleep");
		tracker.cerrarConexion();
		tracker.join();
		}else{
			assert(false,"Se conecto al tracker");
		}

	} else {
		assert(false, "No se pudo parsear el archivo");
	}

}

