#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include<string>
#include<time.h>
#include<sys/time.h>
namespace ZK_ImageServer
{
class TimeStamp
{
    public:
        TimeStamp();
        ~TimeStamp();
        std::string gettimeString();
    protected:
    private:
        std::string time_string_;
        tm*         time_;

};

}//end namespace ZK_ImageServer
#endif // TIMESTAMP_H
