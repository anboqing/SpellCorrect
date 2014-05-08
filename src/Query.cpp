#include "Query.h"
#include "Diction.h"
#include "EditDistance.h"
#include "Configure.h"
#include "Log.h"


#include <queue>

using namespace std;

Query *Query::instance_ = NULL;
MutexLock Query::lock_;

Query::Query(){}
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

    for ( map<string, int>::iterator ix = diction_map.begin(); ix != diction_map.end(); ++ix)
    {
        //find the edit Distance between keyword and each word in the map

        int dis = getEditDistance((*ix).first, keyword);
        
        //id ed < maxdistance then add the word into result
        Configure *pconf = Configure::getInstance();
        string mdis = pconf->getConfigByName("maxdistance");
        int maxdistance = atoi(mdis.c_str());
        if (dis < maxdistance)
        {
            Data data(dis, (*ix).second, (*ix).first);
            result_queue.push(data);
        }
    }

    vector<pair<string, int> > result_vector;
    //get configure of topk parameter
    Configure *pconf = Configure::getInstance();
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
    return result_vector;
}


vector<pair<string, int> > Query::getFamiliarWords(const string &keyword)
{
    //first version 20140506;
    Diction *pdict = Diction::getInstance();
    //load dictionary to memory
    map<string, int> diction_map = pdict->getDictMap();

    //define a priority queue to save:
    //  pair < distance , map < word , frequency > >
    std::priority_queue<pair<int, map<string, int> >,
        std::vector<pair<int, map<string, int> > >,
        std::greater<pair<int, map<string, int> > > > result_queue ;

    for ( map<string, int>::iterator ix = diction_map.begin(); ix != diction_map.end(); ++ix)
    {
        //find the edit Distance between keyword and each word in the map
        int dis = getEditDistance((*ix).first, keyword);
        //id ed < maxdistance then add the word into result
        Configure *pconf = Configure::getInstance();
        string mdis = pconf->getConfigByName("maxdistance");
        int maxdistance = atoi(mdis.c_str());
        if (dis < maxdistance)
        {
            map<string, int> temp;
            temp.insert(*ix);
            pair<int, map<string, int> > dis_pair(dis, temp);
            result_queue.push(dis_pair);
        }
    }
    //select the top k word into a vector<pair<word,frequency> > and return it;
    vector<pair<string, int> > result_vector;
    //get configure of topk parameter
    Configure *pconf = Configure::getInstance();
    string topk_str = pconf->getConfigByName("topk");
    int topk_int = atoi(topk_str.c_str());
    //save result into vector
    while ( topk_int-- && (!result_queue.empty()))
    {
        //result_queue.top().first is distance( type int) ,result_queue.top().second is map<word,frequency>;
        map<string, int> mpp = result_queue.top().second;
        //sort result by the dis and frequency : run to here the distance is small --> large
        int ifrequency = (*(mpp.begin())).second;
        string sword = (*(mpp.begin())).first;
        //make pair<string,int> and push into vector < pair<string,int> >
        result_vector.push_back(make_pair(sword, ifrequency));
        result_queue.pop();
    }
    return result_vector;
}