#include "TcpServer.h"

ZK_ImageServer::net::TcpServer::TcpServer(CallBackFun newConnCB,MessageCallBackFun messageCB,int threadNum,EventLoop* baseLoop):
baseLoop_(baseLoop),
newConnCB_(newConnCB),
messageCB_(messageCB),
acceptor_(new Acceptor(baseLoop_)),
eventLoopThreadPool_(new EventLoopThreadPool(baseLoop_,threadNum)),
connectionMap_()
{
    acceptor_->setNewConnectionCallBack(std::bind(&TcpServer::newConnection,this,std::placeholders::_1));
    eventLoopThreadPool_->start();
    baseLoop_->runInLoop(std::bind(&Acceptor::startListen,&*acceptor_));
    LOG_TRACE<<"Tcp Server has established, start listen";
}

ZK_ImageServer::net::TcpServer::~TcpServer()
{
    LOG_INFO<<"TCP server stop";
}

void ZK_ImageServer::net::TcpServer::newConnection(int connfd)
{
    EventLoop* ioLoop = eventLoopThreadPool_->getNextLoop();
    if(ioLoop == baseLoop_)
    {
        LOG_INFO<< "eventLoopThreadPool_ don't work";
    }
    LOG_INFO << "new connection is established,connfd is "<< connfd;
    TcpConnectionPtr tcpConnectionPtr(new TcpConnection(ioLoop,connfd));
    connectionMap_[connfd] = tcpConnectionPtr;
    tcpConnectionPtr->setNewConnectionCB(newConnCB_);
    tcpConnectionPtr->setMessageCallBackFun(messageCB_);
    ioLoop->runInLoop(std::bind(&TcpConnection::connEstablished,tcpConnectionPtr));
}
