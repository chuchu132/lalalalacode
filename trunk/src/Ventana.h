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
#include "Torrent.h"
#include "Controlador.h"
#include "AttributeView.h"


class TorrentView;
class Controlador;
class AttributesView;
class Torrent;

class Ventana {

private:
	Glib::RefPtr<Gtk::Builder> builder;//obtiene datos del archivo de la vista

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
	Gtk::ToolButton *button_peers;
	Gtk::ToolButton *button_notif;


	/* lista de torrents */
	TorrentView *torrents;
	AttributesView *attr;

	/* controlador de la vista */
	Controlador *controlador;

	bool error; //indica si hubo un error

	//Signal handlers:
	void on_button_add_clicked();
  	void on_button_erase_clicked();
	void on_button_stop_clicked();
	void on_button_pause_clicked();
	void on_button_continue_clicked();
	void on_button_up_clicked();
	void on_button_down_clicked();
	void on_button_peers_clicked();
	void on_button_notifications_clicked();

	void on_menu_about();

	void button_accept_clicked();
	void button_cancel_clicked();


	//obtiene los botones desde el archivo
	void getButtons();

	//obtiene las ventanas desde el archivo
	void getWindows();

	//obtiene los tree view desde el archivo
	void getViews();

	//conecta las señales con los signal handlers
	void connectSignals();


public:

	Torrent *tor;//temporal, solo para probar la vista

	Ventana();

	~Ventana();

	/* setea el controlador */
	void setControlador(Controlador *c);

	/* actualiza el estado del torrent */
	void actualizarEstado(Torrent*);

	/* agrega el torrent a la vista */
	void addTorrent(Torrent*);

	void mostrarNotificacion(std::string);

	/* abre la ventana principal */
	void run();

};

#endif /* VENTANA_H_ */
