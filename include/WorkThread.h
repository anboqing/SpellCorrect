
#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include "Thread.h"

class ThreadPool;

class WorkThread : public Thread
{
public:
    void run();
    int compute(char *);
    bool regeditThreadPool(ThreadPool *p_pool);
private:
    ThreadPool *p_pool_;
};

#endif
