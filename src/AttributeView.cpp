/*
 * AttributesView.cpp
 *
 *  Created on: 31/05/2009
 *      Author: teddy
 */

#include "AttributeView.h"

#define VIEW_ATTRIBUTES "peers"
#define COL_NAME_PEER "Nombre"

AttributesView::AttributesView()
{
	// TODO Auto-generated constructor stub
	num_selected = 1;

}

AttributesView::~AttributesView()
{
	// TODO Auto-generated destructor stub
}

void AttributesView::setAttributesView(Glib::RefPtr<Gtk::Builder> builder)
{
	builder->get_widget(VIEW_ATTRIBUTES,view_peers);
	setPeersView();
	setStatusView();
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
/*Gtk::treeview */
}
void AttributesView::showInfo(Torrent *t)
{
	switch (num_selected)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void AttributesView::showPeers(Torrent *t)
{
	//muestra la lista de peers de este torrent
}

void AttributesView::showStatus(Torrent *t)
{
	//muestra el estado del torrent
}

void AttributesView::showFiles(Torrent *t)
{
	//muestra los archivos del torrent
}
