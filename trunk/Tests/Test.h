/*
 * Test.h
 *
 *  Created on: 03/06/2009
 *      Author: ale
 */

#ifndef TEST_H_
#define TEST_H_
#include <string>

class Test {
public:
	Test();
	virtual ~Test();
	/*Inicializa el test y luego lo corre*/
	void execute();

	void assert(bool condicion, std::string message);
	void fail(bool condicion, std::string  message );

	/*Setup se llama siempre antes del run sirve para inicializar el test*/
	virtual void setup();
	/*En el run se corre el test*/
	virtual void run()=0;

	void resumen();

private:
	int ok;
	int mal;


};

#endif /* TEST_H_ */
