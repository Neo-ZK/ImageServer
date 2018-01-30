#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <functional>
#include <sys/socket.h>
#include "EventLoop.h"
#include "Channel.h"
#include "NetBuffer.h"
namespace ZK_ImageServer
{
namespace net
{

class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{

    enum STATE{Disconnected,Disconnecting,Connecting,Connected};
    public:
        typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
        typedef std::function<void(TcpConnectionPtr)> CallBackFun;
        typedef std::function<void(TcpConnectionPtr,NetBuffer*)> MessageCallBackFun;

        TcpConnection(EventLoop* ownerLoop,int fd);
        ~TcpConnection();
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();

        void setNewConnectionCB(CallBackFun newConnectionCB){newConnectionCB_ = newConnectionCB;}
        void setMessageCallBackFun(MessageCallBackFun messageRecvCB){messageRecvCB_ = messageRecvCB;}

        void connEstablished();
        void connDestroyed();

        void sendInLoop(void* data,int len);

        void setState(int state){state_ = state;}
    protected:
    private:
        int                 connfd_;
        int                 state_;
        EventLoop*          ownerLoop_;
        std::unique_ptr<Channel> channel_;
        CallBackFun         newConnectionCB_;
        MessageCallBackFun  messageRecvCB_;
        CallBackFun         closeCB_;
        NetBuffer           inputBuffer_;
        NetBuffer           outputBuffer_;



};
}//end namesapce net
}//end namespace Zengke
#endif // TCPCONNECTION_H
