#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>          // std::vector
#include <functional>    // std::greater
#include <map>
#include <string>
#include <utility>

using namespace std;
int main ()
{
    // int myints[] = {10, 60, 50, 20};
    map<string, int> mp;
    mp.insert(make_pair(string("a"), 10));
    mp.insert(make_pair(string("ab"), 101));
    mp.insert(make_pair(string("aq"), 1053));
    mp.insert(make_pair(string("asd"), 105));
    mp.insert(make_pair(string("aasdf"), 310));
    mp.insert(make_pair(string("afsadf"), 110));
    mp.insert(make_pair(string("a2d"), 130));

    pair<int, map<string, int> > pr(1, mp);
    pair<int, map<string, int> > pr2(2, mp);
    pair<int, map<string, int> > pr4(9, mp);
    pair<int, map<string, int> > pr3(3, mp);
    // pair<int, map<string, int> > pr1(1, ip);
    // pair<int, map<string, int> > pr2(2, ip);
    // pair<int, map<string, int> > pr3(3, ip);

    std::priority_queue<pair<int, map<string, int> > > intPQueue1;
    std::priority_queue<pair<int, map<string, int> >,
        std::vector<pair<int, map<string, int> > >,
        std::greater<pair<int, map<string, int> > > > intPQueue2 ;

    intPQueue2.push(pr);
    intPQueue2.push(pr2);
    intPQueue2.push(pr4);
    intPQueue2.push(pr3);

    cout << intPQueue2.size() << endl;

    while ( !intPQueue2.empty() )
    {
        pair<int, map<string, int> > pvalue = intPQueue2.top();
        std::cout << pvalue.first << " ";
        // std::cout << (*(pvalue.second.begin())).first << " ";
        intPQueue2.pop();
    }
    // std::cout << "less than: " << std::endl;
    // while ( !intPQueue1.empty() )
    // {
    //     int pvalue = intPQueue1.top();
    //     std::cout << pvalue << " ";
    //     intPQueue1.pop();
    // }
    // std::cout << std::endl;

    // std::cout << "bigger than: " << std::endl;
    // while ( !intPQueue2.empty() )
    // {
    //     int pvalue = intPQueue2.top();
    //     std::cout << pvalue << " ";
    //     intPQueue2.pop();
    // }
    // std::cout << std::endl;

    return 0;
}