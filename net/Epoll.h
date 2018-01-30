#ifndef EPOLL_H
#define EPOLL_H

#include "EventLoop.h"
#include "Channel.h"
#include <vector>
#include <map>
#include <assert.h>
#include <sys/epoll.h>
#include <string>
#include <errno.h>

namespace ZK_ImageServer
{
namespace net
{
class EventLoop;
class Channel;
class Epoll
{
    typedef std::vector<Channel*> ChannelList;
    typedef std::map<int,Channel*> ChannelMap;
    public:
        Epoll(EventLoop* ownerLoop);
        ~Epoll();
        void updateChannel(Channel* channel);
        void update(int operation,Channel* channel);
        void removeChannel(Channel* channel);//just remove channel from epoll and channnelmap_(or can say move from eventloop)
        void process(int timeOutMs,ChannelList* activeChannel);
        void fillActiveChannel(int eventNum,ChannelList* activeChannel);
        const char* operationToString(int operation);
    protected:
    private:
        int epollfd_;
        std::vector<struct epoll_event> epoll_events_;
        ChannelMap channelMap_;
        EventLoop* ownerLoop_;
        static const int minEpollEventSize = 16;


};
}//end namespace net
}//end namespace ZK_ImageServer
#endif // EPOLL_H
