/*
 * TorrentView.h
 *
 *  Created on: 29/05/2009
 *      Author: teddy
 */

#ifndef TORRENTVIEW_H_
#define TORRENTVIEW_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>

#include "Torrent.h"

class TorrentView {
	/* Lista de Torrents perteneciente a la vista */

private:

	Gtk::TreeView *view_torrents; //vista de la lista

	/* columnas */ //todo: cambiar tipos
	Gtk::TreeModelColumnRecord columns;
	Gtk::TreeModelColumn<Glib::ustring> col_name; 	//nombre del torrent
	Gtk::TreeModelColumn<unsigned int> col_size;	//tam del archivo (en kb?? mb??)
	Gtk::TreeModelColumn<Glib::ustring> col_status; 	//estado del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_progress; 	//progreso del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_completed; 	//tamaño completado del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_downspeed; 	//velocidad de descarga del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_upspeed; 	//velocidad de subida del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_time; 	//tiempo restante para completar descarga
	Gtk::TreeModelColumn<Torrent*> col_torrent; 	//puntero al objeto torrent que se esta mostrando

	/* lista */
	Glib::RefPtr<Gtk::ListStore> list_torrents;

	/* fila seleccionada */
	Glib::RefPtr<Gtk::TreeSelection> selection;


	/* handler de la señal emitida al seleccionar una fila */
	//void on_row_selected();

public:

	TorrentView();

	~TorrentView();

	/* settea el tree view donde se va a mostrar la lista de torrents */
	void setTreeView(Gtk::TreeView *view_torrents);

	/* agregar una fila para este torrent */
	void addRow(Torrent*);
//	void removeRow();
//	void updateList();
//	void updateRow();

	/* devuelve el torrent de la columna seleccionada
	 * devuelve NULL en caso de no haber columnas seleccionadas */
	Torrent* getSelectedTorrent();

	/* selecciona la fila anterior. Se queda en el lugar si es la primer fila */
	void selectNext();
	/* selecciona la fila anterior. Se queda en el lugar si es la ultima fila */
	void selectPrevious();

	/* borra la fila seleccionada
	 * si existe, selecciona la siguiente fila */
	void eraseSelectedRow();

};

#endif /* TORRENTVIEW_H_ */
