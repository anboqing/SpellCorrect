
#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include "Thread.h"
#include "Cache.h"

class CacheManagerThread;

class ThreadPool;

class WorkThread : public Thread
{
public:
    void run();
    bool regeditThreadPool(ThreadPool *p_pool);
    bool regeditCacheManager(CacheManagerThread *p_manager);
    Cache &getCacheHandle(){
    	return cache_;
    }
private:
    ThreadPool *p_pool_;
    // has-a Cache instance;
    Cache cache_;
    CacheManagerThread *p_manager_;
};

#endif
