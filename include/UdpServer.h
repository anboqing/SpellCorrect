/*
 *  UdpServer.h
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#include "ThreadPool.h"
#include "Address.h"
#include "Socket.h"
#include "NoneCopyable.h"

class UdpServer : public NoneCopyable{
public:
	UdpServer(Socket &sock,Address &addr,ThreadPool &pool);
	virtual ~UdpServer();
	//  regist a ThreadPool to UdpServer;
	void start();
private:
	Socket &socket_;
	Address &address_;
	ThreadPool &thread_manager_;
};

#endif /* UDPSERVER_H_ */
