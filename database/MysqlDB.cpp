#include "MysqlDB.h"

ZK_ImageServer::database::MysqlDB::MysqlDB():
resultPtr_(NULL),
field_(NULL),
resultRowNum_(0),
resultColNum_(0),
resultTag_(0)
{
    mysql_init(&conn_);
    assert(mysql_real_connect(&conn_,"localhost","root","","ImageServerDB",0,NULL,CLIENT_FOUND_ROWS));
    LOG_TRACE << "success connect to database";
}

ZK_ImageServer::database::MysqlDB::~MysqlDB()
{
    mysql_close(&conn_);
}

bool ZK_ImageServer::database::MysqlDB::sqlInsert(const char* sql)
{
    resultTag_ = mysql_query(&conn_,sql);
    if(resultTag_)
    {
        LOG_ERROR << "insert into database faild";
        return false;
    }
    else
    {
        LOG_INFO << "success insert data into database";
    }
    return true;
}

bool ZK_ImageServer::database::MysqlDB::sqlDelete(const char* sql)
{
    resultTag_ = mysql_query(&conn_,sql);
    if(resultTag_)
    {
        LOG_ERROR << "delete data from database faild";
        return false;
    }
    else
    {
        LOG_INFO << "success delete data from database";
    }
    return true;
}

bool ZK_ImageServer::database::MysqlDB::sqlModify(const char* sql)
{
    resultTag_ = mysql_query(&conn_,sql);
    if(resultTag_)
    {
        LOG_ERROR << "modify data from database faild";
        return false;
    }
    else
    {
        LOG_INFO << "success modify data from database";
    }
    return true;
}

bool ZK_ImageServer::database::MysqlDB::sqlQuery(const char* sql)
{
    resultTag_ = mysql_query(&conn_,sql);
    if(resultTag_)
    {
        LOG_ERROR << "query database false";
        return false;
    }
    else
    {
        LOG_INFO << "query data from database ImageServerDB";
        resultPtr_ = mysql_store_result(&conn_);
        if(!resultPtr_)
        {
            LOG_INFO << "no information in database of this action query";
        }
        else
        {
            resultColNum_ = mysql_num_fields(resultPtr_);
            resultRowNum_ = mysql_num_rows(resultPtr_);
            field_ = mysql_fetch_field(resultPtr_);
            resultRowData_ = mysql_fetch_row(resultPtr_);
        }
    }
    return true;
}


