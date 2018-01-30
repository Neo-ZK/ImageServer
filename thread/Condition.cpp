#include "Condition.h"

bool ZK_ImageServer::Condition::waitForSeconds(int seconds)
{
    struct timeval now;
    struct timespec outtime;
    gettimeofday(&now,NULL);
    outtime.tv_sec = now.tv_sec + seconds;
    outtime.tv_nsec = now.tv_usec*1000;
    Mutex::UnassignGuard ug(mutex_);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_,mutex_.getMutex(),&outtime);
}
