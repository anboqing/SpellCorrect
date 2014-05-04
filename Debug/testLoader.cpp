/*************************************************************************
	> File Name: testLoader.cpp
	> Author: Anboqing
	> Mail:750145240@qq.com 
	> Created Time: 2014年05月04日 星期日 14时57分45秒
 ************************************************************************/

#include "../include/Loader.h"
#include <iostream>
using namespace std;

int main(int argc,char **argv){

	vector<string> vec;

	Loader loader(&vec);

	loader.loadFileToVector("../data/big.txt");

	return 0;
}

