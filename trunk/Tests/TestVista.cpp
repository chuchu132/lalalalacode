/*
 * TestVista.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */


#include "../src/Ventana.h"
#include "../src/Consola.h"
#include "TestVista.h"

TestVista::TestVista() {}

TestVista::~TestVista() {}

void TestVista::run(){

		Gtk::Main kit(0,0);
		ClienteTorrent cliente;
		Controlador controlador(cliente);
		Vista* ventana = new Ventana();

		cliente.setControlador(&controlador);

		char opcion = ' ';

		if (ventana->huboError()) {
			do {
				std::cout<<"Desea correr la vista por consola? Y/N"<<std::endl;
				std::cin>> uppercase >>opcion;
			}while((opcion !='Y') || (opcion !='N'));

			if (opcion =='Y'){
				delete ventana;
				ventana = new Consola();
			}
		}
		if ((!ventana->huboError()) && (opcion != 'N')) {

			ventana->setControlador(&controlador);
			controlador.setVentana(ventana);
			//cliente.execute();
			controlador.correrVista();
			controlador.detenerVista();
		}

		delete ventana;
		assert(true,"Fin Test Vista");
}


