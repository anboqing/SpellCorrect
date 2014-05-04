/*
 * UdpServer.h
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#include "ThreadPool.h"
#include "Address.h"
#include "Socket.h"

class UdpServer {
public:
	UdpServer(Socket sock,Address addr);
	virtual ~UdpServer();
	//regist a ThreadPool to UdpServer;
//	void registPool(ThreadPool *pool){
//		thread_manager_ = pool;
//	}
	void start();
private:
	Socket socket_;
	Address address_;
	ThreadPool thread_manager_;
	//forbid copy;
	UdpServer(const UdpServer&);
	UdpServer &operator=(const UdpServer&);
};

#endif /* UDPSERVER_H_ */
