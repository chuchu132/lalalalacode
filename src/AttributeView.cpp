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

#define COL_NAME_PEER "Nombre"

AttributesView::AttributesView()
{
	// TODO Auto-generated constructor stub
	num_selected = 0;
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

//	Gtk::TreeViewColumn* pColumn;
//	int cols_count;

	//agrego columnas al Tree View
	//this->view_torrents->append_column(COL_NAME_PEER, col_name_peers);

}

void AttributesView::setStatusView()
{
/*Gtk::Label */
}

void AttributesView::setFilesView()
{
	columns_files.add(col_name_files);
	columns_files.add(col_size_files);
	list_files = Gtk::ListStore::create(columns_files);

}
void AttributesView::showInfo(Torrent *t)
{
	torrent = t;
	switch (num_selected)
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
}

void AttributesView::on_page_selected(GtkNotebookPage* page, guint page_num)
{
	num_selected = page_num;
	if (this->torrent != NULL)
		this->showInfo(this->torrent);
}
