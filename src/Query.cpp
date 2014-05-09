#include "Query.h"
#include "Diction.h"
#include "EditDistance.h"
#include "Configure.h"
#include "Log.h"


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


vector<pair<string, int> > Query::getFamiliarWordsByStruct(const string &keyword)
{
    Diction *pdict = Diction::getInstance();
    //load dictionary to memory
    map<string, int> diction_map = pdict->getDictMap();

    priority_queue<Data> result_queue;
    Configure *pconf = Configure::getInstance();
    string mdis = pconf->getConfigByName("maxdistance");
    string s_max_diff_len = pconf->getConfigByName("max_diff_len");
    int maxdistance = atoi(mdis.c_str());
    int max_diff_len = atoi(s_max_diff_len.c_str());
    for ( map<string, int>::iterator ix = diction_map.begin(); ix != diction_map.end(); ++ix)
    {
        //find the edit Distance between keyword and each word in the map
        unsigned int difflen = abs(keyword.size() - (*ix).first.size());
        int dis = 0x11111111;
        if (difflen <= max_diff_len)
        {
            dis = getEditDistance((*ix).first, keyword);
        }
        else
        {
            continue;
        }
        //id ed < maxdistance then add the word into result
        if (dis < maxdistance)
        {
            Data data(dis, (*ix).second, (*ix).first);
            result_queue.push(data);
        }
    }

#ifndef NDEBUG
    cout << "result_queue size is : " << result_queue.size() << endl;
#endif

    vector<pair<string, int> > result_vector;
    //get configure of topk parameter
    string topk_str = pconf->getConfigByName("topk");
    int topk_int = atoi(topk_str.c_str());

    while ( topk_int-- && (!result_queue.empty()))
    {
        Data data = result_queue.top();
        int ifrequency = data.frequency_;
        string sword = data.word_;

        result_vector.push_back(make_pair(sword, ifrequency));
        result_queue.pop();
    }
#ifndef NDEBUG
    cout << "result_vector size is : " << result_vector.size() << endl;
#endif
    return result_vector;
}