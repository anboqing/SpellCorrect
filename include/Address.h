/*
 * Address.h
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Copyable.h"

class Address : public Copyable{
public:
	Address(unsigned int addr,const short port,const short family);
	virtual ~Address();
	struct sockaddr_in getAddressIn(){
		return address_;
	}
	struct sockaddr *getAddressPoint()const{
		return (struct sockaddr*)&address_;
	}
	socklen_t length_;
private:
	//server socket address;
	struct sockaddr_in address_;
};

#endif /* ADDRESS_H_ */
