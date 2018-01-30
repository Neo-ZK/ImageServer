#include "TcpConnection.h"

ZK_ImageServer::net::TcpConnection::TcpConnection(EventLoop* ownerLoop,int fd):
connfd_(fd),
state_(Connecting),
ownerLoop_(ownerLoop),
channel_(new Channel(ownerLoop_,fd)),
newConnectionCB_(NULL),
messageRecvCB_(NULL),
closeCB_(NULL),
inputBuffer_(),
outputBuffer_()
{
    channel_->setReadCallBack(std::bind(&TcpConnection::handleRead,this));
    channel_->setWriteCallBack(std::bind(&TcpConnection::handleWrite,this));
    channel_->setCloseCallBack(std::bind(&TcpConnection::handleClose,this));
    channel_->setErrorCallBack(std::bind(&TcpConnection::handleError,this));
}

ZK_ImageServer::net::TcpConnection::~TcpConnection()
{
    LOG_TRACE<<"TCP connection has been destroyed,connfd is "<< connfd_;
    channel_->disableAll();
}


void ZK_ImageServer::net::TcpConnection::sendInLoop(void* data,int len)
{
    assert(ownerLoop_->isInLoopThread());
    outputBuffer_.append(data,len);
    if(!channel_->isWriting())
    {
        channel_->enableWrite();
    }
}

void ZK_ImageServer::net::TcpConnection::handleRead()
{
    LOG_TRACE<<"Tcp connection handleRead has been called";
    assert(ownerLoop_->isInLoopThread());
    size_t readbyte = inputBuffer_.readFd(channel_->getFd());
    if(readbyte == 0)
    {
        handleClose();
    }
    else if(readbyte > 0)
    {
        messageRecvCB_(shared_from_this(),&inputBuffer_);
    }
    else
    {
        LOG_ERROR << "Error happen in tcpconnection handleread";
        handleError();
    }
}



void ZK_ImageServer::net::TcpConnection::handleWrite()
{
    assert(ownerLoop_->isInLoopThread());

    size_t sendLen = send(channel_->getFd(),outputBuffer_.readBegin(),outputBuffer_.readableByte(),0);
    //size_t sendLen = send(channel_->getFd(),"receive successful",19,0);
    if(channel_->isWriting())
    {
        if(sendLen > 0)
        {
            outputBuffer_.retrieve(sendLen);
            if(outputBuffer_.readableByte() == 0)
            {
                channel_->disableWriting();
            }
        }
        else
        {
            LOG_ERROR << "Error happen in handleWrite send";
        }
    }
    else
    {
        LOG_WARNING << "fd: "<<channel_->getFd()<<" is down, no more writing";
    }


}

void ZK_ImageServer::net::TcpConnection::handleClose()
{
}

void ZK_ImageServer::net::TcpConnection::handleError()
{
}

void ZK_ImageServer::net::TcpConnection::connEstablished()
{
    LOG_TRACE<< "connEstablished has been called in TcpConnection ";
    assert(ownerLoop_->isInLoopThread());
    assert(state_ == Connecting);
    setState(Connected);
    channel_->enableRead();
    assert(newConnectionCB_);
    newConnectionCB_(shared_from_this());
}

void ZK_ImageServer::net::TcpConnection::connDestroyed()
{
    assert(ownerLoop_->isInLoopThread());
    if(state_ == Connected)
    {
        setState(Disconnected);
        channel_->disableAll();
    }
    channel_->removeFromLoop();
}
