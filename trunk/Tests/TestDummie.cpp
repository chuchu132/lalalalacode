/*
 * TestDummie.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "TestDummie.h"
#include <iostream>

TestDummie::TestDummie() {
	// TODO Auto-generated constructor stub

}

TestDummie::~TestDummie() {
	// TODO Auto-generated destructor stub
}

void test1(){
	Test::fail(false," El test 1 debe dar false.");

}
void test2(){
	Test::assert(false," El test 2 debe dar true.");

}

void test3(){
	Test::assert(true," El test 3 debe dar true.");

}

void TestDummie::run(){
	test1();
	test2();
	test3();
}

void TestDummie::setup(){
	std::cout<<"Inicializo"<<std::endl;
}
