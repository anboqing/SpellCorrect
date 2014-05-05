#include "Configure.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	Configure conf("../conf/config.dat");
	cout << conf.getConfigByName("ip")<<endl;
	cout << conf.getConfigByName("port")<<endl;
	cout << conf.getConfigByName("a")<<endl;
	return 0;
}