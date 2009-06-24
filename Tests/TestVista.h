/*
 * TestVista.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */


#include "../src/ClienteTorrent.h"
#include "../src/Torrent.h"
#include "../src/Controlador.h"
#include  <iostream>

#ifndef TESTVISTA_H_
#define TESTVISTA_H_

#include "Test.h"

class TestVista: public Test {
public:
	TestVista();
	virtual ~TestVista();
	void run();
};

#endif /* TESTVISTA_H_ */
