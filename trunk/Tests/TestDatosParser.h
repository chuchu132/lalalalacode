/*
 * TestDatosParser.h
 *
 *  Created on: 09/06/2009
 *      Author: ale
 */

#ifndef TESTDATOSPARSER_H_
#define TESTDATOSPARSER_H_

#include "Test.h"
#include "../src/DatosParser.h"

class TestDatosParser: public Test {
public:
	TestDatosParser();
	virtual ~TestDatosParser();
	void setup();
	void run();
private:
	DatosParser datos;
};

#endif /* TESTDATOSPARSER_H_ */
