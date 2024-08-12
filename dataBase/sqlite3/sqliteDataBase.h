#ifndef _SQLITEDATABASE_H__
#define _SQLITEDATABASE_H__

#include "stdDataBase.h"
#include <sqlite3.h>
class stdsqliteDataBase : public stdDataBase
{
public:
    stdsqliteDataBase();

    ~stdsqliteDataBase();
    //连接数据库
    bool connectDB(const std::string & dbFileName);
    //执行sql语句
    bool execute(const std::string sql);
    //查询sql语句
    vector<vector<string>> query(const std::string sql);
    //关闭数据库
    void closeDB();
private:
    sqlite3 * m_db;

};

#endif