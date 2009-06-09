#include "DatosParser.h"


DatosParser::DatosParser(){


}

DatosParser::~DatosParser(){

   for (it=datos.begin();it!=datos.end();it++)
	delete [] *it;
}

void DatosParser::agregarDato(char *cadena,int longitud){

   datos.push_front(cadena);
   len.push_front(longitud);

}

char* DatosParser::obtenerDato(){
   return *it;
}


int DatosParser::obtenerLongitudDato(){

  return (*itLongitudes);
}


void DatosParser::primero(){

   it=datos.begin();
   itLongitudes = len.begin();
}

void DatosParser::siguiente(){
   it++;
   itLongitudes++;
}

void DatosParser::anterior(){

   if (it!=datos.begin()){
	it--;
	itLongitudes--;
   }

}

bool DatosParser::final(){
	return (it == datos.end());
}



