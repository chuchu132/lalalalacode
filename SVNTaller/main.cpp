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
void ProcesarHash(char*hash,SHA1 sha);
void MostrarSalidaSha(unsigned *message_digest);
FILE* menuInicio();

int main(int argc, char** argv) {


    FILE *fp;
    datosParser *datos;
    SHA1 sha;
  
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
	   if (!strcmp(datos->obtenerDato(),"pieces")){
       		   datos->siguiente();	
	
		   ProcesarHash(datos->obtenerDato(),sha);
		
	   }
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

void ProcesarHash(char * datos, SHA1 sha){

unsigned mensajeDigerido[5];
char hash[20];
int pos;
unsigned aux;

for (aux=0;aux<strlen(datos);aux++){

 for (pos=0;pos<20;pos++){
	hash[pos]= datos[aux];
	
 }
 sha.inicializacion();
 sha.entrada(hash,strlen(hash));
 if (!sha.salida(mensajeDigerido))
	cerr << "ERROR-- no se pudo procesar el mensaje" << endl;			
 else
	MostrarSalidaSha(mensajeDigerido);
}

}

void MostrarSalidaSha(unsigned *mensajeDigerido)
{
	ios::fmtflags	flags;

	cout << '\t';

	flags = cout.setf(ios::hex|ios::uppercase,ios::basefield);
	cout.setf(ios::uppercase);

	for(int i = 0; i < 5 ; i++)
	{
		cout << mensajeDigerido[i] ;
	}

	cout << endl;

	cout.setf(flags);    
        
}





