/*
 * Consola.cpp
 *
 *  Created on: 24/06/2009
 *      Author: ale
 */

#include "Consola.h"
#include "Tipos.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

Consola::Consola() {}

Consola::~Consola() {
	if (controlador != NULL)
		controlador->cerrarCliente();
}

void Consola::setControlador(Controlador *c){
	this->controlador = c;
}

void Consola::actualizarEstado(Torrent*){}

void Consola::addTorrent(Torrent* t){
	torrents.push_back(t);
}

void Consola::mostrarNotificacion(std::string){}

int Consola::correr(){
	pantallaPrincipal();
	return OK;
}

//TODO ver quien tiene este puerto
UINT Consola::getPuerto(){
	return PUERTO_DEFAULT;
}

std::string Consola::getRutaDescargas(){
	return RUTA_DESCARGAS;
}

void Consola::detener(){}

bool Consola::huboError(){
	return false;
}

int Consola::clrscr(){
	return system("clear");
}

void Consola::pantallaPrincipal(){
	bool fin = false;
	do{
		clrscr();
		std::cout<<std::endl
		<< " FI-TORRENT"<<std::endl
		<< " [1] Agregar Torrent"<<std::endl
		<< " [2] Listar Torrents"<<std::endl
		<< " [3] Configurar"<<std::endl
		<< " [4] Salir"<<std::endl
		<< "Opcion: ";
		int opcion;
		std::cin>>opcion;
		switch (opcion) {
		case 1: pantAgrTorrent();
		break;
		case 2: pantListaTorrent();
		break;
		case 3: pantallaConfiguracion();
		break;
		case 4: fin = true;
		break;
		}
	}while(!fin);
}

void Consola::pantAgrTorrent(){
	clrscr();
	std::string url;
	std::cout<<std::endl
	<<" FI-TORRENT"<<std::endl
	<<" [1] Ingresar URL .torrent"<<std::endl
	<<" [2] Volver"<<std::endl
	<< "Opcion: ";
	int opcion;
	std::cin>>opcion;
	if(opcion == 1){
		clrscr();
		std::cout<<std::endl
		<<"  FI-TORRENT"<<std::endl
		<<"  URL: ";
		std::cin>>url;
		Torrent* t =controlador->agregarTorrent(url);
		if(t!=NULL){
			addTorrent(t);
			controlador->continuarTorrent(t);
			std::cout<<"  Se agrego el torrent: "<<t->getNombre()<<std::endl;
		}else
			std::cout<<"  No se pudo agregar el torrent desde "<<url<<std::endl;
		std::cout<<std::endl<<" [1] Volver"<<std::endl
		                    << "Opcion: ";
		std::cin>>opcion;
	}
}

void Consola::pantListaTorrent(){
	bool continuar = true;
	while(continuar){
		clrscr();
		int cantTorrent = torrents.size();
		std::cout<<" FI-TORRENT"<<std::endl;
		int i=1;
		if(cantTorrent != 0){
			std::cout<<"      ESTADO      NOMBRE ARCHIVO O CARPETA"
			<<std::endl;
			std::list<Torrent*>::iterator it = torrents.begin();
			std::list<Torrent*>::iterator end = torrents.end();

			while(it != end){
				std::cout<<" ["<<i<<"] "<<(*it)->getEstado()<<"   "<<(*it)->getNombre()<<std::endl;
				it++;
				i++;
			}
		}
		std::cout<<" ["<<i<<"] Volver al menu principal"<<std::endl
				 << "Opcion: ";
		int opcion;
		std::cin>>opcion;
		if(opcion > 0 && opcion <(cantTorrent+1))
			mostrarDatosTorrent(opcion-1);
		else
			continuar = false;
	}
}

void Consola::mostrarDatosTorrent(int numTorrent){
	std::list<Torrent*>::iterator it = torrents.begin();
	for(int i =0; i<numTorrent;i++)	it++;
	bool volver = false;
	bool borrar = false;
	while(!volver){
		clrscr();
		std::cout<<"\n DATOS TORRENT"<<std::endl
		<<" NOMBRE: "<< (*it)->getNombre()<<std::endl
		<<" ESTADO: "<< (*it)->getEstado()<<std::endl
		<<" DESCARGADO: "<<(*it)->bytesToString((*it)->getTamanioDescargado())<<" SUBIDO: "<<(*it)->bytesToString((*it)->getTamanioSubido())<<std::endl
		<<" PROGRESO: "<< (int) ((((*it)->getTamanioDescargado()/1024) * 100)
				/ ((*it)->getTamanio()/1024))<<" %"<<std::endl
		<<" CANTIDAD PEERS: "<<(*it)->getCantPeers()<<std::endl
		<<" CANTIDAD ARCHIVOS: "<<(*it)->getFileManager()->getCantArchivos()<<std::endl;
		std::cout<<std::endl<<" [1] VOLVER"<<std::endl
		<<" [2] DETENER/CONTINUAR"<<std::endl
		<<" [3] BORRAR (el torrent se borra luego de elegir volver.)"<<std::endl
		<<" [4] REFRESCAR PEERS"<<std::endl
		<< "Opcion: ";
		int opcion;
		std::cin>>opcion;
		switch (opcion) {
		case 1: volver = true;
		break;
		case 2:{
			std::string estado = (*it)->getEstado();
			if( estado == T_DETENIDO){
				if ((*it) != NULL)
				controlador->continuarTorrent((*it));
			}else{
				if ((*it) != NULL)
				controlador->detenerTorrent((*it));
			}
		}
		break;
		case 3:	borrar = true;
		break;
		case 4: controlador->refrescarPeers((*it));
		break;
		}
	}
	if(borrar)
		controlador->borrarTorrent((*it));
}

void Consola::pantallaConfiguracion(){
	bool continuar = true;
	while(continuar){
		clrscr();
		std::cout<<"\n Configuracion"<<std::endl
		<<"  VALORES ACTUALES:"<<std::endl
		<<"  RUTA DE DESCARGAS: "<<RUTA_DESCARGAS<<std::endl
		<<"  PUERTO : "<<PUERTO_DEFAULT<<std::endl
		<<" [1] VOLVER"<<std::endl
		<<" [2] CAMBIAR URL DESCARGAS"<<std::endl
		<<" [3] CAMBIAR PUERTO"<<std::endl
		<< "Opcion: ";
		int opcion;
		std::cin>>opcion;
		switch (opcion) {
		case 1: continuar =  false;
		break;
		case 2:{
			clrscr();
			std::string url;
			std::cout<<"\n  Nueva URL: ";
			std::cin>>url;
		}
		break;
		case 3:{
			clrscr();
			UINT puerto;
			std::cout<<"\n  Nuevo Puerto: ";
			std::cin>>puerto;
		}
		break;
		}
		controlador->guardarConfiguracion();
	}
}

