#include "CountDownLatch.h"

using namespace ZK_ImageServer;
CountDownLatch::CountDownLatch(int countNum):count_(countNum),mutex_(),cond_(mutex_)
{

}

CountDownLatch::~CountDownLatch()
{

}

void CountDownLatch::countDown()
{
    MutexGuard mutexGuard(mutex_);
    if(--count_ == 0)
    {
        cond_.notifyall();
    }
}

void CountDownLatch::wait()
{
    MutexGuard mutexGuard(mutex_);
    while(count_ > 0)
    {
        cond_.wait();
    }
}

int CountDownLatch::getCount()
{
    MutexGuard mutexGuard(mutex_);
    return count_;
}
