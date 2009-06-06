/*
 * AttributesView.h
 *
 *  Created on: 31/05/2009
 *      Author: teddy
 */

#ifndef ATTRIBUTESVIEW_H_
#define ATTRIBUTESVIEW_H_

#include "Torrent.h"

class Torrent;

class AttributesView {
	/* administra la parte inferior de la ventana
	 * donde se muestra el estado del torrent y los peers */

private:
	Gtk::TreeView *view_peers;
	Glib::RefPtr<Gtk::ListStore> list_peers;

	Gtk::TreeView *view_files;
	Glib::RefPtr<Gtk::ListStore> list_files;

	Gtk::TreeModelColumnRecord columns_peers;
	Gtk::TreeModelColumnRecord columns_files;

	Gtk::TreeModelColumn<Glib::ustring> col_name_peers;
	Gtk::TreeModelColumn<Glib::ustring> col_name_files;
	Gtk::TreeModelColumn<Glib::ustring> col_size_files;
	Gtk::TreeModelColumn<Glib::ustring> col_path_files;
	//Gtk::TreeModelColumn<Glib::ustring> col_progress_peers;
	//TODO ver que columnas tiene que tener esta seccion


	Gtk::Notebook *page;
	guint page_selected; //numero de pagina seleccionada

	Gtk::Label lurl;
	Gtk::Label lhash;
	Gtk::Label lparts;

	Torrent *torrent;//torrent seleccionado

	void showPeers();
	void showStatus();
	void showFiles();

	void setPeersView();
	void setStatusView();
	void setFilesView();

	void on_page_selected(GtkNotebookPage* , guint);

public:

	AttributesView();

	~AttributesView();

	/* toma lo que necesita del archivo de la vista */
	void setAttributesView(Glib::RefPtr<Gtk::Builder>);

	/* muestra informacion sobre el torrent */
	void showInfo(Torrent *t);

};

#endif /* ATTRIBUTESVIEW_H_ */
