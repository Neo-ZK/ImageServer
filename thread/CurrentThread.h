#ifndef CURRENTTHTREAD_H_INCLUDED
#define CURRENTTHTREAD_H_INCLUDED

#include<unistd.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<string.h>
namespace ZK_ImageServer
{

class CurrentThread
{
public:
    CurrentThread():t_cachedTid_(0),t_tidStringLength_(0),t_threadName_(NULL)
    {
        memset(t_tidString_,0,sizeof(t_tidString_));
        catchTid();
    }
    ~CurrentThread(){};

    void catchTid()
    {
        if(t_cachedTid_ == 0)
        {
            t_cachedTid_ = gettid();
            t_tidStringLength_ = snprintf(t_tidString_,sizeof(t_tidString_),"%5d",t_cachedTid_);
        }
    }

    int tid()
    {
        return t_cachedTid_;
    }

    char* tidString()
    {
        return t_tidString_;
    }

    int tidStringLength()
    {
        return t_tidStringLength_;
    }

    char* threadName()
    {
        return t_threadName_;
    }

    static pid_t gettid()
    {
        return static_cast<pid_t>(syscall(SYS_gettid));
    }

private:
    int t_cachedTid_;
    char t_tidString_[32];
    int t_tidStringLength_;
    char* t_threadName_;
};//end class CurrentThread


}//end namespace ZK_ImageServer*/


#endif // CURRENTTHTREAD_H_INCLUDED
