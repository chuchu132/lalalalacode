/*
 * TestBitmap.cpp
 *
 *  Created on: 06/06/2009
 *      Author: ale
 */

#include "../src/Bitmap.h"
#include "TestBitmap.h"
#include <string>
#include <string.h>

TestBitmap::TestBitmap() {
}

TestBitmap::~TestBitmap() {
}

void TestBitmap::run(){
	testBitmapNulo();
	testBitmapNoNulo();
	testFuncionLoca();
}

void TestBitmap::testBitmapNulo(){
	Bitmap bitmap;
	assert(!bitmap.estaOk(),"El bitmap no fue inicializado.");
	char bits[4]; // 4 bytes 32 bits

	memset(bits,0,4);
	bitmap.inicializarBitmap(bits,4);
	assert(memcmp(bits,bitmap.getBitmap(),4)==0,"Los bitmaps deben ser iguales");

}

void TestBitmap::testBitmapNoNulo(){
	Bitmap bitmap;
	char bits[4]; // 4 bytes 32 bits
	memset(bits,0,4);
	bitmap.inicializarBitmap(bits,4);
	bits[0]=1;
	bitmap.marcarBit(7);
	assert(memcmp(bits,bitmap.getBitmap(),4)==0,"Los bitmaps deben ser iguales");

	bits[1]=4;
	bitmap.marcarBit(13);
	assert(memcmp(bits,bitmap.getBitmap(),4)==0,"Los bitmaps deben ser iguales");

	bitmap.desmarcarBit(13);
	bits[1]=0;
	assert(memcmp(bits,bitmap.getBitmap(),4)==0,"Los bitmaps deben ser iguales");

	assert(bitmap.estaMarcada(7),"La pieza 7 esta marcada");

	fail(bitmap.estaMarcada(13),"La pieza 13 no esta marcada");
}

void TestBitmap::testFuncionLoca(){
	char A = 3; // 0011
	char B = 5; // 0101
	char R = 4; // 0100 resultado esperado luego de la funcion loca
	Bitmap bitmapA;
	bitmapA.inicializarBitmap(&A,1);
	Bitmap bitmapB;
	bitmapB.inicializarBitmap(&B,1);
	Bitmap* bitmapR = bitmapA.nuevoPorFusion(bitmapB);

	assert((memcmp(bitmapR->getBitmap(),&R,1) == 0),"Los bitmaps deben ser iguales");

	delete bitmapR;

}
