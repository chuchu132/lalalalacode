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

	FILE*fp = fopen("../Tests/AngelsAndDemons.torrent", "r");
	fseek(fp,0,SEEK_END);
	int tam=ftell(fp);
	char * aux=new char[tam+1];
	fseek(fp,0,SEEK_SET);
	fread(aux,1,tam,fp);
	fclose (fp);

	BencodeParser parser(aux,tam);
		if (parser.procesar()) {
			FileManager filemanager(NULL);
			DatosParser* datos =  parser.salidaParser();
			datos->primero();
			if(filemanager.inicializar(datos)){
			assert(filemanager.getBitmap().estaVacio(),"El bitmap generado no tiene ninguna ieza marcada");
			std::cout<<"Tamaño Descarga: "<<filemanager.getTamanio()<<" bytes"<<std::endl;

			filemanager.writeBlock(1,10,7,"FiTorrent");
			char* datoRecuperado = 	filemanager.readBlock(1,10,10);
			assert(memcmp(datoRecuperado,"FiTorrent",10)==0,"El dato recuperado es igual al ingresado");
			delete[] datoRecuperado;
			}
			else{
				assert(false,"NO se pudo inicializar el Filemanager.");
			}
}

}
