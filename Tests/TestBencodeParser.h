/*
 * TestBencodeParser.h
 *
 *  Created on: 09/06/2009
 *      Author: ale
 */

#ifndef TESTBENCODEPARSER_H_
#define TESTBENCODEPARSER_H_

#include "Test.h"

class TestBencodeParser: public Test {
public:
	TestBencodeParser();
	virtual ~TestBencodeParser();
	void run();
	void test(const char* url);
};

#endif /* TESTBENCODEPARSER_H_ */
