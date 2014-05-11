#ifndef INDEX_H_
#define INDEX_H_


#include <vector>
#include <unordered_map>
#include <string>
#include <set>

#include "MutexLock.h"

// 索引用来缩小搜索范围
class Index
{
public:
    static Index *getInstance();
    // vector < word,frequency >
    typedef std::vector<std::pair<std::string, int> > IndexVecType;
    // hash_map < letter, set < index in the vector > >
    typedef std::unordered_map<std::string, std::set<IndexVecType::size_type> > IndexMapType;

    void buildIndexFromDiction();

private:

    Index();

    ~Index();
    // this func used for constructor to call
    void loadIndexMapFromIndexFile();

    void loadIndexVecFromDicttion();

    void writeIndexMapToIndexFile();

    IndexVecType index_vec_;

    IndexMapType index_map_;

    static MutexLock lock_;

    static Index *instance_;
};

#endif