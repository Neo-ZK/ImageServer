#include "AsyncLogger.h"
using namespace ZK_ImageServer;
AsyncLogger::AsyncLogger(const char* filename,size_t rollsize,int flushinteval):
    fileName_(filename),
    rollSize_(rollsize),
    flushInterval_(flushinteval),
    mutex_(),
    thread_(std::bind(&AsyncLogger::threadFun,this),"Logging"),
    cond_(mutex_),
    currentBuf_(new Buffer),
    nextBuf_(new Buffer),
    bufferVector_(),
    runningTag_(false)
{
    bufferVector_.reserve(16);
}

AsyncLogger::~AsyncLogger()
{
    //bufferVector_.push_back(std::move(currentBuf_));
    //cond_.notifyall();
    //usleep(1000);
    if(runningTag_)
    {
        stop();
    }
}
//background process Buffer and write data into file
void AsyncLogger::threadFun()
{
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferPtrVector bufferVectorToWrite;
    LogFile output(fileName_.c_str(),rollSize_);

    while(runningTag_)
    {
        {
            MutexGuard mutexGurad(mutex_);
            if(bufferVector_.empty())
            {
                cond_.waitForSeconds(flushInterval_);
            }
            if(currentBuf_)
            {
                bufferVector_.push_back(std::move(currentBuf_));
            }
            currentBuf_ = std::move(newBuffer1);
            if(!nextBuf_)
            {
                nextBuf_ = std::move(newBuffer2);
            }
            bufferVectorToWrite.swap(bufferVector_);
        }

        assert(!bufferVectorToWrite.empty());

        for(size_t i = 0; i < bufferVectorToWrite.size();i++)
        {
            output.append(bufferVectorToWrite[i]->data(),bufferVectorToWrite[i]->length());
        }

        if(!newBuffer1)
        {
            assert(!bufferVectorToWrite.empty());
            newBuffer1 = std::move(bufferVectorToWrite.back());
            bufferVectorToWrite.pop_back();
            newBuffer1->refresh();
        }

        if(!newBuffer2)
        {
            assert(!bufferVectorToWrite.empty());
            newBuffer2 = std::move(bufferVectorToWrite.back());
            bufferVectorToWrite.pop_back();
            newBuffer2->refresh();
        }

        bufferVectorToWrite.clear();
        //output.flush();
    }

}

//foreground write data
void AsyncLogger::append(const char* line,int len)
{
    MutexGuard mutexGurad(mutex_);
    if(currentBuf_->avil() > len)
    {
        currentBuf_->append(line,len);
    }
    else
    {
        bufferVector_.push_back(std::move(currentBuf_));
        if(nextBuf_)
        {
            currentBuf_ = std::move(nextBuf_);
        }
        else
        {
            currentBuf_.reset(new Buffer);
        }
        currentBuf_->append(line,len);
        cond_.notifyall();
    }

}


void AsyncLogger::start()
{
    runningTag_ = true;
    thread_.start();
}

void AsyncLogger::stop()
{
   runningTag_ = false;
   cond_.notifyall();
   thread_.join();
}
