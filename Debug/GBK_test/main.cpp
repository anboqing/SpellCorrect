#include <hash_map>

#include <string>
#include <iostream>
#include <utility>

using namespace std;

//g++ main.cpp -Wno-deprecated

namespace __gnu_cxx {
template<> struct hash<std::string> {
	std::size_t operator()(const std::string &s) const {
		hash<const char *> h;
		return h(s.c_str());
	}
};
}

int main(int argc, char **argv) {
	__gnu_cxx ::hash_map<string, string, __gnu_cxx ::hash<std::string> > hash;
	hash.insert(make_pair(string("hello"), string("test")));
	cout << hash["hello"] << endl;
}
