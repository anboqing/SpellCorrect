#include "Configure.h"
#include <fstream>

using namespace std;

Configure* Configure::config_ = NULL;
MutexLock Configure::lock_;

Configure *Configure::getInstance(){
	if(config_==NULL){
		lock_.lock();
		if(config_==NULL){
			config_ = new Configure();
		}
		lock_.unlock();
	}
	return config_;
}

Configure::Configure():configure_map_() {
	ifstream ifs("/home/anboqing/workspace/SpellCorrection/conf/confighome.dat");
	if(!(ifs.is_open())){
		throw runtime_error("read config file home  --Configure.cpp");
	}
	string path;
	if(ifs>>path)
		loadConfigureToMap(path);
	else
		throw runtime_error("read config file home  --Configure.cpp");
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