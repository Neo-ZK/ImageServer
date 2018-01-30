#ifndef LOGGER_H
#define LOGGER_H

#include"LogStream.h"
#include"../thread/TimeStamp.h"
#include"../thread/CurrentThread.h"
#include<functional>
#include<iostream>
#include<string>
#include<sstream>

namespace ZK_ImageServer
{
enum LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    LOGLEVEL_NUM
};

class Logger
{
    typedef std::function<void (const char* line,int len)> OutputFun;
    public:
        Logger(const char* filename,int line,LogLevel loglevel):implement_(filename,line,loglevel){};
        ~Logger();
        LogStream& stream(){return implement_.logstream_;}
        static void setOutputFun(OutputFun outfun);

        static OutputFun outputFun;
    private:
        //get real log infomation,use this class to encape,avoid user access
        class Implement
        {
            public:
                Implement(const char* filename,int line,LogLevel loglevel);
                ~Implement();
                void finish();
                TimeStamp       timestamp_;
                std::string     FileName_;
                int             line_;
                LogLevel        loglevel_;
                LogStream       logstream_;
                CurrentThread   currenthread_;
        };
        Implement implement_;
};
#define LOG_TRACE ZK_ImageServer::Logger(__FILE__,__LINE__,ZK_ImageServer::TRACE).stream()
#define LOG_DEBUG ZK_ImageServer::Logger(__FILE__,__LINE__,ZK_ImageServer::DEBUG).stream()
#define LOG_INFO ZK_ImageServer::Logger(__FILE__,__LINE__,ZK_ImageServer::INFO).stream()
#define LOG_WARNING ZK_ImageServer::Logger(__FILE__,__LINE__,ZK_ImageServer::WARNING).stream()
#define LOG_ERROR ZK_ImageServer::Logger(__FILE__,__LINE__,ZK_ImageServer::ERROR).stream()
}//end namespace ZK_ImageServer
#endif // LOGGER_H

