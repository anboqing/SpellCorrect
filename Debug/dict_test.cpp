#include "Diction.h"
#include <string>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    Diction dict("../data/dict.dat");
    map<string, int> mp = dict.getDictMap();
    for( map<string,int>::iterator ix = mp.begin(); ix != mp.end(); ++ix)
    {
        cout << (*ix).first << " " << (*ix).second << endl;
    }
    return 0;
}