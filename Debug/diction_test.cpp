#include "Diction.h"
#include "Configure.h"
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	Diction *pdict = Diction::getInstance();
	pdict->buildDictFromRow();
	return 0;
}
