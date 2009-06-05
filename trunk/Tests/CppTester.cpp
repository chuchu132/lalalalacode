/*
 * CppTester.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "CppTester.h"
#include <iostream>
CppTester::CppTester() {}

CppTester::~CppTester() {
	std::list<Test*>::iterator it;
	for (it = tests.begin(); it != tests.end(); ++it) {
	 		delete (*it);
		}
}

void CppTester::agregarTest(Test* test,std::string titulo = "Test Anonimo"){
	tests.insert(tests.end(),test);
	titulos.insert(titulos.end(),titulo);
}

void CppTester::correrTests(){
	std::list<Test*>::iterator it;
	std::list<std::string>::iterator itTitulos;
	itTitulos = titulos.begin();
	for (it = tests.begin(); it != tests.end(); ++it) {
		std::cout<<std::endl<<*itTitulos<<std::endl<<std::endl;
		itTitulos++;
		(*it)->execute();
	}
}
