/*
 * Test.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "Test.h"
#include <iostream>

Test::Test() {}

Test::~Test() {}

void Test::execute(){
	setup();
	run();
}

void Test::assert(bool condition,std::string message){
	std::cout<< ((condition)?"OK Assertion: ":"ERROR Assertion :") << message <<std::endl;
}

void Test::fail(bool condition,std::string message){
	std::cout<< ((!condition)?"OK Fail: ":"ERROR Fail: ")<< message <<std::endl;
}


void Test::setup(){}
