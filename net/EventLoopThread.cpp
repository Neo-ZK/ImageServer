#include "EventLoopThread.h"



ZK_ImageServer::net::EventLoopThread::EventLoopThread():
mutex_(),
thread_(std::bind(&EventLoopThread::threadFun,this)),
condition_(mutex_),
loop_(NULL)
{
    //ctor
}

ZK_ImageServer::net::EventLoopThread::~EventLoopThread()
{
    //dtor
}

void ZK_ImageServer::net::EventLoopThread::threadFun()
{
    EventLoop eventLoop;
    {
        MutexGuard mutexGuard(mutex_);
        loop_ = &eventLoop;
        condition_.notifyall();
    }
    loop_->loop();
    loop_ = NULL;
}

ZK_ImageServer::net::EventLoop* ZK_ImageServer::net::EventLoopThread::startLoop()
{
    thread_.start();
    {
        MutexGuard mutexGuard(mutex_);
        while(loop_ == NULL)
        {
            condition_.wait();
        }
    }
    return loop_;
}
