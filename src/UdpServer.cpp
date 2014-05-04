/*
 * UdpServer.cpp
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#include "UdpServer.h"
#include <iostream>
#include "Task.h"

using namespace std;

UdpServer::UdpServer(Socket sock, Address addr) :
		socket_(sock), address_(addr), thread_manager_(ThreadPool(10)) {
	thread_manager_.start();
}

UdpServer::~UdpServer() {
	thread_manager_.stop();
}

void UdpServer::start() {
#ifndef NDEBUG
	cout << "waiting for client to sendto msg" << endl;
#endif
	char buf[1024 * 1024];
	memset(buf, 0, 1024 * 1024);
	while (1) {
#ifndef NDEBUG
		cout << "----------------- " << endl;
#endif
		int ret = recvfrom(socket_.getSockFd(), buf, sizeof(buf), 0,
				address_.getAddressPoint(), &address_.length_);
		if (ret == -1) {
			throw std::runtime_error("sendto error");
		}
#ifndef NDEBUG
		cout << "server recv msg : " << buf << endl;
#endif
		//pack the client msg into task
		Task task;
		strncpy(task.expression_, buf, strlen(buf));
		task.server_fd_ = socket_.getSockFd();
		task.address_ = address_.getAddressIn();
		task.len_ = address_.length_;

		thread_manager_.addTaskToQueue(task);
		memset(buf, 0, 1024 * 1024);
	}
}

