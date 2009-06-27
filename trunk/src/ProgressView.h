/*
 * ProgressView.h
 *
 *  Created on: 26/06/2009
 *      Author: teddy
 */

#ifndef PROGRESSVIEW_H_
#define PROGRESSVIEW_H_

#include "Thread.h"

#include <gtkmm.h>
#include <glib.h>
#include <iostream>

class ProgressView: public Thread {

public:

	ProgressView();
	virtual ~ProgressView();

	void setBarra(Gtk::ProgressBar *progress_bar);
	void setWindow(Gtk::Dialog *progress_window);
	void setLabel(Gtk::Label *text);

	/* mueve la barra de progreso */
	void mover();

	/* abre la ventana */
	void* run();

	/* cierra la ventana */
	void hide();

	/* muestra texto en la ventana */
	void setText(std::string);

private:

	Gtk::Dialog *progress_window;//ventana con una barra de progreso
	Gtk::ProgressBar *progress_bar;
	Gtk::Label *text;
	int id_activity;
};

#endif /* PROGRESSVIEW_H_ */
