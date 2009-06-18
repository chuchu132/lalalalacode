/*
 * Bitmap.cpp
 *
 *  Created on: 05/06/2009
 *      Author: ale
 */

#include "Bitmap.h"
#include <cstring>
#include <cmath>



Bitmap::Bitmap() {
	bitmap = NULL;
	tamanio = 0;
	inicializado =  false;
}

Bitmap::Bitmap(const Bitmap& original){
	if(original.inicializado){
	bitmap = new char[original.tamanio];
	tamanio = original.tamanio;
	memcpy(bitmap,original.bitmap,tamanio);
	inicializado =  true;
	}
}

void Bitmap::inicializarBitmap(const char* unBitmap, int tamanioBytes) {
	if(inicializado){
		delete[] bitmap;
	}
	bitmap = new char[tamanioBytes];
	this->tamanio = tamanioBytes;
	memcpy(bitmap, unBitmap, tamanioBytes);
	inicializado =  true;
}

void Bitmap::inicializarBitmap(int tamanioBytes){
	if(bitmap != NULL){
		delete[] bitmap;
	}
	bitmap = new char[tamanioBytes];
	memset(bitmap, 0, tamanioBytes);
	tamanio = tamanioBytes;
	inicializado =  true;
}

Bitmap::~Bitmap() {
	if (inicializado) {
		delete[] bitmap;
	}
}

void Bitmap::marcarBit(int index) {
	int byte = (index / 8);
	int offset = (index % 8);
	int bloqueBits = bitmap[byte];
	bitmap[byte] = (bloqueBits | (int)pow(2, (7 - offset))); //pone en 1 el bit #offset empezando de izq a derecha
}

void Bitmap::desmarcarBit(int index){
	int byte = (index / 8);
		int offset = (index % 8);
		int bloqueBits = bitmap[byte];
		bitmap[byte] = (bloqueBits & ~((int)pow(2, (7 - offset)))); //pone en 0 el bit #offset empezando de izq a derecha
}

bool Bitmap::estaMarcada(int index) {
	int byte = (index / 8);
	int offset = (index % 8);
	return ((bitmap[byte] & (int)pow(2, (7 - offset))) != 0);
}
const char* Bitmap::getBitmap() {
	return bitmap;
}
int Bitmap::getTamanioEnBytes(){
	return tamanio;
}

bool Bitmap::estaOk(){
	return inicializado;
}

Bitmap* Bitmap::nuevoPorFusion(Bitmap& otro){
	Bitmap* resultado = new Bitmap();

	if(tamanio == otro.tamanio){
		char* bitmap = new char[tamanio];
		for (int i = 0; i < tamanio; ++i) {
			bitmap[i] = ~(this->bitmap[i] | (~(otro.bitmap[i])));
		}
		resultado->inicializarBitmap(bitmap,tamanio);
		delete[] bitmap;
	}
	return resultado;
}

bool Bitmap::estaVacio(){
	bool vacio = true;
	int i = 0;
	while(vacio && i < tamanio){
		vacio = ((bitmap[i] & 0xFF) == 0);
		i++;
	}
	return vacio;
}

bool Bitmap::getPrimeroLibre(unsigned int &index){
	bool encontro = false;
	int i = 0;
	while(!encontro && (i<tamanio)){
		encontro = ((bitmap[i] & 0xFF) != 0);
		i++;
	}
	if(encontro){
		index = (--i*8);
		char temp = bitmap[i];
		i=0;
		encontro = false;
		while(!encontro){ //siempre da true.
			encontro = (((temp <<i) & 0x80) == 0x80);
			i++;
		}
		index += --i;
	}

	return encontro;
}

