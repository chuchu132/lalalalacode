/*
 * TorrentView.cpp
 *
 *  Created on: 29/05/2009
 *      Author: Lucia
 */

#include "TorrentView.h"

#define COL_NAME "Nombre del Torrent"
#define COL_SIZE "Tamaño"
#define COL_STATUS "Estado"
#define COL_PROGRESS "Progreso"
#define COL_COMPLETED "Completado"
#define COL_REMAINING "Restante"
#define COL_DOWNSPEED "Vel descarga (kb/s)"
#define COL_UPSPEED "Vel subida (kb/s)"
#define COL_TIME "Tiempo Restante"
#define COL_CAT "Mostrar"

#define SHOW_ALL "Todos"

TorrentView::TorrentView() {

	controlador = NULL;

	columns.add(col_name);
	columns.add(col_size);
	columns.add(col_status);
	columns.add(col_progress);
	columns.add(col_completed);
	columns.add(col_remaining);
	columns.add(col_downspeed);
	columns.add(col_upspeed);
	columns.add(col_time);
	columns.add(col_torrent);

	columns_categories.add(col_categories);
	columns_categories.add(col_cat_status);

	//creo la lista con las columnas
	list_torrents = Gtk::ListStore::create(columns);
	list_categories = Gtk::ListStore::create(columns_categories);

}

TorrentView::~TorrentView() {
	list_torrents->clear();
	list_categories->clear();
}

void TorrentView::setTreeView(Gtk::TreeView *view_torrents) {
	this->view_torrents = view_torrents;

	//agrego la lista de torrents al tree view
	this->view_torrents->set_model(list_torrents);

	Gtk::TreeViewColumn* pColumn;
	int cols_count;

	//agrego columnas al Tree View
	this->view_torrents->append_column(COL_NAME, col_name);
	this->view_torrents->append_column(COL_SIZE, col_size);
	this->view_torrents->append_column(COL_STATUS, col_status);

	//mostrar una progress bar para el porcentaje de progreso
	Gtk::CellRendererProgress* cell =
		Gtk::manage(new Gtk::CellRendererProgress);
	cols_count = this->view_torrents->append_column(COL_PROGRESS, *cell);
	pColumn = this->view_torrents->get_column(cols_count - 1);
	if (pColumn) {
#ifdef GLIBMM_PROPERTIES_ENABLED
		pColumn->add_attribute(cell->property_value(), col_progress);
#else
		pColumn->add_attribute(*cell, "value", col_progress);
#endif
	}

	this->view_torrents->append_column(COL_COMPLETED, col_completed);
	this->view_torrents->append_column(COL_REMAINING, col_remaining);
	this->view_torrents->append_column(COL_DOWNSPEED, col_downspeed);
	this->view_torrents->append_column(COL_UPSPEED, col_upspeed);
	cols_count = this->view_torrents->append_column(COL_TIME, col_time);

	for (int i = 0; i < cols_count; i++) {
		pColumn = this->view_torrents->get_column(i);
		pColumn->set_resizable(true); //hago que sean columnas redimensionables
	}
	this->view_torrents->columns_autosize();

	selection = this->view_torrents->get_selection();
	selection->signal_changed().connect(sigc::mem_fun(*this,
			&TorrentView::on_row_selected));

}

void TorrentView::setCategoriesView(Gtk::TreeView *view_categories) {
	this->view_categories = view_categories;
	this->view_categories->set_model(list_categories);
	this->view_categories->append_column(COL_CAT, col_categories);

	//agrego filas a la lista
	Gtk::TreeModel::Row row = (*list_categories->append());
	row[col_categories] = "Todos";
	row[col_cat_status] = SHOW_ALL;

	row = (*list_categories->append());
	row[col_categories] = "Activos";
	row[col_cat_status] = T_ACTIVO;

	row = (*list_categories->append());
	row[col_categories] = "Completos";
	row[col_cat_status] = T_COMPLETO;

	row = (*list_categories->append());
	row[col_categories] = "Detenidos";
	row[col_cat_status] = T_DETENIDO;

	selection_categories = this->view_categories->get_selection();
	//me conecto a la señal de fila seleccionada
	selection_categories->signal_changed().connect(sigc::mem_fun(*this,
			&TorrentView::on_category_selected));

}

