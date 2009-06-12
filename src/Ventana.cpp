/*
 * Ventana.cpp
 *
 *  Created on: 28/05/2009
 *      Author: teddy
 */

//TODO.. DESCOMENTAR !
#include "Ventana.h"

#define WINDOW_FILE "res/wind.glade"

#define MAIN_WINDOW "main_wind"
#define ABOUT_WINDOW "about_wind"
#define SELECT_WINDOW "select_wind"

#define BUTTON_ADD 	"boton_agregar"
#define BUTTON_ERASE "boton_borrar"
#define BUTTON_STOP	"boton_detener"
#define BUTTON_PAUSE	"boton_pausar"
#define BUTTON_CONTINUE "boton_continuar"
#define BUTTON_UP 	"boton_subir"
#define BUTTON_DOWN	"boton_bajar"
#define BUTTON_PEERS "boton_peers"
#define BUTTON_NOTIF "boton_notificaciones"

#define VIEW_TORRENTS "torrents"
#define VIEW_CATEGORIES "clasificacion"

Ventana::Ventana()
{
	error = false;
	std::cout<<"comienzo ventana"<<std::endl;

	try
	{
		builder = Gtk::Builder::create_from_file(WINDOW_FILE);
		std::cout<<"archivo cargado"<<std::endl;

		attr = new AttributesView();
		torrents = new TorrentView();
		torrents->setAttributesView(attr);

		this->getWindows();
		this->getViews();

		this->getButtons();
		this->connectSignals();

	}catch(Glib::FileError& ex1)
	{
		std::cout<<"error al cargar el archivo de la vista"<<std::endl;
		std::cout<<"falta el archivo "<< WINDOW_FILE<<std::endl;
		error = true;
	}
	catch (Glib::MarkupError& ex2)
	{
		std::cout<<"error al cargar el archivo de la vista"<<std::endl;
		std::cout<<"falta el archivo "<< WINDOW_FILE<<std::endl;
		error = true;
	}
	catch (Gtk::BuilderError& ex3)
	{
		std::cout<<"error al cargar el archivo de la vista"<<std::endl;
		std::cout<<"falta el archivo "<< WINDOW_FILE<<std::endl;
		error = true;
	}
}

Ventana::~Ventana()
{
	if (!error)
	{
		delete torrents;
		delete attr;
		//controlador->cerrarCliente();
	}

	std::cout<<"fin"<<std::endl;
}

void Ventana::setControlador(Controlador *c)
{
	this->controlador = c;
}

void Ventana::getWindows()
{
	//obtengo la ventana principal
	main_window = 0;
	builder->get_widget(MAIN_WINDOW, main_window);
	std::cout<<"ventana ppal cargada"<<std::endl;

	//obtengo la ventana acerca de
	about_window = 0;
	builder->get_widget(ABOUT_WINDOW, about_window);
	std::cout<<"ventana acerca de cargada"<<std::endl;

	//obtengo la ventana de seleccion de archivo
	select_window = 0;
	builder->get_widget(SELECT_WINDOW, select_window);

	filter.set_name("Archivos Torrent (*.torrent)");
	filter.add_pattern("*.torrent");
	select_window->add_filter(filter);

	select_window->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	select_window->add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	std::cout<<"ventana de seleccion de archivo cargada"<<std::endl;
}

void Ventana::getViews()
{
	//obtengo el tree view de torrents
	Gtk::TreeView *view_torrents = 0;
	builder->get_widget(VIEW_TORRENTS,view_torrents);
	torrents->setTreeView(view_torrents);

	Gtk::TreeView *view_cat = 0;
	builder->get_widget(VIEW_CATEGORIES,view_cat);
	torrents->setCategoriesView(view_cat);

	attr->setAttributesView(builder);
	std::cout<<"view torrents cargado"<<std::endl;

}

