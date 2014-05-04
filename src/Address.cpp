/*
 * Address.cpp
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#include "Address.h"

Address::~Address() {

}

Address::Address(unsigned int addr,const short port,const short family):length_(sizeof(address_)){
	address_.sin_addr.s_addr = addr;
	address_.sin_family = family;
	address_.sin_port = port;
}


