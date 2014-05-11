#include "Index.h"
#include "Configure.h"
#include "Diction.h"
#include "Log.h"

#include <fstream>
#include <stdexcept>
#include <cstring>
#include <sstream>


using namespace std;

Index *Index::instance_ = NULL;

MutexLock Index::lock_;

Index *Index::getInstance()
{
    if (instance_ == NULL)
    {
        lock_.lock();
        if (instance_ == NULL)
        {
            instance_ = new Index();
        }
        lock_.unlock();
    }
    return instance_;
}

Index::Index(): index_vec_(), index_map_()
{
    // initial index;
    loadIndexVecFromDicttion();
    loadIndexMapFromIndexFile();
}

Index::~Index()
{
    delete instance_;
    instance_ = NULL;
}

void Index::loadIndexMapFromIndexFile()
{
#ifndef NDEBUG
    WRITE_STR(string("start loadIndexMapFromIndexFile--"));
#endif
    Configure *pconf = Configure::getInstance();
    string home_path = pconf->getConfigByName("home_path");
    string index_path = pconf->getConfigByName("index_path");
    ifstream ifs((home_path + index_path).c_str());
    if (!(ifs.is_open()))
    {
        throw runtime_error("can not open index file");
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream issm(line);
        string keyword;
        issm >> keyword;
        set<IndexVecType::size_type> index_set;
        IndexVecType::size_type index;
        while (issm >> index)
        {
            index_set.insert(index);
        }
        index_map_.insert(make_pair(keyword, index_set));
    }
}


void Index::loadIndexVecFromDicttion()
{
#ifndef NDEBUG
    WRITE_STR(string("start loadIndexVecFromDicttion--"));
#endif
    //load the diction-data from global dictionany
    Diction *pdict = Diction::getInstance();
    map<string, int> dict_map = pdict->getDictMap();
    for (auto iter : dict_map)
    {
        index_vec_.push_back(iter);
    }
}

void Index::buildIndexFromDiction()
{
#ifndef NDEBUG
    WRITE_STR(string("start buildIndexFromDiction--"));
#endif
    loadIndexVecFromDicttion();
    // traverse the index_vec_ and build index;
    for (IndexVecType::size_type ix = 0 ; ix != index_vec_.size(); ++ix)
    {
        string word = (index_vec_[ix]).first;
        // split it into one letter by one letter
        vector<string> word_letters;
        string::size_type len = word.size();
        char *buf = new char[len + 1];
        strcpy(buf, word.c_str());
        for (string::size_type i = 0; i < len + 1 ;)
        {
            if (buf[i] < 0)
            {
                // is gbk
                char *let = new char[3];
                let[0] = buf[i++];
                let[1] = buf[i++];
                let[2] = '\0';
                // insert the letter and it's index in vector into index_map
                string keyword(let);
                delete[] let;
                // find out wether the index map contains this keywrod or not
                // if it contains this keyword then add the index-in-vec into set
                (index_map_[keyword]).insert(ix);
            }else{
                // is ASCII letter
                char *let = new char[2];
                let[0] = buf[i++];
                let[1] = '\0';
                string keyword(let);
                (index_map_[keyword]).insert(ix);
                delete[] let;
            }
        }
    }
    writeIndexMapToIndexFile();
}

void Index::writeIndexMapToIndexFile()
{
#ifndef NDEBUG
    WRITE_STR(string("start writeIndexMapToIndexFile--"));
#endif
    Configure *pconf = Configure::getInstance();
    string home_path = pconf->getConfigByName("home_path");
    string index_path = pconf->getConfigByName("index_path");
    ofstream ofs((home_path + index_path).c_str());
    if (!(ofs.is_open()))
    {
        throw runtime_error("can not open index file");
    }
    for (auto iter : index_map_)
    {
        ofs << iter.first << "\t";
        for (auto it_set : iter.second)
        {
            ofs << it_set << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}
