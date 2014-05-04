#ifndef MUTEX_SAFE_GUARDE_H_
#define MUTEX_SAFE_GUARDE_H_

#include "MutexLock.h"
#include "NoneCopyable.h"

class MutexSafeGuarde : public NoneCopyable{
public:
	MutexSafeGuarde(MutexLock &l):lock_(l){
		lock_.lock();
	}
	~MutexSafeGuarde(){
		lock_.unlock();
	}
private:
	MutexLock &lock_;
};

#endif
