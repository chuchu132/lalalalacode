/*
 * Controlador.cpp
 *
 *  Created on: 29/05/2009
 *      Author: teddy
 */

#include "Controlador.h"

Controlador::Controlador(ClienteTorrent& c):cliente(c)
{
	// TODO Auto-generated constructor stub
}

Controlador::~Controlador()
{
	// TODO Auto-generated destructor stub
}

void Controlador::setVentana(Ventana *ventana)
{
	this->ventana = ventana;
}

Torrent* Controlador::agregarTorrent(std::string ruta)
{
	return cliente.agregarTorrent(ruta);
}

void Controlador::borrarTorrent(Torrent *t)
{
	cliente.borrarTorrent(t);
}

void Controlador::continuarTorrent(Torrent *t)
{
	t->continuar();
}

void Controlador::detenerTorrent(Torrent *t)
{
	t->detener();
}

void Controlador::pausarTorrent(Torrent *t)
{
	t->pausar();
}

void Controlador::actualizarEstado(Torrent *t)
{
	ventana->actualizarEstado(t);
}



int main (int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);

	ClienteTorrent cliente;
	Ventana ventana;
	Controlador controlador(cliente);

	ventana.setControlador(&controlador);
	controlador.setVentana(&ventana);

	kit.run(ventana.getVentana());

	std::cout<<"fin"<<std::endl;

	return 0;
}
