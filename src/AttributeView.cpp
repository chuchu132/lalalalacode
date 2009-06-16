/*
 * AttributesView.cpp
 *
 *  Created on: 31/05/2009
 *      Author: teddy
 */

#include "AttributeView.h"

#define VIEW_PEERS "peers"
#define VIEW_FILES "archivos"
#define VIEW_NOTIF "notificaciones"

#define PESTANIAS "Pestanias"
#define L_NAME "l_name"
#define L_URL "l_url"
#define L_PEERS "l_peers"
#define L_PATH "l_ruta"
#define L_HASH "l_hash"
#define L_PARTS "l_parts"
#define L_FILES "l_num_files"


AttributesView::AttributesView()
{
	page_selected = 0;
	torrent = NULL;
}

AttributesView::~AttributesView()
{
	list_files->clear();
	list_notif->clear();
	list_peers->clear();
}

void AttributesView::setAttributesView(Glib::RefPtr<Gtk::Builder> builder)
{
	builder->get_widget(PESTANIAS,page);
	builder->get_widget(VIEW_PEERS,view_peers);
	builder->get_widget(VIEW_FILES,view_files);
	builder->get_widget(VIEW_NOTIF,view_notif);
	setPeersView();
	setInformationView(builder);
	setFilesView();
	setNotificationsView();
	// me conecto a la señal de pagina cambiada para refrescar la vista
	page->signal_switch_page().connect(sigc::mem_fun (*this,&AttributesView::on_page_selected));
}

void AttributesView::setPeersView()
{
	columns_peers.add(col_name_peers);
	list_peers =  Gtk::ListStore::create(columns_peers);
	view_peers->set_model(list_peers);

	int cols_count = this->view_peers->append_column("Nombre del Peer", col_name_peers);
	Gtk::TreeViewColumn* pColumn;
	for (int i=0; i<cols_count; i++)
	{
		pColumn = this->view_peers->get_column(i);
		pColumn->set_resizable(true);
	}
}

void AttributesView::setInformationView(Glib::RefPtr<Gtk::Builder> builder)
{
	builder->get_widget(L_FILES,lfiles);
	builder->get_widget(L_HASH,lhash);
	builder->get_widget(L_NAME,lname);
	builder->get_widget(L_PARTS,lparts);
	builder->get_widget(L_PATH,lpath);
	builder->get_widget(L_PEERS,lpeers);
	builder->get_widget(L_URL,lurl);
}

void AttributesView::setFilesView()
{
	columns_files.add(col_size_files);
	columns_files.add(col_path_files);
	list_files = Gtk::ListStore::create(columns_files);
	view_files->set_model(list_files);

	this->view_files->append_column("Ruta del Archivo", col_path_files);
	int cols_count = this->view_files->append_column("Tamaño", col_size_files);

	Gtk::TreeViewColumn* pColumn;
	for (int i=0; i<cols_count; i++)
	{
		pColumn = this->view_files->get_column(i);
		pColumn->set_resizable(true);
	}
}

void AttributesView::setNotificationsView()
{
	columns_notif.add(col_notif);
	list_notif = Gtk::ListStore::create(columns_notif);
	view_notif->set_model(list_notif);

	int i = this->view_notif->append_column("Notificaciones", col_notif);
	Gtk::TreeViewColumn* pColumn = this->view_files->get_column(i);
	pColumn->set_resizable(true);

}

void AttributesView::showInfo(Torrent *t)
{
	torrent = t;
	switch (page_selected)
	{
	case 1:
		showInformation();
		break;
	case 2:
		showPeers();
		break;
	case 3:
		showFiles();
		break;
	}
}

void AttributesView::showPeers()
{
	//muestra la lista de peers de este torrent
	//implementar hacer lo mismo que en show files
}

void AttributesView::showInformation()
{
	std::stringstream texto1;
	texto1 << (int)(torrent->getTamanio() / torrent->getFileManager()->getTamanioPieza());
	lparts->set_text(texto1.str());

	std::stringstream texto2;
	texto2 << torrent->getFileManager()->getCantArchivos();
	lfiles->set_text(texto2.str());

	std::stringstream texto3;
	texto3 << torrent->getCantPeers();
	lpeers->set_text(texto3.str());

	lpath->set_text(torrent->getPath());
	lname->set_text(torrent->getNombre());
	lurl->set_text(torrent->getUrlTracker());
	lhash->set_text(torrent->getHashString());
}

void AttributesView::showFiles()
{
	//muestra los archivos del torrent
	list_files->clear();

	std::list<Archivo*>::iterator it = torrent->getFileManager()->getIteratorArchivos();
	std::list<Archivo*>::iterator end = torrent->getFileManager()->getEndArchivos();
	Gtk::TreeModel::Row row;
	while (it != end)
	{
		Gtk::TreeModel::Row row = (* list_files->append());
		//row[col_size_file] = (*it)->getTamanio();//show bytes!!!
		row[col_path_files] = (*it)->getPath();
		it++;
	}
}

void AttributesView::on_page_selected(GtkNotebookPage* page, guint page_num)
{
	page_selected = page_num;
	if (this->torrent != NULL)
		this->showInfo(this->torrent);
}

void AttributesView::addNotification(std::string notif)
{
	Gtk::TreeModel::Row row = (* list_notif->append());
	row[col_notif] = notif;
	Gtk::TreePath::TreePath path(row);
	view_notif->scroll_to_row(path);
}

void AttributesView::clearNotifications()
{
	list_notif->clear();
}

void AttributesView::torrentDeleted(Torrent *t)
{
	if (torrent == t)
	{
		torrent = NULL;
		list_files->clear();
		list_peers->clear();
	}
}
