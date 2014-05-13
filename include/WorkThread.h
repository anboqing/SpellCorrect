
#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include "Thread.h"
#include "Cache.h"

#include <string>

class CacheManagerThread;

class ThreadPool;

class WorkThread : public Thread
{
public:
    void run();
    bool registThreadPool(ThreadPool *p_pool);
    bool registCacheManager(CacheManagerThread *p_manager);
    Cache &getCacheHandle()
    {
        return cache_;
    }
private:
    ThreadPool *p_pool_;
    // has-a Cache instance;
    Cache cache_;
    CacheManagerThread *p_manager_;
    std::string excute(std::string keyword);
};

#endif
