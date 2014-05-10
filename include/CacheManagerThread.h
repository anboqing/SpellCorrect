#ifndef CACHE_MANAGER_THREAD_
#define CACHE_MANAGER_THREAD_

#include "Thread.h"
#include "WorkThread.h"
#include "Cache.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "Cache.h"

class CacheManagerThread : public Thread
{
public:
    CacheManagerThread(std::vector<WorkThread> &worker_handles_vec);
    ~CacheManagerThread();
    void run();
private:
    /*
    after some specific time ,
    merge cache data with disk file and update every thread's cache
    then every thread has the same cache
    */
    void synchronizeGlobalCacheWithDisk();
    /*
       1.collect all the thread's cache data to global cache map
       2.write the global data to disk file(call member func)
       3.update thread's cachedata(call thread's-cache's func);
    */
    void updateCache();
    std::vector<WorkThread> &worker_handles_vec_;
    Cache::cache_map_type global_cache_map_;
};

#endif