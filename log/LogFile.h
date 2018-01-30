#ifndef LOGFILE_H
#define LOGFILE_H

#include "../thread/Mutex.h"
#include "AppendFile.h"
#include "../thread/TimeStamp.h"
#include <string>
#include <time.h>
#include <iostream>
#include <memory>


namespace ZK_ImageServer
{
class LogFile
{
    public:
        LogFile(const char* filename,size_t rollSize = 10000000);
        ~LogFile();
        void append(const char* line,int len);
        bool rollfile();
        void flush(){file_->flush();};
        std::string getLogFileName();
    protected:
    private:
        std::string     fileName_;
        const size_t    rollSize_;
        std::unique_ptr<Mutex>  mutex_;
        std::unique_ptr <AppendFile> file_;


};

}//end namespace ZK_ImageServer

#endif // LOGFILE_H
