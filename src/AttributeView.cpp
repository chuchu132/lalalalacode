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

void AttributesView::showInfo(Torrent *t)
{
	this->showPeers(t);
	this->showStatus(t);
}

void AttributesView::showPeers(Torrent *t)
{
	//muestra la lista de peers de este torrent
}

void AttributesView::showStatus(Torrent *t)
{
	//muestra el estado del torrent
}

