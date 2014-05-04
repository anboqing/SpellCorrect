/*
 * MutexLock.h
 *
 *  Created on: Apr 22, 2014
 *      Author: wing
 */

#ifndef MUTEXLOCK_H_
#define MUTEXLOCK_H_


#include <pthread.h>
#include <stdexcept>

class Condition;

class MutexLock{
	friend class Condition;
public:
	MutexLock();
	~MutexLock();

	void lock();
	void unlock();

private:
	pthread_mutex_t _mutex;
};

#endif /* MUTEXLOCK_H_ */
