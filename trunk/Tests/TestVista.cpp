/*
 * TestVista.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestVista.h"

TestVista::TestVista() {
	// TODO Auto-generated constructor stub

}

TestVista::~TestVista() {
	// TODO Auto-generated destructor stub
}

void TestVista::run(){
		Gtk::Main kit(0, 0);
		ClienteTorrent cliente;
		Torrent t;//para probar la vista
		Ventana ventana;
		ventana.tor = &t;
		Controlador controlador(cliente);

		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		kit.run(ventana.getVentana());

		assert(true,"No message.");
}


