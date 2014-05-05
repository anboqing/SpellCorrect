#include "Configure.h"
#include <fstream>

using namespace std;

Configure::Configure(const string &path):configure_map_() {
	loadConfigureToMap(path);
}

Configure::~Configure() {

}

//load config
void Configure::loadConfigureToMap(const string &path){
	ifstream ifs(path.c_str());
	string key,value;
	while(ifs>>key){
		ifs>>value;
		configure_map_[key] = value;
	}
}
//get Config by name
string Configure::getConfigByName(const string &name){
	map<string,string>::iterator value;
	if((value = configure_map_.find(name))!=configure_map_.end()){
		return value->second;
	}else{
		return string("not_found");
	}
}