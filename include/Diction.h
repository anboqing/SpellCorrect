#ifndef DICTION_H_
#define DICTION_H_

#include <map>
#include <string>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <MutexLock.h>

class Diction
{
public:
	static Diction *getInstance();
    std::map<std::string, int>::iterator find(const std::string &keyword);
    std::map<std::string, int> &getDictMap()
    {
        return dict_map_;
    }
    void buildMapFromRow(const std::string &row_path);
private:
    Diction();
    Diction(const std::string &dict_path);
    ~Diction();
    void loadDictToMap(const std::string &dictName);
    void traverseDir(const char *row_path, std::map<std::string, int> &mp);
    std::map<std::string, int> dict_map_;
    static Diction *instance_;
    static MutexLock lock_;
};
#endif