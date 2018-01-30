#ifndef CONDITION_H
#define CONDITION_H

#include"Mutex.h"
#include<sys/time.h>
#include<errno.h>

namespace ZK_ImageServer
{

class Condition
{
    public:
        explicit Condition(Mutex& mutex):mutex_(mutex)
        {
            CHECK(pthread_cond_init(&cond_,NULL));
        }
        ~Condition()
        {
            CHECK(pthread_cond_destroy(&cond_));
        }
        void wait()
        {
            CHECK(pthread_cond_wait(&cond_,mutex_.getMutex()));
        }
        void notifyone()
        {
            CHECK(pthread_cond_signal(&cond_));
        }
        void notifyall()
        {
            CHECK(pthread_cond_broadcast(&cond_));
        }
        bool waitForSeconds(int seconds);
    protected:
    private:
        pthread_cond_t  cond_;
        Mutex&          mutex_;
};

}//end namespace ZK_ImageServer

#endif // CONDITION_H
