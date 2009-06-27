/*
 * Ventana.cpp
 *
 *  Created on: 28/05/2009
 *      Author: Lucia
 */

#include "Ventana.h"

#define WINDOW_FILE "res/wind.glade"

#define MAIN_WINDOW "main_wind"
#define ABOUT_WINDOW "about_wind"
#define SELECT_WINDOW "select_wind"
#define PREFERENCES_WINDOW "preferences_wind"
#define PROGRESS_WINDOW "progress_wind"

#define BUTTON_ADD 	"boton_agregar"
#define BUTTON_ERASE "boton_borrar"
#define BUTTON_STOP	"boton_detener"
#define BUTTON_CONTINUE "boton_continuar"
#define BUTTON_UP 	"boton_subir"
#define BUTTON_DOWN	"boton_bajar"
#define BUTTON_PEERS "boton_peers"
#define BUTTON_NOTIF "boton_notificaciones"
#define ENTRY_PUERTO "entry1"
#define PROGRESS_BAR "progressbar1"
#define MENU_VBOX "vbox1"
#define OPEN_HELP "firefox help/main.html"
//xdg-open help/main.html

#define VIEW_TORRENTS "torrents"
#define VIEW_CATEGORIES "clasificacion"

Ventana::Ventana():attr(new AttributesView()),torrents(new TorrentView()) {
	error = false;
	controlador = NULL;
	procesar = 0;
	try {
		builder = Gtk::Builder::create_from_file(WINDOW_FILE);
		torrents->setAttributesView(attr);

		this->getWindows();
		this->getViews();
		this->getButtons();
		this->setMenu();
		this->connectSignals();

	} catch (Glib::FileError& ex1) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex1.what() << std::endl;
		error = true;
	} catch (Glib::MarkupError& ex2) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex2.what() << std::endl;
		error = true;
	} catch (Gtk::BuilderError& ex3) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex3.what() << std::endl;
		error = true;
	}
}

Ventana::~Ventana() {
	delete torrents;
	delete attr;
	if(activo)
		detener();
	if (controlador != NULL)
		controlador->cerrarCliente();
}

void Ventana::getWindows() {
	//obtengo la ventana principal
	main_window = 0;
	builder->get_widget(MAIN_WINDOW, main_window);

	//obtengo la ventana acerca de
	about_window = 0;
	builder->get_widget(ABOUT_WINDOW, about_window);

	//obtengo la ventana de seleccion de archivo
	select_window = 0;
	builder->get_widget(SELECT_WINDOW, select_window);

	//obtengo la ventana de preferencias
	preferences_window = 0;
	builder->get_widget(PREFERENCES_WINDOW, preferences_window);

	//obtengo la ventana de progreso
	builder->get_widget(PROGRESS_BAR, progress_bar);
	builder->get_widget(PROGRESS_WINDOW, progress_window);

	filter.set_name("Archivos Torrent (*.torrent)");
	filter.add_pattern("*.torrent");
	select_window->add_filter(filter);

	select_window->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	select_window->add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	preferences_window->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	preferences_window->add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_APPLY);
	builder->get_widget(ENTRY_PUERTO, entry_puerto);
	std::stringstream buf;
	buf << PUERTO_DEFAULT;
	entry_puerto->set_text(buf.str());
}

void Ventana::getViews() {
	//obtengo el tree view de torrents
	Gtk::TreeView *view_torrents = 0;
	builder->get_widget(VIEW_TORRENTS, view_torrents);
	torrents->setTreeView(view_torrents);

	Gtk::TreeView *view_cat = 0;
	builder->get_widget(VIEW_CATEGORIES, view_cat);
	torrents->setCategoriesView(view_cat);

	attr->setAttributesView(builder);
}

