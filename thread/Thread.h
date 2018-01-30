#ifndef THREAD_H
#define THREAD_H

#include<iostream>
#include<stdio.h>
#include<functional>
#include<string>
#include<pthread.h>
#include<atomic>
#include<stdio.h>
#include<assert.h>
#include"Mutex.h"
#include"CountDownLatch.h"
#include<sys/prctl.h>
namespace ZK_ImageServer
{

class Thread
{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc& func,const std::string& name = std::string());
    ~Thread();

    void start();
    int join();
    bool Isstart() const {return Isstart_;}
    pid_t gettid() const {return tid_;}
    std::string getname() const {return name_;}

protected:
private:
    void setDefaultName();

    bool        Isstart_;
    bool        Isjoin_;
    pthread_t   pthreadId_;
    pid_t       tid_;
    ThreadFunc  func_;
    std::string name_;
    CountDownLatch countDownlatch_;
    static std::atomic<int> numCreated_;

};

}//end namespace ZK_ImageServer


#endif // THREAD_H
