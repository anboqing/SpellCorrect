#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "MutexLock.h"
#include "Condition.h"
#include "WorkThread.h"
#include "Task.h"
#include "NoneCopyable.h"
#include "MutexSafeGuarde.h"
#include "CacheManagerThread.h"

#include <vector>
#include <queue>

class ThreadPool : public NoneCopyable{
public:
    ThreadPool(std::vector<WorkThread>::size_type);
    ~ThreadPool();

    bool addTaskToQueue(Task);
    bool getTaskFromQueue(Task *);

    bool isPoolOpen()const;
    std::queue<Task>::size_type getTaskQueuSize()const;

    bool start();
    bool stop();
    std::vector<WorkThread>::size_type getThreadQuantity(){
        return total_thread_quantity_;
    }
private:

    std::vector<WorkThread>::size_type total_thread_quantity_;
    std::vector<WorkThread> thread_pool_;
    std::queue<Task> task_que_;
    /*
        mutalbe的中文意思是“可变的，易变的”，跟constant（既C++中的const）是反义词。
    　　 在C++中，mutable也是为了突破const的限制而设置的。被mutable修饰的变量，
        将永远处于可变的状态，即使在一个const函数中。
    */
    mutable MutexLock locker_;
    mutable MutexLock mutex_;
    mutable Condition cond_;
    bool is_pool_open_;

    // thread's cache_manager ;
    CacheManagerThread cache_manager_;

};

#endif

/*
    
*/
