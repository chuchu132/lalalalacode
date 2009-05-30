/*
 * Ventana.cpp
 *
 *  Created on: 28/05/2009
 *      Author: teddy
 */
#include "Ventana.h"

#define WINDOW_FILE "res/wind.vista"

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
#define BUTTON_ABOUT "about"

#define MENU_HELP "Ayuda"

#define VIEW_TORRENTS "torrents"
#define VIEW_CATEGORIES "clasificacion"

Ventana::Ventana()
{
	std::cout<<"comienzo ventana"<<std::endl;
	try
	{
		builder = Gtk::Builder::create_from_file(WINDOW_FILE);
		std::cout<<"archivo cargado"<<std::endl;

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
		std::cout<<"ventana de seleccion de archivo cargada"<<std::endl;

		//	builder->get_widget(MENU_HELP, menu_help);

		//obtengo el tree view de torrents
		Gtk::TreeView *view_torrents = 0;
		builder->get_widget(VIEW_TORRENTS,view_torrents);
		torrents.setTreeView(view_torrents);

		Gtk::TreeView *view_cat = 0;
		builder->get_widget(VIEW_CATEGORIES,view_cat);
		torrents.setCategoriesView(view_cat);
		std::cout<<"view torrents cargado"<<std::endl;

		//obtengo los botones
		this->getButtons();
		this->connectSignals();

	}catch(Glib::FileError& ex)
	{
		std::cout<<"error al cargar el archivo de la vista"<<std::endl;
	}
}

Ventana::~Ventana()
{
	// TODO Auto-generated destructor stub
}

Gtk::Window& Ventana::getVentana()
{
	return (*main_window);
}

void Ventana::setControlador(Controlador *c)
{
	this->controlador = c;
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

	//button_about->signal_activate().connect(sigc::mem_fun(*this,&Ventana::on_button_about_clicked));
}

void Ventana::on_button_add_clicked()
{
	std::cout<<"añadir clickeado"<<std::endl;
	//select_window->show();
	//el accept de esta ventana deberia hacer un agregar torrent con la ruta del archivo
	//el cancelar deberia cerrar la ventana
	torrents.addRow(NULL);
}

void Ventana::on_button_erase_clicked()
{
	std::cout<<"borrar clickeado"<<std::endl;
	Torrent *t = torrents.getSelectedTorrent();
	if (t != NULL)
	{
		controlador->borrarTorrent(t);
		torrents.eraseSelectedRow();
	}
}

void Ventana::on_button_stop_clicked()
{
	std::cout<<"detener clickeado"<<std::endl;
	Torrent *t = torrents.getSelectedTorrent();
	if (t != NULL)
	{
		controlador->detenerTorrent(t);
	}
}

void Ventana::on_button_pause_clicked()
{
	std::cout<<"pausar clickeado"<<std::endl;
	Torrent *t = torrents.getSelectedTorrent();
	if (t != NULL)
	{
		controlador->pausarTorrent(t);
	}
}

void Ventana::on_button_continue_clicked()
{
	std::cout<<"continuar clickeado"<<std::endl;
	Torrent *t = torrents.getSelectedTorrent();
	if (t != NULL)
	{
		controlador->continuarTorrent(t);
	}
}

void Ventana::on_button_up_clicked()
{
	std::cout<<"subir clickeado"<<std::endl;
	torrents.selectPrevious();
}

void Ventana::on_button_down_clicked()
{
	std::cout<<"bajar clickeado"<<std::endl;
	torrents.selectNext();
}

void Ventana::on_menu_about()
{
	std::cout<<"about clickeado"<<std::endl;
	about_window->show();
}

