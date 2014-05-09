#include "Diction.h"
#include "Configure.h"
#include "Log.h"
#include "MixSegment.hpp"
#include "EncodingConverter.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <string>

using namespace CppJieba;

Diction *Diction::instance_ = NULL;
MutexLock Diction::lock_;

Diction *Diction::getInstance()
{
    if (instance_ == NULL)
    {
        lock_.lock();
        if (instance_ == NULL)
        {
            Configure *pconf = Configure::getInstance();
            string home_path = pconf->getConfigByName("home_path");
            string data_path = pconf->getConfigByName("data_path");

            string model_path = pconf->getConfigByName("hmm_gbk_path");
            string dict_path = pconf->getConfigByName("mp_gbk_path");

            dict_path = home_path+dict_path;
            model_path = home_path + model_path;

            instance_ = new Diction(home_path + data_path,dict_path,model_path);
        }
        lock_.unlock();
    }
    return instance_;
}


Diction::Diction(const string &path, const string &dict_path, const string &model_path):
    dict_map_(), segementor_(dict_path.c_str(), model_path.c_str()),exclude_set_()
{
    loadDictToMap(path);
    // buildExcludeSet();
}

Diction::~Diction() {}

void Diction::loadDictToMap(const string &path)
{
    ifstream ifs(path.c_str());
    if (!ifs.is_open())
    {
        WRITE_STR(string(" load gbk dict to map error "));
        throw runtime_error("load dict to map");
    }
    string line;
    while (getline(ifs, line))
    {
        string word;
        int frequency;
        istringstream issm(line);
        issm >> word;
        issm >> frequency;
        dict_map_[word] = frequency;
        // issm.close();
    }
}



void Diction::buildExcludeSet(){
    Configure *pconf = Configure::getInstance();
    string home_path = pconf->getConfigByName("home_path");
    string exclude_path = pconf->getConfigByName("exclude_path");
    string path = home_path+exclude_path;
    ifstream ifs;
    if(!open_file(ifs,path.c_str())){
        WRITE_STR("open file error");
        throw runtime_error("open file error");
    }
    string token;
    while(ifs>>token){
        exclude_set_.insert(token);
    }
    ifs.close();
}

void Diction::buildDictFromRow()
{
    buildExcludeSet();
    Configure *pconf = Configure::getInstance();
    string home_path = pconf->getConfigByName("home_path");
    string row_path = pconf->getConfigByName("row_path");
    // traverse row directory and read every file into dict_map_;
    traverseDir((home_path + row_path).c_str());
    // save dict_map_ to file mydict.dat;
    ofstream ofs;
    string mydict_path = pconf->getConfigByName("mydict_path");
    if(!open_file(ofs,(home_path+mydict_path).c_str())){
        WRITE_STR("can't open mydict_path");
        throw runtime_error("cannot open mydict_path");
    }

    for(map<string,int>::iterator iter = dict_map_.begin();
        iter!=dict_map_.end();++iter){
        ofs << (*iter).first << " " << (*iter).second <<"\n";
    }

    ofs.close();

}

void Diction::traverseDir(const char *row_path)
{

    DIR *dp = opendir(row_path);
    if (NULL == dp)
    {
        WRITE_STR(string("cannot open directory"));
        throw std::runtime_error("cannot open directory");
    }
    chdir(row_path);
    struct dirent *entry;
    struct stat statbuf;

    while ((entry = readdir(dp)) != NULL)
    {
        stat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            traverseDir(entry->d_name);
        }
        else
        {
            readFileToMap(entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}

void Diction::readFileToMap(const char *filename)
{
    //open file and read it to dict_map_;
    ifstream ifs;
    if (!open_file(ifs, filename))
    {
        WRITE_STR("open file error");
        throw runtime_error("open file error");
    }
    vector<string> words;
    string line;
    while (ifs >> line)
    {
        //clean the tokens
        // for (string::size_type ix = 0 ; ix != line.size(); ++ix)
        // {
        //     if (isascii(line[ix]))
        //     {
        //         line[ix] = ' ';
        //     }
        // }
        words.clear();
        segementor_.cut(line,words);
        for(vector<string>::iterator iter = words.begin();iter!=words.end();++iter){
            if(!exclude_set_.count((*iter)))
            {
                ++(dict_map_[(*iter)]);
            }
        }
    }
}

ifstream &Diction::open_file(ifstream &is, const char *filename)
{
    is.close();
    is.clear();
    is.open(filename);
    return is;
}

ofstream &Diction::open_file(ofstream &os, const char *filename)
{
    os.close();
    os.clear();
    os.open(filename);
    return os;
}
