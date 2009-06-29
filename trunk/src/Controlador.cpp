/*
 * Controlador.cpp
 *
 *  Created on: 29/05/2009
 *      Author: Lucia
 */

#include "Controlador.h"

Controlador::Controlador(ClienteTorrent& c) :
	cliente(c) {
}

Controlador::~Controlador() {
}

void Controlador::setVentana(Vista *ventana) {
	this->vista = ventana;
}

Torrent* Controlador::agregarTorrent(std::string ruta) {
	Torrent *t = cliente.agregarTorrent(ruta);
	if (t != NULL)
		t->continuar();
	return t;
}

void Controlador::borrarTorrent(Torrent *t) {
	cliente.borrarTorrent(t);
}

void Controlador::continuarTorrent(Torrent *t) {
	t->continuar();
}

void Controlador::detenerTorrent(Torrent *t) {
	t->detener();
}

void Controlador::actualizarEstado(Torrent *t) {
	mutex_lista.lock();
	actualizaciones.push_back(t);
	mutex_lista.unlock();
}

void Controlador::agregarTorrentsEnVista() {
	std::list<Torrent*>::iterator it;
	for (it = cliente.getListaTorrents()->begin(); it
	!= cliente.getListaTorrents()->end(); it++) {
		vista->addTorrent((*it));
	}
}

void Controlador::cerrarCliente() {
	cliente.finalizar();
}

void Controlador::refrescarPeers(Torrent *t) {
	t->refrescarPeers();
}

void Controlador::notificarVista(std::string notificacion) {
	vista->mostrarNotificacion(notificacion);
}

int Controlador::correrVista() {
	agregarTorrentsEnVista();
	return vista->correr();
}


void Controlador::guardarConfiguracion() {
	Configuracion *config = cliente.getConfiguracion();
	UINT puerto = vista->getPuerto();
	if (puerto != 0)
		config->guardarPuerto(vista->getPuerto());
	config->guardarRutaDescargas(vista->getRutaDescargas());
}

bool Controlador::hayCambios() {
	bool cambio;
	mutex_lista.lock();
	cambio = !actualizaciones.empty();
	mutex_lista.unlock();
	return cambio;
}

Torrent* Controlador::getCambio() {
	Torrent *t;
	mutex_lista.lock();
	t = actualizaciones.front();
	actualizaciones.pop_front();
	mutex_lista.unlock();
	return t;
}
