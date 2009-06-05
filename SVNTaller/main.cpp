/* 
 * File:   main.cpp
 *
 * Created on 30 de mayo de 2009, 23:25
 */

#include <stdlib.h>
#include "BencodeParser.h"
#include "sha1.h"
/*
 * 
 */
void ProcesarHash(char*hash, SHA1 sha);
void MostrarSalidaSha(unsigned *message_digest);
FILE* menuInicio();

int main(int argc, char** argv) {


    FILE *fp;
    datosParser *datos;
    SHA1 sha;

    if ((fp = menuInicio()) == NULL) return 1;

    std::cout << " --- Comienzo del Parser --- " << std::endl;

    BencodeParser parser(fp);

    parser.procesar();

    //Se obtienen los resultados del parser
    datos = parser.salidaParser();

    //Imprime por pantalla los resultados del parser
    datos->primero();
    while (datos->final()) {
        std::cout << datos->obtenerDato() << std::endl;
        if (!strcmp(datos->obtenerDato(), "pieces")) {
            datos->siguiente();

            ProcesarHash(datos->obtenerDato(), sha);

        }
        datos->siguiente();
    }
    //Fin de la impresion de los datos obtenidos

    std::cout << " --- Fin del Parser --- " << std::endl;

    delete datos;
    return (EXIT_SUCCESS);
}

FILE* menuInicio() {

    char bufferTorrent[512];
    int seleccion;
    FILE*fp;

    std::cout << std::endl << " --- Se utilizara el archivo defaut ( PSP.torrent ) \n --- Si desea ingresar otro .torrent presionar 1---" << std::endl;
    std::cout << "     ";
    std::cin >> seleccion;

    if (seleccion == 1) {
        std::cout << " --- Ingresar nombre del archivo torrent (Ejemplo: Nombre.torrent ) --- " << std::endl;
        std::cout << "     ";
        std::cin >> bufferTorrent;

        fp = fopen(bufferTorrent, "rb");

    } else {
        fp = fopen("PSP.torrent", "rb");
    }

    return fp;

}

void ProcesarHash(char * datos, SHA1 sha) {


    char hash[20];
    int pos;
    unsigned aux;

    for (aux = 0; aux < strlen(datos); aux++) {

        for (pos = 0; pos < 20; pos++) {
            hash[pos] = datos[aux];
            aux++;
        }
        aux--;
        //   Test para verificar la correcta conversion a binario 
        //   descomentando este codigo, el resultado deveria ser el que se encuentra continuacion

        /*   strcpy(hash,"This is a test");
             std::cout << hash << std::endl;
         */
        /*
         *
         01010100  T
         01101000  h
         01101001  i
         01110011  s
         00100000
         01101001  i
         01110011  s
         00100000
         01100001  a
         00100000
         01110100  t
         01100101  e
         01110011  s
         01110100  t
         *
         */

        char *cadena = new char [20 * sizeof (char) ];

        //Pasa los caracteres de sha1 a binario
        cadena = sha.sha1Abinario(hash);
        //Imprime la cadena binaria correspondiente al sha1
        sha.imprimirShaBinario(cadena);
        
        std::cout << std::endl << std::endl;
        delete []cadena;
    }

}


