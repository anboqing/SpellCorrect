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



Diction::Diction(const string &path): dict_map_()
{
    loadDictToMap(path);
}

Diction::Diction() {}
Diction::~Diction() {}

void Diction::loadDictToMap(const string &path)
{
    ifstream ifs(path.c_str());
    if (!ifs.is_open())
    {
        throw runtime_error("load dict to map");
        // WRITE_STR(string(" load dict to map error "));
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
    }
#ifndef NDEBUG
    WRITE_STR(string("loadDictToMap completely"));
#endif
}
void cleanToken(string &line)
{
    char *buf = new char[line.size()+1];
    // strcpy(buf,line.c_str());
    int i = 0;
    for(string::size_type ix = 0;ix!=line.size();++ix){
        if(!isascii(line[ix])){
            buf[i++] = line[ix];
        }
    }
    line = string(buf);
    delete[] buf;
}

void process(ifstream &ifs, map<string, int> &mp,
             MixSegment &segementor,
             EncodingConverter &converter,
             set<string> &exclude_set
            )
{
    vector<string> words;
    string line;
    while (getline(ifs, line))
    {
        cleanToken(line);
        words.clear();
        // line = converter.gbk_to_utf8(line);
        segementor.cut(line, words);
        for (vector<string>::iterator iter = words.begin();
                iter != words.end(); ++iter)
        {
            if (!exclude_set.count((*iter)))
            {
                ++(mp[(*iter)]);
            }
        }
    }
#ifndef NDEBUG
    WRITE_STR(string(" process completely"));
#endif
}

void traverseDir(const char *row_path,
                 map<string, int> &mp,
                 MixSegment &segementor,
                 EncodingConverter &converter,
                 set<string> &exclude_set)
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
            traverseDir(entry->d_name, mp, segementor, converter,exclude_set);
        }
        else
        {
            //处理函数
            ifstream ifs(entry->d_name);
            process(ifs, mp, segementor, converter,exclude_set);
            ifs.close();
        }
    }
    chdir("..");
    closedir(dp);
}

void Diction::buildDictFromRow(const std::string &row_path)
{
    Configure *conf = Configure::getInstance();
    string home_path = conf->getConfigByName("home_path");

    string dict_path = conf->getConfigByName("mp_gbk_path");
    string model_path = conf->getConfigByName("hmm_gbk_path");
    dict_path = home_path + dict_path;
    model_path = home_path + model_path;

    MixSegment segementor(dict_path.c_str(), model_path.c_str());

    EncodingConverter converter;
    map<string, int> mp;

    set<string> exclude_set;
    string exclude_path = conf->getConfigByName("exclude_path");
    ifstream readExclude((home_path + exclude_path).c_str());
    if (!(readExclude.is_open()))
    {
        WRITE_STR(string("open exclude_path failed!"));
        throw runtime_error("open exclude_path failed");
    }
    string ex_token;
    while (readExclude >> ex_token)
    {
        exclude_set.insert(ex_token);
    }
    readExclude.close();

    //traverse the directory and load every file:word to map
    traverseDir(row_path.c_str(), mp, segementor, converter, exclude_set);

    string diction_path = conf->getConfigByName("diction_path");
    //open output file
    ofstream ofs((home_path + diction_path).c_str());

    if (ofs.is_open())
    {
        //write map to data
        for (map<string, int>::iterator iter = mp.begin(); iter != mp.end(); ++iter)
        {
            ofs << iter->first << " " << iter->second << "\n";
        }
    }
    else
    {
        WRITE_STR(string("open output file error"));
        throw runtime_error("open output file error");
    }
    ofs.close();
}

map<string, int>::iterator Diction::find(const string &keyword)
{
    return dict_map_.find(keyword);
}

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
            instance_ = new Diction(home_path + data_path);
        }
        lock_.unlock();
    }
    return instance_;
}
// void cleanToken(char *pattern)
// {
//     char *p = pattern;
//     char *buf = new char[sizeof(char)*strlen(pattern) + 1];
//     char ch, pre = ' ';
//     int i = 0;
//     while (*p != '\0')
//     {
//         if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9'))
//         {
//             if (*p >= 'A' && *p <= 'Z')
//             {
//                 //大写换小写
//                 ch = *p + 32;
//             }
//             else
//             {
//                 ch = *p;
//             }
//         }
//         else if (*p < 0)
//         {
//             //考虑中文情况，第一个字节小于0,将连续2个字节全部置为空格
//             *p = ' ';
//             *(++p) = ' ';
//         }
//         else
//         {
//             ch = ' ';
//         }
//         if (!(ch == ' ' && pre == ' '))
//         {
//             buf[i++] = ch;
//         }
//         pre = ch;
//         p++;
//     }
//     buf[i] = '\0';
//     memset(pattern, 0, strlen(pattern)*sizeof(char));
//     strcpy(pattern, buf);
// }
