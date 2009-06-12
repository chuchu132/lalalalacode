/*
 * TestVista.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestVista.h"

TestVista::TestVista() {}

TestVista::~TestVista() {}

void TestVista::run(){

		Gtk::Main kit(0,0);
		ClienteTorrent cliente;
		//Torrent t(&cliente);//para probar la vista
		Ventana ventana;
		//ventana.tor = &t;
		Controlador controlador(cliente);
	//	t.setControlador(&controlador);

		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		ventana.run();

		assert(true,"Ventana cerrada.");
}


