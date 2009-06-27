/*
 * ProgressView.cpp
 *
 *  Created on: 26/06/2009
 *      Author: teddy
 */

#include "ProgressView.h"

ProgressView::ProgressView() {}

ProgressView::~ProgressView() {
	// TODO Auto-generated destructor stub
}

void ProgressView::setText(std::string texto) {
	text->set_text(texto);
}

void ProgressView::setBarra(Gtk::ProgressBar *progress_bar){
	this->progress_bar = progress_bar;
}

void ProgressView::setWindow(Gtk::Dialog *progress_window) {
	this->progress_window = progress_window;
}

void ProgressView::setLabel(Gtk::Label *text){
	this->text = text;
}

void ProgressView::mover() {
	progress_bar->pulse();
	std::cout<<"pulse"<<std::endl;
}

void* ProgressView::run() {
	std::cout<<"show progreso"<<std::endl;
//	id_activity = Glib::signal_timeout().connect(sigc::mem_fun(*this,
//	              &ProgressView::mover), 50 );
	progress_window->run();//ver si con el hide sale de aca
	progress_window->hide();
	std::cout<<"fin progreso run"<<std::endl;
	return NULL;
}

void ProgressView::hide() {
	std::cout<<"hide progreso"<<std::endl;
	progress_window->response(Gtk::RESPONSE_CANCEL);
	//std::cout<<"paso response"<<std::endl;

}
