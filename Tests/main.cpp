/*
 * main.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "CppTester.h"
#include "TestDummie.h"
#include "TestVista.h"
#include "TestParserCgi.h"
#include "TestSha1.h"

int main (int argc, char* argv[])
{
	CppTester tester;

	/*  Agregar los tests al final, antes de correrTests() */
	tester.agregarTest(new TestDummie(),"Test dummie 1");
	tester.agregarTest(new TestDummie(),"Test dummie 2");
	//tester.agregarTest(new TestVista(),"Test Vista");
	tester.agregarTest(new TestParserCgi(),"Test ParserCgi");
	tester.agregarTest(new TestSha1(),"Test Sha1");

	tester.correrTests();

	return 0;
}
