#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace ZK_ImageServer
{
namespace client
{

class Client
{
    public:
        Client(char* serveraddr,int defaultport = 8000);
        ~Client();
        void start();
        int usrLogging();
        int usrRegister();
    protected:
    private:
        int sockfd_;
        struct sockaddr_in serverAddr_;
        const int defaultPort_;

};


}//end namespace client
}//end namespace ZK_ImageServer
#endif // CLIENT_H
