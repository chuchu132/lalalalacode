/*
 * TestConfiguracion.cpp
 *
 *  Created on: 13/06/2009
 *      Author: teddy
 */

#include "TestConfiguracion.h"
#include "../src/Configuracion.h"
#include "../src/Constantes.h"

TestConfiguracion::TestConfiguracion()
{}

TestConfiguracion::~TestConfiguracion()
{}

void TestConfiguracion::run() {
	Configuracion *config = new Configuracion();
	config->cargarConfiguracionDefault();
	Test::assert(config->getPuerto()== PUERTO_DEFAULT, "Puerto Default");
	Test::assert(config->getRutaDescargas()== RUTA_DESCARGAS, "Ruta de descargas default");

	config->guardarPuerto(35001);
	config->guardarRutaDescargas("Archivos Bajados/otra_carpeta");

	delete config;

	Configuracion config2;
	Test::assert(config2.getPuerto()== 35001, "Puerto Cargado desde Archivo");
	Test::assert(config2.getRutaDescargas()== "Archivos Bajados/otra_carpeta", "Ruta de descargas Cargada desde Archivo");

}
