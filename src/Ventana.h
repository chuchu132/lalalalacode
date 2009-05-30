/*
 * Ventana.h
 *
 *  Created on: 28/05/2009
 *      Author: teddy
 */

#ifndef VENTANA_H_
#define VENTANA_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>

#include "TorrentView.h"
#include "Controlador.h"

class Controlador;

class Ventana {

private:
	Glib::RefPtr<Gtk::Builder> builder;//builder que obtiene datos del archivo de la vista

	/* ventanas */
	Gtk::Window *main_window; //ventana principal de la aplicacion
	Gtk::AboutDialog *about_window; //ventana acerca de
	Gtk::FileChooserDialog *select_window; //ventana de seleccion de archivo

	Gtk::FileFilter filter; //filtro para la seleccion de archivos

	/* botones */
	Gtk::ToolButton *button_add;
	Gtk::ToolButton *button_erase;
	Gtk::ToolButton *button_stop;
	Gtk::ToolButton *button_pause;
	Gtk::ToolButton *button_continue;
	Gtk::ToolButton *button_up;
	Gtk::ToolButton *button_down;

	/* botones de la ventana de seleccion de archivo */
	Gtk::Button *button_accept;
	Gtk::Button *button_cancel;



	/* menu */
	//Gtk::MenuItem *menu_help;
	//TODO conectar señales del menu

	/* lista de torrents */
	TorrentView torrents;

	/* controlador de la vista */
	Controlador *controlador;

	Torrent tor;

  //Signal handlers:
	void on_button_add_clicked();
  	void on_button_erase_clicked();
	void on_button_stop_clicked();
	void on_button_pause_clicked();
	void on_button_continue_clicked();
	void on_button_up_clicked();
	void on_button_down_clicked();
	void on_button_accept_clicked();
	void on_button_cancel_clicked();

	void on_menu_about();

	//obtiene los botones desde el archivo
	void getButtons();

	//conecta las señales con los signal handlers
	void connectSignals();


public:
	Ventana();
	~Ventana();

	Gtk::Window& getVentana();

	void setControlador(Controlador *c);

	void actualizarEstado(Torrent*);

};

#endif /* VENTANA_H_ */