void Ventana::setMenu() {
	menu_archivo = Gtk::ActionGroup::create();
	menu_editar = Gtk::ActionGroup::create();
	menu_ayuda = Gtk::ActionGroup::create();

	menu_archivo->add(Gtk::Action::create("FileMenu", "Archivo"));
	menu_archivo->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &Ventana::on_menu_quit));
	menu_archivo->add(Gtk::Action::create("FileAdd", Gtk::Stock::ADD,
			"_Agregar", "Agrega un archivo .torrent para descargar"),
			sigc::mem_fun(*this, &Ventana::on_button_add_clicked));
	menu_archivo->add(Gtk::Action::create("FileDelete", Gtk::Stock::DELETE,
			"_Borrar", "Borra el Torrent seleccionado"), sigc::mem_fun(*this,
			&Ventana::on_button_erase_clicked));
	menu_archivo->add(Gtk::Action::create("FileStop", Gtk::Stock::MEDIA_STOP,
			"_Detener", "Detiene el Torrent seleccionado"), sigc::mem_fun(
			*this, &Ventana::on_button_stop_clicked));
	menu_archivo->add(Gtk::Action::create("FileContinue",
			Gtk::Stock::MEDIA_PLAY), sigc::mem_fun(*this,
			&Ventana::on_button_continue_clicked));

	menu_editar->add(Gtk::Action::create("EditMenu", "Editar"));
	menu_editar->add(Gtk::Action::create("EditPref", Gtk::Stock::PREFERENCES,
			"_Preferencias", "Preferencias de Configuracion"),
			sigc::mem_fun(*this, &Ventana::on_menu_preferences));

	menu_ayuda->add(Gtk::Action::create("HelpMenu", "Ayuda"));
	menu_ayuda->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT,"A_cerca De", "Acerca de FiTorrent"),
			sigc::mem_fun(*this, &Ventana::on_menu_about));
	menu_ayuda->add(Gtk::Action::create("HelpMan", Gtk::Stock::HELP,"_Manual", "Manual de usuario"),
			sigc::mem_fun(*this, &Ventana::on_menu_help));

	menu_UIManager = Gtk::UIManager::create();
	menu_UIManager->insert_action_group(menu_archivo);
	menu_UIManager->insert_action_group(menu_editar);
	menu_UIManager->insert_action_group(menu_ayuda);

	main_window->add_accel_group(menu_UIManager->get_accel_group());

	//layout
	Glib::ustring ui_info = "<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"      <menuitem action='FileAdd'/>"
		"      <menuitem action='FileDelete'/>"
		"	   <separator/>"
		"      <menuitem action='FileStop'/>"
		"      <menuitem action='FileContinue'/>"
		"      <separator/>"
		"      <menuitem action='FileQuit'/>"
		"    </menu>"
		"    <menu action='EditMenu'>"
		"      <menuitem action='EditPref'/>"
		"    </menu>"
		"    <menu action='HelpMenu'>"
		"      <menuitem action='HelpMan'/>"
		"      <menuitem action='HelpAbout'/>"
		"    </menu>"
		"  </menubar>"
		"</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{
		menu_UIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " << ex.what();
	}
#else
	std::auto_ptr<Glib::Error> ex;
	menu_UIManager->add_ui_from_string(ui_info, ex);
	if (ex.get()) {
		std::cerr << "building menus failed: " << ex->what();
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
	menu = menu_UIManager->get_widget("/MenuBar");
	Gtk::Box *box;
	builder->get_widget(MENU_VBOX, box);
	box->pack_end(*menu, Gtk::PACK_SHRINK);

}

void Ventana::getButtons() {
	builder->get_widget(BUTTON_ADD, button_add);
	builder->get_widget(BUTTON_ERASE, button_erase);
	builder->get_widget(BUTTON_STOP, button_stop);
	builder->get_widget(BUTTON_CONTINUE, button_continue);
	builder->get_widget(BUTTON_UP, button_up);
	builder->get_widget(BUTTON_DOWN, button_down);
	builder->get_widget(BUTTON_PEERS, button_peers);
	builder->get_widget(BUTTON_NOTIF, button_notif);
}

void Ventana::connectSignals() {

	button_add->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_add_clicked));
	button_erase->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_erase_clicked));
	button_stop->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_stop_clicked));
	button_continue->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_continue_clicked));
	button_up->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_up_clicked));
	button_down->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_down_clicked));
	button_peers->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_peers_clicked));
	button_notif->signal_clicked().connect(sigc::mem_fun(*this,
			&Ventana::on_button_notifications_clicked));
}

void Ventana::on_button_add_clicked() {
	int result = select_window->run();
	select_window->hide();
	if (result == Gtk::RESPONSE_OK) {
		this->button_accept_clicked();
	}
}

void Ventana::on_button_erase_clicked() {
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL){
		showBar("Borrando Torrent");
		procesar = 1;
	}
}

