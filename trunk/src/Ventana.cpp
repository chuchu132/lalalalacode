/*
 * Ventana.cpp
 *
 *  Created on: 28/05/2009
 *      Author: teddy
 */
#include "Ventana.h"

#define FILE_WINDOW "wind.vista"
#define MAIN_WINDOW "main_wind"
#define ABOUT_WINDOW "about_wind"
#define BUTTON_ADD 	"boton_agregar"
#define BUTTON_ERASE "boton_borrar"
#define BUTTON_STOP	"boton_detener"
#define BUTTON_PAUSE	"boton_pausar"
#define BUTTON_CONTINUE "boton_continuar"
#define BUTTON_UP 	"boton_subir"
#define BUTTON_DOWN	"boton_bajar"
#define BUTTON_ABOUT "about"

#define MENU_HELP "Ayuda"


Ventana::Ventana()
{
	std::cout<<"comienzo ventana"<<std::endl;
	try
	{
		builder = Gtk::Builder::create_from_file(FILE_WINDOW);
	}catch(Glib::FileError& ex)
	{
		std::cout<<"error al cargar el archivo de la vista"<<std::endl;
	}
	std::cout<<"archivo cargado"<<std::endl;

	//obtengo la ventana
	main_window = 0;
	builder->get_widget(MAIN_WINDOW, main_window);
	std::cout<<"obtengo ventana"<<std::endl;
	builder->get_widget(ABOUT_WINDOW, about_window);

//	builder->get_widget(MENU_HELP, menu_help);

	//obtengo los botones
	this->getButtons();
	this->connectSignals();

}

Ventana::~Ventana()
{
	// TODO Auto-generated destructor stub
}

Gtk::Window& Ventana::getVentana()
{
	return (*main_window);
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
}

void Ventana::on_button_erase_clicked()
{
	std::cout<<"borrar clickeado"<<std::endl;
}

void Ventana::on_button_stop_clicked()
{
	std::cout<<"detener clickeado"<<std::endl;
}

void Ventana::on_button_pause_clicked()
{
	std::cout<<"pausar clickeado"<<std::endl;
}

void Ventana::on_button_continue_clicked()
{
	std::cout<<"continuar clickeado"<<std::endl;
}

void Ventana::on_button_up_clicked()
{
	std::cout<<"subir clickeado"<<std::endl;
}

void Ventana::on_button_down_clicked()
{
	std::cout<<"bajar clickeado"<<std::endl;
}

void Ventana::on_menu_about()
{
	std::cout<<"about clickeado"<<std::endl;
	about_window->show();
}




int main (int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);

	Ventana ventana;

	kit.run(ventana.getVentana());

	std::cout<<"fin"<<std::endl;

	return 0;
}

