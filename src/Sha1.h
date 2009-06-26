/*
 * File:   sha1.h
 *
 * Created on 27 de mayo de 2009, 11:04
 * Author: Adrian
 */
#include "iostream"
#include <string>
using namespace std;

#ifndef _SHA1_H_
#define _SHA1_H_


/* ********************************************************************
 * Sha1 es la clase encargada de la conversion de una cadena en su    *
 * correspondiente codificacion con el algoritmo sha1 obteniendose    *
 * el mensaje digerido del metodo									  *
 **********************************************************************/

class Sha1 {
private:

	unsigned constanteSha1[5]; // Constantes que utiliza el algoritmo Sha1

	unsigned bufferMensaje[5]; // Buffers para el mensaje a ser digerido
	unsigned longitudInferior; // Longitud en bits del mensaje
	unsigned longitudSuperior; // Longitud en bits del mensaje

	bool error; // Atributo para verificar si hubo un error en el mensaje

	unsigned char bloquesMensaje[64]; // bloques de mensaje de 512 bits 512-bit
	int IndiceArrayBloques; // Indice para el array de bloques del mensaje

	//Metodos de la clase Sha1

public:

	/* Constructor*/
	Sha1();

	/* Destructor*/
	virtual ~Sha1();

	/* Inicializa los atributos de la clase para el ingreso de un nuevo mensaje a procesar */
	void inicializacion();

	/*
	 * Realiza la conversion del mensaje a su correspondiente codificacion sha1
	 * Retorna el sha1 resultante en un string
	 */
	std::string codificar(const char* mensaje,int longitud);

	/* Recibe como parametros la salida del sha1 y la pasa a una cadena que se devuelve finalizado el metodo */
	std::string salidaAstring(unsigned *salidaSha1);

	/*
	 * Devuelve el mensaje procesado en el parametro mensajeDigerido.
	 * Si hubo un error devuelve false , en caso contrario true
	 */
	bool salida(unsigned *mensajeDigerido);

	/* Entrada para el algoritmo Sha1 , recibe el  mensaje y la londitud del mismo */
	void entrada(const char *mensaje, unsigned longitud);

	/*realiza la conversion de la salida del algoritmo sha1 y lo codifica en Url */
	std::string binAUrlEncode(unsigned int* salidaSha1);

private:

	/*  Procesa los bloques del mensaje */
	void procesarBloques();

	/*  Setea los primeros 16 bits del bloque */
	void setearBloque(unsigned Aux[], unsigned bufferTemp[]);

	/*  Asigna los valores parciales procesados */
	void asignacionParcial(unsigned & temp, unsigned bufferTemp[]);

	/*  Realiza el padding de bloque actual a 512 bits */
	void rellenado();

	/*  Rellena con ceros hasta el valor del limite que se le envia como parametro */
	void rellenadoParcial(int limite);

	/*  Realiza la operacion de  shift del algoritmo */
	inline unsigned circularShift(int cantBits, unsigned bloque);

};
#endif
