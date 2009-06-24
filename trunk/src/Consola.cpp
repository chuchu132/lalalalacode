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

const UCHAR DOWN = 0x50;
const UCHAR UP = 0x48;
const UCHAR ENTER = 0x0D;

Consola::Consola() {}

Consola::~Consola() {}

void Consola::setControlador(Controlador *c){
	this->controlador = c;
}

void Consola::actualizarEstado(Torrent*){

}

void Consola::addTorrent(Torrent* t){
	torrents.push_back(t);
}

void Consola::mostrarNotificacion(std::string){

}

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



void Consola::clrscr(){
	system("clear");
	wherex = 0;
	wherey = 0;
}

void Consola::gotoXY(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
	wherex = x;
	wherey = y;
}

void Consola::flechitas(int &opcion, int cantOpc){
	UCHAR tecla;
	int x,y;
	x =(wherex+2);
	y =(wherey - cantOpc);
	gotoXY(x,y);
	printf("*");
	opcion =1;
	do{
		tecla = getchar();
		if( (tecla == DOWN) && (opcion != cantOpc)){
			gotoXY(wherex-1, wherey);
			printf("_");
			gotoXY(wherex-1, wherey+1);
			printf("*");
			opcion++;
		}
		else{
			if ((tecla == UP) && (opcion != 1)){
				gotoXY(wherex-1, wherey);
				printf("_");
				gotoXY(wherex-1, wherey-1);
				printf("*");
				opcion--;
			}
		}
	}while (tecla != ENTER);
	gotoXY(wherex -1, wherey);
	printf("*");
	gotoXY(x+2,wherey+(cantOpc-(opcion -1)));
}

void Consola::pantallaPrincipal(){
	bool fin = false;
	do{
		clrscr();
		std::cout<<std::endl
		<< " Fi-Torrent"<<std::endl
		<< " [_] Agregar Torrent"<<std::endl
		<< " [_] Listar Torrents"<<std::endl
		<< " [_] Configurar"<<std::endl
		<< " [_] Salir"<<std::endl;
		int opcion;
		opcion = 0;
		flechitas(opcion,4);
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
	}while(fin);
}

void Consola::pantAgrTorrent(){
	clrscr();
	std::string url;
	std::cout<<std::endl
	<<" Fi-Torrent"<<std::endl
	<<" [_] Ingresar URL .torrent"<<std::endl
	<<" [_] Volver"<<std::endl;
	int opcion = 0;
	flechitas(opcion,2);
	if(opcion == 1){
		clrscr();
		std::cout<<std::endl
		<<"  Fi-Torrent"<<std::endl
		<<"  URL: ";
		std::cin>>url;

		std::cout<<std::endl<<" [_] Volver"<<std::endl;
		flechitas(opcion,1);
	}
}

void Consola::pantListaTorrent(){
	bool continuar = true;
	while(continuar){
	clrscr();
	int cantTorrent = torrents.size();
	std::cout<<" Fi-Torrent"<<std::endl;
	if(cantTorrent != 0){
		std::cout<<"      ESTADO      NOMBRE ARCHIVO O CARPETA"
		<<std::endl;
		std::list<Torrent*>::iterator it = torrents.begin();
		std::list<Torrent*>::iterator end = torrents.end();
		while(it != end){
			std::cout<<" [_] "<<(*it)->getEstado()<<"   "<<(*it)->getNombre()<<std::endl;
			it++;
		}
	}
	std::cout<<" [_] Volver al menu principal"<<std::endl;
	int opcion = 0;
	flechitas(opcion,cantTorrent +1);
	if(opcion != cantTorrent)
		mostrarDatosTorrent(opcion-1);
	else
		continuar = false;
	}
}

void Consola::mostrarDatosTorrent(int numTorrent){
	std::list<Torrent*>::iterator it = torrents.begin();
	for(int i =0; i<numTorrent;i++)	it++;
	clrscr();
	std::cout<<"\n DATOS TORRENT"<<std::endl
	<<" NOMBRE: "<< (*it)->getNombre()<<std::endl
	<<" ESTADO: "<< (*it)->getEstado()<<std::endl
	<<" DESCARGADO: "<<(*it)->getTamanioDescargado()<<" SUBIDO: "<<(*it)->getTamanioSubido()<<std::endl
	<<" PROGRESO: "<< (int)((*it)->getTamanioDescargado() / (*it)->getTamanio() )<<" %"<<std::endl
	<<" CANTIDAD PEERS: "<<(*it)->getCantPeers()<<std::endl
	<<" CANTIDAD ARCHIVOS: "<<(*it)->getFileManager()->getCantArchivos()<<std::endl;
	std::cout<<std::endl<<" [_] VOLVER"<<std::endl
	<<" [_] DETENER/CONTINUAR"<<std::endl
	<<" [_] BORRAR (el torrent se borra luego de elegir volver.)"<<std::endl
	<<" [_] REFRESCAR PEERS"<<std::endl;
	int opcion = 0;
	bool volver = false;
	bool borrar = false;
	while(!volver){
		flechitas(opcion,4);
		switch (opcion) {
		case 1: volver = true;
		case 2:{
			std::string estado = (*it)->getEstado();
			if( estado == T_DETENIDO){
				controlador->continuarTorrent((*it));
			}else{
				controlador->detenerTorrent((*it));
			}
			gotoXY(0,2);
			std::cout<<" ESTADO: "<< (*it)->getEstado()<<"          ";
			gotoXY(0,11);
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
			 <<" [_] VOLVER"<<std::endl
			 <<" [_] CAMBIAR URL DESCARGAS"<<std::endl
			 <<" [_] CAMBIAR PUERTO"<<std::endl;
	int opcion =0;
	flechitas(opcion,3);
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

