#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <sys/epoll.h>
#include <memory>
#include <string>
#include "EventLoop.h"
#include "../log/Logger.h"

namespace ZK_ImageServer
{
namespace net
{
class EventLoop;

class Channel
{
    typedef std::function<void()> CallBackFun;
    public:
        Channel(EventLoop* loop,int fd);
        ~Channel();
        //set call back function
        void setReadCallBack(CallBackFun cb){readCB_ = cb;}
        void setWriteCallBack(CallBackFun cb){writeCB_ = cb;}
        void setCloseCallBack(CallBackFun cb){closeCB_ = cb;}
        void setErrorCallBack(CallBackFun cb){errorCB_ = cb;}
        //update channel
        void update();
        void removeFromLoop();
        void enableRead(){event_ = ReadEvent;update();}
        void enableWrite(){event_ = WriteEvent;update();}
        void setRevent(int ev){revent_ = ev;}
        void disableWriting(){event_ &= ~WriteEvent;}
        void disableAll(){event_ = NoneEvent;update();}

        //handle event
        void handleEvent();
        //void handleEventWithGuard();
        EventLoop* getOwnerLoop(){return ownerLoop_;}
        void setIndex(int index){index_ = index;}
        int getIndex(){return index_;}
        int getFd(){return fd_;}
        int getEvent(){return event_;}
        bool isNoneEvent(){return event_ == NoneEvent;}
        bool isWriting(){return event_ == WriteEvent;}
        bool isReading(){return event_ == ReadEvent;}

        const char* getEventString();

        void tie(std::shared_ptr<void>& obj){tie_ = obj;tied_ = true;}

    protected:
    private:
        int event_;//event is used to record current event in this channel
        int revent_; //revent is used as a event reuse,to receive the event type of epoll
        int fd_;
        bool isEventHanding_;//reflect channel whether is hadling event
        int  index_;//use to determine whether this channel is added to eventloop

        bool isAddedToLoop_;

        static const int NoneEvent;
        static const int ReadEvent;
        static const int WriteEvent;
        //std::weak_ptr<void> tied_;

        EventLoop* ownerLoop_;

        CallBackFun readCB_;
        CallBackFun writeCB_;
        CallBackFun closeCB_;
        CallBackFun errorCB_;

        std::weak_ptr<void> tie_;
        bool tied_;

};
}//end namespace net
}//end namespace ZK_ImageServer
#endif // CHANNEL_H
