/*
 * AttributesView.h
 *
 *  Created on: 31/05/2009
 *      Author: Lucia
 */

#ifndef ATTRIBUTESVIEW_H_
#define ATTRIBUTESVIEW_H_

#include <gtkmm.h>
#include <glib.h>
#include "Torrent.h"

class Torrent;

class AttributesView {
	/* administra la parte inferior de la ventana
	 * donde se muestra informacion sobre el torrent */

private:
	Gtk::TreeView *view_peers;
	Glib::RefPtr<Gtk::ListStore> list_peers;

	Gtk::TreeView *view_files;
	Glib::RefPtr<Gtk::ListStore> list_files;

	Gtk::TreeView *view_notif;
	Glib::RefPtr<Gtk::ListStore> list_notif;

	Gtk::TreeModelColumnRecord columns_peers;
	Gtk::TreeModelColumnRecord columns_files;
	Gtk::TreeModelColumnRecord columns_notif;

	Gtk::TreeModelColumn<std::string> col_name_peers;
	Gtk::TreeModelColumn<unsigned int> col_port_peers;
	Gtk::TreeModelColumn<Glib::ustring> col_type_peers;
	Gtk::TreeModelColumn<Glib::ustring> col_size_files;
	Gtk::TreeModelColumn<std::string> col_path_files;
	Gtk::TreeModelColumn<std::string> col_notif;

	Gtk::Notebook *page;
	guint page_selected; //numero de pagina seleccionada

	Gtk::Label *lurl;
	Gtk::Label *lhash;
	Gtk::Label *lparts;
	Gtk::Label *lpeers;
	Gtk::Label *lfiles;
	Gtk::Label *lpath;
	Gtk::Label *lname;

	Torrent *torrent;//torrent seleccionado

	void showPeers();
	void showInformation();
	void showFiles();

	void setPeersView();
	void setFilesView();
	void setInformationView(Glib::RefPtr<Gtk::Builder> builder);
	void setNotificationsView();

	void on_page_selected(GtkNotebookPage*, guint);

public:

	AttributesView();

	~AttributesView();

	/* toma lo que necesita del archivo de la vista */
	void setAttributesView(Glib::RefPtr<Gtk::Builder>);

	/* muestra informacion sobre el torrent */
	void showInfo(Torrent *t);

	/* agrega una notificacion en la vista */
	void addNotification(std::string);

	/* borra las notificaciones de la vista */
	void clearNotifications();

	void torrentDeleted(Torrent*);

};

#endif /* ATTRIBUTESVIEW_H_ */
