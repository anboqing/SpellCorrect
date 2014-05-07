#include "Diction.h"
#include "Configure.h"
#include "Log.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;



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
        WRITE_STR(string(" load dict to map error "));
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
void cleanToken(char *);

void process(ifstream &ifs, map<string, int> &mp)
{
    string line;
    while (getline(ifs, line))
    {
        char *buf = new char[line.size() + 1];
        strcpy(buf, line.c_str());
        cleanToken(buf);
        string sline(buf);
        string word;
        istringstream sstr(sline);
        while (sstr >> word)
        {
            ++(mp[word]);
        }
    }
#ifndef NDEBUG
    WRITE_STR(string(" process completely"));
#endif
}

void Diction::traverseDir(const char *row_path, map<string, int> &mp)
{
    DIR *dp = opendir(row_path);
    if (NULL == dp)
    {
        throw std::runtime_error("cannot open directory");
        WRITE_STR(string("cannot open directory"));
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
            //处理函数
            ifstream ifs(entry->d_name);
            process(ifs, mp);
            ifs.close();
            traverseDir(entry->d_name, mp);
        }
        else
        {
            //处理函数
            ifstream ifs(entry->d_name);
            process(ifs, mp);
            ifs.close();
        }
    }
    chdir("..");
    closedir(dp);
}

void Diction::buildMapFromRow(const std::string &row_path)
{
    //traverse the directory and load every file
    //这里要添加遍历目录！！！
    map<string, int> mp;
    traverseDir(row_path.c_str(), mp);
    //write map to data
    Configure *conf = Configure::getInstance();
    string diction_path = conf->getConfigByName("diction_path");
    string home_path = conf->getConfigByName("home_path");
    ofstream ofs((home_path + diction_path).c_str());
    if (ofs.is_open())
    {
        for (map<string, int>::iterator iter = mp.begin(); iter != mp.end(); ++iter)
        {
            // #ifndef NDEBUG
            //             cout << iter->first << " --Diction buildMapFromRow " << __LINE__ << endl;
            // #endif
            ofs << iter->first << " " << iter->second << "\n";
        }
    }
    else
    {
        throw runtime_error("open output file error");
        WRITE_STR(string("open output file error"));
    }
    ofs.close();
}


void cleanToken(char *pattern)
{
    char *p = pattern;
    char *buf = new char[sizeof(char)*strlen(pattern) + 1];
    char ch, pre = ' ';
    int i = 0;
    while (*p != '\0')
    {
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9'))
        {
            if (*p >= 'A' && *p <= 'Z')
            {
                //大写换小写
                ch = *p + 32;
            }
            else
            {
                ch = *p;
            }
        }
        else if (*p < 0)
        {
            //考虑中文情况，第一个字节小于0,将连续2个字节全部置为空格
            *p = ' ';
            *(++p) = ' ';
        }
        else
        {
            ch = ' ';
        }
        if (!(ch == ' ' && pre == ' '))
        {
            buf[i++] = ch;
        }
        pre = ch;
        p++;
    }
    buf[i] = '\0';
    memset(pattern, 0, strlen(pattern)*sizeof(char));
    strcpy(pattern, buf);
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
            instance_ = new Diction(home_path+data_path);
        }
        lock_.unlock();
    }
    return instance_;
}