void Ventana::getButtons()
{
	builder->get_widget(BUTTON_ADD, button_add);
	builder->get_widget(BUTTON_ERASE, button_erase);
	builder->get_widget(BUTTON_STOP, button_stop);
	builder->get_widget(BUTTON_PAUSE, button_pause);
	builder->get_widget(BUTTON_CONTINUE, button_continue);
	builder->get_widget(BUTTON_UP, button_up);
	builder->get_widget(BUTTON_DOWN, button_down);
	builder->get_widget(BUTTON_PEERS, button_peers);
	builder->get_widget(BUTTON_NOTIF, button_notif);
}

void Ventana::connectSignals()
{
	button_add->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_add_clicked) );
	button_erase->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_erase_clicked) );
	button_stop->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_stop_clicked) );
	button_pause->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_pause_clicked) );
	button_continue->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_continue_clicked) );
	button_up->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_up_clicked) );
	button_down->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_down_clicked) );
	button_peers->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_peers_clicked) );
	button_notif->signal_clicked().connect( sigc::mem_fun(*this,&Ventana::on_button_notifications_clicked) );

}

void Ventana::on_button_add_clicked()
{
	std::cout<<"añadir clickeado"<<std::endl;
	int result = select_window->run();

	if (result == Gtk::RESPONSE_OK)
	{
		this->button_accept_clicked();
	}
	else if (result == Gtk::RESPONSE_CANCEL)
	{
		this->button_cancel_clicked();
	}
}

void Ventana::on_button_erase_clicked()
{
	//tal vez deberia desabilitar las señales de row selected
	std::cout<<"borrar clickeado"<<std::endl;
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL)
	{
		torrents->eraseSelectedRow();
		//controlador->borrarTorrent(t);
	}
}

void Ventana::on_button_stop_clicked()
{
	std::cout<<"detener clickeado"<<std::endl;
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL)
	{
		controlador->detenerTorrent(t);
	}
}

void Ventana::on_button_pause_clicked()
{
	std::cout<<"pausar clickeado"<<std::endl;
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL)
	{
		controlador->pausarTorrent(t);
	}
}

void Ventana::on_button_continue_clicked()
{
	std::cout<<"continuar clickeado"<<std::endl;
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL)
	{
		controlador->continuarTorrent(t);
	}
}
void Ventana::on_button_peers_clicked()
{
	std::cout<<"refrescar peers clickeado"<<std::endl;
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL)
	{
		controlador->refrescarPeers(t);
	}
}

void Ventana::on_button_up_clicked()
{
	std::cout<<"subir clickeado"<<std::endl;
	torrents->selectPrevious();
}

void Ventana::on_button_down_clicked()
{
	std::cout<<"bajar clickeado"<<std::endl;
	torrents->selectNext();
}

void Ventana::on_button_notifications_clicked()
{
	std::cout<<"borrar notificaciones clickeado"<<std::endl;
	attr->clearNotifications();
}

void Ventana::button_accept_clicked()
{
	 std::string filename = select_window->get_filename();
     std::cout << "archivo seleccionado: " << filename << std::endl;

     select_window->hide();
//     controlador->agregarTorrent(filename);
//     Torrent *t = controlador->agregarTorrent(filename);
//
//      if (t != NULL)
//		  torrents->addRow(t);
//VER sacar!!
     torrents->addRow(tor);
}

void Ventana::button_cancel_clicked()
{
	select_window->hide();
}

void Ventana::actualizarEstado(Torrent* t)
{
	std::cout<<"actualizar fila "<<std::endl;
	torrents->updateRow(t);
	if ( t == torrents->getSelectedTorrent())
		attr->showInfo(t);
}

void Ventana::addTorrent(Torrent *t)
{
	torrents->addRow(t);
}

void Ventana::mostrarNotificacion(std::string notificacion)
{
	attr->addNotification(notificacion);
}

void Ventana::on_menu_about()
{
	about_window->run();
	about_window->hide();
}

void Ventana::run()
{
	if (!error)
	{
		Gtk::Main* kit = Gtk::Main::instance();
		kit->run(*main_window);
	}
	else
	{
		std::cout<<"No se puede cargar la ventana "<<std::endl;
	}
}
