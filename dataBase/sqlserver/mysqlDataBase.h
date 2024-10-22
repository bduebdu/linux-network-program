#ifndef __MYSQLDATABASE__H__
#define __MYSQLDATABASE__H__
#include "stdDataBase.h"
#include<mysql/mysql.h>
#include<unistd.h>

class stdMysqlDataBase : public stdDataBase
{
public:
    stdMysqlDataBase();

    ~stdMysqlDataBase();
    //连接数据库
    bool connectDB(const std::string & dbFileName);
    //执行sql语句
    bool execute(const std::string sql);
    //查询sql语句
    vector<vector<string>> query(const std::string sql);
    //关闭数据库
    void closeDB();
public:


private:
    MYSQL * m_sqlDB;

};









#endif