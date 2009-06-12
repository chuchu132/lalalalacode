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

		Controlador controlador(cliente);
		Ventana ventana;
		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		if (!ventana.huboError())
		{
			controlador.correrVista();
		}

		assert(true,"Ventana cerrada.");
}


