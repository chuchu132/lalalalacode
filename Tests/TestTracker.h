/*
 * TestTracker.h
 *
 *  Created on: 11/06/2009
 *      Author: ale
 */

#ifndef TESTTRACKER_H_
#define TESTTRACKER_H_

#include "../src/DatosParser.h"
#include "Test.h"

class TestTracker: public Test {
public:
	TestTracker();
	virtual ~TestTracker();
	void run();

private:
	DatosParser* datos;
};

#endif /* TESTTRACKER_H_ */
