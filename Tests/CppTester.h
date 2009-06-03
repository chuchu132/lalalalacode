/*
 * CppTester.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef CPPTESTER_H_
#define CPPTESTER_H_

#include "Test.h"
#include <list>
#include <string>

class CppTester {
public:
	CppTester();
	virtual ~CppTester();
	void agregarTest(Test* test,std::string titulo);
	void correrTests();
private:
	std::list<Test*> tests;
	std::list<std::string> titulos;
};

#endif /* CPPTESTER_H_ */
