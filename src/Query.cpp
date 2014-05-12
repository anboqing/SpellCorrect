#include "Query.h"
#include "Diction.h"
#include "EditDistance.h"
#include "Configure.h"
#include "Log.h"
#include "Cache.h"
#include "Index.h"
#include "EncodingConverter.h"

#include <cstdlib>
#include <queue>
#include <iostream>

using namespace std;

Query *Query::instance_ = NULL;
MutexLock Query::lock_;

Query::Query() {}
Query::~Query() {}

Query *Query::getInstance()
{
    if (instance_ == NULL)
    {
        lock_.lock();
        if (instance_ == NULL)
        {
            instance_ = new Query();
        }
        lock_.unlock();
    }
    return instance_;
}


//search from cache
bool searchFromCache(const string &keyword, Cache &cache, CacheData &result)
{
#ifndef NDEBUG
    WRITE_STR(string("start searchFromCache -----------------"));
#endif
    Cache::cache_map_type &cache_map = cache.getCacheDataMap();

    Cache::cache_map_type::iterator iter;

    if ((iter = cache_map.find(keyword)) != cache_map.end())
    {
        result = (*iter).second;
        return true;
    }
    else
    {
        return false;
    }
}


//test
#ifndef NDEBUG
EncodingConverter converter;
#endif
set<Index::IndexVecType::size_type> &queryFromIndex(string &keyword,
        Index::IndexMapType &index_map,
        set<Index::IndexVecType::size_type> &allIndexes)
{
#ifndef NDEBUG
    WRITE_STR(string(" start find result in index "));
#endif
    Index *pindex = Index::getInstance();
    // split keyword into letters
    std::vector<string> letters = pindex->splitStringIntoLetters(keyword);
    // search in unorderd-map , find out every indexes in index_vec_;

    for (auto aLetter : letters)
    {
        // #ifndef NDEBUG
        //         string msg = aLetter;
        //         msg = converter.gbk_to_utf8(msg);
        //         WRITE_STR(msg);
        // #endif
        Index::IndexMapType::iterator iter = index_map.find(aLetter);
        if (iter != index_map.end())
        {
            //found!
            allIndexes.insert((*iter).second.begin(), (*iter).second.end());
        }
    }
    return allIndexes;
}

/*
    1st. query from the thread's cache
    2nd. if not found in cache, then query from the dictionary and add the query-result into cache-map
*/
vector<pair<string, int> > Query::getSimiliarWordsByStruct(string &keyword, Cache &cache)
{
    //load config message:
    Configure *pconf = Configure::getInstance();
    string mdis = pconf->getConfigByName("maxdistance");
    string s_max_diff_len = pconf->getConfigByName("max_diff_len");
    int maxdistance = atoi(mdis.c_str());

    Diction *pdict = Diction::getInstance();
    //load the WHOLE dictionary to memory
    map<string, int> diction_map = pdict->getDictMap();
    //  if we don't have index.dat , then add -DBUILD_INDEX in makefile,and run once
    // use priority_queue to build an small-root-heap to select the top k similar word
    priority_queue<Data> result_queue;

    //result will store in this vector;
    vector<pair<string, int> > result_vector;
    //===============================================================
    // search from cache firstly ...
    CacheData result_data;
    if (searchFromCache(keyword, cache, result_data) == true)
    {
#ifndef NDEBUG
        WRITE_STR(string(" find result in cache "));
#endif
        // if found in cache then add the result to vector
        vector<pair<string, int> > &res_vc = result_data.getDataVec();

        // c++11 support this usage ( like foreach() in Java !)
        for (auto iter : res_vc)
        {
            result_vector.push_back(iter);
        }

        return result_vector;
    }

#ifndef NDEBUG
    WRITE_STR(string(" not find result in cache ,start find in index "));
#endif
    //===============================================================
    //if not found in cache , then search in index :)
    // prepair the index;
    Index *pindex = Index::getInstance();
    Index::IndexMapType &index_map = pindex->getIndexMap();
    Index::IndexVecType &index_vec = pindex->getIndexVec();

    // get the counterpart words of the indexes;
    set<Index::IndexVecType::size_type> allIndexes;
    queryFromIndex(keyword, index_map, allIndexes);
#ifndef NDEBUG
    WRITE_STR(string(" start find word in index_vec "));
#endif
    // getEditDistance with keyword,put in priority_queue,select topk...
    // for (auto index : allIndexes)
    int dis = 0x11111111;

    for (set<Index::IndexVecType::size_type>::iterator iter = allIndexes.begin();
            iter != allIndexes.end(); ++iter)
    {
        string word = (index_vec[(*iter)]).first;
        if (word.size() != 0)
        {
            dis = getEditDistance(word, keyword);
        }
        //id ed < maxdistance then add the word into result
        if (dis < maxdistance)
        {
            Data data(dis, (index_vec[(*iter)]).second, (index_vec[(*iter)]).first);
            result_queue.push(data);
        }
    }
// #ifndef NDEBUG
//     string after(" after find word in index_vec , index_vec.size() is ");
//     priority_queue<Data>::size_type size = result_queue.size();
//     cout << after << size << endl;
// #endif

 // since we have Index,I don't need to query diction anymore!

    // #ifndef NDEBUG
    //     WRITE_STR(string(" not find result in index ,start find in diction? "));
    // #endif
    // for ( map<string, int>::iterator ix = diction_map.begin(); ix != diction_map.end(); ++ix)
    // {
    //     //find the edit Distance between keyword and each word in the map
    //     int difflen = abs(keyword.size() - (*ix).first.size());
    //     int dis = 0x11111111;
    //     if (difflen <= max_diff_len)
    //     {
    //         dis = getEditDistance((*ix).first, keyword);
    //     }
    //     else
    //     {
    //         continue;
    //     }
    //     //id ed < maxdistance then add the word into result
    //     if (dis < maxdistance)
    //     {
    //         Data data(dis, (*ix).second, (*ix).first);
    //         result_queue.push(data);
    //     }
    // }
    //==========================================================================
    //get configure of topk parameter
    
    string topk_str = pconf->getConfigByName("topk");
    int topk_int = atoi(topk_str.c_str());

    // select the top k ( ensured by priority_queue,it is a small-top heap ) most similar word to send to client
    CacheData cache_data;

    vector<pair<string, int> > &cache_data_vec = cache_data.getDataVec();

    while ( topk_int-- && (!result_queue.empty()))
    {
        Data data = result_queue.top();
        int ifrequency = data.frequency_;
        string sword = data.word_;
        // add the result to res vec
        pair<string, int> pr = make_pair(sword, ifrequency);
        result_vector.push_back(pr);
        // add to the cache->cachedata->data-map;
        cache_data_vec.push_back(pr);
        result_queue.pop();
    }
    // add query result to cache map
    cache.addQueryResultToCache(keyword, cache_data);
    return result_vector;
}


