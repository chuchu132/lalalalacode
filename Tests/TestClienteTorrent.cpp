/*
 * TestClienteTorrent.cpp
 *
 *  Created on: 14/06/2009
 *      Author: ale
 */

#include "TestClienteTorrent.h"
#include "../src/ClienteTorrent.h"
#include "../src/Controlador.h"
#include "../src/Torrent.h"

TestClienteTorrent::TestClienteTorrent() {}

TestClienteTorrent::~TestClienteTorrent() {}

void TestClienteTorrent::run(){
	ClienteTorrent cliente;
	Controlador control(cliente);

	Torrent* torrent = cliente.agregarTorrent("./Tests/disco.torrent");

	torrent->run();

	sleep(10); // 5 min

	torrent->detener();
	sleep(10);

}
