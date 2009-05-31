/*
 * ClienteTorrent.cpp
 *
 *  Created on: 28/05/2009
 *      Author: ale
 */

#include "ClienteTorrent.h"

ClienteTorrent::ClienteTorrent() {
	// TODO Auto-generated constructor stub

}

ClienteTorrent::~ClienteTorrent() {
	// TODO Auto-generated destructor stub
}

std::string ClienteTorrent::getPeerId(){
	std::string temp = peer_id;
	return temp;
}
