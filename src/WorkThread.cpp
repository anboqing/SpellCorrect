
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <queue>
#include <stdlib.h>

#include "Task.h"
#include "stdio.h"
#include "Diction.h"
#include "EditDistance.h"
#include "WorkThread.h"
#include "Configure.h"
#include "ThreadPool.h"
#include "Log.h"


#include "json/json.h"
using namespace std;

vector<pair<string,int> > getFamiliarWords(const string &keyword)
{
    Configure *conf = Configure::getInstance();
    string diction_path = conf->getConfigByName("diction_path");
    string home_path = conf->getConfigByName("home_path");
    string path = home_path + diction_path;

    Diction dict(path.c_str());
    //load dictionary to memory
    map<string, int> diction_map = dict.getDictMap();

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
    vector<pair<string,int> > result_vector;
        //get configure of topk parameter
    Configure *pconf = Configure::getInstance();
    string topk_str = pconf->getConfigByName("topk");
    int topk_int = atoi(topk_str.c_str());
    //save result into vector 
    while ( topk_int-- && (!result_queue.empty()))
    { 
        //result_queue.top().first is distance( type int) ,result_queue.top().second is map<word,frequency>; 
        map<string, int> mpp = result_queue.top().second;

        int ifrequency = (*(mpp.begin())).second;
        string sword = (*(mpp.begin())).first;
        //make pair<string,int> and push into vector < pair<string,int> >
        result_vector.push_back(make_pair(sword, ifrequency));
        result_queue.pop();
    }
    return result_vector;
}

string json_string(string keyword)
{
    //get edit distance
    Json::Value root ;
    Json::Value arr ;
    vector<pair<string,int> > res_vec = getFamiliarWords(keyword);
    for (vector<pair<string,int> >::iterator iter = res_vec.begin(); iter != res_vec.end(); ++iter)
    {
        //serilize result into json;
        Json::Value elem ;
        elem["title"] = (*iter).first;
        elem["content"] = (*iter).second ;
        arr.append(elem);
    }
    root["files"] = arr;
    Json::FastWriter writer ;
    Json::StyledWriter stlwriter ;
    return stlwriter.write(root);
}


void WorkThread::run()
{
#ifndef NDEBUG
    string str( "thread start id is : ");
    Log::APEND_NUM(str, get_tid());
#endif
    while (true)
    {
        Task t;
        p_pool_->getTaskFromQueue(&t);
        string result = json_string(t.expression_);
        int ret = sendto(t.server_fd_, result.c_str(), result.size(), 0, (struct sockaddr *)&t.address_, t.len_);
        if (ret == -1)
        {
            throw std::runtime_error("sendto error");
        }
        sleep(1);
    }
}

bool WorkThread::regeditThreadPool(ThreadPool *p_pool)
{
    if (!p_pool)
    {
        throw runtime_error("can not regeditThreadPool ");
        return false;
    }
    p_pool_ = p_pool;
    return true;
}