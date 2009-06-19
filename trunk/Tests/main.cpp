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
#include "TestConfiguracion.h"
#include "TestClienteTorrent.h"
#include "TestFileManager.h"


int main (int argc, char* argv[])
{
	CppTester tester;
//
//	tester.agregarTest(new TestSha1(),"Test Sha1");
//	tester.agregarTest(new TestBitmap(),"Test Bitmap");
//	tester.agregarTest(new TestDatosParser(),"Test DatosParser");
	tester.agregarTest(new TestVista(),"Test Vista");
//	tester.agregarTest(new TestClienteTorrent(),"Test ClienteTorrent");
//	tester.agregarTest(new TestBencodeParser(),"Test BencodeParser");
//	tester.agregarTest(new TestConfiguracion(),"Test Archivo Configuracion");
//	tester.agregarTest(new TestFileManager(),"Test Filemanager");
//	tester.agregarTest(new TestTracker(),"Test Tracker"); //sin testear


	tester.correrTests();

	return 0;
}
