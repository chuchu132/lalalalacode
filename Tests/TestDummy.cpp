/*
 * TestDummy.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestDummy.h"
#include <iostream>

TestDummy::TestDummy() {}

TestDummy::~TestDummy() {}

void TestDummy::test1(){
	fail(false," El test 1 debe dar false.");
}
void TestDummy::test2(){
	assert(false," El test 2 debe dar true.");
}

void TestDummy::test3(){
	assert(true," El test 3 debe dar true.");
}

void TestDummy::run(){
	test1();
	test2();
	test3();
}

void TestDummy::setup(){
	std::cout<<"Inicializo"<<std::endl;
}
