/* 
 * File:   main.cpp
 *
 * Created on 30 de mayo de 2009, 23:25
 */

#include <stdlib.h>
#include "BencodeParser.h"

/*
 * 
 */

FILE* menuInicio();

int main(int argc, char** argv) {


    FILE *fp;
    datosParser *datos;

    if ((fp = menuInicio()) == NULL) return 1;

    std::cout<< " --- Comienzo del Parser --- "<<std::endl;
 
    BencodeParser parser(fp);
    
    parser.procesar();
    
    //Se obtienen los resultados del parser
    datos=parser.salidaParser();

    //Imprime por pantalla los resultados del parser
    datos->primero();
    while (datos->final()){
	   std::cout<< datos->obtenerDato()<<std::endl;	
       	   datos->siguiente();
    }
    //Fin de la impresion de los datos obtenidos
  
    std::cout<< " --- Fin del Parser --- "<<std::endl;


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

