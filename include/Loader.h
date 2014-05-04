/*************************************************************************
	> File Name: Loader.h
	> Author: Anboqing
	> Mail:750145240@qq.com 
	> Created Time: 2014年05月04日 星期日 11时05分28秒
 ************************************************************************/
#ifndef LOADER_H_
#define LOADER_H_
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
//单词从配置文件读入到vector
class Loader{
private:
	std::vector<std::string> *p_word_vector_;//全局vector的指针	
public:
	Loader(std::vector<std::string> *p);
	~Loader();
	void loadFileToVector(std::string filename);
	void cleanToken(char *line);
};
#endif
