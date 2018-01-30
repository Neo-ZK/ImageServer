#include "TimeStamp.h"
#include <sstream>

using namespace ZK_ImageServer;
TimeStamp::TimeStamp():time_string_(),time_(NULL)
{

}

TimeStamp::~TimeStamp()
{
    //dtor
}

std::string TimeStamp::gettimeString()
{
    time_t tt = time(NULL);
    time_ = localtime(&tt);
    struct timeval start;
    gettimeofday(&start,NULL);
    std::stringstream ss;
    ss<<time_->tm_year + 1900<<'.';
    ss<<time_->tm_mon + 1<<'.';
    ss<<time_->tm_mday<<'.';
    ss<<time_->tm_hour<<'.';
    ss<<time_->tm_min<<'.';
    ss<<time_->tm_sec<<'.';
    ss<<start.tv_usec;
    time_string_ = ss.str();
    return time_string_;
}
