#include "MixSegment.hpp"
#include "EncodingConverter.h"

#include <fstream>

const char *const dict_path = "../dict/jieba.dict.utf8";
const char *const model_path = "../dict/hmm_model.utf8";
/*
## 分词
### jieba.dict.utf8/gbk
作为最大概率法(MPSegment: Max Probability)分词所使用的词典。
### hmm_model.utf8/gbk
作为隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典。
__对于MixSegment(混合MPSegment和HMMSegment两者)则同时使用以上两个词典__
*/
using namespace CppJieba;

int main(int argc, char const *argv[])
{
	MixSegment segementor(dict_path,model_path);
	EncodingConverter converter;

	vector<string> words;
	map<string,int> wmp;

	ifstream ifs("in.txt");
	string line;

	while(getline(ifs,line)){
		words.clear();
		line = converter.gbk_to_utf8(line);
		segementor.cut(line,words);
		// print(words);
		// save to map;
		for(vector<string>::iterator iter = words.begin();iter!=words.end();++iter){
			++(wmp[(*iter)]);
		}
	}
	ifs.close();
	//save map to file
	ofstream ofs;
	ofs.open("mydick.dat");
	
	for(map<string,int>::iterator iter = wmp.begin();iter!=wmp.end();++iter){
		ofs << (*iter).first << " " << (*iter).second<< "\n";
	}

	ofs.close();

	return 0;
}