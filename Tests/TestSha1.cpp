/*
 * TestSha1.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestSha1.h"
#include "../src/Sha1.h"

TestSha1::TestSha1() {
	// TODO Auto-generated constructor stub

}

TestSha1::~TestSha1() {
	// TODO Auto-generated destructor stub
}

void MostrarSalidaSha(unsigned *mensajeDigerido)
{
	ios::fmtflags	flags;

	flags = cout.setf(ios::hex|ios::uppercase,ios::basefield);
	cout.setf(ios::uppercase);
	for(int i = 0; i < 5 ; i++)
	{
		cout << mensajeDigerido[i] << ' ';
	}
	cout << endl;
	cout.setf(flags);
}

void TestSha1::run(){
	SHA1		sha;
	unsigned	mensajeDigerido[5];
	const char* prueba1 = "abc";
	const char* prueba2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

	sha.inicializacion();
	sha.entrada(prueba1,strlen(prueba1));
	Test::assert(sha.salida(mensajeDigerido),"Se puede procesar prueba 1");
	std::cout<<"SHA1 obtenido = ";
	MostrarSalidaSha(mensajeDigerido);
	std::cout<<"SHA1 esperado = "<<"A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D"<<std::endl;

	sha.inicializacion();
	sha.entrada(prueba2,strlen(prueba2));
	Test::assert(sha.salida(mensajeDigerido),"Se puede procesar prueba 2");
	std::cout<<"SHA1 obtenido = ";
	MostrarSalidaSha(mensajeDigerido);
	std::cout<<"SHA1 esperado = "<<"84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1"<<std::endl;
}



