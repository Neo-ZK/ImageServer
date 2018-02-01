#include "Epoll.h"

const int ChannelNew = -1; //a new channel
const int ChannelAdded = 1; //the channel has been added into eventloop
const int ChanneDeleted = 2;//the channel has been deleted from eventloop

ZK_ImageServer::net::Epoll::Epoll(EventLoop* ownerLoop):
epollfd_(epoll_create1(EPOLL_CLOEXEC)),
epoll_events_(minEpollEventSize),
channelMap_(),
ownerLoop_(ownerLoop)
{
    //ctor
}

ZK_ImageServer::net::Epoll::~Epoll()
{
    //dtor
}

void ZK_ImageServer::net::Epoll::updateChannel(Channel* channel)
{
    int index = channel->getIndex();
    int fd = channel->getFd();
    if(index == ChannelNew || index == ChanneDeleted)
    {
        if(index == ChannelNew)
        {
            assert(channelMap_.find(fd) == channelMap_.end());
            channelMap_[fd] = channel;
        }
        else
        {
            assert(channelMap_.find(fd) != channelMap_.end());
            assert(channelMap_[fd] == channel);
        }
        channel->setIndex(ChannelAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else if(index == ChannelAdded)
    {
        assert(channelMap_.find(fd) != channelMap_.end());
        assert(channelMap_[fd] == channel);
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->setIndex(ChanneDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }

    }

}

void ZK_ImageServer::net::Epoll::removeChannel(Channel* channel)
{
    int index = channel->getIndex();
    int fd = channel->getFd();
    assert(index == ChannelAdded);
    assert(channelMap_.find(fd) != channelMap_.end());
    assert(channelMap_[fd] == channel);
    assert(channel->isNoneEvent());
    int n = channelMap_.erase(fd);
    assert(n == 1);
    update(EPOLL_CTL_DEL,channel);
    channel->setIndex(ChannelNew);
}


void ZK_ImageServer::net::Epoll::update(int operation,Channel* channel)
{
    struct epoll_event event;
    event.events = channel->getEvent();
    event.data.ptr = channel;
    int fd = channel->getFd();
    LOG_INFO<<"epoll operation "<<operationToString(operation)<<" of fd: "<<fd <<" Channel event is: "<<channel->getEventString();
    if(epoll_ctl(epollfd_,operation,fd,&event) < 0)
    {
        LOG_ERROR << "epoll ctl error,operation is "<< operationToString(operation) <<" fd is: "<<fd;
    }
}



void ZK_ImageServer::net::Epoll::process(int timeOutMs,ChannelList* activeChannel)
{
    //LOG_INFO<<"fd total size is "<<static_cast<int>(channelMap_.size());
    int eventNum = epoll_wait(epollfd_,&*epoll_events_.begin(),static_cast<int>(epoll_events_.size()),timeOutMs);
    if(eventNum == 0)
    {
        //LOG_INFO << "Nothing happen in epoll";
    }
    else if(eventNum > 0)
    {
        //LOG_TRACE << "events happen,events num is "<< eventNum;
        fillActiveChannel(eventNum,activeChannel);
        if(eventNum == static_cast<int>(epoll_events_.size()))
        {
            epoll_events_.resize(epoll_events_.size()*2);
        }
    }
    else
    {
        int errNUM = errno;
        LOG_ERROR << "Error happen in epoll,eventNum is " << eventNum<<" errno is "<<errNUM;

    }
}

void ZK_ImageServer::net::Epoll::fillActiveChannel(int eventNum,ChannelList* activeChannel)
{
    for(int i = 0;i < eventNum;i++)
    {
        Channel* channel = static_cast<Channel*>(epoll_events_[i].data.ptr);
        int fd = channel->getFd();
        assert(channelMap_.find(fd) != channelMap_.end());
        channel->setRevent(channel->getEvent());
        activeChannel->push_back(channel);
    }
}

const char* ZK_ImageServer::net::Epoll::operationToString(int operation)
{
    switch(operation)
    {
        case EPOLL_CTL_ADD:
            return "EPOLL ADD";
        case EPOLL_CTL_MOD:
            return "EPOLL MOD";
        case EPOLL_CTL_DEL:
            return "EPOLL DEL";
        default:
            return "Unknow operation";
    }
}
