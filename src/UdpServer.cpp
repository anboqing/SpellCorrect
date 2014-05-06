/*
 * UdpServer.cpp
 *
 *  Created on: 2014年5月3日
 *      Author: anboqing
 */

#include "UdpServer.h"
#include "Task.h"
#include "Log.h"

#include <iostream>
#include <unistd.h>

using namespace std;

UdpServer::UdpServer(Socket &sock, Address &addr,ThreadPool &pool) :
		socket_(sock), address_(addr), thread_manager_(pool) {
	thread_manager_.start();
}

UdpServer::~UdpServer() {
	thread_manager_.stop();
}

void UdpServer::start() {
#ifndef NDEBUG
    Log::WRITE_LOG( string(" ** server start up!! ") );
#endif
	char buf[1024];
	memset(buf, 0, 1024);
	while (1) {
		int ret = recvfrom(socket_.getSockFd(), buf, sizeof(buf), 0,
				address_.getAddressPoint(), &address_.length_);
		if (ret == -1) {
			throw std::runtime_error("sendto error");
		}
		//pack the client msg into task
		Task task;
		strncpy(task.expression_, buf, strlen(buf));
		task.server_fd_ = socket_.getSockFd();
		task.address_ = address_.getAddressIn();
		task.len_ = address_.length_;
		thread_manager_.addTaskToQueue(task);
		memset(buf, 0, 1024);
	}
}

