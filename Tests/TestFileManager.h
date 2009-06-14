/*
 * TestFileManager.h
 *
 *  Created on: 13/06/2009
 *      Author: ale
 */

#ifndef TESTFILEMANAGER_H_
#define TESTFILEMANAGER_H_

#include "Test.h"
#include <string>

class TestFileManager: public Test {
public:
	TestFileManager();
	virtual ~TestFileManager();
	void run();
	void test(std::string urlTorrent);
};

#endif /* TESTFILEMANAGER_H_ */
