#include <unordered_map>

#include <string>
#include <iostream>

using namespace std;

//g++ unordered_map.cpp -std=gnu++11

int main(int argc, char **argv) {
	unordered_map<string, string> _hash; // C++11 standard

	_hash["hello"] = "test";
	_hash["你好"] = "是的";

	for (auto & x : _hash) {
		cout << x.first << " : " << x.second << endl;
	}

	for(auto x = _hash.begin(); x != _hash.end(); ++x){
		cout << x->first << endl;
	}

}

