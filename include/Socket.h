/*
 * Socket.h
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#ifndef SOCKET_H_
#define SOCKET_H_
#include "Address.h"
#include <sys/socket.h>
#include <sys/types.h>

class Socket{
private:
	int socket_file_descriptor_;
public:
	Socket(const char* type);
	virtual ~Socket();
	int getSockFd(){
		return socket_file_descriptor_;
	}
	//if success return true;else return false;
	bool bindToAddess(const Address &addr);
};

#endif /* SOCKET_H_ */
