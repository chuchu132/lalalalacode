/*
 * Bitmap.h
 *
 *  Created on: 05/06/2009
 *      Author: ale
 */

#ifndef BITMAP_H_
#define BITMAP_H_

class Bitmap {
public:

	/*Crea un Bitmap vacio*/
	Bitmap();

	/*Constructor copia*/
	Bitmap(const Bitmap& original);

	virtual ~Bitmap();

	/*Inicializa un bitmap con los datos pasados*/
	void inicializarBitmap(const char* unBitmap,int tamanioBytes);

	/*Inicializa un Bitmap con el tamanio pasado y rellena con 0*/
	void inicializarBitmap(int tamanioBytes);

	/*Pone en 1 el bit indicado por index*/
	void marcarBit(int index);

	/*Pone en 0 el bit indicado por index*/
	void desmarcarBit(int index);

	/*Devuelve true si el bit indicado por index esta en 1*/
	bool estaMarcada(int index);

	/*Devulve un puntero al bitmap*/
	const char* getBitmap();

	int getTamanioEnBytes();

	/*Devuelve el estado del Bitmap*/
	bool estaOk();

	/*Devuelve false si hay almenos 1 bit en 1*/
	bool estaVacio();

	/*TODO nombre poco feliz pero es lo que hay <- LU: esta bien el nombre.. sino se podria llamar Partes.. es lo mismo XD
	 * Aplica bit a bit la funcion ~(A + ~(B))
	 * donde A son los bits del Bitmap "this"
	 * y B son los bits del Bitmap "otro".
	 * Tabla de verdad:
	 *  A | B | Res
	 *  0 | 0 | 0
	 *  0 | 1 | 1
	 *  1 | 0 | 0
	 *  1 | 1 | 0
	 * Devuelve un new Bitmap con el resultado.
	 * Si los bits en 1 representan partes de un archivo que poseo,
	 * el resultado me indicaria con un 1 la parte que B tiene y a A
	 * le falta.
	 *
	 * Hay que liberar el nuevo Bitmap.
	 */
	Bitmap* nuevoPorFusion(Bitmap& otro);

	 bool getPrimeroLibre(unsigned int &index);

private:
	char* bitmap;
	int tamanio;
	bool inicializado;

};

#endif /* BITMAP_H_ */
