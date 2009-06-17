/*
 * Socket.cpp
 *
 *  Created on: 23/04/2009
 *      Author: ale
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include "Constantes.h"
#include "Socket.h"

Socket::Socket() {
	valido = true;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == ERROR) {
		valido = false;
	}
}
Socket::Socket(int fdNuevo) {
	this->fd = fdNuevo;
	valido = true;
}

Socket::Socket(const Socket& original) {
	fd = original.fd;
	valido = original.valido;
}


Socket::~Socket() {
	if (valido) {
		close();
	}
}

int Socket::connect(const std::string &host, unsigned int port) {

	if (valido) {
		hostent* host_add;
		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);

		if ((host_add = gethostbyname(host.c_str()))) {

			address.sin_addr.s_addr
					= ((struct in_addr*) (host_add->h_addr))->s_addr;

			return ::connect(fd, (struct sockaddr *) &address, sizeof(address));
		}
	}
	return ERROR;
}

int Socket::listen(unsigned int port, unsigned int numClientesEspera) {

	if (valido) {
		struct sockaddr_in address;
		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(fd, (struct sockaddr*) &(address), sizeof(address)) != ERROR) {
			return ::listen(fd, numClientesEspera);
		}
	}
	return ERROR;
}

int Socket::close() {
	shutdown(fd, 2);
	valido = false;
	return ::close(fd);
}

int Socket::send(const char* stream, unsigned int size) {

	unsigned int enviado = 0;
	int aux = 0;
	while (enviado < size) {
		aux = ::send(fd, stream + enviado, size - enviado, 0);
		if (aux > 0) {
			enviado += aux;
		} else {
			if (aux < 0) {
				return ERROR;
			}
			return enviado;
		}
	}
	return size;
}

int Socket::receive(char* stream, unsigned int size) {
	return recv(fd, stream, size, 0);
}

int Socket::receiveExact(char* stream, unsigned int size) {
	unsigned int recibido = 0;
	int aux = 0;
	while (recibido < size) {
		aux = recv(fd, stream + recibido, size - recibido, 0);
		if (aux > 0) {
			recibido += aux;
			if (recibido == size) {
				return size;
			}
		} else {
			if (aux < 0) {
				return ERROR;
			}
			return recibido;
		}
	}
	return ERROR;
}

Socket* Socket::accept() {
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	unsigned int sizeClient = sizeof(client);
	int fdNuevo;
	if ((fdNuevo = ::accept(fd, (struct sockaddr *) &(client), &sizeClient))) {
		return new Socket(fdNuevo);
	}
	return NULL;
}

void Socket::setIp(std::string ipInt) {
	this->ip = ip;
}

void Socket::setIp(int ipInt) {
	std::stringstream ip;
	unsigned char* ipArr = (unsigned char*) &ipInt;
	unsigned char temp = ipArr[0];
	ip << (int) temp << ".";
	temp = (unsigned char) ipArr[1];
	ip << (int) temp << ".";
	temp = (unsigned char) ipArr[2];
	ip << (int) temp << ".";
	temp = (unsigned char) ipArr[3];
	ip << (int) temp;
	this->ip = ip.str();
}

std::string Socket::getIp() {
	return ip;
}

bool Socket::is_valid() {
	return valido;
}
