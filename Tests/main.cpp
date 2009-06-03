/*
 * main.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */



#include "CppTester.h"
#include "TestDummie.h"
#include "TestVista.h"

int main (int argc, char* argv[])
{
	CppTester tester;

	tester.agregarTest(new TestDummie(),"Test dummie 1");
	tester.agregarTest(new TestDummie(),"Test dummie 2");
	tester.agregarTest(new TestVista(),"Test Vista");

	tester.correrTests();





	return 0;
}
