#ifndef MYSQLDB_H
#define MYSQLDB_H


#include<mysql.h>
#include<assert.h>
#include "../log/Logger.h"


namespace ZK_ImageServer
{
namespace database
{
class MysqlDB
{
    public:
        MysqlDB();
        ~MysqlDB();
        bool sqlInsert(const char* sql);
        bool sqlDelete(const char* sql);
        bool sqlModify(const char* sql);
        bool sqlQuery(const char* sql);

    protected:
    private:
        MYSQL           conn_;
        MYSQL_RES*      resultPtr_;
        MYSQL_FIELD*    field_;
        MYSQL_ROW       resultRowData_;
        int             resultRowNum_,resultColNum_;
        int             resultTag_;
};
}//end namespace database
}//end namespace ZK_ImageServer

#endif // MYSQLDB_H
