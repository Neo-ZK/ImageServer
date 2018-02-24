#include "ImageServer.h"

const int ZK_ImageServer::server::ImageServer::defaultThreadNum_ = 2;

ZK_ImageServer::server::ImageServer::ImageServer(ZK_ImageServer::net::EventLoop* baseLoop):
baseLoop_(baseLoop),
tcpServer_(std::bind(&ImageServer::connectionCallBackFun,this,std::placeholders::_1),
           std::bind(&ImageServer::messageCallBackFun,this,std::placeholders::_1,std::placeholders::_2),
           defaultThreadNum_,
           baseLoop_),
mysqlDB_()
{
    LOG_TRACE<<"Image server has established";
    successStr_ = "Success";
    faildStr_ = "Faild";
    begin_ = "begin";
}

ZK_ImageServer::server::ImageServer::~ImageServer()
{
    //dtor
}

void ZK_ImageServer::server::ImageServer::connectionCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn)
{
    LOG_TRACE << "message has arrived,connectionCallBackFun has been called";
}

void ZK_ImageServer::server::ImageServer::messageCallBackFun(ZK_ImageServer::net::TcpConnection::TcpConnectionPtr conn,ZK_ImageServer
        ::net::NetBuffer* buffer)
{
    LOG_TRACE << "messageCallBackFun has been called";
    buffer_ += buffer->retrieveAsString(buffer->readableByte());
    int pos = buffer_.find_first_of('*');
    if(pos > 0)
    {
        std::string operation = buffer_.substr(0,pos);
        if(operation == "Reg")
        {
           if(handleRegister(pos))
           {
                //send success message to client
                conn->sendInLoop(&*successStr_.begin(),successStr_.size());
           }
           else
           {
                conn->sendInLoop(&*faildStr_.begin(),faildStr_.size());
           }

        }
        else if(operation == "Log")
        {
            if(handleLog(pos))
            {
                conn->sendInLoop(&*successStr_.begin(),successStr_.size());
            }
            else
            {
                conn->sendInLoop(&*faildStr_.begin(),faildStr_.size());
            }

        }
        else if(operation == "Upload")
        {
            conn->sendInLoop(&*begin_.begin(),begin_.size());
            if(handleUpload(pos))
            {
                conn->sendInLoop(&*successStr_.begin(),successStr_.size());
            }
            else
            {
                conn->sendInLoop(&*faildStr_.begin(),faildStr_.size());
            }
        }
    }
    //conn->sendInLoop(&*temp.begin(),temp.size());
}

bool ZK_ImageServer::server::ImageServer::handleRegister(int pos)
{
    //a little complicated, but can avoid half packet problem
    std::string usrName;
    std::string password;
    std::string temp = buffer_.substr(pos+1);
    pos = temp.find_first_of('*');
    if(pos > 0)
    {
        usrName = temp.substr(0,pos);
        int lastPos = temp.rfind('*');
        if(lastPos != pos)
        {
            password = temp.substr(pos+1,lastPos-pos-1);
            std::string location;
            location += "/usr/ImageServer/";
            location += usrName;
            std::string sql;
            //create sql statement
            sql += "INSERT INTO BasicInfoTable (usr_name,usr_password,usr_location) values ('";
            sql += usrName;
            sql += "','";
            sql += password;
            sql += "','";
            sql += location;
            sql += "')";
            mysqlDB_.sqlInsert(sql.c_str());
            //create folder
            mkdir(location.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);
            //erase those message from buffer
            pos = buffer_.rfind('*');
            buffer_.erase(0,pos+1);
            return true;
        }
    }
    return false;
}

bool ZK_ImageServer::server::ImageServer::handleLog(int pos)
{
    std::string usrName;
    std::string password;
    std::string temp = buffer_.substr(pos+1);
    pos = temp.find_first_of('*');
    if(pos > 0)
    {
        usrName = temp.substr(0,pos);
        int lastPos = temp.rfind('*');
        if(lastPos != pos)
        {
            password = temp.substr(pos+1,lastPos-pos-1);
            if(password == mysqlDB_.getPassword(usrName.c_str()))
            {
                pos = buffer_.rfind('*');
                buffer_.erase(0,pos+1);
                return true;
            }
        }
    }
    return false;
}

bool ZK_ImageServer::server::ImageServer::handleUpload(int pos)
{
    std::string usrName;
    std::string password;
    std::string temp = buffer_.substr(pos+1);
    pos = temp.find_first_of('*');
    if(pos > 0)
    {
        usrName = temp.substr(0,pos);
    }
}
