#include "LogStream.h"
using namespace ZK_ImageServer;

LogStream::LogStream()
{
    //ctor
}

LogStream::~LogStream()
{
    //dtor
}

LogStream& LogStream::operator<<(const char* line)
{
    buffer_.append(line,strlen(line));
    return *this;
}

LogStream& LogStream::operator<<(const std::string line)
{
    buffer_.append(line.c_str(),line.size());
    return *this;
}

LogStream& LogStream::operator<<(const char str)
{
    buffer_.append(&str,1);
    return *this;
}

LogStream& LogStream::operator << (const int str)
{
    std::stringstream ss;
    ss << str;
    std::string out;
    out = ss.str();
    buffer_.append(out.c_str(),out.size());
    return *this;
}
