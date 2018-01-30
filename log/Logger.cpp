#include "Logger.h"
using namespace ZK_ImageServer;



const char* LogLevelStr[LogLevel::LOGLEVEL_NUM] =
{
    "TRACE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

void defaultOutput(const char* line,int len)
{
    fwrite(line,1,len,stdout);
}

Logger::OutputFun Logger::outputFun = defaultOutput;

Logger::~Logger()
{
    implement_.finish();
    const LogStream::Buffer& buffer(stream().buffer());
    outputFun(buffer.data(),buffer.length());
}

void Logger::setOutputFun(Logger::OutputFun outfun)
{
    outputFun = outfun;
}


Logger::Implement::Implement(const char* filename,int line,LogLevel loglevel):
    timestamp_(),
    FileName_(filename),
    line_(line),
    loglevel_(loglevel),
    logstream_(),
    currenthread_()
{
    std::string data;
    data += timestamp_.gettimeString();
    currenthread_.catchTid();
    data += "  ";
    data += currenthread_.tidString();
    data += "  ";
    data += LogLevelStr[loglevel];
    logstream_ << data << "   ";
}

Logger::Implement::~Implement()
{

}

void Logger::Implement::finish()
{
    logstream_ << "   ";
    int pos = FileName_.find_last_of('/');
    std::string enddata(FileName_,pos+1,FileName_.size());
    enddata += ":";
    std::stringstream ss;
    ss << line_;
    enddata += ss.str();
    logstream_ << enddata << '\n';
}
