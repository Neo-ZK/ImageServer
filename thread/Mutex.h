#ifndef MUTEX_H
#define MUTEX_H

#include<pthread.h>
#include<assert.h>
#include"CurrentThread.h"
#include<unistd.h>
#include<sys/syscall.h>
//#include"Thread.h"

#define CHECK(ret) (assert((ret) == 0))

namespace ZK_ImageServer
{

class Mutex
{
public:
    Mutex():currentThread_(),holder_(0) {CHECK(pthread_mutex_init(&mutex_,NULL));}
    ~Mutex() {assert(holder_ == 0);CHECK(pthread_mutex_destroy(&mutex_));}
    void lock()
    {
        CHECK(pthread_mutex_lock(&mutex_));
        assignholder();
    }

    void unlock()
    {
        CHECK(pthread_mutex_unlock(&mutex_));
        unassignholder();
    }

    bool IsLockedbyThisThread()
    {
        return holder_ == currentThread_.gettid();
    }

    pthread_mutex_t* getMutex()
    {
        return &mutex_;
    }
private:
    friend class Condition;
    class UnassignGuard
    {
    public:
        UnassignGuard(Mutex& mutex):mutex_(mutex)
        {
            mutex_.unassignholder();
        }
        ~UnassignGuard()
        {
            mutex_.assignholder();
        }
    private:
        Mutex& mutex_;
    };

    void assignholder()
    {
        holder_ = currentThread_.gettid();
    }

    void unassignholder()
    {
        holder_ = 0;
    }

    CurrentThread   currentThread_;
    pthread_mutex_t mutex_;
    pid_t           holder_;
};//end class Mutex

class MutexGuard
{
public:
    explicit MutexGuard(Mutex& mutex):mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexGuard()
    {
        mutex_.unlock();
    }
private:
    Mutex& mutex_;
};


}//end namespace ZK_ImageServer
#endif // MUTEX_H
