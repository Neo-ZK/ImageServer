#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <functional>
#include "../thread/Thread.h"
#include "../thread/Condition.h"
#include "../thread/Mutex.h"
#include "EventLoop.h"

namespace ZK_ImageServer
{
namespace net
{


class EventLoopThread
{
    public:
        EventLoopThread();
        ~EventLoopThread();

         EventLoop* startLoop();


    private:
        void threadFun();

        Mutex         mutex_;
        Thread        thread_;
        Condition     condition_;
        EventLoop*    loop_;


};

}//end namesapce net
}//end namespace Zengke
#endif // EVENTLOOPTHREAD_H
