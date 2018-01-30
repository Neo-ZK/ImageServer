#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include <vector>
#include <memory>
#include <assert.h>
#include "EventLoopThread.h"
namespace ZK_ImageServer
{
namespace net
{

class EventLoopThreadPool
{
    typedef std::unique_ptr<EventLoopThread> EventLoopThreadPtr;
    public:
        EventLoopThreadPool(EventLoop* baseLoop,int threadNum);
        ~EventLoopThreadPool();

        EventLoop* getNextLoop();
        void start();

    protected:
    private:
        EventLoop*  baseLoop_;
        int         threadNum_;
        int         next_;
        bool        isStart_;
        std::vector<EventLoopThreadPtr> eventLoopThreads_;
        std::vector<EventLoop*> loops_;

};
}//end namesapce net
}//end namespace Zengke
#endif // EVENTLOOPTHREADPOOL_H
