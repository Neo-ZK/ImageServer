#include "Channel.h"

 const int ZK_ImageServer::net::Channel::NoneEvent = 0;
 const int ZK_ImageServer::net::Channel::ReadEvent = EPOLLIN;
 const int ZK_ImageServer::net::Channel::WriteEvent = EPOLLOUT;

ZK_ImageServer::net::Channel::Channel(EventLoop* loop,int fd):
event_(0),
revent_(0),
fd_(fd),
isEventHanding_(false),
index_(-1),
isAddedToLoop_(false),
ownerLoop_(loop),
readCB_(NULL),
writeCB_(NULL),
closeCB_(NULL),
errorCB_(NULL),
tied_(false)
{
    //ctor
}

ZK_ImageServer::net::Channel::~Channel()
{

}

void ZK_ImageServer::net::Channel::update()
{
    isAddedToLoop_ = true;
    ownerLoop_->updateChannel(this);
}

void ZK_ImageServer::net::Channel::removeFromLoop()
{
    isAddedToLoop_ = false;
    assert(event_ == NoneEvent);
    ownerLoop_->removeChannel(this);
}

void ZK_ImageServer::net::Channel::handleEvent()
{
    isEventHanding_ = true;
    //LOG_TRACE<<"Channel begin to handleEvent,revent num is "<< revent_;
    if(revent_ & EPOLLIN)
    {
        LOG_TRACE<<"call read callback ";
        if(readCB_)
        {
            LOG_TRACE<<"there is a readcallback ,call it";
            readCB_();
        }
    }

    if(revent_ & EPOLLOUT)
    {
        if(writeCB_)
        {
            writeCB_();
        }
    }

    if((revent_ & EPOLLHUP)&&!(revent_ & EPOLLIN))
    {
        if(closeCB_)
        {
            closeCB_();
        }
    }

    if(revent_ & EPOLLERR)
    {
        if(errorCB_)
        {
            errorCB_();
        }
        LOG_ERROR << "error happen in Epoll event";
    }
    isEventHanding_ = false;
}

const char* ZK_ImageServer::net::Channel::getEventString()
{
    switch(event_)
    {
        case 0:
            return "NoneEvent";
        case EPOLLIN:
            return "ReadEvent";
        case EPOLLOUT:
            return "WriteEvent";
        case EPOLLHUP:
            return "HubEvent";
        default:
            return "unknow event";
    }
}
