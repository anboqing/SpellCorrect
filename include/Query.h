#ifndef QUERY_H_
#define QUERY_H_

#include <vector>
#include <string>

#include "MutexLock.h"
#include "Cache.h"

class Data
{
public:
    int distance_;
    int frequency_;
    std::string word_;
    Data() {}
    Data(int dis, int fre, std::string word): distance_(dis), frequency_(fre), word_(word)
    {
    }
    ~Data() {}
    friend bool operator<(const Data &left, const Data &right)
    {
        if (left.distance_ != right.distance_)
        {
            return left.distance_ > right.distance_;
        }
        else
        {
            return left.frequency_ < right.frequency_;
        }
    }
};

class Query
{
private:
    static MutexLock lock_;
    static Query *instance_;
    Query();
    ~Query();
public:
    static Query *getInstance();
    /*
    1st. query from the thread's cache
    2nd. if not found in cache, then query from the dictionary and add the query-result into cache-map
    */
    std::vector<std::pair<std::string, int> > getSimiliarWordsByStruct(std::string &keyword,Cache &cache);
};


#endif