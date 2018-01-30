#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include"Condition.h"
#include"Mutex.h"

namespace ZK_ImageServer
{

class CountDownLatch
{
    public:
        CountDownLatch(int countNum);
        ~CountDownLatch();
        void countDown();
        void wait();
        int getCount();
    protected:
    private:
        int         count_;
        Mutex       mutex_;
        Condition   cond_;

};

}//end namespace ZK_ImageServer

#endif // COUNTDOWNLATCH_H
