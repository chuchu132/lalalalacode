/*
 * File:   BencodeParser.h
 * Author:
 * Created on 30 de mayo de 2009, 23:25
 */

#ifndef _BENCODEPARSER_H
#define	_BENCODEPARSER_H
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Constantes.h"

/*
 * TODO
 * El parser deberia gusrdar en alguna estructura dinamica tal vez un hashmap, el archivo
 * parseado, e implementar algunas funciones para hacer consultas.
 *
 * Ver metodos que no tiene sentido llamar desde afuera del objeto, ponerlos private.
 */


class BencodeParser {
private:

    char buf[BUFSIZE]; //Almacena las cadenas temporarias
    int pos; //posicion dentro del buffer
    int buf_lim; //limite del buffer

    FILE *fp; //File pointer al archivo .torrent que se esta parseando
    int ident; //Atributo para visualizacion por pantalla
    int salida; //Atributo para la salida del parser

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

    //Funcion que se encarga de realizar una tabulacion cuando se imprime
    //es una funcion temporal solo para la visualizacion por pantalla
    void identacion(int indent);

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

};

#endif	/* _BENCODEPARSER_H */

