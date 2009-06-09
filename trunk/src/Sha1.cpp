
#include "Sha1.h"
#include "Constantes.h"

Sha1::Sha1() {

    constanteSha1[0] = 0x5A827999;
    constanteSha1[1] = 0x6ED9EBA1;
    constanteSha1[2] = 0x8F1BBCDC;
    constanteSha1[3] = 0xCA62C1D6;

    inicializacion();
}

Sha1::~Sha1() {
    // The destructor does nothing
}

void Sha1::inicializacion() {

    //Inicializo los atributos para procesar un nuevo mensaje
    error = false;
    longitudInferior = 0;
    longitudSuperior = 0;
    IndiceArrayBloques = 0;

    //Condicion del buffer para el algoritmo
    bufferMensaje[0] = 0x67452301;
    bufferMensaje[1] = 0xEFCDAB89;
    bufferMensaje[2] = 0x98BADCFE;
    bufferMensaje[3] = 0x10325476;
    bufferMensaje[4] = 0xC3D2E1F0;
}

void Sha1::entrada(const char *mensaje, unsigned longitud) {

    //Verifico que no hay ningun error y que no se encuentre procesado el mensaje
    if ((!longitud) || (error)) {
        error = true;
    } else {

        //Coloco el mensaje de entrad en el buffer
        while (longitud-- && !error) {
            bloquesMensaje[IndiceArrayBloques++] = (*mensaje & 0xFF);

            longitudSuperior += 8;
            //Fuerzo a que sea 32 bits
            longitudSuperior &= 0xFFFFFFFF;
            //Proceso los bloques
            if (IndiceArrayBloques == 64) {
                procesarBloques();
            }
            mensaje++;
        }

    }
}

bool Sha1::salida(unsigned *mensajeSalida) {

    int contador;

    //Verifico que no se haya dado un error en el proceso
    if (error) {
        return false;
    }
    rellenado();

    //Copio el contenido del buffer con el mensaje procesado al mensaje de salida
    for (contador = 0; contador < 5; contador++) {
        mensajeSalida[contador] = bufferMensaje[contador];
    }

    return true;
}

void Sha1::procesarBloques() {

    int cont;
    unsigned temp;
    unsigned Aux[100];
    unsigned bufferTemp[5];

    //Inicializo el array
    setearBloque(Aux, bufferTemp);

    //Proceso del algoritmo Sha1
    for (cont = 0; cont < 80; cont++) {

        if (cont >= 0 && cont < 20)
            temp = circularShift(5, bufferTemp[0]) + ((bufferTemp[1] & bufferTemp[2]) | ((~bufferTemp[1]) & bufferTemp[3])) + bufferTemp[4] + Aux[cont] + constanteSha1[0];

        if (cont >= 20 && cont < 40)
            temp = circularShift(5, bufferTemp[0]) + (bufferTemp[1] ^ bufferTemp[2] ^ bufferTemp[3]) + bufferTemp[4] + Aux[cont] + constanteSha1[1];

        if (cont >= 40 && cont < 60)
            temp = circularShift(5, bufferTemp[0]) + ((bufferTemp[1] & bufferTemp[2]) | (bufferTemp[1] & bufferTemp[3]) | (bufferTemp[2] & bufferTemp[3])) + bufferTemp[4] + Aux[cont] + constanteSha1[2];

        if (cont >= 60 && cont < 80)
            temp = circularShift(5, bufferTemp[0]) + (bufferTemp[1] ^ bufferTemp[2] ^ bufferTemp[3]) + bufferTemp[4] + Aux[cont] + constanteSha1[3];

        asignacionParcial(temp, bufferTemp);
    }


    for (cont = 0; cont < 5; cont++) {
        bufferMensaje[cont] = (bufferMensaje[cont] + bufferTemp[cont]) & 0xFFFFFFFF;
    }


    IndiceArrayBloques = 0;
}

