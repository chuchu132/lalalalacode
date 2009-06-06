/*
 * AttributesView.cpp
 *
 *  Created on: 31/05/2009
 *      Author: teddy
 */

#include "AttributeView.h"

#define VIEW_PEERS "peers"
#define VIEW_FILES "archivos"
#define PESTANIAS "Pestanias"
//#define labels


AttributesView::AttributesView()
{
	// TODO Auto-generated constructor stub
	page_selected = 0;
	torrent = NULL;
}

AttributesView::~AttributesView()
{
	// TODO Auto-generated destructor stub
}

void AttributesView::setAttributesView(Glib::RefPtr<Gtk::Builder> builder)
{
	builder->get_widget(PESTANIAS,page);
	builder->get_widget(VIEW_PEERS,view_peers);
	builder->get_widget(VIEW_FILES,view_files);
	setPeersView();
	setStatusView();
	setFilesView();
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

void AttributesView::setStatusView()
{
/*Gtk::Label */
}

void AttributesView::setFilesView()
{
	columns_files.add(col_name_files);
	columns_files.add(col_size_files);
	columns_files.add(col_path_files);
	list_files = Gtk::ListStore::create(columns_files);
	view_files->set_model(list_files);

	this->view_files->append_column("Nombre del Archivo", col_name_files);
	this->view_files->append_column("Tamaño", col_size_files);
	int cols_count = this->view_files->append_column("Ruta del Archivo", col_path_files);

	Gtk::TreeViewColumn* pColumn;
	for (int i=0; i<cols_count; i++)
	{
		pColumn = this->view_files->get_column(i);
		pColumn->set_resizable(true);
	}

}
void AttributesView::showInfo(Torrent *t)
{
	torrent = t;
	switch (page_selected)
	{
	case 0:
		showStatus();
		break;
	case 1:
		showPeers();
		break;
	case 2:
		showFiles();
		break;
	}
}

void AttributesView::showPeers()
{
	//muestra la lista de peers de este torrent
	std::cout<<"mostrar peers"<<std::endl;
	//implementar hacer lo mismo que en show files
}

void AttributesView::showStatus()
{
	//muestra el estado del torrent
	std::cout<<"mostrar estado"<<std::endl;
}

void AttributesView::showFiles()
{
	//muestra los archivos del torrent
	std::cout<<"mostrar archivos"<<std::endl;
/* todo descomentar
	std::list<Archivo*>::iterator it = torrent->getFileManager()->getIteratorArchivos();
	Gtk::TreeModel::Row row;
	while (torrent->getFileManager()->getEndArchivos())
	{
		Gtk::TreeModel::Row row = (* list_torrents->append());
		row[col_size_file] = (*it)->getTamanio();//show bytes!!!
		row[col_name_file] = (*it)->getNombre();
		row[col_path_file] = (*it)->getPath();
		it++;
	}
*/
}

void AttributesView::on_page_selected(GtkNotebookPage* page, guint page_num)
{
	page_selected = page_num;
	if (this->torrent != NULL)
		this->showInfo(this->torrent);
}
