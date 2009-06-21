/*
 * Controlador.cpp
 *
 *  Created on: 29/05/2009
 *      Author: Lucia
 */

#include "Controlador.h"

Controlador::Controlador(ClienteTorrent& c):cliente(c){}

Controlador::~Controlador(){}

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
	mutex_lista.lock();
	actualizaciones.push_back(t);
	//std::cout<<".........controlador push back ........"<<std::endl;
	mutex_lista.unlock();
}

void Controlador::agregarTorrentsEnVista()
{
	std::list<Torrent*>::iterator it;
	for (it = cliente.getListaTorrents()->begin(); it != cliente.getListaTorrents()->end(); it++)
	{
		ventana->addTorrent((*it));
	}
}

void Controlador::cerrarCliente()
{
	cliente.finalizar();
}

void Controlador::refrescarPeers(Torrent *t)
{
	t->refrescarPeers();
}

void Controlador::notificarVista(std::string notificacion)
{
	ventana->mostrarNotificacion(notificacion);
}

int Controlador::correrVista()
{
	agregarTorrentsEnVista();
	return ventana->correr();
}

void Controlador::detenerVista() {
	ventana->detener();
}

void Controlador::guardarConfiguracion()
{
	Configuracion *config = cliente.getConfiguracion();
	unsigned int puerto = ventana->getPuerto();
	if (puerto != 0)
		config->guardarPuerto(ventana->getPuerto());
	config->guardarRutaDescargas(ventana->getRutaDescargas());
}

bool Controlador::hayCambios() {
	bool cambio;
	mutex_lista.lock();
	cambio = !actualizaciones.empty();
	mutex_lista.unlock();
	return cambio;
}

Torrent* Controlador::getCambio(){
	Torrent *t;
	mutex_lista.lock();
	t = actualizaciones.front();
	actualizaciones.pop_front();
	//std::cout<<".........front ........"<<std::endl;
	mutex_lista.unlock();
	return t;
}
