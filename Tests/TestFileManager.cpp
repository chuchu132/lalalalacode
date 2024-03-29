/*
 * TestFileManager.cpp
 *
 *  Created on: 13/06/2009
 *      Author: ale
 */

#include "TestFileManager.h"
#include "../src/BencodeParser.h"
#include "../src/ClienteTorrent.h"
#include "../src/DatosParser.h"
#include "../src/FileManager.h"
#include <cstring>
#include <string>

TestFileManager::TestFileManager() {
}

TestFileManager::~TestFileManager() {
}

void TestFileManager::run(){
//	test("./Tests/AngelsAndDemons.torrent"); // single file
//	test("./Tests/blues.torrent"); // multi file
//	testSplitFiles("./Tests/disco.torrent");
	verificarPiezas("./Tests/boom.torrent");
	}

void TestFileManager::test(std::string urlTorrent){
	BencodeParser parser(urlTorrent.c_str());

	if (parser.procesar()) {
		FileManager filemanager(NULL,NULL);
		DatosParser* datos =  parser.salidaParser();
		datos->primero();
		if(filemanager.inicializar(datos)){
			char* datoTemp = NULL;
			int tam;
			Sha1 sha;
			datos->primero();
			datos->obtenerDatoPorNombre("info_hash",&datoTemp,tam);
			std::string url = URL_CARPETA_TEMP;
			url += sha.salidaAstring((unsigned int*)datoTemp);
			url += ".bitmap";
			filemanager.guardarDatos();
			delete[] datoTemp;

			/*OJO La primer vez que se corre el test esto da ok xq el bitmap se genera desde 0
			 * si se vuelve a ejecutar el test da mal xq el bitmap se genera en base a lo guardado
			 * en el test anterior y hay un bit marcado*/
			assert(filemanager.getBitmap()->estaVacio(),"El bitmap generado no tiene ninguna pieza marcada");
			std::cout<<"Tamaño Descarga: "<<filemanager.getTamanio()<<" bytes"<<std::endl;

			unsigned int bytes = 0;
			filemanager.writeBlock(1,10,10,"FiTorrent",bytes);
			filemanager.getBitmap()->marcarBit(1); //marco la pieza como que esta completa para poder leer una parte
			char* datoRecuperado = 	filemanager.readBlock(1,10,10);
			assert(memcmp(datoRecuperado,"FiTorrent",10)==0,"El dato recuperado es igual al ingresado");
			filemanager.getBitmap()->desmarcarBit(1);
			filemanager.guardarDatos();
			delete[] datoRecuperado;


			std::list<Archivo*>::iterator itA = filemanager.getIteratorArchivos();
			std::list<Archivo*>::iterator finListaArchivo = filemanager.getEndArchivos();
			while(itA != finListaArchivo){
				std::list<std::string>::iterator itPath = (*itA)->getPathList()->begin();
				std::list<std::string>::iterator finPath = (*itA)->getPathList()->end();
				std::cout<<" Tam: "<<(*itA)->getTamanio()<<std::endl;
				while(itPath != finPath){
					std::cout<<"Archivo: "<<(*itPath)<<std::endl;
					itPath++;
				}
				itA++;
			}

		}
		else{
			assert(false,"NO se pudo inicializar el Filemanager.");

		}
		delete datos;
	}else{
		assert(false,"El archivo "+urlTorrent + " no pudo se procesado.");
	}

}

void TestFileManager::testSplitFiles(std::string urlTorrent){
	BencodeParser parser(urlTorrent.c_str());

	if (parser.procesar()) {
		ClienteTorrent cliente;
		FileManager filemanager(&cliente,NULL);
		DatosParser* datos =  parser.salidaParser();
		datos->primero();
		if(filemanager.inicializar(datos)){
			filemanager.descargaAarchivos();
		}
		delete datos;
	}
}

/*Este test debe correrse con el archivo original dentro de la carpeta
 * incompletos con el info hash como nombre */
void TestFileManager::verificarPiezas(std::string urlTorrent){
	BencodeParser parser(urlTorrent.c_str());

	if (parser.procesar()) {
		ClienteTorrent cliente;
		FileManager filemanager(&cliente,NULL);
		DatosParser* datos =  parser.salidaParser();
		datos->primero();
		if(filemanager.inicializar(datos)){
			unsigned int aux =(unsigned int ) filemanager.getTamanio() / filemanager.getTamanioPieza();
			for(unsigned int i = 0 ; i < aux; i++){
			std::cout<<i<<std::endl;
				assert(filemanager.verificarHashPieza(i),"El hash debe dar ok");
			}
		}
		delete datos;
	}
}
