/*
 * Test.cpp
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#include "Test.h"
#include <iostream>

Test::Test() {
	ok = 0;
	mal = 0;
}

Test::~Test() {}

void Test::execute(){
	setup();
	run();
	resumen();
}

void Test::assert(bool condition,std::string message){
	std::cout<< ((condition)?"OK Assertion: ":"ERROR Assertion :") << message <<std::endl;
	if(condition){ ok++;}
	else{mal++;}
}

void Test::fail(bool condition,std::string message){
	std::cout<< ((!condition)?"OK Fail: ":"ERROR Fail: ")<< message <<std::endl;
	if(!condition){ ok++;}
		else{mal++;}
}


void Test::setup(){}

void Test::resumen(){
	int total = ok + mal;
	std::cout<<std::endl<<"Tests Ok: "<<ok<<"/"<<total<<"  Tests Mal: "<<mal<<"/"<<total<<std::endl;
}
