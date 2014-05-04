/*
 * Condition.h
 *
 *  Created on: Apr 22, 2014
 *      Author: wing
 */

#ifndef CONDITION_H_
#define CONDITION_H_


#include <pthread.h>
#include "MutexLock.h"

class Condition{
public:

	Condition(MutexLock *p_lock);
	~Condition();

	void wait();
	void notify();
	void notify_all(); //谨慎使用

private:
	pthread_cond_t _cond;
	MutexLock *_p_lock;  //这里的lock要用指针

};

#endif /* CONDITION_H_ */
