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

void Controlador::agregarTorrentEnVista(Torrent *t)
{
	ventana->addTorrent(t);
}

void Controlador::cerrar()
{
	cliente.finalizar();
}

