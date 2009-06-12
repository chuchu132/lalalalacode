/*
 * TestParserCgi.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef TESTPARSERCGI_H_
#define TESTPARSERCGI_H_

#include "Test.h"
#include "../src/ParserCgi.h"

class TestParserCgi: public Test {
public:
	TestParserCgi();
	virtual ~TestParserCgi();
	void run();
	void TestCodificar();
	void TestDecodificar();
private:
	ParserCgi parser;
	std::string original;
	std::string codificado;
	std::string codificadoEntero;
};

#endif /* TESTPARSERCGI_H_ */
