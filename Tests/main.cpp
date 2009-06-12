/*
 * main.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "CppTester.h"
#include "TestBitmap.h"
#include "TestDummy.h"
#include "TestVista.h"
#include "TestParserCgi.h"
#include "TestSha1.h"
#include "TestDatosParser.h"
#include "TestBencodeParser.h"
#include "TestTracker.h"

int main (int argc, char* argv[])
{
	CppTester tester;

	/*  Agregar los tests al final, antes de correrTests() */
	tester.agregarTest(new TestDummy(),"Test dummy");
	//tester.agregarTest(new TestVista(),"Test Vista");
	tester.agregarTest(new TestParserCgi(),"Test ParserCgi");
	tester.agregarTest(new TestSha1(),"Test Sha1");
	tester.agregarTest(new TestBitmap(),"Test Bitmap");
	tester.agregarTest(new TestDatosParser(),"Test DatosParser");
	tester.agregarTest(new TestBencodeParser(),"Test BencodeParser");
	tester.agregarTest(new TestTracker(),"Test Tracker");

	tester.correrTests();

	return 0;
}
