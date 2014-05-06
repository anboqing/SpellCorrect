#include "Configure.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	Configure *conf = Configure::getInstance();
	cout << conf->getConfigByName("ip")<<endl;
	cout << conf->getConfigByName("row_home")<<endl;
	cout << conf->getConfigByName("a")<<endl;
	return 0;
}
