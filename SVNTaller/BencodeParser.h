/*
 * File:   BencodeParser.h
 *
 * Created on 30 de mayo de 2009, 23:25
 */

#ifndef _BENCODEPARSER_H
#define	_BENCODEPARSER_H
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include "DatosParser.h"
#include "Constantes.h"

using namespace std;

class BencodeParser {
private:

    char buf[BUFSIZE]; //Almacena las cadenas temporarias
    int pos; //posicion dentro del buffer
    int buf_lim; //limite del buffer

    FILE *fp; //File pointer al archivo .torrent que se esta parseando
    int ident; //Atributo para visualizacion por pantalla

    DatosParser datos;//objeto que almacena los datos obtenidos del parser

    int contador;
    int diccionario; 
    
public:

    //Constructor
    BencodeParser(FILE *fp);

    //Destructor
    ~BencodeParser();

    //Proceso del parser
    void procesar();

    //Parser para el tipo diccionario
    void parserDiccionario(FILE *fp);

    //Parser para el tipo lista
    void parserLista(FILE*fp);

    //Parser para los datos numericos
    void parserNumerico(FILE *fp);

    //Parser para las cadenas de caracteres
    void parserCadena(FILE *fp);

    //Realiza la Carga del buffer
    void cargarBuffer();

    //Verifica si el buffer esta listo
    void verificarBuffer();

    //Verifica cual es el proximo caracter en el buffer
    char verCaracterSiguiente();

    //Obtiene el proximo caracter del buffer
    char obtenerCaracter();

    //Verifica que el caracter sea un match con el que se le pasa como parametro
    void compararCaracter(char c);

    //Retorna la salida del parser
    DatosParser* salidaParser ();


};

#endif	/* _BENCODEPARSER_H */

