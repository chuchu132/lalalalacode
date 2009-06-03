/*
 * TestDummie.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef TESTDUMMIE_H_
#define TESTDUMMIE_H_

#include "Test.h"

class TestDummie: public Test {
public:
	TestDummie();
	virtual ~TestDummie();
	void run();
	void setup();
};

#endif /* TESTDUMMIE_H_ */
