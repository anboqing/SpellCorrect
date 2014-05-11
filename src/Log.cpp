#include "Log.h"
#include "Configure.h"
#include <iostream>
using namespace std;

Log::Log(): writer_()
{
    Configure *conf = Configure::getInstance();
    string log_path = conf->getConfigByName("log_path");
    string home_path = conf->getConfigByName("home_path");
    string path = home_path+log_path;
    writer_.open(path.c_str());
    if (!(writer_.is_open()))
    {
        throw runtime_error("open log writer fail");
    }
}

Log::~Log()
{
    writer_.close();
}

Log *Log::loger_ = NULL;
MutexLock Log::locker_;
Log *Log::getInstance()
{
    if (loger_ == NULL)
    {
        locker_.lock();
        if (loger_ == NULL)
        {
            loger_ = new Log();
        }
        locker_.unlock();
    }
    return loger_;
}

void Log::writeLog(const std::string &log_msg,
                   const char *file,
                   int line ,
                   const char *date,
                   const char *timer ,
                   const char *func)
{
    //consider the condition of muti-thread!!!
    locker_.lock();
    writer_ << "[TIME]:"<<timer<< "  ";
    writer_ << log_msg;
    // writer_ << "   [FUNCTION]: "<< func;
    // writer_ << " [FILE] "<<file;
    // writer_ << " [LINE] " << line << " ";
    writer_ << endl;
    locker_.unlock();
}