void Sha1::setearBloque(unsigned Aux[], unsigned bufferTemp[]) {

    int cont;

    for (cont = 0; cont < 16; cont++) {
        Aux[cont] = ((unsigned) bloquesMensaje[cont * 4]) << 24;
        Aux[cont] |= ((unsigned) bloquesMensaje[cont * 4 + 1]) << 16;
        Aux[cont] |= ((unsigned) bloquesMensaje[cont * 4 + 2]) << 8;
        Aux[cont] |= ((unsigned) bloquesMensaje[cont * 4 + 3]);
    }

    for (cont = 16; cont < 80; cont++) {
        Aux[cont] = circularShift(1, Aux[cont - 3] ^ Aux[cont - 8] ^ Aux[cont - 14] ^ Aux[cont - 16]);
    }

    for (cont = 0; cont < 5; cont++) {
        bufferTemp[cont] = bufferMensaje[cont];
    }


}

void Sha1::asignacionParcial(unsigned & temp, unsigned bufferTemp[]) {

    int aux = 4;

    temp &= 0xFFFFFFFF;

    for (aux = 4; aux >= 0; aux--) {
        if (aux != 2 && aux != 0)
            bufferTemp[aux] = bufferTemp[aux - 1];
        else {
            if (aux == 2)
                bufferTemp[aux] = circularShift(30, bufferTemp[aux - 1]);
            else
                bufferTemp[aux] = temp;
        }

    }

}

void Sha1::rellenado() {


    //Se realiza el padding como establece el estandar, debe realizarse el padding a 512bits
    //Despues de terminado este procedimiento se obtiene el mensaje procesado por completo

    bloquesMensaje[IndiceArrayBloques++] = 0x80;

    if (IndiceArrayBloques > 55) {

        rellenadoParcial(64);
        procesarBloques();
    }
    rellenadoParcial(56);

    // Se almacena la longitud del mensaje en las ultimas 8 posiciones

    int cont, mov = 24;
    for (cont = 56; cont < 64; cont++) {

        if (cont < 60)
            bloquesMensaje[cont] = (longitudInferior >> mov) & 0XFF;
        else {
            if (cont == 60) mov = 24;
            bloquesMensaje[cont] = (longitudSuperior >> mov) & 0XFF;
        }
        mov -= 8;
    }

    procesarBloques();

}

void Sha1::rellenadoParcial(int limite) {

    while (IndiceArrayBloques < limite) {
        bloquesMensaje[IndiceArrayBloques++] = 0;
    }

}

unsigned Sha1::circularShift(int cantBits, unsigned bloque) {
    return ((bloque << cantBits) & 0xFFFFFFFF) | ((bloque & 0xFFFFFFFF) >> (32 - cantBits));
}

std::string Sha1::salidaAstring(unsigned *salidaSha1) {
    int i;
    char *hash = new char [20 * CHAR_BIT];
    hash[0] = '\0';

    for (i = 0; i < 5; i++)
        sprintf(hash, "%s%u", hash, salidaSha1[i]);

    std::string salida = hash;
    delete[] hash;
    //retorna la salida del sha1 en forma de una cadena
    return salida;
}

char* Sha1::sha1Abinario(string hash) {

    int n_bits, it, i;

    char *cadenaBinaria = new char [LEN_SHA1 * sizeof (char) ];

    char ret, val;

    for (it = 0; it < LEN_SHA1; it++) {

        ret = 0;
        val = hash[it];
        n_bits = sizeof ( char) * CHAR_BIT;

        for (i = 0; i < n_bits; ++i) {
            ret = (ret << 1) | (val & 1);
            val >>= 1;
        }

        cadenaBinaria[it] = ret;
    }
    return cadenaBinaria;
}

void Sha1::imprimirShaBinario(string hash) {

    int n_bits, i, it;
    char val;

     for (it = 0; it < LEN_SHA1; it++) {
        val = hash[it];
        n_bits = sizeof ( char) * CHAR_BIT;

        for (i = 0; i < n_bits; ++i) {
            std::cout << !!(val & 1);
            val >>= 1;
        }
    }


}

std::string Sha1::codificar(const char* mensaje, int longitud){
	unsigned int mensajeDigerido[5];
	inicializacion();
	entrada(mensaje,longitud);
	salida(mensajeDigerido);
	return salidaAstring(mensajeDigerido);
}
