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
	Bitmap();
	Bitmap(Bitmap& original);
	virtual ~Bitmap();
	void inicializarBitmap(const char* unBitmap,int tamanioBytes);
	void inicializarBitmap(int tamanioBytes);
	void marcarPieza(int index);
	bool estaMarcada(int index);
	const char* getBitmap();
	int getTamanioEnBytes();
	bool estaOk();

	/*TODO nombre poco feliz pero es lo que hay
	 * Aplica bit a bit la funcion ~(A + ~(B))
	 * donde A son los bits del Bitmap "this"
	 * y B son los bits del Bitmap "otro".
	 * Tabla de verdad:
	 *  A | B | Res
	 *  0 | 0 | 0
	 *  0 | 1 | 1
	 *  1 | 0 | 0
	 *  1 | 1 | 0
	 * Devuelve un Bitmap con el resultado.
	 * Si los bits en 1 representan partes de un archivo que poseo,
	 * el resultado me indicaria con un 1 la parte que B tiene y a A
	 * le falta.
	 */
	Bitmap nuevoPorFusion(Bitmap& otro);

private:
	char* bitmap;
	int tamanio;
	bool inicializado;
};

#endif /* BITMAP_H_ */
