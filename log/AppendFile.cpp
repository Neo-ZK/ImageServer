#include "AppendFile.h"
using namespace ZK_ImageServer;

AppendFile::AppendFile(const char* filename):fp_(fopen(filename,"ae")),writenBytes_(0)
{
    assert(fp_);
    setbuffer(fp_,buffer_,sizeof(buffer_));
}

AppendFile::~AppendFile()
{
    fclose(fp_);
}

void AppendFile::append(const char* line,int len)
{
    size_t n = fwrite_unlocked(line,1,len,fp_);
    size_t remain = len - n;
    while(remain > 0)
    {
        size_t x = fwrite_unlocked(line+n,1,remain,fp_);
        if(x == 0)
        {
            //int err = ferror(fp_);
            printf("write error");
            abort();
            break;
        }
        n += x;
        remain = len - n;
    }
    fflush(fp_);
    writenBytes_ += len;
}

void AppendFile::flush()
{
    fflush(fp_);
}
