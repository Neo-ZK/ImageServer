#ifndef ECHO_H
#define ECHO_H

#include <functional>
#include "../net/TcpServer.h"
#include "../net/EventLoop.h"


namespace ZK_ImageServer
{
namespace server
{
class Echo
{
    public:
        Echo(ZK_ImageServer::net::EventLoop* baseLoop);
        ~Echo();
    protected:
    private:
        ZK_ImageServer::net::EventLoop*    baseLoop_;
        void connectionCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn);
        void messageCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn,
                                ZK_ImageServer::net::NetBuffer* buffer);

        ZK_ImageServer::net::TcpServer tcpServer_;

        static const int defaultThreadNum_;

};
}//end namespace server
}//end namespace ZK_ImageServer
#endif // ECHO_H
