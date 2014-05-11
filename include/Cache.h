#ifndef CACHE_H_
#define CACHE_H_

#include <unordered_map>
#include <vector>
#include <string>

struct CacheData
{
private:
    std::vector<std::pair<std::string, int> > data_vec_;    
public:
    CacheData(const std::vector<std::pair<std::string, int> > &data_vec)
    {
        data_vec_ = data_vec;
    }
    CacheData():data_vec_(){}
    ~CacheData(){}
    std::vector<std::pair<std::string, int> > &getDataVec(){
    	return data_vec_;
    }
};

class Cache
{
public:
    Cache();
    ~Cache();
    /*
        when the client request word not found in cache , add the result to cache;
    */
    void addQueryResultToCache(const std::string &keyword, CacheData &data);
    /*
        search in cache first;
    */
    CacheData searchCacheData(const std::string &keyword);

    /*
    	when thread start , load the disk cache file to memory;
    */
    void loadCacheFileToMemory();

    /*after some specific time ,
     write cache data to disk file ,
     then every thread has the same cache*/
    void writeCacheToDisk();

    typedef std::unordered_map<std::string, CacheData> cache_map_type;

    cache_map_type &getCacheDataMap(){
    	return cache_map_;
    }

private:
    std::unordered_map<std::string, CacheData> cache_map_;
};

#endif


