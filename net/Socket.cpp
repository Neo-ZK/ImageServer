#include "Socket.h"
using namespace ZK_ImageServer;

const int net::Socket::defaultListenPort = 8000;

net::Socket::Socket(int listenPort):
listenfd_(0),
listenPort_(listenPort),
serverAddr_(),
clientAddr_(),
clientLen_(0)
{
   listenfd_ = createSocket();
   setNonBlock(listenfd_);
   memset(&serverAddr_,0,sizeof(serverAddr_));
   serverAddr_.sin_family = AF_INET;
   serverAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
   serverAddr_.sin_port = htons(listenPort_);

}

net::Socket::~Socket()
{
    //dtor
}

void net::Socket::setNonBlock(int fd)
{
    int flags = fcntl(fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(fd,F_SETFL,flags);
}

void net::Socket::bindAndListen()
{
    if(bind(listenfd_,(sockaddr*)&serverAddr_,sizeof(sockaddr)) == -1)
    {
        LOG_ERROR << "bind error";
    }
    else
    {
        LOG_INFO << "bind " <<listenfd_ <<" to "<<listenPort_;
    }

    if(listen(listenfd_,SOMAXCONN) == -1)
    {
        LOG_ERROR << "listen error";
    }
    else
    {
        LOG_INFO << "listen: " <<listenfd_;
    }
}

int net::Socket::createSocket()
{
    int listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listenfd == -1)
    {
        LOG_ERROR<< "create socket error";
    }
    else
    {
        LOG_INFO << "create socket :"<<listenfd;
    }
    return listenfd;
}

int net::Socket::getAcceptSocket()
{
    int connfd = accept(listenfd_,(sockaddr*)&clientAddr_,&clientLen_);
    if(connfd < 0)
    {
        LOG_ERROR << "Accept Error";
        int errNum = errno;
        switch(errNum)
        {
            case EAGAIN:
            default:
                break;
        }
    }
    return connfd;
}
