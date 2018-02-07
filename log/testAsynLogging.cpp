/*
#include <iostream>
#include <stdio.h>
#include "thread/CurrentThread.h"
#include "thread/Mutex.h"
#include "log/LogFile.h"
#include "log/Logger.h"
#include "log/AsyncLogger.h"
#include "server/Echo.h"
#include <unistd.h>
#include <sys/time.h>
using namespace std;

ZK_ImageServer::AsyncLogger* g_AsyncLogger = NULL;
void testOutput(const char* line,int len)
{
    g_AsyncLogger->append(line,len);
}


void testAsyn()
{
    for(int i = 0;i < 10000;i++)
    {
        LOG_INFO<<"ASYNlogging is OK ........."<<i;
    }
}


int main()
{

    ZK_ImageServer::Logger::setOutputFun(testOutput);
    ZK_ImageServer::AsyncLogger asynlogger(__FILE__,1000000);
    g_AsyncLogger = &asynlogger;
    g_AsyncLogger->start();

    testAsyn();
    return 0;
}*/
