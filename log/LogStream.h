#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include<string.h>
#include<string>
#include<sstream>
namespace ZK_ImageServer
{

const int SmallBufferSize = 4000;
const int LargeBufferSize = 4000*1000;

template<int BUFFER_SIZE>
class FixBuffer
{
    public:
        FixBuffer():cur_(data_){refresh();};
        ~FixBuffer(){};
        void append(const char* line,int len)
        {
            if(avil() > len)
            {
                memcpy(cur_,line,len);
                cur_ += len;
            }
        }
        void refresh() {bzero(data_,BUFFER_SIZE);reset();}
        const char* data() const{return data_;}
        char* cur() {return cur_;}
        int length() const {return static_cast<int>(cur_ - data_);}
        int avil(){return (BUFFER_SIZE - length());}
        void reset(){cur_ = data_;}
    private:
        char    data_[BUFFER_SIZE];
        char*   cur_;
        char*   start_;
};


class LogStream
{

    public:
    typedef FixBuffer<SmallBufferSize> Buffer;
        LogStream();
        ~LogStream();
        const Buffer& buffer(){return buffer_;}
        LogStream& operator << (const char* str);
        LogStream& operator << (const std::string);
        LogStream& operator << (const char str);
        LogStream& operator << (const int str);
    private:
        Buffer buffer_;
};

}//end namespace ZK_ImageServer
#endif // LOGSTREAM_H
