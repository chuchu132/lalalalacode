/*
 * TestClienteTorrent.cpp
 *
 *  Created on: 14/06/2009
 *      Author: ale
 */

#include "TestClienteTorrent.h"
#include "../src/ClienteTorrent.h"

TestClienteTorrent::TestClienteTorrent() {}

TestClienteTorrent::~TestClienteTorrent() {}

void TestClienteTorrent::run(){
	ClienteTorrent cliente;
	cliente.inicializarDirectorios();
}
