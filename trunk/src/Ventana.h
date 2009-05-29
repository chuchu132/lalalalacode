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

#include "TorrentListStore.h"

class Ventana {

private:
	Glib::RefPtr<Gtk::Builder> builder;//builder que obtiene datos del archivo de la vista

	/* ventanas */
	Gtk::Window *main_window; //ventana principal de la aplicacion
	Gtk::AboutDialog *about_window; //ventana acerca de


	/* botones */
	Gtk::ToolButton *button_add;
	Gtk::ToolButton *button_erase;
	Gtk::ToolButton *button_stop;
	Gtk::ToolButton *button_pause;
	Gtk::ToolButton *button_continue;
	Gtk::ToolButton *button_up;
	Gtk::ToolButton *button_down;

	/* menu */
	//Gtk::MenuItem *menu_help;
	//TODO conectar señales del menu

	/* lista de torrents */
	TorrentListStore torrents;
	Gtk::TreeView *view_torrents;


protected:
  //Signal handlers:
	void on_button_add_clicked();
  	void on_button_erase_clicked();
	void on_button_stop_clicked();
	void on_button_pause_clicked();
	void on_button_continue_clicked();
	void on_button_up_clicked();
	void on_button_down_clicked();

	void on_menu_about();



public:
	Ventana();
	~Ventana();

	Gtk::Window& getVentana();

private:
	//obtiene los botones desde el archivo
	void getButtons();

	//conecta las señales con los signal handlers
	void connectSignals();

};

#endif /* VENTANA_H_ */
