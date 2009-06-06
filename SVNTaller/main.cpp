/* 
 * File:   main.cpp
 *
 * Created on 30 de mayo de 2009, 23:25
 */
#include <iomanip>
#include <ios>
#include <sstream>
#include <iostream>

#include <stdlib.h>
#include "BencodeParser.h"
#include "sha1.h"
/*
 * 
 */
using namespace std;

void obtenerInfoHash(datosParser*datos, SHA1 sha);
void ProcesarHash(char*hash, SHA1 sha);
void MostrarSalidaSha(unsigned *message_digest);
void mostrarInfoHash(unsigned *hash);
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

    obtenerInfoHash(datos, sha);

    delete datos;
    return (EXIT_SUCCESS);
}

void obtenerInfoHash(datosParser*datos, SHA1 sha) {


    //Se obtiene el info hash calculando el valor de sha1 de la cadena del torrent perteneciente al diccionario "info"
    string buffer; 
    int salida = 1;
    unsigned mensajeDigerido[5];
    
    char *hashBinario;
    datos->primero();
    while (datos->final()) {

        if (!strcmp(datos->obtenerDato(), "info")) {
            datos->siguiente();
            do {
                if (strcmp(datos->obtenerDato(), "pieces")) {
                    buffer = buffer + datos->obtenerDato();
                } else {
                    datos->siguiente();
                    buffer = buffer + datos->obtenerDato();
                    break;
                }
                datos->siguiente();
            } while (salida != 0);
        }
        datos->siguiente();
    }

    char *bufferAux = new char [buffer.size() + 1];
    int it;
    for (it = 0; it <= buffer.size(); it++)
        bufferAux[it] = buffer[it];

    sha.inicializacion();
    sha.entrada(bufferAux, strlen(bufferAux));
    sha.salida(mensajeDigerido);

    std::cout << " --- Info Hash del archivo .torrent ---" << std::endl;
    mostrarInfoHash(mensajeDigerido);
    std::cout << std::endl;
    
    // Pasaje de la salida del sha1 a string y luego a binario 
    hashBinario=sha.sha1Abinario(sha.salidaAstring(mensajeDigerido));
    std::cout<<hashBinario<<std::endl;
    
    delete []hashBinario;
    delete []bufferAux;


}

FILE* menuInicio() {

    char bufferTorrent[512];
    int seleccion = 0;
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

    // Particiona el string con los hash de cada piece en cadenas de 20 bytes y los muestra por pantalla
    // Al ser datos binarios los que se envian del hash los caracteres que se muestran no son legibles 
    char hash[20];
    int pos;
    unsigned aux;

    for (aux = 0; aux < strlen(datos); aux++) {

        for (pos = 0; pos < 20; pos++) {
            hash[pos] = datos[aux];
            aux++;
        }
        aux--;
        std::cout << hash << std::endl;
    }

}

void mostrarInfoHash(unsigned *hash) {
    ios::fmtflags flags;

    flags = cout.setf(ios::hex | ios::uppercase, ios::basefield);
    cout.setf(ios::uppercase);

    for (int i = 0; i < 5; i++) {
        cout << hash[i] << ' ';
    }

    cout << endl;

    cout.setf(flags);




}