void TorrentView::setAttributesView(AttributesView *attr) {
	this->attr = attr;
}

void TorrentView::on_category_selected() {
	//veo que fila esta seleccionada
	Gtk::TreeModel::iterator iter = selection_categories->get_selected();
	if (iter) //si hay algo seleccionado
	{
		Gtk::TreeModel::Row row = *iter;
		//veo que fila es y muestro solo los torrents en ese estado
		std::string type = row[col_cat_status];

		showAll();
		if (type != SHOW_ALL)
			hideRows(type);
	}
}

void TorrentView::on_row_selected() {
	Torrent *t = getSelectedTorrent();
	if (t != NULL)
		attr->showInfo(t);
}

void TorrentView::updateRowValues(Gtk::TreeModel::Row &row, Torrent *t) {
	row[col_status] = t->getEstado();
	row[col_remaining] = t->bytesToString(t->left());
	row[col_downspeed] = t->getVelocidadBajada();
	row[col_upspeed] = t->getVelocidadSubida();
	row[col_time] = t->getTiempoRestante();
	row[col_progress] = (int) (((t->getTamanioDescargado() / 1024) * 100)
			/ (t->getTamanio() / 1024));
	row[col_completed] = t->bytesToString(t->getTamanioDescargado());
}

void TorrentView::addRow(Torrent *t) {
	Gtk::TreeModel::Row row = (*list_torrents->append());

	row[col_name] = t->getNombre();
	row[col_size] = t->bytesToString(t->getTamanio());
	row[col_torrent] = t;
	row[col_name] = t->getNombre();

	updateRowValues(row, t);
}

Torrent* TorrentView::getSelectedTorrent() {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (iter) //si hay algo seleccionado
	{
		Gtk::TreeModel::Row row = *iter;
		return row.get_value(col_torrent);
	} else {
		return NULL;
	}
}

void TorrentView::eraseSelectedRow() {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (iter) //si hay algo seleccionado
	{
		iter = list_torrents->erase(iter); //borro y obtengo un iter a la sgte fila
		if (iter) //si existe la fila
			selection->select(iter); //selecciono la fila
	}

}

void TorrentView::selectNext() {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (iter) //si hay algo seleccionado
	{
		iter++;
		if (iter) //si existe la fila
		{
			selection->select(iter); //selecciono la fila
			Gtk::TreePath::TreePath path(iter);
			view_torrents->scroll_to_row(path); //muevo el scroll de la vista
		}
	}
}

void TorrentView::selectPrevious() {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (iter) //si hay algo seleccionado
	{
		if (iter != list_torrents->children().begin()) {
			iter--; //retrocedo
			selection->select(iter); //selecciono la fila
			Gtk::TreePath::TreePath path(iter);
			view_torrents->scroll_to_row(path); //muevo el scroll de la vista
		}
	}
}

void TorrentView::empty() {
	list_torrents->clear();
}

void TorrentView::updateRow(Torrent *t) {
	Gtk::TreeModel::Children::iterator iter = list_torrents->children().begin();
	Gtk::TreeModel::Row row;
	while (iter != list_torrents->children().end()) {
		row = *iter;
		if (row[col_torrent] == t) {
			updateRowValues(row, t);
			break;
		}
		iter++;
	}
}

void TorrentView::hideRows(std::string type) {
	std::cout << "mostrar " << type << "s" << std::endl;
	//todo concurrencia
//	Gtk::TreeModel::Children::iterator iter = list_torrents->children().begin();
//	Gtk::TreeModel::Row row;
//	Torrent *t;
//	while (iter != list_torrents->children().end()) {
//		row = *iter;
//		t = row[col_torrent];
//		if (type != t->getEstado()) {
//			attr->torrentDeleted(t);
//			list_torrents->remove(iter);
//		}
//		iter++;
//	}
}

void TorrentView::showAll() {
	std::cout << "mostrar todos" << std::endl;
//	this->empty();//todo.. ver concurrencia!!!!
//	controlador->agregarTorrentsEnVista();
}

void TorrentView::setControlador(Controlador *c) {
	controlador = c;
}

