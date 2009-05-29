/*
 * TorrentView.cpp
 *
 *  Created on: 29/05/2009
 *      Author: teddy
 */

#include "TorrentView.h"

#define COL_NAME "Nombre del Torrent"
#define COL_SIZE "Tamaño"
#define COL_STATUS "Estado"
#define COL_PROGRESS "Progreso"
#define COL_COMPLETED "Completado"
#define COL_DOWNSPEED "Vel descarga (kb/s)"
#define COL_UPSPEED "Vel subida (kb/s)"
#define COL_TIME "Tiempo Restante"


TorrentView::TorrentView()
{
	//agrego las columnas
	columns.add(col_name);
	columns.add(col_size);
	columns.add(col_status);
	columns.add(col_progress);
	columns.add(col_completed);
	columns.add(col_downspeed);
	columns.add(col_upspeed);
	columns.add(col_time);
	columns.add(col_torrent);

	//creo la lista con las columnas
	list_torrents = Gtk::ListStore::create(columns);
}

TorrentView::~TorrentView()
{
	// TODO Auto-generated destructor stub
}

void TorrentView::setTreeView(Gtk::TreeView *view_torrents)
{
	this->view_torrents = view_torrents;

	//agrego la lista de torrents al tree view
	this->view_torrents->set_model(list_torrents);

	//agrego columnas al Tree View
	this->view_torrents->append_column(COL_NAME, col_name);
	this->view_torrents->append_column(COL_SIZE, col_size);
	this->view_torrents->append_column(COL_STATUS, col_status);
	this->view_torrents->append_column(COL_PROGRESS, col_progress);
	this->view_torrents->append_column(COL_COMPLETED, col_completed);
	this->view_torrents->append_column(COL_DOWNSPEED, col_downspeed);
	this->view_torrents->append_column(COL_UPSPEED, col_upspeed);
	this->view_torrents->append_column(COL_TIME, col_time);

	this->view_torrents->columns_autosize();
//todo.. set columns resizable!


	selection = this->view_torrents->get_selection();
	//me conecto a la señal de fila seleccionada
	//selection->signal_changed().connect( sigc::mem_fun(*this, &TorrentView::on_row_selected) );
}

void TorrentView::addRow()
{
	//list_torrents.append();
}

Torrent* TorrentView::getSelectedTorrent()
{
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if(iter) //si hay algo seleccionado
	{
	  Gtk::TreeModel::Row row = *iter;
	  return row.get_value(col_torrent);
	}
	else
	{
		return NULL;
	}
}






/*
void TorrentView::on_row_selected()
{
//capaz no es necesaria esta funcion.
	//veo que fila esta seleccionada
	TreeModel::iterator iter = selection->get_selected();
	if(iter) //If anything is selected
	{
	  TreeModel::Row row = *iter;
	  //Do something with the row.
	}
}*/


