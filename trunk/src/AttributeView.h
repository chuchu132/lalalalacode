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
	Gtk::ListStore *list_peers;

	Gtk::TreeModelColumnRecord columns;
	Gtk::TreeModelColumn<Glib::ustring> col_name_peers;
	//Gtk::TreeModelColumn<Glib::ustring> col_progress_peers;
	//TODO ver que columnas tiene que tener esta seccion


	GtkNotebookPage *selected_page;
	guint num_selected; //conectar señal.. que llama a showinfo

	Gtk::Label lurl;
	Gtk::Label lhash;
	Gtk::Label lparts;

	void showPeers(Torrent *t);
	void showStatus(Torrent *t);
	void showFiles(Torrent *t);


	void setPeersView();
	void setStatusView();
	void setFilesView();

public:

	AttributesView();

	~AttributesView();

	/* toma lo que necesita del archivo */
	void setAttributesView(Glib::RefPtr<Gtk::Builder>);

	/* muestra informacion sobre el torrent */
	void showInfo(Torrent *t);


};

#endif /* ATTRIBUTESVIEW_H_ */
