#include "Client.h"

ZK_ImageServer::client::Client::Client(char* serveraddr,int defaultport):
sockfd_(0),
serverAddr_(),
defaultPort_(defaultport),
state_(0)
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
        std::string sendBuffer("Log*");
        std::string userName;
        std::cout << "input your user name:"<<std::endl;
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
        if(recvLen > 0 )
        {
            std::string state = recvBuffer.substr(0,recvLen);
            if(state == "Success")
            {
                std::cout << "Logging success";
                recvBuffer.clear();
                return 1;
            }
            else if(state == "Faild")
            {
                std::cout << "password error or No user";
                recvBuffer.clear();
                return 1;
            }
        }
        else
        {
            std::cout << "logging faild,connect error";
            recvBuffer.clear();
            return 0;
        }
    }
    return 0;
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
    if(recvLen > 0)
    {
        std::string state = recvBuffer.substr(0,recvLen);
        if(state == "Success")
        {
            std::cout << "Logging success";
            recvBuffer.clear();
            return 1;
        }
        else if(state == "Faild")
        {
            std::cout << "password error or No user";
            recvBuffer.clear();
            return 1;
        }
    }
    else
    {
        std::cout << "register faild,connect error";
        recvBuffer.clear();
    }
    return 0;
}

int ZK_ImageServer::client::Client::usrUpload(const char* location)
{
    std::string recvBuffer;
    std::ifstream infile;
    infile.open(location,std::ios::binary);
    if(!infile)
    {
        std::cout << "file don't exist"<<std::endl;
    }
    //send operation
    std::string operation = "Upload*";
    send(sockfd_,operation.c_str(),operation.length(),0);
    //recv confirm information,then start transimit file
    int recvLen = recv(sockfd_,&*recvBuffer.begin(),recvBuffer.capacity(),0);
    if(recvLen > 0)
    {
        std::string state = recvBuffer.substr(0,recvLen);
        if(state == "begin")
        {
            std::cout << "start transmission";
            char buffer[1024];
            while(!infile.eof())
            {
                memset(buffer,0,1024);
                infile >> buffer;
                send(sockfd_,buffer,1024,0);
            }
            operation.clear();
            operation = "over";
            send(sockfd_,operation.c_str(),operation.length(),0);
            recvLen = recv(sockfd_,&*recvBuffer.begin(),recvBuffer.capacity(),0);
            if(recvLen > 0)
            {
                std::string state = recvBuffer.substr(0,recvLen);
                if(state == "Success")
                {
                    infile.close();
                    return 1;
                }
            }
        }
    }

    infile.close();
    return 0;
}

void ZK_ImageServer::client::Client::start()
{
    int connRes = connect(sockfd_,(sockaddr*)&serverAddr_,sizeof(serverAddr_));
    assert(connRes >= 0);
    while(true)
    {
        std::string operation;
        std::cout << "input your operation(Logging or Register)"<<std::endl;
        std::cin >> operation;
        if(operation == "Logging")
        {
            if(usrLogging())
            {
                state_ = 1;
                break;
            }
        }
        else if(operation == "Register")
        {
            if(usrRegister())
            {
                state_ = 1;
                break;
            }
        }
        else
        {
            std::cout << "wrong operation";
        }
        operation.clear();
    }

    while(state_)
    {
        std::cout << "input your operation(upload or download or process):"<<std::endl;
        std::string operation;
        std::cin >> operation;
        if(operation == "upload")
        {
            std::cout << "input your file location:"<<std::endl;
            std::string location;
            std::cin >> location;

        }
    }

}
