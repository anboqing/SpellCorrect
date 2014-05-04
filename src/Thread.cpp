/*
 * Thread.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: wing
 */

#include "Thread.h"



Thread::Thread() :
		_tid(0) {

}
Thread::~Thread() {

}

void Thread::start() {
	//采用静态函数的指针作为回调函数
	//this作为线程的参数
	pthread_create(&_tid, NULL, Thread::thread_func, this);
}
void Thread::join() {
	pthread_join(_tid, NULL);
}
void *Thread::thread_func(void *arg) {
	//arg实际上是线程对象的指针，类型为实际线程的类型
	Thread *p_thread = static_cast<Thread*>(arg);
	//这里利用了动态绑定
	p_thread->run();
	return NULL;
}

pthread_t Thread::get_tid() const {
	return _tid;
}
