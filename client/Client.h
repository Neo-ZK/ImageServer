#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <fstream>
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
        int usrUpload(const char* location);
    protected:
    private:
        int sockfd_;
        struct sockaddr_in serverAddr_;
        const int defaultPort_;
        int state_;//0 means not Logging,1 means has been Log in

};


}//end namespace client
}//end namespace ZK_ImageServer
#endif // CLIENT_H
