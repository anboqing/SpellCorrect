#include "ThreadPool.h"

ThreadPool::ThreadPool(std::vector<WorkThread>::size_type num):
    total_thread_quantity_(num),
    thread_pool_(total_thread_quantity_),
    task_que_(),
    locker_(),
    mutex_(),
    cond_(&locker_),
    is_pool_open_(false)
{
    //regedit all the work thread;
    for (std::vector<WorkThread>::iterator iter = thread_pool_.begin();
            iter != thread_pool_.end(); iter++)
    {
        iter -> regeditThreadPool(this);
    }
}
ThreadPool::~ThreadPool()
{
    stop();
}

bool ThreadPool::start()
{
    if (isPoolOpen())
    {
        return true;
    }
    is_pool_open_ = true;
    for (std::vector<WorkThread>::iterator iter = thread_pool_.begin();
            iter != thread_pool_.end(); iter++)
    {
        iter->start();
    }
    return true;
}
bool ThreadPool::stop()
{
    if (!isPoolOpen()) //already stopped
    {
        return true;
    }

    is_pool_open_ = false;

    cond_.notify_all();

    for (std::vector<WorkThread>::iterator iter = thread_pool_.begin();
            iter != thread_pool_.end(); iter++)
    {
        iter->join();
    }
    while(getTaskQueuSize()!=0){
        task_que_.pop();
    }
    return true;
}

bool ThreadPool::addTaskToQueue(Task new_task){
    // locker_.lock();
    MutexSafeGuarde g(locker_);
    int count = 5;
    while (!isPoolOpen()&&count--)
    {
        start();
        sleep(1);
    }
    if (count<=0)
    {
        // locker_.unlock();
        return false;
    }
    task_que_.push(new_task);
    cond_.notify();
    // locker_.unlock();
    return true;
}

bool ThreadPool::getTaskFromQueue(Task *task){
    // locker_.lock();
    MutexSafeGuarde g(locker_);
    while(isPoolOpen() && getTaskQueuSize()==0){
        cond_.wait();
    }

    if(!isPoolOpen()){
        locker_.unlock();
        return false;
    }

    *task = task_que_.front();
    task_que_.pop();

    // locker_.unlock();
    return true;
}

bool ThreadPool::isPoolOpen()const
{
    // mutex_.lock();
    MutexSafeGuarde g(mutex_);
    bool ret = is_pool_open_;
    // mutex_.unlock();
    // if other thread change the is_pool_open here ..... what will hapen?
    return ret;
}

std::queue<Task>::size_type ThreadPool::getTaskQueuSize()const
{
    // mutex_.lock();
    MutexSafeGuarde g(mutex_);
    std::queue<Task>::size_type size = task_que_.size();
    // mutex_.unlock();
    // if other thread change the task_que_.size() here ..... what will hapen?
    return size;
}
