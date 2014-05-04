/*************************************************************************
	> File Name: Loader.cpp
	> Author: Anboqing
	> Mail:750145240@qq.com 
	> Created Time: 2014年05月04日 星期日 11时13分30秒
 ************************************************************************/
#include "../include/Loader.h"
#include <string.h>
#include <sstream>
#include <iostream>

using namespace std;

Loader::Loader(vector<string> *p):p_word_vector_(p)
{

}

Loader::~Loader(){
	p_word_vector_=NULL;
}

void Loader::loadFileToVector(string filename){
	ifstream ifs;
	//这里要添加遍历目录！！！
	ifs.open(filename.c_str());
	string line;
	while(getline(ifs,line)){
		char *buf = new char[line.size()+1];
		strcpy(buf,line.c_str());
		cleanToken(buf);
		string sline(buf);
		string word;
		istringstream sstr(sline);
		while(sstr >> word){
			p_word_vector_->push_back(word);
		}
	}
}

void Loader::cleanToken(char *pattern){
    char *p = pattern;
	char *buf = new char[sizeof(char)*strlen(pattern)+1];
    char ch,pre = ' ';
    int i = 0;
    while(*p!='\0'){
        if((*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||(*p>='0'&&*p<='9')){
            if(*p>='A'&&*p<='Z'){
				//大写换小写
                ch = *p+32;
            }else{
                ch = *p;
            }
        }else if(*p<0){
//考虑中文情况，第一个字节小于0,将连续2个字节全部置为空格
			*p = ' ';
			*(++p) = ' ';
		}else{
            ch = ' ';
        }
		if(!(ch==' '&&pre==' ')){
			buf[i++] = ch;
		}
		pre = ch;
		p++;
    }
	buf[i] = '\0';
	memset(pattern,0,strlen(pattern)*sizeof(char));
	strcpy(pattern,buf);
#ifndef NDEBUG
	cout << "pattern after process is : " << pattern << endl;
#endif
}
