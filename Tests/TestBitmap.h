/*
 * TestBitmap.h
 *
 *  Created on: 06/06/2009
 *      Author: ale
 */

#ifndef TESTBITMAP_H_
#define TESTBITMAP_H_

#include "Test.h"

class TestBitmap: public Test {
public:
	TestBitmap();
	virtual ~TestBitmap();
	void run();
	void testBitmapNulo();
	void testBitmapNoNulo();
	void testFuncionLoca();
};

#endif /* TESTBITMAP_H_ */
