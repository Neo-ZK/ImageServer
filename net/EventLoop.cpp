#include "EventLoop.h"

ZK_ImageServer::net::EventLoop::EventLoop():
currentActiveChannel_(NULL),
runningTag_(false),
quitTag_(false),
isEventHanding_(false),
pendingFuncList_(),
callPendingFuncTag_(false),
wakeupfd_(0),
mutex_(),
currentThreadID_(0),
epoll_(new Epoll(this))
{
    currentThreadID_ = CurrentThread::gettid();
    wakeupfd_ = eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
    if(wakeupfd_ < 0)
    {
        LOG_ERROR << "create eventfd error";
    }
    wakeupChannel_.reset(new Channel(this,wakeupfd_));
    wakeupChannel_->setReadCallBack(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->enableRead();
}

ZK_ImageServer::net::EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->removeFromLoop();
}

void ZK_ImageServer::net::EventLoop::updateChannel(Channel* channel)
{
    assert(isInLoopThread());
    assert(channel->getOwnerLoop() == this);
    epoll_->updateChannel(channel);
}

void ZK_ImageServer::net::EventLoop::removeChannel(Channel* channel)
{
    assert(isInLoopThread());
    assert(channel->getOwnerLoop() == this);
    if(isEventHanding_)
    {
        assert(currentActiveChannel_ == channel || std::find(activeChannel_.begin(),activeChannel_.end(),channel) == activeChannel_.end());
    }
    epoll_->removeChannel(channel);
}

void ZK_ImageServer::net::EventLoop::loop()
{
    assert(!runningTag_);
    assert(isInLoopThread());
    runningTag_ = true;
    LOG_TRACE<<"eventloop start, thread id is "<< CurrentThread::gettid();
    while(!quitTag_)
    {
        activeChannel_.clear();
        epoll_->process(0,&activeChannel_);
        size_t num = activeChannel_.size();
        if(num == 0)
        {
            //LOG_INFO<< "Nothing happen";
        }
        if(num > 0)
        {
            //LOG_TRACE << "events happend and has been filled into activechannel,size is "<< (int)num;
            isEventHanding_ = true;
            for(size_t i = 0;i < num;i++)
            {
                currentActiveChannel_ = activeChannel_[i];
                activeChannel_[i]->handleEvent();
            }
        }
        currentActiveChannel_ = NULL;
        isEventHanding_ = false;
        doPendingFunctor();
    }
    runningTag_ = false;
    LOG_INFO << "thread "<< currentThreadID_ <<" loop is quit";
}

void ZK_ImageServer::net::EventLoop::runInLoop(CallBackFun cb)
{
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        MutexGuard mutexGurad(mutex_);
        pendingFuncList_.push_back(cb);
        if(!isInLoopThread() || callPendingFuncTag_)
        {
            wakeup();
        }
    }
}

void ZK_ImageServer::net::EventLoop::wakeup()
{
    uint64_t temp = 1;
    ssize_t n = write(wakeupfd_,&temp,sizeof(temp));
    if(n != sizeof(temp))
    {
        LOG_ERROR << "did not write enough byte into wakeupfd_";
    }
}

void ZK_ImageServer::net::EventLoop::handleRead()
{
    uint64_t temp = 1;
    ssize_t n = read(wakeupfd_,&temp,sizeof(temp));
    if(n != sizeof(temp))
    {
        LOG_ERROR << "did not read enough byte from wakeupfd_";
    }
}

void ZK_ImageServer::net::EventLoop::doPendingFunctor()
{
    std::vector<PendingFunc> tempFunList;
    callPendingFuncTag_ = true;
    {
    MutexGuard mutexGurad(mutex_);
    tempFunList.swap(pendingFuncList_);
    }

    for(size_t i = 0;i < tempFunList.size();i++)
    {
        tempFunList[i]();
    }
    callPendingFuncTag_ = false;
}

void ZK_ImageServer::net::EventLoop::quit()
{
    quitTag_ = true;
    if(!isInLoopThread())
    {
        wakeup();
    }
}
