/*
 * TestFileManager.cpp
 *
 *  Created on: 13/06/2009
 *      Author: ale
 */

#include "TestFileManager.h"
#include "../src/BencodeParser.h"
#include "../src/DatosParser.h"
#include "../src/FileManager.h"
#include <cstring>
#include <string>

TestFileManager::TestFileManager() {
	// TODO Auto-generated constructor stub

}

TestFileManager::~TestFileManager() {
	// TODO Auto-generated destructor stub
}

void TestFileManager::run(){
	test("./Tests/AngelsAndDemons.torrent"); // single file
	test("./Tests/blues.torrent"); // multi file
}

void TestFileManager::test(std::string urlTorrent){
	BencodeParser parser(urlTorrent.c_str());

		if (parser.procesar()) {
			FileManager filemanager(NULL);
			DatosParser* datos =  parser.salidaParser();
			datos->primero();
			if(filemanager.inicializar(datos)){
			assert(filemanager.getBitmap().estaVacio(),"El bitmap generado no tiene ninguna pieza marcada");
			std::cout<<"Tamaño Descarga: "<<filemanager.getTamanio()<<" bytes"<<std::endl;

			filemanager.writeBlock(1,10,10,"FiTorrent");
			filemanager.getBitmap().marcarBit(1); //marco la pieza como que esta completa para poder leer una parte
			char* datoRecuperado = 	filemanager.readBlock(1,10,10);
			assert(memcmp(datoRecuperado,"FiTorrent",10)==0,"El dato recuperado es igual al ingresado");
			delete[] datoRecuperado;
			delete datos;

			std::list<Archivo*>::iterator itA = filemanager.getIteratorArchivos();
			std::list<Archivo*>::iterator finListaArchivo = filemanager.getEndArchivos();
			while(itA != finListaArchivo){
				std::cout<<"Archivo: "<<(*itA)->getPath()<<" Tam: "<<(*itA)->getTamanio()<<std::endl;
				itA++;
			}

			}
			else{
				assert(false,"NO se pudo inicializar el Filemanager.");
			}
}else{
	assert(false,"El archivo "+urlTorrent + " no pudo se procesado.");
}

}

