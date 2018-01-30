#include "Thread.h"

namespace ZK_ImageServer
{
class ThreadData
{
   typedef ZK_ImageServer::Thread::ThreadFunc ThreadFunc;
public:
    ThreadData(const ThreadFunc& func,const std::string& name,pid_t* tid,CountDownLatch* countDownlatch)
    :func_(func),
     name_(name),
     tid_(tid),
     countDownLatch_(countDownlatch)
    {};

    void runThread()
    {
        *tid_ = static_cast<pid_t>(syscall(SYS_gettid));
        tid_ = NULL;
        countDownLatch_->countDown();
        countDownLatch_ = NULL;

        std::string temp_name = name_.empty()?"ZengkeThread":name_;
        prctl(PR_SET_NAME,temp_name.c_str());

        try
        {
            func_();
        }
        catch(const std::exception& ex)
        {
            temp_name = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", temp_name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch(...)
        {
            temp_name = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", temp_name.c_str());
            throw; // rethrow
        }
    }

private:
    ThreadFunc   func_;
    std::string  name_;
    pid_t*        tid_;
    CountDownLatch*  countDownLatch_;


};

void* startThread(void* obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runThread();
    delete data;
    return NULL;
}


}//end namespace ZK_ImageServer




using namespace ZK_ImageServer;
std::atomic<int> Thread::numCreated_(0);
void Thread::setDefaultName()
{
    if(name_.empty())
    {
        char buf[32];
        int num = numCreated_;
        snprintf(buf,sizeof(buf),"Thread%d",num);
        name_ = buf;
    }
}

Thread::Thread(const ThreadFunc& func,const std::string& name)
:Isstart_(false),
 Isjoin_(false),
 pthreadId_(0),
 tid_(0),
 func_(func),
 name_(name),
 countDownlatch_(1)
{
    setDefaultName();
}


Thread::~Thread()
{
    if(Isstart_ && !Isjoin_)
    {
        pthread_detach(pthreadId_);
    }
}


void Thread::start()
{
    assert(!Isstart_);
    Isstart_ = true;
    ThreadData* data = new ThreadData(func_,name_,&tid_,&countDownlatch_);
    if(pthread_create(&pthreadId_,NULL,startThread,data) == 0)
    {
        countDownlatch_.wait();
        assert(tid_ > 0);
    }
    else
    {
        Isstart_ = false;
        delete data;
    }
}

int Thread::join()
{
    assert(Isstart_);
    assert(!Isjoin_);
    Isjoin_ = true;
    return pthread_join(pthreadId_,NULL);
}
