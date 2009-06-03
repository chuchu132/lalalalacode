/*
 * TestDummy.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef TESTDUMMY_H_
#define TESTDUMMY_H_

#include "Test.h"

class TestDummy: public Test {
public:
	TestDummy();
	virtual ~TestDummy();
	void run();
	void setup();
	void test1();
	void test2();
	void test3();
};

#endif /* TESTDUMMY_H_ */
