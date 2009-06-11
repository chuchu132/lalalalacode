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
		Gtk::Main kit(0, 0);
		ClienteTorrent cliente;
		Torrent t;//para probar la vista
		Ventana ventana;
		ventana.tor = &t;
		Controlador controlador(cliente);
		t.setControlador(&controlador);

		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		kit.run(ventana.getVentana());

		assert(true,"Ventana cerrada.");
}


