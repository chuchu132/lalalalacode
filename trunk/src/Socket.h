/*
 * Socket.h
 *
 *  Created on: 23/04/2009
 *      Author: ale
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <iostream>
#include <string>

class Socket {
public:
	Socket();
	virtual ~Socket();
	int connect(const std::string &host,unsigned int port);
	int connectWithTimeout(const std::string &host, unsigned int port,int timeout);
	int listen(unsigned int port,unsigned int numClientesEspera);
	int close();
	int send(const char* stream,unsigned int size);
	/*No garantiza que el buffer quede lleno*/
	int receive(char* stream,unsigned int size);
	/*En caso de error devuelve ERROR, en caso de que se cierre la conexion
	 * devuelve la cantidad de bytes almacenados en el buffer y sino
	 * devuelve en el buffer "size" bytes*/
	int receiveExact(char* stream,unsigned int size);
	Socket* accept();
	bool is_valid();
	std::string getIp();
	void setIp(int ip);
	void setIp(std::string ip);
	void setPuerto(unsigned int port);
	unsigned int getPuerto();
	int setNonblocking();
	int setBlocking();

protected:
	int fd;
	bool valido;
	std::string ip;
	unsigned int puerto;

private:
	Socket(int fd);
	Socket(const Socket& original);
};

#endif /* SOCKET_H_ */