void Ventana::on_button_stop_clicked() {
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL){
		showBar("Deteniendo Torrent");
		procesar = 2;
	}
}

void Ventana::on_button_continue_clicked() {
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL){
		showBar("Iniciando Torrent");
		procesar = 3;
	}
}

void Ventana::on_button_peers_clicked() {
	Torrent *t = torrents->getSelectedTorrent();
	if (t != NULL) {
		showBar("Refrescando Peers");
		procesar = 4;
	}
}

void Ventana::on_button_up_clicked() {
	torrents->selectPrevious();
}

void Ventana::on_button_down_clicked() {
	torrents->selectNext();
}

void Ventana::on_button_notifications_clicked() {
	attr->clearNotifications();
}

void Ventana::button_accept_clicked() {
	showBar("Agregando Torrent");
	procesar = 5;
}

void Ventana::actualizarEstado(Torrent* t) {
	torrents->updateRow(t);//tal vez va el mutex aca tb
	if (t == torrents->getSelectedTorrent())
		attr->showInfo(t);
}

void Ventana::addTorrent(Torrent *t) {
	mutex_torrents.lock();
	torrents->addRow(t);
	mutex_torrents.unlock();
}

void Ventana::mostrarNotificacion(std::string notificacion) {
	attr->addNotification(notificacion);
}

void Ventana::on_menu_about() {
	about_window->run();
	about_window->hide();
}

void Ventana::on_menu_quit() {
	main_window->hide();
}

void Ventana::on_menu_help() {
	system(OPEN_HELP);
}

void Ventana::on_menu_preferences() {
	int result = preferences_window->run();

	if (result == Gtk::RESPONSE_APPLY) {
		controlador->guardarConfiguracion();
	}
	preferences_window->hide();
}

int Ventana::correr() {
	if (!error) {
		Gtk::Main* kit = Gtk::Main::instance();
		this->execute();
		kit->run(*main_window);
		return 0;
	}
	return 1;
}

void* Ventana::run() {
	activo = true;//ver si hay que usar un mutex para activo
	Torrent *t;
	while (activo) {

		if (procesar){
			switch (procesar) {
			case 1:{//borrar torrent
				mutex_torrents.lock();
				Torrent *t = torrents->getSelectedTorrent();
				if (t != NULL) {//ver si este if no es redundante
					torrents->eraseSelectedRow();
					controlador->borrarTorrent(t);
					attr->torrentDeleted(t);
				}
				mutex_torrents.unlock();
			}
				break;
			case 2:{//detener torrent
				Torrent *t = torrents->getSelectedTorrent();
				if (t != NULL){
					controlador->detenerTorrent(t);
				}
			}
				break;
			case 3: {//continuar torrent
				Torrent *t = torrents->getSelectedTorrent();
				if (t != NULL)
					controlador->continuarTorrent(t);
			}
				break;
			case 4:{//refrescar peers
				Torrent *t = torrents->getSelectedTorrent();
				if (t != NULL)
					controlador->refrescarPeers(t);
			}
				break;
			case 5: {//agregar torrent
				Torrent *t = controlador->agregarTorrent(select_window->get_filename());
				if (t != NULL) {
					addTorrent(t);
				}
			}
				break;
			}
			id_activity.disconnect();
			progress_window->hide();
			procesar = 0; //ya proceso
		}

		if (controlador->hayCambios()) {
			t = controlador->getCambio();
			actualizarEstado(t);
			sleep(1);
		} else {
			sleep(2);
		}
	}
	return NULL;
}

void Ventana::detener() {
	activo = false;
	this->join();
}

bool Ventana::huboError() {
	return error;
}

unsigned int Ventana::getPuerto() {
	std::stringstream buffer;
	unsigned int puerto = 0;
	buffer << entry_puerto->get_text();
	buffer >> puerto;
	return puerto;
}

std::string Ventana::getRutaDescargas() {
	return RUTA_DESCARGAS;//TODO implementar!!
}

void Ventana::setControlador(Controlador *c){
	controlador = c;
}

bool Ventana::mover() {
	progress_bar->pulse();
	return true;
}

void Ventana::showBar(Glib::ustring texto){
	progress_bar->set_text(texto);
	progress_window->show();
	//actualiza la barra de progreso
	id_activity = Glib::signal_timeout().connect(sigc::mem_fun(*this,
		        &Ventana::mover), 200 );
}

