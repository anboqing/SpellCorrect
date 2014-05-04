/*
 * Thread.h
 *
 *  Created on: Apr 22, 2014
 *      Author: wing
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <stdexcept>

/*
 * 这个线程类是个抽象类，希望派生类去改写它
 */
class Thread {

public:
	Thread();
	virtual ~Thread();

	void start();
	void join();
	static void *thread_func(void *);
	/*
	 * 这是个纯虚函数
	 */
	virtual void run() = 0;
	pthread_t get_tid() const;

protected:
	pthread_t _tid;
};

#endif /* THREAD_H_ */
