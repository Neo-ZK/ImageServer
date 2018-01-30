#include "LogFile.h"


using namespace ZK_ImageServer;

LogFile::LogFile(const char* filename,size_t rollSize):
fileName_(filename),
rollSize_(rollSize),
mutex_(new Mutex)
{
    assert(rollfile());
}

LogFile::~LogFile()
{
    //dtor
}

void LogFile::append(const char* line, int len)
{
    MutexGuard mutexGuard(*mutex_);
    file_->append(line,len);
    if(file_->getWritenBytes() > rollSize_)
    {
        assert(rollfile());
        file_->setWritenBytes(0);
    }

}

bool LogFile::rollfile()
{
    std::string fileName = getLogFileName();
    while(fopen(fileName.c_str(),"r") != NULL)
    {
        int pos = fileName.find_last_of(')');
        fileName[pos-1] = fileName[pos-1]+1;
    }
    file_.reset(new AppendFile(fileName.c_str()));
    return true;
}

std::string LogFile::getLogFileName()
{
    TimeStamp   timestamp;
    std::string filename = fileName_;
    filename += "-time-";
    std::string timestring = timestamp.gettimeString();
    filename += timestring;
    filename += "-pid-";
    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, "%d", getpid());
    filename += pidbuf;
    filename += ".log";
    filename += "(1)";
    return filename;
}
