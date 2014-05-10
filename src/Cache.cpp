#include "Cache.h"
#include "Configure.h"
#include "Log.h"

#include <sstream>
#include <fstream>

using namespace std;

Cache::Cache(): cache_map_()
{

}
Cache::~Cache() {}
/*
    when the client request word not found in cache , add the result to cache;
*/
void Cache::addResultToCatchMap(std::string &keyword, CacheData &data)
{
    pair<string, CacheData> data_pair = make_pair(keyword, data);
    cache_map_.insert(data_pair);
}
/*
    search in cache first;
*/
CacheData Cache::searchCacheData(const string &keyword)
{
    cache_map_type::iterator result ;
    if ((result = cache_map_.find(keyword)) != cache_map_.end())
    {
        return (*result).second;
    }
    else
    {
        return CacheData();
    }
}

/*
    when thread start , load the disk cache file to memory;
*/
void Cache::loadCacheFileToMemory()
{
#ifndef NDEBUG
	WRITE_STR(string("thread's cache read disk cache file to it's cache_map_"));
#endif
    Configure *pconf = Configure::getInstance();
    string cache_path = pconf->getConfigByName("cache_file_path");
    string home_path = pconf->getConfigByName("home_path");
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
        map<string, int> mp;
        while (istr >> pairword)
        {
            istr >> frequency;
            mp.insert(make_pair(pairword, frequency));
        }
        CacheData data(mp);
        cache_map_.insert(make_pair(keyword, data));
    }
    ifs.close();
}

/*after some specific time ,
 write cache data to disk file ,
 then every thread has the same cache*/
void Cache::writeCacheToDisk()
{
    Configure *pconf = Configure::getInstance();
    string cache_path = pconf->getConfigByName("cache_file_path");
    string home_path = pconf->getConfigByName("home_path");
    ofstream ofs((home_path + cache_path).c_str());
    //file lock;
    if (!(ofs.is_open()))
    {
        throw runtime_error("open cache_path file");
    }
    for (Cache::cache_map_type::iterator iter = cache_map_.begin() ; iter != cache_map_.end(); ++iter)
    {
        ofs << (*iter).first<< "\t";
        map<string, int> mp = (*iter).second.getDataMap();
        for (map<string, int>::iterator it = mp.begin(); it != mp.end(); ++it)
        {
            ofs << (*it).first <<"\t"<< (*it).second;
        }
        ofs << "\n";
    }
    ofs.close();
}