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

		cliente.setControlador(&controlador);
		ventana.setControlador(&controlador);
		controlador.setVentana(&ventana);

		if (!ventana.huboError())
		{
			//aca faltaria un cliente.excecute() para escuchar a los peers entrantes

			//cliente.execute();
			controlador.correrVista();
			controlador.detenerVista();
		}

		assert(true,"Ventana cerrada.");
}


