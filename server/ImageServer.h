#ifndef IMAGESERVER_H
#define IMAGESERVER_H


#include <functional>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include "../net/TcpServer.h"
#include "../net/EventLoop.h"
#include "../database/MysqlDB.h"


namespace ZK_ImageServer
{
namespace server
{

class ImageServer
{
    public:
        ImageServer(ZK_ImageServer::net::EventLoop* baseLoop);
        ~ImageServer();

    protected:
    private:
        ZK_ImageServer::net::EventLoop*    baseLoop_;
        void connectionCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn);
        void messageCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn,
                                ZK_ImageServer::net::NetBuffer* buffer);
        bool handleRegister(int pos);
        bool handleLog(int pos);
        bool handleUpload(int pos);

        ZK_ImageServer::net::TcpServer tcpServer_;

        ZK_ImageServer::database::MysqlDB mysqlDB_;

        static const int defaultThreadNum_;

        std::string buffer_;
        std::string successStr_;
        std::string faildStr_;
        std::string begin_;

};

}//end namespace server
}//end namespace ZK_ImageServer
#endif // IMAGESERVER_H
