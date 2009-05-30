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
#define COL_CAT "Mostrar"


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

	columns_categories.add(col_categories);

	//creo la lista con las columnas
	list_torrents = Gtk::ListStore::create(columns);
	list_categories = Gtk::ListStore::create(columns_categories);

	i = 0;
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

	//mostrar una progress bar para el porcentaje de progreso
	Gtk::CellRendererProgress* cell = Gtk::manage(new Gtk::CellRendererProgress);
	int cols_count = this->view_torrents->append_column(COL_PROGRESS, *cell);
	Gtk::TreeViewColumn* pColumn = this->view_torrents->get_column(cols_count - 1);
 	if(pColumn)
 	{
#ifdef GLIBMM_PROPERTIES_ENABLED
 		pColumn->add_attribute(cell->property_value(), col_progress);
#else
 		pColumn->add_attribute(*cell, "value", col_progress);
#endif
 	}

	this->view_torrents->append_column(COL_COMPLETED, col_completed);
	this->view_torrents->append_column(COL_DOWNSPEED, col_downspeed);
	this->view_torrents->append_column(COL_UPSPEED, col_upspeed);
	this->view_torrents->append_column(COL_TIME, col_time);

	this->view_torrents->columns_autosize();
//todo.. set columns resizable!

	selection = this->view_torrents->get_selection();
	selection->signal_changed().connect( sigc::mem_fun(*this, &TorrentView::on_row_selected) );

}

void TorrentView::setCategoriesView(Gtk::TreeView *view_categories)
{
	this->view_categories = view_categories;
	this->view_categories->set_model(list_categories);
	this->view_categories->append_column(COL_CAT, col_categories);

	//agrego filas a la lista
	Gtk::TreeModel::Row row = (* list_categories->append());
	row[col_categories] = "Todos";
	row = (* list_categories->append());
	row[col_categories] = "Completos";
	row = (* list_categories->append());
	row[col_categories] = "Activos";

	selection_categories = this->view_categories->get_selection();
	//me conecto a la señal de fila seleccionada
	selection_categories->signal_changed().connect( sigc::mem_fun(*this, &TorrentView::on_category_selected) );

}

void TorrentView::on_category_selected()
{
	//veo que fila esta seleccionada
	Gtk::TreeModel::iterator iter = selection_categories->get_selected();
	if (iter) //si hay algo seleccionado
	{
	  Gtk::TreeModel::Row row = *iter;
	  //todo hago algo con la fila...
	}
}

void TorrentView::on_row_selected()
{
	std::cout<<"fila seleccionada"<<std::endl;
	//agrego todos los datos que me interesen
	//en la parte de estado del torrent
}

void TorrentView::setRowValues(Gtk::TreeModel::Row &row, Torrent *t)
{
	//todo crear los metodos necesarios y ver tipos
	/*row[col_name] = t->getNombre();
	row[col_size] = t->getTamanio();
	row[col_status] = t->getEstado();
	row[col_progress] = t->getPorcentaje();
	row[col_completed] = t->getTamanioDescargado();
	row[col_downspeed] = t->getVelocidadBajada();
	row[col_upspeed] = t->getVelocidadSubida();
	row[col_time] = t->getTiempoRestante();*/
	row[col_torrent] = t;
}

void TorrentView::addRow(Torrent* t)
{
	Gtk::TreeModel::Row row = (* list_torrents->append());
	row[col_name] = "Lucia.torrent";
	row[col_size] = 35000;
	row[col_status] = "Pausado";
	row[col_progress] = i;
	row[col_completed] = (35000*i/100);
	row[col_downspeed] = "25 kb/s";
	row[col_upspeed] = "-";
	row[col_time] = "1h 25m";
	row[col_torrent] = t;
	i++;
	//setRowValues(row,t);
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

void TorrentView::eraseSelectedRow()
{
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if(iter) //si hay algo seleccionado
	{
	  iter = list_torrents->erase(iter); //borro y obtengo un iter a la sgte fila
	  if(iter)						//si existe la fila
		  selection->select(iter);	//selecciono la fila
	}
}

void TorrentView::selectNext()
{
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if(iter) //si hay algo seleccionado
	{
	  iter++;
	  if(iter)						//si existe la fila
		  selection->select(iter);	//selecciono la fila
	}
}

void TorrentView::selectPrevious()
{
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if(iter) //si hay algo seleccionado
	{
		if (iter != list_torrents->children().begin())
		{
			iter--;	//retrocedo
			selection->select(iter);	//selecciono la fila
		}
	}
}

void TorrentView::empty()
{
	list_torrents->clear();
}

void TorrentView::updateRow(Torrent *t)
{
	//busco la fila
	Gtk::TreeModel::Children::iterator iter = list_torrents->children().begin();
	Gtk::TreeModel::Row row;
	while (iter != list_torrents->children().end())
	{
		row = *iter;
		if (row[col_torrent] == t)
		{
			this->setRowValues(row,t);
			break;
		}
		iter++;
	}
}


