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
	/*Conecta el socket al host en el puerto dado.*/
	int connect(const std::string &host,unsigned int port);
	/*Intenta conectarse con el host durante un tiempo sino devuelve ERROR*/
	int connectWithTimeout(const std::string &host, unsigned int port,int timeout);
	/*Bindea el socket al puerto y se queda escuchando*/
	int listen(unsigned int port,unsigned int numClientesEspera);
	/*Cierra el socket*/
	int close();
	/*Envia por la red los size bytes del stream*/
	int send(const char* stream,unsigned int size);
	/*No garantiza que el buffer quede lleno*/
	int receive(char* stream,unsigned int size);
	/*En caso de error devuelve ERROR y sino
	 * devuelve en el buffer "size" bytes*/
	int receiveExact(char* stream,unsigned int size);
	/*Cuando llega una conexion de afuera, se crea un nuevo socket.*/
	Socket* accept();
	bool is_valid();

	std::string getIp();
	void setIp(int ip);
	void setIp(std::string ip);
	void setPuerto(unsigned int port);
	unsigned int getPuerto();

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
