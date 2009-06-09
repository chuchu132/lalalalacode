/*
 * TestSha1.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestSha1.h"
#include "../src/Sha1.h"
#include <string.h>

TestSha1::TestSha1() {}

TestSha1::~TestSha1() {}

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
	Sha1		sha;
	const char* prueba1 = "abc";
	const char* prueba2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

	std::string salida;
	salida = sha.codificar(prueba1,strlen(prueba1));
	Test::assert(true,"Se puede procesar prueba 1");
	std::cout<<"SHA1 obtenido = "<<salida<<std::endl;
	std::cout<<"SHA1 esperado = "<<"A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D"<<std::endl;


	salida = sha.codificar(prueba2,strlen(prueba2));
	Test::assert(true,"Se puede procesar prueba 2");
	std::cout<<"SHA1 obtenido = "<<salida<<std::endl;
	std::cout<<"SHA1 esperado = "<<"84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1"<<std::endl;

}



