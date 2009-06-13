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

	config->guardarPuerto(35000);
	config->guardarRutaDescargas("Archivos_Bajados/otra_carpeta");

	delete config;

	Configuracion config2;
	Test::assert(config2.getPuerto()== 35000, "Puerto Cargado desde Archivo");
	Test::assert(config2.getRutaDescargas()== "Archivos_Bajados/otra_carpeta", "Ruta de descargas Cargada desde Archivo");
	std::cout<<config2.getRutaDescargas()<<std::endl;

}
