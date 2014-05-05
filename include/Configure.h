#ifndef CONFIGURE_H_
#define CONFIGURE_H_
#include <map>
#include <string>
class Configure
{
public:
    Configure(const std::string &path);
    virtual ~Configure();
    //get Config by name
    std::string getConfigByName(const std::string &name);
private:

    //load config
    void loadConfigureToMap(const std::string &path);
    std::map<std::string, std::string> configure_map_;
};
#endif