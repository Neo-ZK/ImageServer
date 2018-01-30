#include "Echo.h"


const int ZK_ImageServer::server::Echo::defaultThreadNum_ = 2;

ZK_ImageServer::server::Echo::Echo(ZK_ImageServer::net::EventLoop* baseLoop):
baseLoop_(baseLoop),
tcpServer_(std::bind(&Echo::connectionCallBackFun,this,std::placeholders::_1),
           std::bind(&Echo::messageCallBackFun,this,std::placeholders::_1,std::placeholders::_2),
           defaultThreadNum_,
           baseLoop_)
{
    LOG_TRACE<<"echo server has established";
}

ZK_ImageServer::server::Echo::~Echo()
{
    //dtor
}

void ZK_ImageServer::server::Echo::connectionCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn)
{
    LOG_TRACE << "message has arrived,connectionCallBackFun has been called";
}

void ZK_ImageServer::server::Echo::messageCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn,ZK_ImageServer
        ::net::NetBuffer* buffer)
{
    LOG_TRACE << "messageCallBackFun has been called";
    std::string temp = buffer->retrieveAsString(buffer->readableByte());
    conn->sendInLoop(&*temp.begin(),temp.size());
}
