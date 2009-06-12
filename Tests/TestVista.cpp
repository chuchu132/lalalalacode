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

		Ventana ventana;

		Controlador controlador(cliente);

		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		if (!ventana.huboError())
		{
			Torrent t(&cliente);
			ventana.tor = &t;
			t.setControlador(&controlador);
			ventana.run();
		}

		assert(true,"Ventana cerrada.");
}


