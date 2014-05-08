#include "Diction.h"
#include "Configure.h"
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	Diction *pdict = Diction::getInstance();
	Configure *pconf = Configure::getInstance();
	string home_path = pconf->getConfigByName("home_path");
	string row_path = pconf->getConfigByName("row_path");
	pdict->buildDictFromRow(home_path+row_path);
	return 0;
}