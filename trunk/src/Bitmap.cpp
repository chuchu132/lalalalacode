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

Bitmap::Bitmap(Bitmap& original){
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

void Bitmap::marcarPieza(int index) {
	int byte = (index / 8);
	int offset = (index % 8);
	int bloqueBits = bitmap[byte];
	bitmap[byte] = (bloqueBits | (int)pow(2, (7 - offset))); //pone en 1 el bit offset empezando de izq a derecha
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

Bitmap Bitmap::nuevoPorFusion(Bitmap& otro){
	Bitmap resultado;

	if(tamanio == otro.tamanio){
		char* bitmap = new char[tamanio];
		for (int i = 0; i < tamanio; ++i) {
			bitmap[i] = ~(bitmap[i] | (~(otro.bitmap[i])));
		}
		resultado.inicializarBitmap(bitmap,tamanio);
		delete[] bitmap;
	}
	return resultado;
}
