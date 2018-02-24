#include "Client.h"

ZK_ImageServer::client::Client::Client(char* serveraddr,int defaultport):
sockfd_(0),
serverAddr_(),
defaultPort_(defaultport)
{
    sockfd_ = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd_ > 0);
    memset(&serverAddr_,0,sizeof(serverAddr_));
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(defaultPort_);
    inet_pton(AF_INET,serveraddr,&serverAddr_.sin_addr);
}

ZK_ImageServer::client::Client::~Client()
{
    //dtor
}

int ZK_ImageServer::client::Client::usrLogging()
{
    while(true)
    {

    }
    return 1;
}

int ZK_ImageServer::client::Client::usrRegister()
{
    std::string sendBuffer("Reg*");
    std::string userName;
    std::cout << "input your user name,name can't have special string like *,/... :"<<std::endl;
    std::cin >> userName;
    sendBuffer += userName;
    sendBuffer += "*";
    std::cout << "input your password:"<<std::endl;
    std::string password;
    std::cin >> password;
    sendBuffer += password;
    sendBuffer += '*';
    send(sockfd_,sendBuffer.c_str(),sendBuffer.length(),0);
    std::string recvBuffer;
    recvBuffer.reserve(1024);
    int recvLen = recv(sockfd_,&*recvBuffer.begin(),recvBuffer.capacity(),0);
    if(recvLen <= 0)
    {
        std::cout << "register faild,connect error";
        return 0;
    }
    else
    {
        std::cout << "register success";
        return 1;
    }
}

void ZK_ImageServer::client::Client::start()
{
    int connRes = connect(sockfd_,(sockaddr*)&serverAddr_,sizeof(serverAddr_));
    assert(connRes >= 0);
    std::string operation;
    while(true)
    {
        std::cout << "input your operation(Logging or Register)";
        std::cin >> operation;
        if(operation == "Logging")
        {
            if(usrLogging())
            {
                break;
            }
        }
        else if(operation == "Register")
        {
            if(usrRegister())
            {
                break;
            }
        }
        else
        {
            std::cout << "wrong operation";
        }
        operation.clear();
    }

}
