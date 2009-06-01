/*
 * Bitmap.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "Bitmap.h"

/*TODO Parser deberia tener un lista de Archivos que adentro tienen nombre, path, piezas y sha1*/

Bitmap::Bitmap(BencodeParser* parser) {
//	IteradorArchivosParser it = parser->iterador();
//	int acum = 0;
//	int aux=0;
//	while(it->tieneSiguiente()){
//		aux = it->siguiente()->getCantidadPiezas();
//		acum += aux;
//		piezasXarchivo.insert(piezasXarchivo.end(),aux);
//	}
//	bytesTotales = acum;
//	tamanioPieza = parser->getTamanioPiezas();
//	bloquesXPieza = (tamanioPieza > TAM_BLOQUES)?(tamanioPieza / TAM_BLOQUES):1; // division entre potencias de 2
//
//	tamBitmapBytes = (bytesTotales / 8) + ( ((bytesTotales % 8 )== 0)?0:1 );
//	bitmap =  new char[ tamBitmapBytes ];
//	memset(bitmap,0,tamBitmapBytes);
}

Bitmap::~Bitmap() {
	// TODO Auto-generated destructor stub
}

int Bitmap::getTamanio(){
	return bytesTotales;
}

void Bitmap::getMap(char* map,int* length){
	map = new char[tamBitmapBytes];
	mempcpy(map,bitmap,tamBitmapBytes);
	length* = tamBitmapBytes;
}

bool Bitmap::getBlock(int index,int begin,char* data,int* longitud){
//	int acum = 0;
//	int contador = 0;
//
//	while( ){
//		acum += piezasXarchivo[contador];
//
//	}

	return true;

}
