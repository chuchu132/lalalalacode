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
#include "Constantes.h"
#include "AttributeView.h"

class Torrent;
class AttributesView;

class TorrentView {
	/* Lista de Torrents perteneciente a la vista */

private:

	Gtk::TreeView *view_torrents; //vista de la lista
	Gtk::TreeView *view_categories; //vista del selector

	/* columnas */
	Gtk::TreeModelColumnRecord columns;
	Gtk::TreeModelColumnRecord columns_categories;

	Gtk::TreeModelColumn<Glib::ustring> col_name; 	//nombre del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_size;	//tam del archivo
	Gtk::TreeModelColumn<std::string> col_status; 	//estado del torrent
	Gtk::TreeModelColumn<int> col_progress; 	//progreso del torrent (porcentaje)
	Gtk::TreeModelColumn<Glib::ustring> col_completed; 	//tamaño completado del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_remaining; 	//tamaño restante para completar descarga
	Gtk::TreeModelColumn<int> col_downspeed; 	//velocidad de descarga del torrent
	Gtk::TreeModelColumn<int> col_upspeed; 	//velocidad de subida del torrent
	Gtk::TreeModelColumn<Glib::ustring> col_time; 	//tiempo restante para completar descarga
	Gtk::TreeModelColumn<Torrent*> col_torrent; 	//puntero al objeto torrent que se esta mostrando

	Gtk::TreeModelColumn<Glib::ustring> col_categories;
	Gtk::TreeModelColumn<std::string> col_cat_status;

	/* lista */
	Glib::RefPtr<Gtk::ListStore> list_torrents;
	Glib::RefPtr<Gtk::ListStore> list_categories;

	/* fila seleccionada */
	Glib::RefPtr<Gtk::TreeSelection> selection;
	Glib::RefPtr<Gtk::TreeSelection> selection_categories;

	AttributesView *attr;//muestra los atributos del torrent

	/* Signal handler para fila del categories view seleccionada */
	void on_category_selected();

	/* Signal handler para fila del tree view seleccionada */
	void on_row_selected();

	/* actualiza la fila con los datos del torrent */
	void updateRowValues(Gtk::TreeModel::Row &, Torrent*);

	/* transforma un tamaño enn bytes en un string */
	std::string showBytes(float bytes);

	/* muestra todos los torrents */
	void showAll();

	/* oculta los que no tienen este estado */
	void hideRows(std::string);

public:

	TorrentView();

	~TorrentView();

	/* settea el tree view donde se va a mostrar la lista de torrents */
	void setTreeView(Gtk::TreeView *view_torrents);

	/* vista que muestra a los torrents segun su estado */
	void setCategoriesView(Gtk::TreeView *view_categories);

	void setAttributesView(AttributesView *attr);

	/* agregar una fila para este torrent */
	void addRow(Torrent *t);

	/* actualiza los datos del torrent en la vista */
	void updateRow(Torrent*);

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

	/* vacia la lista de torrents */
	void empty();

};

#endif /* TORRENTVIEW_H_ */
