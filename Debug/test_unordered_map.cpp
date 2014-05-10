#include <unordered_map>
#include <map>
#include <string>
#include <iostream>

using namespace std;

struct MyData{
private:
	map<string,int> data_map_;
public:
	MyData(const std::map<std::string,int> &data_map){
		data_map_ = data_map;
	}
	MyData(){

	}
	~MyData(){

	}
};

class str_hash{
      public:
       size_t operator()(const string& str) const
        {
                unsigned long __h = 0;
                for (size_t i = 0 ; i < str.size() ; i ++)
                __h = 5*__h + str[i];
                return size_t(__h);
        }
};

int main(int argc, char const *argv[])
{
	unordered_map<string,MyData > hash_map_;
	string key("anboqing");
	map<string,int> mp;
	mp.insert(make_pair(string("age"),25));
	mp.insert(make_pair(string("birthday"),19880510));

	MyData data(mp);
	hash_map_.insert(make_pair(key,mp));
	cout << (*(hash_map_.begin())).first<<endl;
	return 0;
}