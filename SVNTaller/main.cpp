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
        //Verifico antes de imprimir que el proximo dato no sea uno de los flags de inicio o fin de diccionario
        if (strcmp(datos->obtenerDato(), "Fin") && strcmp(datos->obtenerDato(), "Inicio"))
            std::cout << datos->obtenerDato() << std::endl;
      //  Verifico el campo pieces para obtener los hash de cada piece
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
    int salida = 1, contador = 0;
    unsigned mensajeDigerido[5];

    char *hashBinario;
    datos->primero();
    //Recorro la lista de datos desde el comienzo hasta el final
    while (datos->final()) {
        // Verifico si el campo actual es de campo clave "info"
        if (!strcmp(datos->obtenerDato(), "info")) {
            datos->siguiente(); 
            //Proceso todo el diccionario "info" en un string para luego aplicarle el sha1
            do {
                if (!strcmp(datos->obtenerDato(), "Inicio")) {
                    contador++;
                } else {
                    if (!strcmp(datos->obtenerDato(), "Fin")) {
                        contador--;
                    } else {
                        buffer = buffer + datos->obtenerDato(); 
                    }
                }
                datos->siguiente();
                //Verifico que la lista no este en el final
                if (!datos->final())
                    break;
            } while (contador != 0);
            break;
        }
        datos->siguiente();
    }

    char *bufferAux = new char [buffer.size() + 1];
    int it;
    for (it = 0; it <= buffer.size(); it++)
        bufferAux[it] = buffer[it];

    //Inicializo el sha1
    sha.inicializacion();
    //Ingreso la cadena a calcularle el sha1
    sha.entrada(bufferAux, strlen(bufferAux));
    //Obtengo la salida del sha1 en el mensajeDigerido 
    sha.salida(mensajeDigerido);

    std::cout << " --- Info Hash del archivo .torrent ---" << std::endl;
    //Muestro por pantalla el hash tal cual sale del algoritmo 
    mostrarInfoHash(mensajeDigerido);
    std::cout << std::endl;

    //Pasa de la salida del sha1 a string y luego a su correspondiente binario 
    hashBinario = sha.sha1Abinario(sha.salidaAstring(mensajeDigerido));
    //Imprime por pantalla el binario del sha1 
    std::cout << hashBinario << std::endl;

    delete []hashBinario;
    delete []bufferAux;


}

FILE * menuInicio() {

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

