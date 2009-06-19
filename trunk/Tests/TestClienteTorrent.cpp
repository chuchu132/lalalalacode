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

	Torrent* torrent = cliente.agregarTorrent("./Tests/boom.torrent");

	torrent->continuar();

	sleep(60); // 5 min

	torrent->detener();
	sleep(10);
	delete torrent;



//	ClienteTorrent cliente;
//		Controlador control(cliente);
//
//		Torrent* torrent = cliente.agregarTorrent("./Tests/PSP.torrent");
//
//		Socket* soc = new Socket();
//		soc->connect("localhost",9999);
//		Peer* peer = new PeerDown(soc,torrent);
//		//torrent->agregarPeer(peer);
//		peer->sendHandshake();
//		peer->sendBitfield();
//		peer->sendKeepAlive();
//
//		torrent->detener();
//		sleep(10);

}
