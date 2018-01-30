#include "EventLoopThreadPool.h"

ZK_ImageServer::net::EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop,int threadNum):
baseLoop_(baseLoop),
threadNum_(threadNum),
next_(0),
isStart_(false)
{
    //ctor
}

ZK_ImageServer::net::EventLoopThreadPool::~EventLoopThreadPool()
{
    //dtor
}

void  ZK_ImageServer::net::EventLoopThreadPool::start()
{
    assert(baseLoop_->isInLoopThread());
    isStart_ = true;
    for(int i = 0;i < threadNum_;i++)
    {
        EventLoopThreadPtr eventLoopThreadPtr;
        eventLoopThreadPtr.reset(new EventLoopThread());
        loops_.push_back(eventLoopThreadPtr->startLoop());
        eventLoopThreads_.push_back(std::move(eventLoopThreadPtr));
    }
}

ZK_ImageServer::net::EventLoop* ZK_ImageServer::net::EventLoopThreadPool::getNextLoop()
{
    assert(baseLoop_->isInLoopThread());
    EventLoop* loop = baseLoop_;
    if(!loops_.empty())
    {
        loop = loops_[next_];
        next_++;
        if(next_ == static_cast<int>(loops_.size()))
        {
            next_ = 0;
        }
    }
    return loop;
}
