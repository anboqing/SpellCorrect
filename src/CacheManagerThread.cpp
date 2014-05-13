#include "CacheManagerThread.h"
#include "Log.h"
#include "Configure.h"

#include <stdlib.h>
#include <sstream>

using namespace std;

CacheManagerThread::CacheManagerThread(std::vector<WorkThread> &worker_handles_vec): worker_handles_vec_(worker_handles_vec)
{
    for (vector<WorkThread>::iterator iter = worker_handles_vec_.begin();
            iter != worker_handles_vec_.end(); ++iter)
    {
        iter->regeditCacheManager(this);
    }
}

CacheManagerThread::~CacheManagerThread() {}

/*
    after some specific time ,
    merge cache data with disk file and update every thread's cache
    then every thread has the same cache
*/
void CacheManagerThread::synchronizeGlobalCacheWithDisk()
{
#ifndef NDEBUG
    WRITE_STR(string(" start synchronize Global Cache With Disk ......."));
#endif
    Configure *pconf = Configure::getInstance();
    string cache_path = pconf->getConfigByName("cache_file_path");
    string home_path = pconf->getConfigByName("home_path");
#ifndef NDEBUG
    WRITE_STR(string(" 1st. merge the disk cache data to memory ......."));
#endif
    // 1st. merge the disk cache data to memory;
    ifstream ifs((home_path + cache_path).c_str());
    if (!(ifs.is_open()))
    {
        throw runtime_error("open cache_path file");
    }
    string line, keyword, pairword;
    int frequency;
    while (getline(ifs, line))
    {
        istringstream istr(line);
        istr >> keyword;
        vector<pair<string, int> > vec;
        while (istr >> pairword)
        {
            istr >> frequency;
            vec.push_back(make_pair(pairword, frequency));
        }
        CacheData data(vec);
        global_cache_map_.insert(make_pair(keyword, data));
    }
    ifs.close();
#ifndef NDEBUG
    WRITE_STR(string(" 2nd. write the cache data back to disk...... "));
#endif
    // 2nd. write the global cache data back to disk;
    ofstream ofs((home_path + cache_path).c_str());
    // file lock;
    if (!(ofs.is_open()))
    {
        throw runtime_error("open cache_path file");
    }
    for (Cache::cache_map_type::iterator iter = global_cache_map_.begin() ; iter != global_cache_map_.end(); ++iter)
    {
        ofs << (*iter).first << "\t";
        vector<pair<string, int> > vec = (*iter).second.getDataVec();
        for (vector<pair<string, int> >::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            ofs << (*it).first << " " << (*it).second<<" ";
        }
        ofs << "\n";
    }
    ofs.close();
}

/*
   1.collect all the thread's cache data to global cache map
   2.write the global data to disk file(call member func)
   3.update thread's cachedata(call thread's-cache's func);
*/
void CacheManagerThread::updateCache()
{
#ifndef NDEBUG
    WRITE_STR(string(" start merge every thread's cache to global_cache_map_ ...... "));
#endif
    for (vector<WorkThread>::iterator iter = worker_handles_vec_.begin();
            iter != worker_handles_vec_.end(); ++iter)
    {
        Cache &cache = iter->getCacheHandle();
        // merge every thread's cache data to manager's cache-map
        Cache::cache_map_type &thread_cache_map = cache.getCacheDataMap();
        for (Cache::cache_map_type::iterator it = thread_cache_map.begin();
                it != thread_cache_map.end(); ++it)
        {
            global_cache_map_[(*it).first] = (*it).second;
        }
    }
    // synchronize global cache data with disk file
    synchronizeGlobalCacheWithDisk();
#ifndef NDEBUG
    WRITE_STR(string("update every thread's cache : call their loadCacheFileToMemory()"));
#endif
    // update every thread's cache : call their loadCacheToDisk();
    for (vector<WorkThread>::iterator iter = worker_handles_vec_.begin();
            iter != worker_handles_vec_.end(); ++iter)
    {
        Cache &cache = iter->getCacheHandle();
// strategy 1: every thread load from disk;
        // cache.loadCacheFileToMemory();

// strategy 2: every thread's cache data from manager's cache-map
        
        // update every thread's cache data from manager's cache-map
        Cache::cache_map_type &thread_cache_map = cache.getCacheDataMap();
        for (Cache::cache_map_type::iterator it = global_cache_map_.begin();
                it != global_cache_map_.end(); ++it)
        {
            thread_cache_map[(*it).first] = (*it).second;
        }
    }
}

void CacheManagerThread::run()
{
    while (true)
    {
#ifndef NDEBUG
        WRITE_STR("start updateCache .......");
#endif
        updateCache();
        // 每隔一段时间就更新所有cache
        Configure *pconf = Configure::getInstance();
        string s_time = pconf->getConfigByName("cache_manager_sleep_seconds");
        int sleep_time = atoi(s_time.c_str());
        sleep(sleep_time);
    }
}