#ifndef LOG_H_
#define LOG_H_
#include "MutexLock.h"
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string>


class Log
{
public:
    static Log *getInstance();
    void writeLog(const std::string &log_msg,
                  const char *file = __FILE__,
                  int line = __LINE__,
                  const char *date = __DATE__,
                  const char *timer = __TIME__,
                  const char *func = __PRETTY_FUNCTION__
                 );
    static void WRITE_LOG(const std::string &str)
    {
        Log *p_log = Log::getInstance();
        p_log->writeLog(str, __FILE__, __LINE__, __DATE__, __TIME__, __PRETTY_FUNCTION__);
    }
    static void APEND_NUM(std::string &str, int n)
    {
        char *buf = new char[20];
        sprintf(buf, "%d", n);
        std::string len(buf);
        str += len;
        delete[] buf;
        WRITE_LOG(str);
    }

private:
    Log();
    virtual ~Log();
    static Log *loger_;
    static MutexLock locker_;
    std::ofstream writer_;
};
#define WRITE_LOGG(str) {Log *p_log = Log::getInstance();p_log->writeLog(str,__FILE__,__LINE__,__DATE__,__TIME__,__PRETTY_FUNCTION__);}

#endif