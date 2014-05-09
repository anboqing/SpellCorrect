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

#include "MixSegment.hpp"
#include "EncodingConverter.h"

class Diction
{
public:
    static Diction *getInstance();
    std::map<std::string, int> &getDictMap()
    {
        return dict_map_;
    }
    void buildDictFromRow();

private:
    Diction(const std::string &path,
            const std::string &dict_path,
            const std::string &model_path);
    ~Diction();
    void loadDictToMap(const std::string &dictName);
    void readFileToMap(const char *filename);
    void buildExcludeSet();
    void traverseDir(const char *row_path);
    std::ofstream &open_file(std::ofstream &os, const char *filename);
    std::ifstream &open_file(std::ifstream &is, const char *filename);

    std::map<std::string, int> dict_map_;
    CppJieba::MixSegment segementor_;
    std::set<std::string> exclude_set_;

    static Diction *instance_;
    static MutexLock lock_;
};
#endif